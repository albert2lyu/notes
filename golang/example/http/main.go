package main

import (
    "flag"
    "log"
    "net/http"
    "text/template"
)

var (
	addr = flag.String("addr", ":8080", "http service address") // Q=17, R=18

	templ = template.Must(template.New("qr").Parse(templateStr))
	index = template.Must(template.ParseFiles("index.html"))
)


func main() {
    flag.Parse()
    http.Handle("/", http.HandlerFunc(QR))
    log.Println("Listened On" + *addr)
    err := http.ListenAndServe(*addr, nil)
    if err != nil {
        log.Fatal("ListenAndServe:", err)
    }
}

func QR(w http.ResponseWriter, req *http.Request) {
    //templ.Execute(w, req.FormValue("s"))
    //index.Execute(w, req.FormValue("s"))
    w.Write([]byte("hello, world"))
}

const templateStr = `
<html>
<head>
<title>QR Link Generator</title>
</head>
<body>
{{if .}}
<img src="http://chart.apis.google.com/chart?chs=300x300&cht=qr&choe=UTF-8&chl={{urlquery .}}" />
<br>
{{html .}}
<br>
<br>
{{end}}
<form action="/" name=f method="GET"><input maxLength=1024 size=70
name=s value="" title="Text to QR Encode"><input type=submit
value="Show QR" name=qr>
</form>
</body>
</html>
`
