package main

import (
	"fmt"
	"runtime"
	"strconv"
	"sync"
	"time"
)

type Msg struct {
	mutex sync.Mutex
	count map[int64]string
}

func (m *Msg) InitMsg(key int64, value string) {
	m.mutex.Lock()
	defer m.mutex.Unlock()
	m.count[key] = value
}

func (m *Msg) Get(key int64, c chan bool) (str string) {
	defer func() { <-c }()
	m.mutex.Lock()
	if _, ok := m.count[key]; !ok {
		m.mutex.Unlock()
		m.InitMsg(key, strconv.Itoa(int(key)))
	} else {
		m.mutex.Unlock()
	}
	m.mutex.Lock()
	defer m.mutex.Unlock()
	if str, ok := m.count[key]; ok {
		return str
	}
	return
}

func main() {
	runtime.GOMAXPROCS(runtime.NumCPU() * 2)
	msg := &Msg{count: map[int64]string{}}
	var i int64
	c := make(chan bool, 2000)
	t1 := time.Now().UnixNano()
	for i = 0; i <= 1024000; i++ {
		msg.InitMsg(i, strconv.Itoa(int(i)))
	}
	for i = 0; i <= 2048000; i++ {
		c <- true
		go msg.Get(i, c)
	}
	t2 := time.Now().UnixNano()
	fmt.Println(t2 - t1)
}
