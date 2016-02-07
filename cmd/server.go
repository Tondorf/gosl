package cmd // code.bitsetter.de/fun/gosl/cmd

import (
	"encoding/gob"
	"log"
	"net"
	"sort"
	"time"

	"github.com/spf13/cobra"

	"code.bitsetter.de/fun/gosl/data"
)

var cmdServer = &cobra.Command{
	Use:   "server",
	Short: "Runs Gosl as a server",
	Long: `Runs Gosl as a server

[TODO:]
gosl.json for configuration (Port/Address, Level)
`,
	//	Run:
}

type goslClient struct {
	con *net.TCPConn
	id  int
	w   int
	h   int
}

/* GLOBAL SERVER STATE */
var (
	LevelFile  string
	level      *data.Level
	ServerPort int
	TotalWidth int                = 0
	clients    map[int]goslClient = make(map[int]goslClient)
	clientKeys []int              = make([]int, 100)
)

func handleConn(conn *net.TCPConn) {
	var hs data.Handshake
	log.Println("Got a connection!")
	dec := gob.NewDecoder(conn)
	dec.Decode(&hs) // decode handshake
	log.Println("Got client! ID:", hs.ID, "dimensions:", hs.W, hs.H)
	clientKeys = append(clientKeys, hs.ID)
	sort.Ints(clientKeys)
	clients[hs.ID] = goslClient{con: conn, id: hs.ID, w: hs.W, h: hs.H}
	TotalWidth += hs.W
	// conn.Close()
}

func serveClients() {
	level = data.LoadLevel(LevelFile)
	log.Println("canvas X:", canvasX())
	fCounter := 0
	for { // while true
		for _, k := range clientKeys {
			id, client := k, clients[k]
			if id > 0 {
				oFrame := level.GetFrame(0, client.w, fCounter)
				enc := gob.NewEncoder(client.con)
				err := enc.Encode(oFrame)
				if err != nil {
					// client disconnected
					//log.Println("BEFORE remove: clients:", clients, " clientKeys:", clientKeys)
					// delete client
					delete(clients, client.id)
					// delete client key
					// ugly as fuck in go to remove from a slice
					// it *should* work though
					idInKeys := sort.SearchInts(clientKeys, client.id)
					clientKeys = append(clientKeys[:idInKeys], clientKeys[idInKeys+1:]...)
					//log.Println("AFTER remove: clients:", clients, " clientKeys:", clientKeys)
				}
				//log.Println("ID:", id, "Client:", client)
			}
		}
		fCounter++
		time.Sleep(time.Second / time.Duration(level.FPS))
	}
}

func runServer(cmd *cobra.Command, args []string) {
	log.Println("running server on port", ServerPort)
	listener, err := net.ListenTCP("tcp", &net.TCPAddr{Port: ServerPort})
	if err != nil {
		log.Fatal(err)
		panic("Could not open Listener")
	}
	defer listener.Close()
	go serveClients()
	for {
		conn, err := listener.AcceptTCP()
		if err != nil {
			log.Fatal(err)
			panic("Listener could not accept connection!")
		}
		go handleConn(conn)
	}
}

func init() {
	CmdGosl.AddCommand(cmdServer)
	cmdServer.Run = runServer

	cmdServer.Flags().StringVarP(&LevelFile, "level", "l", "default.lvl", "Use specific levelfile")
	cmdServer.Flags().IntVarP(&ServerPort, "port", "p", 8090, "Run server on this port")
}

func canvasX() (x int) {
	x = 0
	for _, k := range clientKeys {
		_, client := k, clients[k]
		x += client.w
	}
	x += level.Width()
	return
}
