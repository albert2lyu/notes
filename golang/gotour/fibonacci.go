package main

import "fmt"

func fibonacci() func() int {
	var count, sum1, sum2 int = 0, 1, 1
	return func() int {
		// fib(n) = fib(n-1) + fib(n-2)
		if count == 0 || count == 1 {
			count++
		} else {
			sum1, sum2 = sum2, sum1+sum2
		}
		return sum2
	}
}

func main() {
	f := fibonacci()
	for i := 0; i < 10; i++ {
		fmt.Println(f())
	}
}
