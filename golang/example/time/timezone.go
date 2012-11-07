package main

import (
	"fmt"
	"time"
)

func show(s string) {
	t, err := time.Parse("02/Jan/2006:15:04:05 +0800", s)
	if err != nil {
		fmt.Printf("Parse(%q): %v\n", s, err)
		return
	}
	fmt.Printf("Parse(%q) = %v\n", s, t)
	fmt.Println(t.Unix())
}

func main() {
	show("09/Aug/2012:23:56:43 +0800")
	lc, err := time.LoadLocation("Asia/Chongqing")
	if err != nil {
		fmt.Println("error: " + err.Error())
	}
	t := time.Date(2012, 8, 9, 23, 56, 43, 0, lc)
	fmt.Printf("Go launched at %v\n", t.Unix())
}
