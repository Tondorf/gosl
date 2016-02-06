package data // code.bitsetter.de/fun/gosl/data

type Layer struct {
	data []byte
	w, h int
}

type Level struct {
	Layers map[int]*Layer
}

func (lvl *Level) AddLayer(z int, l *Layer) {
	lvl.Layers[z] = l
}

func LoadLevel(filename string) Level
