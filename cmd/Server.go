package cmd // code.bitsetter.de/fun/gosl/cmd

import (
	"fmt"
	"log"
	"net"

	"github.com/spf13/cobra"

	"code.bitsetter.de/fun/gosl/data"
)

var cmdServer = &cobra.Command{
	Use:   "server",
	Short: "Runs Gosl as a server",
	Long:  "Runs Gosl as a server",
	//	Run:
}

func handleConn(conn *net.TCPConn) {
	log.Println("Got a connection!")
	conn.Close()

}

func runServer(cmd *cobra.Command, args []string) {
	fmt.Println("running server ...")

	listener, err := net.ListenTCP("tcp", &net.TCPAddr{Port: 8989})
	if err != nil {
		log.Fatal(err)
		panic("Could not open Listener")
	}
	defer listener.Close()
	for {
		conn, err := listener.AcceptTCP()
		if err != nil {
			log.Fatal(err)
			panic("Listener could not accept connection!")
		}

		go handleConn(conn)
	}

	data.TestNC()
}

func init() {
	cmdServer.Run = runServer
}
