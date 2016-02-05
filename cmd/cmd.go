package cmd // code.bitsetter.de/fun/gosl/cmd

import (
	"os"

	"github.com/spf13/cobra"
)

const SERVERPORT int = 8989

type handshake struct{ ID, W, H int }

var cmdGosl = &cobra.Command{
	Use:   "gosl [command]",
	Short: "Run a gosl instance",
	Long: `Run a gosl server or client.

For further instructions run 'gosl help [command]'
`,
}

func Execute() {
	cmdGosl.AddCommand(cmdServer)
	cmdGosl.AddCommand(cmdClient)
	if err := cmdGosl.Execute(); err != nil {
		os.Exit(-1)
	}
}
