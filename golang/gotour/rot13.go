package main

import (
	"io"
	"os"
	"strings"
)

type rot13Reader struct {
	r io.Reader
}

/*
ROT13: http://zh.wikipedia.org/wiki/ROT13
*/
func rot13(b byte) byte {
	switch {
	case 'A' <= b && b <= 'M':
		b = (b - 'A') + 'N'
	case 'N' <= b && b <= 'Z':
		b = (b - 'N') + 'A'
	case 'a' <= b && b <= 'm':
		b = (b - 'a') + 'n'
	case 'n' <= b && b <= 'z':
		b = (b - 'n') + 'a'
	}
	return b
}

func (b *rot13Reader) Read(p []byte) (n int, err error) {
	n, err = b.r.Read(p)
	for i := range p[:n] {
		p[i] = rot13(p[i])
	}
	return
}

func main() {
	s := strings.NewReader(
		"Lbh penpxrq gur pbqr!\n")
	r := rot13Reader{s}
	io.Copy(os.Stdout, &r)
}
