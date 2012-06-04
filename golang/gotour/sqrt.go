package main

import (
	"fmt"
	"math"
)

func Sqrt(x float64) float64 {
	z := x
	for i := 0; i < 10; i++ {
		z = (z + x/z) / 2
	}
	return z
}

func main() {
	for i := 1; i < 10; i++ {
		fmt.Println(i, Sqrt(float64(i)), math.Sqrt(float64(i)))
	}
}
