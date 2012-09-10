package main

import (
	"fmt"
	"runtime"
	"sync"
	"time"
)

const MAX = 100

func watching() {
	fmt.Printf("NumGoroutine: %d\n", runtime.NumGoroutine())
}

func doSomething(url int, wg *sync.WaitGroup) {
	// Fetch URL
	// TODO something...
	time.Sleep(100 * time.Microsecond)
	wg.Done()
}

func main() {
	runtime.GOMAXPROCS(runtime.NumCPU())
	urls := make(chan int, MAX)
	total := 10000000
	count := 0
	var wg sync.WaitGroup

	// Get urls
	go func() {
		for i := 0; i < total; i++ {
			urls <- i
		}
	}()

	fmt.Printf("Looking for fetch\n")
	t := time.Tick(1 * time.Second)
	for {
		select {
		case url := <-urls:
			wg.Add(1)
			go func(u int, wg *sync.WaitGroup) {
				doSomething(u, wg)
			}(url, &wg)
			count++
		case <-t:
			watching()
		}

		if count%MAX == 0 {
			wg.Wait()
			//fmt.Printf("N: %d ", count/MAX)
		}
		if count == total {
			break
		}
	}

	fmt.Printf("%d Done\n", count)
}
