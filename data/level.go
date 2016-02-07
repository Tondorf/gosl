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
	log.Println("Loading lvl", filename)
	ret := &Level{}
	file, err := os.Open(filename)
	if err != nil {
		log.Println("Error reading lvl:", err)
		return nil
	}
	dec := gob.NewDecoder(file)
	dec.Decode(ret)
	//log.Println(ret.Layers["locomotive"])
	return ret
}

func (lvl *Level) Height() int {
	max := 0
	for _, l := range lvl.Layers {
		for _, f := range l.Frames {
			if len(f) > max {
				max = len(f)
			}
		}
	}
	return max
}

func (lvl *Level) Width() (max int) {
	for _, lay := range lvl.Layers {
		for _, fra := range lay.Frames {
			for _, fra2 := range fra {
				if len(fra2) > max {
					max = len(fra2)
				}
			}
		}
	}
	return
}

func (lvl *Level) GetFrame(off, w, frameNo int) (ret *Frame) {
	h := lvl.Height()

	ret = &Frame{
		W: w,
		H: h,
	}

	for _, layer := range lvl.Layers {
		if layer.Z == 0 {
			for row := 0; row < h; row++ {
				ret.Data = append(ret.Data, []rune{})
				f := (frameNo % len(layer.Frames)) + 1
				if row <= len(layer.Frames[f]) {
					//log.Println(len(layer.Frames[f][row]))
					ret.Data[row] = append(ret.Data[row], (layer.Frames[f][row][off:])...)
				}
			}
		}
	}

	return
}
