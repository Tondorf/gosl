package data // code.bitsetter.de/fun/gosl/data

import (
	"strconv"
)

type Handshake struct {
	ID int
	//	Name   string
	W int
	H int
}

func (h Handshake) String() string {
	return (strconv.Itoa(h.ID) + ": " + strconv.Itoa(h.W) + "×" + strconv.Itoa(h.H))
}
