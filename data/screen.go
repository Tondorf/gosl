package data // code.bitsetter.de/fun/gosl/data

import (
	"fmt"
	"log"
	"os"

	nc "github.com/rthornton128/goncurses"
)

var (
	win *nc.Window
)

func RenderFrame(f *Frame) {
	win.Clear()
	for k, _ := range f.Data {
		win.MovePrint(k+1, 1, string(f.Data[k]))
	}
}

func InitNC() {
	var err error
	win, err = nc.Init()
	if err != nil {
		log.Fatal(err)
		os.Exit(1)
	}
}

func ExitNC() {
	//if win != nil {

	//	}
	nc.End()
}

func TestNC() (int, int) {
	win, err := nc.Init()
	if err != nil {
		log.Fatal(err)
		os.Exit(1)
	}
	win.Clear()
	h, w := win.MaxYX()
	nc.End()
	fmt.Printf("screen: %d x %d\n", w, h)

	return w, h
}
