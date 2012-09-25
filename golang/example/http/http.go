package main

import (
	"fmt"
	"log"
	"net/http"
	"time"
)

// Main goroutine
func main() {

	http.HandleFunc("/", hello)
	http.HandleFunc("/test", test)
	http.HandleFunc("/hijack", hijack)
	fmt.Println("Linsten on: 8080")
	http.ListenAndServe(":8080", nil)
}

func hijack(w http.ResponseWriter, r *http.Request) {
	hj, ok := w.(http.Hijacker)
	if !ok {
		http.Error(w, "webserver doesn't support hijacking", http.StatusInternalServerError)
		return
	}
	conn, bufrw, err := hj.Hijack()
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	// Don't forget to close the connection:
	defer conn.Close()
	bufrw.WriteString("Now we're speaking raw TCP. Say hi: \n")
	bufrw.Flush()
	///*
	s, err := bufrw.ReadString('\n')
	if err != nil {
		log.Printf("error reading string: %v", err)
		return
	}
	fmt.Fprintf(bufrw, "You said: %q\nBye.\n", s)
	bufrw.Flush()
	//*/
}

func hello(w http.ResponseWriter, r *http.Request) {
	w.Write([]byte("hello, world\n"))
	w.Write([]byte(time.Now().String()))
}

func test(w http.ResponseWriter, r *http.Request) {
	w.Write([]byte("test1"))
	d, _ := time.ParseDuration("30s")
	time.Sleep(d)
	w.Write([]byte("test2\n"))
	w.Write([]byte(time.Now().String()))
}
