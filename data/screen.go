package data // code.bitsetter.de/fun/gosl/data

import (
	"fmt"
	"log"
	"os"
	"sync"

	nc "github.com/rthornton128/goncurses"
)

var (
	win      *nc.Window
	ncquit   = make(chan bool)
	winMutex = &sync.Mutex{}
)

func RenderFrame(f *Frame) {
	winMutex.Lock()
	if win != nil {
		//win.Clear()
		winY, _ := win.MaxYX()
		yOff := (winY - f.H) / 2
		for k, _ := range f.Data {
			win.MovePrint(yOff+k, 0, string(f.Data[k]))
		}
	}
	winMutex.Unlock()
}

func InitNC(killchan chan<- os.Signal) {
	var err error
	winMutex.Lock()
	win, err = nc.Init()
	if err != nil {
		log.Fatal(err)
		os.Exit(1)
	}
	win.Timeout(1)
	winMutex.Unlock()
	go func() {
		select {
		case <-ncquit:
			log.Println("Exiting NC")
			killchan <- os.Kill
		}
	}()
}

func ExitNC() {
	winMutex.Lock()
	if win != nil {
		win = nil
		nc.End()
		ncquit <- true
	}
	winMutex.Unlock()
}

func GetChar() int {
	winMutex.Lock()
	defer winMutex.Unlock()
	if win != nil {
		k := win.GetChar()
		return int(k)
	}
	return 0
}

func GetXY() (int, int) {
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
