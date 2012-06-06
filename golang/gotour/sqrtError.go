package main

import (
	"fmt"
)

type ErrNegativeSqrt float64

func (e ErrNegativeSqrt) Error() string {
	return fmt.Sprintf("cannot Sqrt negative number: %v", float64(e))
}

func Sqrt(f float64) (float64, error) {
	if f < 0 {
		return 0, ErrNegativeSqrt(f)
	}
	z := f
	for i := 0; i < 10; i++ {
		z = (z + f/z) / 2
	}

	return 0, nil
}

func main() {
	if value, err := Sqrt(-2); err != nil {
		fmt.Println(err)
	} else {
		fmt.Println(value)
	}
}
