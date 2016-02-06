package cmd // code.bitsetter.de/fun/gosl/cmd

import (
	"bytes"
	"encoding/gob"
	"encoding/json"
	"fmt"
	"github.com/spf13/cobra"
	"io/ioutil"
	"log"

	"path"
	"strconv"
	"strings"

	"code.bitsetter.de/fun/gosl/data"
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

	lvlMan := &data.Level{}
	buf, err := ioutil.ReadFile(levelDir + "/Manifest.json")
	if err != nil {
		log.Fatal("Error reading file: ", err)
	}

	err = json.Unmarshal(buf, lvlMan)
	if err != nil {
		log.Fatal("Error reading level manifest: ", err)
	}

	fmt.Println(lvlMan)

	for k, _ := range lvlMan.Layers {
		log.Println("Processing layer: ", k)
		dirlist, derr := ioutil.ReadDir(levelDir + "/" + k)
		if derr != nil {
			log.Println("Layer directory error: ", derr)
		} else {
			lvlMan.Layers[k].Frames = make(map[int]([][]rune))
			for _, fi := range dirlist {
				if path.Ext(fi.Name()) == ".frame" {
					log.Println(fi.Name())
					fdata, derr := ioutil.ReadFile(levelDir + "/" + k + "/" + fi.Name())
					if derr != nil {
						log.Println("Error reading frame: ", derr)
					} else {
						rdata := make([][]rune, 0, 25)
						for _, line := range strings.Split(string(fdata), "\n") {
							rdata = append(rdata, []rune(line))
						}
						idx, ierr := strconv.Atoi(strings.TrimSuffix(fi.Name(), path.Ext(fi.Name())))
						if ierr != nil {
							log.Printf("Invalid Index in Filename: ", ierr)
						} else {
							lvlMan.Layers[k].Frames[idx] = rdata
						}
					}
				}
			}
		}
	}
	obuf := &bytes.Buffer{}
	enc := gob.NewEncoder(obuf)
	enc.Encode(&lvlMan)
	ioutil.WriteFile(levelFile, obuf.Bytes(), 0644)
	log.Println("done.")
}

func init() {
	CmdGosl.AddCommand(cmdCompile)
	cmdCompile.Run = compile
	cmdCompile.Flags().StringVarP(&levelFile, "output", "o", "", "Output filename")
}
