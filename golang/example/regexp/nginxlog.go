package main

import (
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"regexp"
	"strconv"
	"time"
)

var (
	LOG_FILE = "./access.log"
)

const (
	DATE_FORMART = "02/Jan/2006:15:04:05 -0700"
)

type Access struct {
	IP         string
	Identity   string
	UserID     string
	AccessTime int64
	Request    string
	Status     int
	ByteSent   string
	Referer    string
	UserAgent  string
}

func TimeFormart(s string) (time.Time, error) {
	return time.Parse(DATE_FORMART, s)
}

func readAccessLog() {

	datas, err := ioutil.ReadFile(LOG_FILE)
	if err != nil {
		log.Fatalf("Read File Error: %s", err.Error())
		os.Exit(1)
	}

	weatherRegStr := `(?m)^(\d+\.\d+\.\d+\.\d+)\s([\w.-]+)\s([\w.-]+)\s\[([^\[\]]+)\]\s"((?:[^"])+)"\s(\d{3})\s(\d+|-)\s"((?:[^"])+)"\s"((?:[^"])+)"$`
	weatherRegex, _ := regexp.Compile(weatherRegStr)

	pos := weatherRegex.FindAllSubmatch(datas, -1)

    var ac Access
	for i := 0; i < len(pos); i++ {
		ac.IP = string(pos[i][1])
		ac.Identity = string(pos[i][2])
		ac.UserID = string(pos[i][3])
		t, _ := TimeFormart(string(pos[i][4]))
		ac.AccessTime = t.Unix()
		ac.Request = string(pos[i][5])
		ac.Status, _ = strconv.Atoi(string(pos[i][6]))
		ac.ByteSent = string(pos[i][7])
		ac.Referer = string(pos[i][8])
		ac.UserAgent = string(pos[i][9])
		fmt.Printf("%v\n", ac)
	}
	fmt.Println(len(pos))
}

func main() {
	readAccessLog()
}
