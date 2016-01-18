package cmd // code.bitsetter.de/fun/gosl/cmd

import (
	"fmt"

	"github.com/spf13/cobra"
)

var cmdServer = &cobra.Command{
	Use:   "server",
	Short: "Runs Gosl as a server",
	Long:  "Runs Gosl as a server",
	//	Run:
}

func runServer(cmd *cobra.Command, args []string) {
	fmt.Println("running server ...")
}

func init() {
	cmdServer.Run = runServer
}
