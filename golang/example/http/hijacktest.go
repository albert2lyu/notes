package main

import (
	"flag"
	"fmt"
	"net"
	"net/http"
	"net/http/httputil"
	"net/url"
	"time"
)

var (
	nurl *url.URL
	u    = flag.String("url", "http://127.0.0.1:8080/hijack", "Connection URL")
	c    = flag.Int("c", 10000, "Connection Count")
)

func Connect() {
	req := http.Request{
		Method: "GET",
		Header: http.Header{},
		URL:    nurl,
	}
	for {
		dial, err := net.Dial("tcp", nurl.Host)
		if err != nil {
			fmt.Println(err)
			return
		}
		clientconn := httputil.NewClientConn(dial, nil)
		clientconn.Do(&req)

		netconn, r := clientconn.Hijack()
		num := r.Buffered()
		fmt.Println(num)
		/*
				s, err := r.ReadString('\n')
				if err != nil {
					fmt.Println("read error: " + err.Error())
				}
		        fmt.Println("Hijack Reader: " + s)
		*/
		netconn.Close()
		clientconn.Close()
	}
}

func main() {
	flag.Parse()
	nurl, _ = url.Parse(*u)
	//for i := 0; i < *c; i++ {
	go Connect()
	//}
	time.Sleep(1e10)
}
