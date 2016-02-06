package cmd // code.bitsetter.de/fun/gosl/cmd

import (
	"encoding/gob"
	"fmt"
	"log"
	"net"
	"os"
	"os/signal"
	"strconv"
	"syscall"

	"github.com/spf13/cobra"

	"code.bitsetter.de/fun/gosl/data"
)

var cmdClient = &cobra.Command{
	Use:   "client <NUMBER>",
	Short: "Runs Gosl as a client",
	Long:  "Runs Gosl as a client",
	//Run:   runClient,
}

var (
	ServerHost  string
	renderQueue = make(chan data.Frame)
)

func register(con net.Conn, id int) error {
	w, h := 0, 0               //data.TestNC()
	enc := gob.NewEncoder(con) // Encoder
	err := enc.Encode(handshake{ID: id, H: h, W: w})
	return err
}

func render() {
	var f data.Frame
	for f = range renderQueue {
		data.RenderFrame(&f)
	}
}

func runClient(cmd *cobra.Command, args []string) {
	c := make(chan os.Signal)
	signal.Notify(c, os.Interrupt, os.Kill)
	signal.Notify(c, syscall.SIGTERM)

	data.InitNC(c)
	// really important???
	defer data.ExitNC()

	fmt.Println("running client ...")

	if len(args) < 1 {
		fmt.Println("Params: <NUMBER>")
		os.Exit(1)
	}
	id, _ := strconv.Atoi(args[0])

	con, err := net.Dial("tcp", ServerHost+":"+strconv.Itoa(ServerPort))
	if err != nil {
		fmt.Println("connect error:", err)
	}
	defer con.Close()
	register(con, id)

	go render()
	run := true
	for run {
		var oFrame data.Frame
		// always use new decoder - reusing may lead to errors
		gd := gob.NewDecoder(con)
		err = gd.Decode(&oFrame)
		if err != nil {
			log.Println("RGS:", err)
			run = false
		}
		renderQueue <- oFrame

		select {
		case <-c:
			// funzt noch nicht:
			log.Println("Got a KILL")
			con.Close()
			close(renderQueue)
			run = false
		default:
			if data.GetChar() != 0 {
				data.ExitNC()
			}
		}
	}

}

func init() {
	CmdGosl.AddCommand(cmdClient)
	cmdClient.Run = runClient

	cmdClient.Flags().StringVarP(&ServerHost, "host", "H", "127.0.0.1", "Connect to this server")
	cmdClient.Flags().IntVarP(&ServerPort, "port", "p", 8090, "Connect to server on this port")
}
