package data // code.bitsetter.de/fun/gosl/data

import (
	"fmt"
	"log"
	"os"

	"github.com/rthornton128/goncurses"
)

type Screen struct {
	w, h int
	data []rune
}

func TestNC() (int, int) {
	win, err := goncurses.Init()
	if err != nil {
		log.Fatal(err)
		os.Exit(1)
	}
	win.Clear()
	h, w := win.MaxYX()
	goncurses.End()
	fmt.Printf("screen: %d x %d\n", w, h)
	return w, h
}
