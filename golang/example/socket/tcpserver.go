package main

import (
	"fmt"
	"net"
	"runtime"
	"time"
)

var (
	count = 0
)

func main() {
	runtime.GOMAXPROCS(runtime.NumCPU()*2 - 1)
	fmt.Println("Starting the server :50000 ...")
	// create listener:
	listener, err := net.Listen("tcp", ":50000")
	if err != nil {
		fmt.Println("Error listening", err.Error())
		return // terminate program
	}
	go countfun()
	// listen and accept connections from clients:
	for {
		conn, err := listener.Accept()
		count++

		if err != nil {
			fmt.Println("Error accepting", err.Error())
			return // terminate program
		}
		go func(conn net.Conn) {
			doServerStuff(conn)
		}(conn)
	}
}

func countfun() {
	t := time.Tick(1 * time.Second)
	for {
		select {
		case <-t:
			fmt.Println(count)
		}
	}
}

func doServerStuff(conn net.Conn) {
	defer conn.Close()
	buf := make([]byte, 1024)
	for {
		s, err := conn.Read(buf)
		if s == 0 {
			return
		}
		if err != nil {
			fmt.Println("Error reading", err.Error())
			return // terminate program
		}
		//fmt.Printf("Received data: %v", string(buf))
	}
	buf = nil
}
