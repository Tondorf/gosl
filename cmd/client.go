package cmd // code.bitsetter.de/fun/gosl/cmd

import (
	"encoding/gob"
	"fmt"
	"log"
	"net"
	"os"
	"strconv"

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
	data.InitNC()
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
	for {
		var oFrame data.Frame
		// always use new decoder - reusing may lead to errors
		gd := gob.NewDecoder(con)
		err = gd.Decode(&oFrame)
		if err != nil {
			log.Fatal("RGS:", err)
		}
		renderQueue <- oFrame
	}

}

func init() {
	CmdGosl.AddCommand(cmdClient)
	cmdClient.Run = runClient

	cmdClient.Flags().StringVarP(&ServerHost, "host", "H", "127.0.0.1", "Connect to this server")
	cmdClient.Flags().IntVarP(&ServerPort, "port", "p", 8090, "Connect to server on this port")
}
