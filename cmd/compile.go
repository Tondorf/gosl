package cmd // code.bitsetter.de/fun/gosl/cmd

import (
	"github.com/spf13/cobra"
	"log"
	"path"
)

var (
	levelDir  string
	levelFile string
)

var cmdCompile = &cobra.Command{
	Use:   "compile <leveldir>",
	Short: "Compiles a gosl level into a loadable level file",
	Long: `Compiles a level file from a given input directory.
It's contents contain at least:

  / leveldir
     +-- Manifest.json        Basic level Info
	 +-- Layer1/              At least one layer dir containing …
	      +-- Manifest.json    Basic layer Info
	      +-- 1.Frame          At least one frame

See the example level for further details (which does not exist, yet :p)`,
}

func compile(cmd *cobra.Command, args []string) {
	if len(args) < 1 {
		log.Fatal("You need to specify a level directory to compile!")
	}
	levelDir = args[0]
	if levelFile == "" {
		levelFile = path.Base(levelDir) + ".lvl"
	}
	log.Println("will compile to", levelFile)

}

func init() {
	CmdGosl.AddCommand(cmdCompile)
	cmdCompile.Run = compile
	cmdCompile.Flags().StringVarP(&levelFile, "output", "o", "", "Output filename")
}
