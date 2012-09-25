package main

import (
    "net/http"
    "time"
    "fmt"
    )

// Main goroutine
func main() {

    http.HandleFunc("/", hello)
    http.HandleFunc("/test", test)
    fmt.Println("Linsten on: 8080")
    http.ListenAndServe(":8080", nil)
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


