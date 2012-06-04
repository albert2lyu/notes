package main

import (
	"bitbucket.org/mikespook/go-tour-zh/wc"
	"strings"
)

func WordCount(s string) map[string]int {
	m := make(map[string]int)
	strArr := strings.Fields(s)
	for _, word := range strArr {
		_, ok := m[word]
		if ok {
			m[word] += 1
		} else {
			m[word] = 1
		}
	}
	return m
}

func main() {
	wc.Test(WordCount)
}
