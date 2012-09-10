package main

import (
	"log"
	"net/http"
	"net/http/httputil"
	"net/url"
)

func main() {
	u, err := url.Parse("http://www.golang.com/")
	if err != nil {
		log.Fatal(err)
	}

	reverse_proxy := httputil.NewSingleHostReverseProxy(u)
	http.Handle("/", reverse_proxy)
	log.Println("Listen On: 9000")
	if err = http.ListenAndServe(":9000", nil); err != nil {
		log.Fatal(err)
	}
	log.Println("Server started")
}
