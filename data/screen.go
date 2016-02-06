package data // code.bitsetter.de/fun/gosl/data

import (
	"fmt"
	"log"
	"os"

	nc "github.com/rthornton128/goncurses"
)

var (
	win    *nc.Window
	ncquit = make(chan bool)
)

func RenderFrame(f *Frame) {
	win.Clear()
	for k, _ := range f.Data {
		win.MovePrint(k+1, 1, string(f.Data[k]))
	}
}

func InitNC(killchan chan<- os.Signal) {
	var err error
	win, err = nc.Init()
	if err != nil {
		log.Fatal(err)
		os.Exit(1)
	}
	win.Timeout(1)
	go func() {
		select {
		case <-ncquit:
			log.Println("Exiting NC")
			killchan <- os.Kill
		}
	}()
}

func ExitNC() {
	//if win != nil {
	nc.End()
	ncquit <- true
	//	}
}

func GetChar() int {
	if win != nil {
		k := win.GetChar()
		return int(k)
	}
	return 0
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
