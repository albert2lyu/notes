package main

import (
	"flag"
	"fmt"
	"net"
	"strconv"
	"sync"
)

var (
	c = flag.Int("count", 30000, "Connection Count")
)

func main() {
	var wg sync.WaitGroup
	for i := 0; i < *c; i++ {
		wg.Add(1)
		go func(wg *sync.WaitGroup) {
			client(wg)
		}(&wg)
	}
	wg.Wait()
	fmt.Println(strconv.Itoa(*c) + " Done!")
}

func client(wg *sync.WaitGroup) {
	defer wg.Done()
	dial, err := net.Dial("tcp", "127.0.0.1:50000")
	if err != nil {
		fmt.Println(err)
		return
	}
	defer dial.Close()
	dial.Write([]byte("Hello, world\n"))
}
