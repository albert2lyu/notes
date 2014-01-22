<html>
<head>
<meta http-equiv="Content-Type" content="application/xhtml+xml; charset=UTF-8"/>
</head>
<body>
<form method="post" action="/signup">
用户名:(汉字\字母\数字,4-20字符)<br />
<input size="20" name="username" value="{{.CurUserName}}"/><br />
密码:(字母&数字,6-16字符)<br />
<input type="password" size="20" name="password" value="{{.CurPassWord}}" /><br />
验证码:（<span>不区分大小写</span>）<br/><div><img src="{{.CheckCode}}" width="90" height="30"><a href="/register/reset_code">换一张</a></div>
<input name="checkcode" size="10" type="text"></div>
<div>
<input type="submit" value="立即注册" /> <br/>
{{with .Users}} 
{{range .}}  
{{.UserName}} - {{.PassWord}} - {{.RegStatus}}<br/>
{{end}}  
{{end}}  
</div>
</form>
</body>
</html>
