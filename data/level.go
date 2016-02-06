package data // code.bitsetter.de/fun/gosl/data

import (
	"encoding/gob"
	"log"
	"os"
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

type Layer struct {
	Z      int           `json:"Z-Index"`
	D      directionType `json:"Direction"`
	S      int           `json:"Speed"`
	T      string        `json:"Transparent"`
	Repeat bool
	Frames map[int]([][]rune)
}

type Level struct {
	Name   string
	FPS    int
	Layers map[string]*Layer
}

//func (lvl *Level) AddLayer(z int, l *Layer) {
//	lvl.Layers[z] = l
//}

func LoadLevel(filename string) *Level {
	log.Println("Loading lvl ", filename)
	ret := &Level{}
	file, err := os.Open(filename)
	if err != nil {
		log.Println("Error reading lvl: ", err)
		return nil
	}
	dec := gob.NewDecoder(file)
	dec.Decode(ret)
	//log.Println(ret.Layers["locomotive"])
	return ret
}
