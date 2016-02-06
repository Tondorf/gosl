package main // code.bitsetter.de/fun/gosl

import (
	"code.bitsetter.de/fun/gosl/cmd"
	"runtime"
)

func main() {
	// Max out processor usage:
	// since go 1.5 it's the default to use all available cores,
	// let's stay compatible to go <= 1.4
	runtime.GOMAXPROCS(runtime.NumCPU())

	// let each cmd handle itself
	cmd.Execute()
}
