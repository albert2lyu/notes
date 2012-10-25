package main

import (
	"bufio"
	"fmt"
	"io"
	"log"
	"net/http"
	"runtime"
	"strconv"
	"time"
)

func main() {
	runtime.GOMAXPROCS(runtime.NumCPU()*2 - 1)
	http.HandleFunc("/", hello)
	http.HandleFunc("/hijack", hijack)
	log.Println("Linsten on: 8080")
	http.ListenAndServe(":8080", nil)
}

func hijack(w http.ResponseWriter, r *http.Request) {
	hj, ok := w.(http.Hijacker)
	if !ok {
		http.Error(w, "webserver doesn't support hijacking", http.StatusInternalServerError)
		return
	}
	rwc, _, err := hj.Hijack()
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}

	defer rwc.Close()

	/*
		go func(rwc io.ReadWriteCloser) {
			bufRead(rwc)
		}(rwc)

		bufWrite(rwc)
	*/

	count := 0
	for {
		count++
		fmt.Println(count)
		time.Sleep(10 * time.Second)
	}

	time.Sleep(10 * time.Second)
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
		bufrw.WriteString(", Server Say: ")
		bufrw.WriteString(time.Now().String())
		bufrw.Flush()
		time.Sleep(1 * time.Second)
	}
}

func bufRead(rwc io.ReadWriteCloser) {
	br := bufio.NewReader(rwc)
	buf := make([]byte, 100)
	for {
		s, err := br.Read(buf)
		if err != nil {
			// fmt.Println("Read Err: " + err.Error())
			time.Sleep(1 * time.Second)
			continue
		}
		fmt.Printf("buf: %s, size: %d\n", string(buf), s)
	}
}

func hello(w http.ResponseWriter, r *http.Request) {
	w.Write([]byte("hello, world\n"))
	w.Write([]byte(time.Now().String()))
}
