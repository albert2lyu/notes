package main

import (
	"flag"
	"fmt"
	"io/ioutil"
	"net"
	"net/http"
	"net/http/httputil"
	"net/url"
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

	body, err := ioutil.ReadAll(br)
	fmt.Println(string(body))
}

func main() {
	flag.Parse()
	nurl, _ := url.Parse(*u)
	Connect(nurl)
}
