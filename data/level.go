package data

import (
	"encoding/gob"
	"log"
	"os"
	"sort"
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
	DIR_NW                        //  only 4,5,6 supported
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
	V      int           `json:"V-Offset"`
	Repeat bool
	Frames map[int]([][]rune)
}

type Level struct {
	Name   string
	FPS    int
	Layers map[string]*Layer
	lorder []zString // ordered key list (by z index)
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

// that's a func
func (lvl *Level) Height() int {
	max := 0
	for _, l := range lvl.Layers {
		for _, f := range l.Frames {
			if (len(f) + l.V) > max {
				max = len(f) + l.V
			}
		}
	}
	return max
}

func (l *Layer) Width() (max int) {
	max = 0
	for _, frame := range l.Frames {
		for _, row := range frame {
			if len(row) > max {
				max = len(row)
			}
		}
	}
	return
}

// Funzt so nicht: Layer liegen nicht zwingend übereinander
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

// Implement Sorter
type zString struct {
	id string
	z  int
}
type ByZ []zString

func (a ByZ) Len() int           { return len(a) }
func (a ByZ) Swap(i, j int)      { a[i], a[j] = a[j], a[i] }
func (a ByZ) Less(i, j int) bool { return a[i].z < a[j].z }

func (lvl *Level) initOrder() {
	if lvl.lorder == nil {
		lvl.lorder = []zString{}
		for k, v := range lvl.Layers {
			lvl.lorder = append(lvl.lorder, zString{k, v.Z})
		}
		sort.Sort(ByZ(lvl.lorder))
		sort.Reverse(ByZ(lvl.lorder))
	}
}

//testcomment
func (lvl *Level) GetFrame(o, w, maxW, frameNo int) (ret *Frame) {
	lvl.initOrder()
	h := lvl.Height()

	ret = &Frame{
		W: w,
		H: h,
	}

	// generate an array by initializing a slice and …
	var mdata = make([]rune, w*h)
	for y := 0; y < h; y++ {
		// … let the resulting Frame data point into it:
		ret.Data = append(ret.Data, mdata[y*w:(y+1)*w])
	}

	for zli, zl := range lvl.lorder {
		layer := lvl.Layers[zl.id]
		//for _, layer := range lvl.Layers {
		//if layer.Z == 0 {
		for row := 0; row < h; row++ {
			//ret.Data = append(ret.Data, []rune{})

			// which frame of the layer to use
			f := (frameNo % len(layer.Frames)) + 1
			off := 0
			switch layer.D {
			case 4:
				off = (frameNo * layer.S) + o
			case 5:
				off = o
			case 6:
				off = -(frameNo * layer.S) + o
			}

			// max width
			lW := layer.Width()
			if !layer.Repeat {
				lW += maxW
				if layer.D != 0 {
					off += maxW
				}
			}

			for off < 0 {
				off += lW
			}
			off %= lW
			// log.Println(lW, off, o, w, maxW, frameNo)
			if row < len(layer.Frames[f]) {
				r := layer.Frames[f][row][:]
				for col := 0; col < w; col++ {
					ro := (off + col) % lW

					if 0 < ro && ro < len(r) && string(r[ro]) != layer.T {
						ret.Data[row+layer.V][col] = r[ro]
					} else {
						if zli == 0 {
							ret.Data[row+layer.V][col] = rune(' ')
						}
					}
				}
				//for col := 0
				//log.Println(len(layer.Frames[f][row]))
				//ret.Data[row] = append(ret.Data[row], (layer.Frames[f][row][off%w:])...)
			} else {
				if zli == 0 {
					for col := 0; col < w; col++ {
						ret.Data[row][col] = rune(' ')
					}
				}
			}
		}
		//}
	}

	return
}
