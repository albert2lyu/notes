package main

import (
	"errors"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"net/http"
	"net/http/cookiejar"
	"net/url"
	"os"
	"regexp"
	"strings"

	"github.com/codegangsta/martini-contrib/render"
)

var gCurCookies []*http.Cookie
var gCurCookieJar *cookiejar.Jar

var (
	registerUrl  = "http://wap.ganji.com/bj/user/register/"
	checkcodeUrl = "http://wap.ganji.com"
	//proxyAddr    = "http://127.0.0.1:8888/"
	proxyAddr = ""
	signupUrl = "http://wap.ganji.com/bj/user/register/?ifid=ganji_v2_quickreg_btn"
)

const (
	StatusContinue = 0
	StatusSuccess  = 1
	StatusError    = 2
)

type UserInfo struct {
	UserName  string
	PassWord  string
	RegStatus int // 0:待注册 1:注册成功 2: 注册失败
}

type SignupUser struct {
	CurUserName string
	CurPassWord string
	CheckCode   string
	Users       []*UserInfo
}

var gUser SignupUser

//do init before all others
func init() {
	gCurCookies = nil
	gUser = SignupUser{}
	//var err error;
	gCurCookieJar, _ = cookiejar.New(nil)
	/*
		for i := 0; i < 10; i++ {
			user := &UserInfo{fmt.Sprintf("bjershouche200%d", i), "beijingershouche", StatusContinue}
			gUser.Users = append(gUser.Users, user)
		}
	*/
}

// get url response html
func getUrl(url string, v url.Values) (int, []byte) {
	return urlReqHtml("GET", url, strings.NewReader(v.Encode()))
}

// post url response html
func postUrl(url string, v url.Values) (int, []byte) {
	return urlReqHtml("POST", url, strings.NewReader(v.Encode()))
}

// 指定代理ip
func getTransportFieldURL(proxy_addr *string) (transport *http.Transport) {
	url_i := url.URL{}
	url_proxy, _ := url_i.Parse(*proxy_addr)
	transport = &http.Transport{Proxy: http.ProxyURL(url_proxy)}
	return
}

// 从环境变量$http_proxy或$HTTP_PROXY中获取HTTP代理地址
func getTransportFromEnvironment() (transport *http.Transport) {
	transport = &http.Transport{Proxy: http.ProxyFromEnvironment}
	return
}

func myRedirect(req *http.Request, via []*http.Request) error {
	return errors.New("Redirected!")
}

// get url response html
// method: GET, POST, PUT, etc.
// int http.status
func urlReqHtml(method, url string, value io.Reader) (int, []byte) {
	var transport *http.Transport = nil
	var httpClient *http.Client = nil
	if proxyAddr != "" {
		transport = getTransportFieldURL(&proxyAddr)
	}

	if transport == nil {
		httpClient = &http.Client{
			CheckRedirect: myRedirect,
			Jar:           gCurCookieJar,
		}
	} else {
		httpClient = &http.Client{
			CheckRedirect: myRedirect,
			Jar:           gCurCookieJar,
			Transport:     transport,
		}
	}

	httpReq, err := http.NewRequest(method, url, value)
	httpReq.Header.Add("Connection", "Keep-Alive")
	if method == "POST" {
		httpReq.Header.Add("Content-Type", "application/x-www-form-urlencoded")
	}
	httpReq.Header.Add("User-Agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/31.0.1650.63 Safari/537.36")
	httpResp, err := httpClient.Do(httpReq)

	if err != nil {
		log.Printf("http get url=%s response error=%s\n", url, err.Error())
	}

	defer httpResp.Body.Close()
	log.Println("httpResp status:", httpResp.StatusCode, url, method)

	body, errReadAll := ioutil.ReadAll(httpResp.Body)
	if errReadAll != nil {
		log.Printf("get response for url=%s got error=%s\n", url, errReadAll.Error())
	}

	gCurCookies = gCurCookieJar.Cookies(httpReq.URL)

	return httpResp.StatusCode, body
}

func dbgPrintCurCookies() {
	var cookieNum int = len(gCurCookies)
	log.Printf("cookieNum=%d", cookieNum)
	for i := 0; i < cookieNum; i++ {
		var curCk *http.Cookie = gCurCookies[i]
		//log.Printf("curCk.Raw=%s", curCk.Raw)
		log.Printf("Cookie [%d]", i)
		log.Printf("Name\t=%s", curCk.Name)
		log.Printf("Value\t=%s", curCk.Value)
		log.Printf("Path\t=%s", curCk.Path)
		log.Printf("Domain\t=%s", curCk.Domain)
		log.Printf("Expires\t=%s", curCk.Expires)
		log.Printf("RawExpires=%s", curCk.RawExpires)
		log.Printf("MaxAge\t=%d", curCk.MaxAge)
		log.Printf("Secure\t=%t", curCk.Secure)
		log.Printf("HttpOnly=%t", curCk.HttpOnly)
		log.Printf("Raw\t=%s", curCk.Raw)
		log.Printf("Unparsed=%s", curCk.Unparsed)
	}
}

