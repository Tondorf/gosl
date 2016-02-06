package cmd // code.bitsetter.de/fun/gosl/cmd

import (
	"encoding/json"
	"fmt"
	"github.com/spf13/cobra"
	"io/ioutil"
	"log"

	"path"
	"strconv"
	"strings"
)

type directionType int

const (
	DIR_NULL directionType = iota // Undefined direction: NO Motion
	DIR_SW                        //
	DIR_S                         //  NW   N   NE
	DIR_SE                        //     7 8 9
	DIR_W                         //  W  4 5 6  E
	DIR_NONE                      //     1 2 3
	DIR_E                         //  SW   S   SE
	DIR_NW
	DIR_N
	DIR_NE
)

type Direction interface {
	Base() directionType
}

func (i directionType) Base() directionType { return i }

type LayerManifest struct {
	Z      int           `json:"Z-Index"`
	D      directionType `json:"Direction"`
	S      int           `json:"Speed"`
	T      string        `json:"Transparent"`
	Frames map[int]([][]rune)
}

type LevelManifest struct {
	Name   string
	FPS    int
	Layers map[string]*LayerManifest
}

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

	lvlMan := &LevelManifest{}
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

}

func init() {
	CmdGosl.AddCommand(cmdCompile)
	cmdCompile.Run = compile
	cmdCompile.Flags().StringVarP(&levelFile, "output", "o", "", "Output filename")
}
