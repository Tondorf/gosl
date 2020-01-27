package cmd

import (
	"github.com/spf13/cobra"
)

var CmdGosl = &cobra.Command{
	Use:   "gosl [command]",
	Short: "Run a gosl instance",
	Long: `Run a gosl server or client.

For further instructions run 'gosl help [command]'
`,
}
