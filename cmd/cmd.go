package cmd // code.bitsetter.de/fun/gosl/cmd

import (
	"os"

	"github.com/spf13/cobra"
)

var CmdGosl = &cobra.Command{
	Use:   "gosl [command]",
	Short: "Run a gosl instance",
	Long: `Run a gosl server or client.

For further instructions run 'gosl help [command]'
`,
}

func Execute() {
	if err := CmdGosl.Execute(); err != nil {
		os.Exit(-1)
	}
}