// Get Ganji Register Info
// http://wap.ganji.com/bj/user/register/
func getRegisterUrl() string {
	log.Printf("==== RegisterUrl=%s ====", registerUrl)
	_, respHtml := getUrl(registerUrl, nil)
	// log.Printf("respHtml=%s", respHtml)
	return checkcodeUrl + getCheckCode(respHtml)
}

func getCheckCode(html []byte) string {
	beginRegStr := `(?Usmi)\<p\ class=\"gj-checkcode\"\>.*src=\"(.*)\".*\<\/div\>`
	beginRegex, _ := regexp.Compile(beginRegStr)
	pos := beginRegex.FindAllSubmatch(html, 512)
	if pos == nil {
		fmt.Println("Nothing matched!")
		return ""
	}
	/*
		for i := 0; i < len(pos); i++ {
			fmt.Println(i, string(pos[i][1]))
		}
	*/
	return string(pos[0][1])
}

// Save The Checkcode Image
func saveCheckImg(url, filename string) {
	_, respHtml := getUrl(url, nil)
	//ioutil.WriteFile("public/images/checkcode.png", respHtml, 0x777)
	fd, _ := os.OpenFile("public/images/"+filename, os.O_RDWR|os.O_CREATE|os.O_TRUNC, 0644)
	defer fd.Close()
	fd.Write(respHtml)
}

// Signup User
func signup(user, password, checkcode string) bool {
	v := url.Values{}
	v.Set("username", user)
	v.Add("password", password)
	v.Add("apassword", password)
	v.Add("checkcode", checkcode)
	v.Add("doPost", "1")
	v.Add("backUrl", "")
	v.Add("ifid", "ganji_v2_reg_btn")
	status, resp := postUrl(signupUrl, v)
	if status == http.StatusFound || status == http.StatusMovedPermanently {
		return true
	} else {
		log.Println(string(resp))
		return false
	}
}

func RegIndex(r render.Render) {
	checkUrl := getRegisterUrl()
	fmt.Printf("checkUrl: %s\n", checkUrl)
	index := strings.Index(checkUrl, "nocach=") + 7
	filename := checkUrl[index:index+10] + ".png"
	saveCheckImg(checkUrl, filename)
	log.Println("checkUrl filename", checkUrl, filename)
	gUser.CheckCode = "/images/" + filename
	for i, user := range gUser.Users {
		log.Printf("%d-%v\n", i, user)
		if user.RegStatus != StatusSuccess {
			gUser.CurUserName = user.UserName
			gUser.CurPassWord = user.PassWord
			break
		}
	}
	r.HTML(200, "index", gUser)
}

func saveUserInfo(user *UserInfo) {
	//ioutil.WriteFile("public/images/checkcode.png", respHtml, 0x777)
	fd, _ := os.OpenFile("userinfo.txt", os.O_RDWR|os.O_CREATE|os.O_APPEND, 0644)
	defer fd.Close()
	fd.Write([]byte(fmt.Sprintf("%s,%s,%d\n", strings.TrimSpace(user.UserName), strings.TrimSpace(user.PassWord), user.RegStatus)))
}

func RegSignup(w http.ResponseWriter, r *http.Request) {
	r.ParseForm()
	ret := signup(r.FormValue("username"), r.FormValue("password"), r.FormValue("checkcode"))
	if ret {
		for _, user := range gUser.Users {
			if user.UserName == gUser.CurUserName {
				user.RegStatus = StatusSuccess
				saveUserInfo(user)
				break
			}
		}
	} else {
		log.Println("Register Error")
	}
	http.Redirect(w, r, "/", http.StatusFound)
}

func RegConfig(render render.Render, w http.ResponseWriter, r *http.Request) {
	log.Println("RegConfig", r.Method)
	if r.Method == "GET" {
		render.HTML(200, "config", "")
	} else {
		gUser = SignupUser{}
		// POST
		r.ParseForm()
		names := strings.Split(r.FormValue("username"), "\n")
		for _, name := range names {
			//log.Printf("names: %d-%s\n", i, name)
			user := &UserInfo{name, r.FormValue("password"), StatusContinue}
			gUser.Users = append(gUser.Users, user)
		}
		http.Redirect(w, r, "/", http.StatusFound)
	}

}

/*
func main() {
	//step1: Get Checkcode URL
	checkUrl := getRegisterUrl()
	fmt.Println(checkUrl)
	saveCheckImg(checkUrl)

	// signup user
	signup("bjershouchekv01", "bjershouche", "jvsj")
}
*/
