package data // code.bitsetter.de/fun/gosl/data

import (
	"fmt"
	"log"
	"os"

	nc "github.com/rthornton128/goncurses"
)

type Screen struct {
	w, h int
	data []rune
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
