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

const ClientOffset = 0

type goslClient struct {
	con *net.TCPConn
	id  int
	w   int
	h   int
	off int
}

/* GLOBAL SERVER STATE */
var (
	LevelFile  string
	level      *data.Level
	ServerPort int
	TotalWidth int                 = 0
	clients    map[int]*goslClient = make(map[int]*goslClient)
	clientKeys []int               = make([]int, 0)
	// big server canvas
	//canvas       [][]rune
	canvasX      int
	frameCounter int = 0
)

func handleConn(conn *net.TCPConn) {
	var hs data.Handshake
	log.Println("Got a connection!")
	dec := gob.NewDecoder(conn)
	dec.Decode(&hs) // decode handshake
	log.Println("Got client! ID:", hs.ID, "dimensions:", hs.W, hs.H)
	// memorize client in server state
	clientKeys = append(clientKeys, hs.ID)
	sort.Ints(clientKeys)
	clients[hs.ID] = &goslClient{con: conn, id: hs.ID, w: hs.W, h: hs.H}
	TotalWidth += hs.W
	// reset server
	resetServer()
}

func resetServer() {
	// adjust canvas width
	canvasX = -1 * ClientOffset
	for _, k := range clientKeys {
		_, client := k, clients[k]
		canvasX += client.w + ClientOffset
	}
	//canvasX += level.Width()
	// adjust client offsets
	off := 0
	for _, k := range clientKeys {
		clients[k].off = off
		off += clients[k].w
		off += ClientOffset
	}
}

func serveClients() {
	level = data.LoadLevel(LevelFile)
	frameCounter = 0
	for { // while true
		if len(clients) > 0 { // pause if no clients are connected
			for _, k := range clientKeys {
				id, client := k, clients[k]
				if id > 0 {
					oFrame := level.GetFrame(client.off, client.w, canvasX, frameCounter)
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
						resetServer()
					}
					//log.Println("ID:", id, "Client:", client)

				}
			}
			frameCounter++
			time.Sleep(time.Second / time.Duration(level.FPS))
		}
	}
}

func runServer(cmd *cobra.Command, args []string) {
	log.Println("Running server on port", ServerPort)
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
