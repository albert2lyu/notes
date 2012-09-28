package main

import (
	"bufio"
	"flag"
	"fmt"
	"io"
	"net"
	"net/http"
	"net/http/httputil"
	"net/url"
	"strconv"
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

	rwc, _ := clientconn.Hijack()
	defer rwc.Close()

	go func(rwc io.ReadWriteCloser) {
		bufRead(rwc)
	}(rwc)

	bufWrite(rwc)

	time.Sleep(5 * time.Second)
}

func bufWrite(rwc io.ReadWriteCloser) {
	bufrw := bufio.NewWriter(rwc)
	count := 0

	if bufrw != nil {
		bufrw.Flush()
	}

	bufrw.WriteRune('\n') // 为什么必须要这句，如果没有client收不到数据

	for {
		count++
		bufrw.WriteString(strconv.Itoa(count))
		bufrw.WriteString(", Client Say: ")
		bufrw.WriteString(time.Now().String())
		bufrw.Flush()
		time.Sleep(1 * time.Second)
	}
}

func bufRead(rwc io.ReadWriteCloser) {
	buf := make([]byte, 100)
	for {
		br := bufio.NewReader(rwc)
		s, err := br.Read(buf)
		if err != nil {
			// fmt.Println("Read Err: " + err.Error())
			time.Sleep(1 * time.Second)
			continue
		}
		fmt.Printf("buf: %s, size: %d\n", string(buf), s)
	}
}

func main() {
	flag.Parse()
	nurl, _ := url.Parse(*u)
	Connect(nurl)
}
