package main

import (
	"log"
	"net/http"

	"github.com/codegangsta/martini"
	"github.com/codegangsta/martini-contrib/render"
)

var (
	ListenPort = ":8080"
)

func main() {
	m := martini.Classic()

	m.Use(martini.Static("public"))

	// render html templates from templates directory
	//m.Use(render.Renderer())
	m.Use(render.Renderer(render.Options{
		Directory:  "templates",                        // specify what path to load the templates from
		Extensions: []string{".tpl", ".tmpl", ".html"}, // Specify extensions to load for templates
	}))

	m.Get("/", RegIndex)
	m.Post("/signup", RegSignup)
	m.Get("/config", RegConfig)
	m.Post("/config", RegConfig)

	m.Get("/hello", func(r render.Render, req *http.Request) {
		reqParams := req.URL.Query()
		var str string = ""
		log.Printf("req: %v", reqParams)
		for k, _ := range reqParams {
			str += k + " "
		}
		r.HTML(200, "hello", str)
	})

	m.Get("/name/:name", func(params martini.Params, r render.Render, req *http.Request) {
		reqParams := req.URL.Query()
		log.Printf("params: %v", params)
		log.Printf("req: %v", reqParams)
		r.HTML(200, "hello", params["name"])
	})

	/*
		m.Get("/hello/**", func(params martini.Params, r render.Render) {
			r.HTML(200, "hello", params["_1"])
		})
	*/

	//m.Run()
	log.Println("Listen on", ListenPort)
	http.ListenAndServe(ListenPort, m)
}
