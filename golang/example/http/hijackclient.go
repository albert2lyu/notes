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
	u = flag.String("url", "http://127.0.0.1:8080/hijack", "Connection URL")
)

func Connect(nurl *url.URL) {
	req := http.Request{
		Method: "GET",
		Header: http.Header{},
		URL:    nurl,
	}
	dial, err := net.Dial("tcp", nurl.Host)
	if err != nil {
		fmt.Println(err)
		return
	}
	clientconn := httputil.NewClientConn(dial, nil)
	clientconn.Do(&req)
	defer clientconn.Close()

	rwc, br := clientconn.Hijack()
	defer rwc.Close()

	for {
		buf := make([]byte, 100)
		s, err := br.Read(buf)
		if err != nil {
			fmt.Println("Read Err: " + err.Error())
		}
		fmt.Printf("size: %d, buf: %s\n", s, string(buf))
		time.Sleep(1 * time.Second)
		buf = nil
	}
	time.Sleep(5 * time.Second)
}

func main() {
	flag.Parse()
	nurl, _ := url.Parse(*u)
	Connect(nurl)
}
