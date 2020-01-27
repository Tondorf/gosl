package main

import (
	"os"

	"github.com/Tondorf/gosl/cmd"
)

func main() {
	if err := cmd.CmdGosl.Execute(); err != nil {
		os.Exit(-1)
	}
}
