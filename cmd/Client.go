package cmd // code.bitsetter.de/fun/gosl/cmd

import (
	"fmt"

	"github.com/spf13/cobra"

	"code.bitsetter.de/fun/gosl/data"
)

var cmdClient = &cobra.Command{
	Use:   "client",
	Short: "Runs Gosl as a client",
	Long:  "Runs Gosl as a client",
	//	Run:
}

func runClient(cmd *cobra.Command, args []string) {
	fmt.Println("running client ...")
	data.TestNC()
}

func init() {
	cmdClient.Run = runClient
}
