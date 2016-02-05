package data // code.bitsetter.de/fun/gosl/data

import (
	"strconv"
)

type Handshake struct {
	Client int
	Name   string
}

func (h Handshake) String() string {
	return (h.Name + ": " + strconv.Itoa(h.Client))
}
