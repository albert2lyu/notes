package main

import (
	"log"
	"net/http"
	"time"
)

func main() {
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
	rwc, bufrw, err := hj.Hijack()
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	// Don't forget to close the connection:
	defer rwc.Close()
	if bufrw != nil {
		bufrw.Flush()
	}
	bufrw.WriteRune('\n')
	bufrw.WriteString("Now we're speaking raw TCP. Say hi: ")
	bufrw.WriteString(time.Now().String())
	bufrw.Flush()
	//time.Sleep(10 * time.Second)
}

func hello(w http.ResponseWriter, r *http.Request) {
	w.Write([]byte("hello, world\n"))
	w.Write([]byte(time.Now().String()))
}
