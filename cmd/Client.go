package cmd // code.bitsetter.de/fun/gosl/cmd

import (
	"encoding/gob"
	"fmt"
	"net"
	"os"
	"strconv"

	"github.com/spf13/cobra"

	"code.bitsetter.de/fun/gosl/data"
)

var cmdClient = &cobra.Command{
	Use:   "client <IP> <NUMBER>",
	Short: "Runs Gosl as a client",
	Long:  "Runs Gosl as a client",
	Run:   runClient,
}

//func init() {
//	cmdClient.Run = runClient
//}

func register(con net.Conn, id int) error {
	w, h := data.TestNC()
	enc := gob.NewEncoder(con) // Encoder
	err := enc.Encode(handshake{ID: id, H: h, W: w})
	return err
}

func runClient(cmd *cobra.Command, args []string) {
	fmt.Println("running client ...")

	if len(args) < 2 {
		fmt.Println("Params: <IP> <NUMBER>")
		os.Exit(1)
	}
	server := args[0]
	id, _ := strconv.Atoi(args[1])

	con, err := net.Dial("tcp", server+":"+strconv.Itoa(SERVERPORT))
	if err != nil {
		fmt.Println("connect error:", err)
	}

	register(con, id)
}
