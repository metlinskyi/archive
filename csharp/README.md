![alt text][curl-logo] curl
============
[curl-logo]: https://raw.github.com/metlinskyi/www.metlinskyi.com/master/csharp/curl.png "Curl.net it's easy for HTTP requests"

###### Usage
Curl is a sexy library for easy HTTP requests
```C#
	Curl
		.Post("http://metlinskyi.com/api/")
		.Proxy("127.0.0.1", 8888)
		.Data("grant_type", "client_credentials")
		.Data("scope", "basic user")
		.Auth("username","password")
		.Responce()
		.Content();
```
[read more](https://github.com/metlinskyi/www.metlinskyi.com/tree/master/csharp/curl)

![alt text][doctor-parse-logo] doctor-parse 
============
[doctor-parse-logo]: https://raw.github.com/metlinskyi/www.metlinskyi.com/master/csharp/doctor-parser.png "Dr.Parse is batch processing of text data"

Dr. Parse is a solution for easy management of the process of parsing.

###### Usage
Make YAML instruction for processor parsing.
``` Yaml
Settings:
 Pacients: Directory
  - Source: <your source folder>
  - Destination: <your destination folder>
  - Subfolders: True
  - Extension: .html
 MultiThread: Off
  - MinThreads: 8
 DataBase: MSSQLServer
  - Connection: <your connection string>
Operations:
 remove:
  - options: IgnoreCase
  - regex: <meta(.*?)> 
 remove:
  - options: IgnoreCase|Singleline
  - regex: <script(.*?)</script>
 remove:
  - options: IgnoreCase | Singleline 
  - regex: <style(.*?)</style>
 remove:
  - regex: <span(.*?)>
 remove:
  - regex: </span>
 remove:
  - regex: <font(.*?)>
 remove:
  - regex: </font>
 remove:
  - regex: class="([^"]*)"
 remove:
  - regex: name="([^"]*)"
 remove:
  - options: IgnoreCase
  - regex: style="([^"]*)"
 remove:
  - options: IgnoreCase
  - regex: onload="([^"]*)"
 remove:
  - regex: id="([^"]*)"
 replace:
  - pattern: </title>
  - value: </title><link rel='stylesheet' type='text/css' href='default.css'>
 append:
  - file: %CurrentDirectory%index.xml
  - regex: <div(.*?)>Rule\s*(.*)\.\s(.*?)\.</div>
  - template: <rule name="$name" citation="$2" index="$index">Rule $2. $3.</rule>\r\n
 sql:
  - regex: <div(.*?)>Rule\s*(.*)\.\s(.*?)\.</div>
  - query: 
  INSERT INTO Rule (name,act,rule) 
  VALUES ('$name','$2','$3')
 sql:
  - regex: <div(.*?)>Rule\s*(.*)\.\s(.*?)\.</div>
  - query: UPDATE Rule SET index = $index, rule = '$3' WHERE act LIKE '%$2%'
 replace:
  - regex: <div(.*?)>Rule\s*(.*)\.\s(.*?)\.</div>
  - value: <div><b>Rule $2. $3.</b></div>\
```

[read more](https://github.com/metlinskyi/www.metlinskyi.com/tree/master/csharp/doctor-parse)	
	
![alt text][html-normalizer-logo] html-normalizer
===========
[html-normalizer-logo]: https://raw.github.com/metlinskyi/www.metlinskyi.com/master/csharp/html-normalizer.png "HTML Normalizer"

HTML-normalizer is a functionality for page speeding.

###### Before
```HTML
<!DOCTYPE html>
<html>
<head>
 <meta charset="utf-8" />
 <title></title>
 <script>function fn() { alert(""); }</script>
 <base href="/">
 <link rel="stylesheet" type="text/css" href="style-a.css" />
 <script src="script-a.js"></script>
 <meta name="viewport" content="width=1020" />
</head>
<body>
 <script src="script-b.js"></script>
 <p>some content</p>
 <style>body { background-color: #fff }</style>
 <link rel="stylesheet" type="text/css" href="style-b.css" />
</body>
</html>
```

###### After
```HTML
<!DOCTYPE html>
<html>
<head>
 <base href="/">
 <meta charset="utf-8" />
 <meta name="viewport" content="width=1020" />
 <link rel="stylesheet" type="text/css" href="style-a.css" />
 <link rel="stylesheet" type="text/css" href="style-b.css" />
 <style>body { background-color: #fff; }</style>
 <title></title>
</head>
<body>
 <p>some content</p>
 <script>function fn() { alert(""); }</script>
 <script src="script-a.js"></script>
 <script src="script-b.js"></script>
</body>
</html>
```

[read more](https://github.com/metlinskyi/www.metlinskyi.com/tree/master/csharp/html-normalizer)	

![alt text][html-reader-logo] html-reader 
===========
[html-reader-logo]: https://raw.github.com/metlinskyi/www.metlinskyi.com/master/csharp/html-reader.png "HTML Reader is html parser and tag mapping"

The utility to simple mapping a HTML content on objects. You need just add attributes.
```C#
[Tag("A")]
class Link {
  
  [Attribute("HREF")]
  public string Url { get; set; }
   
  [Attribute]
  public string Title { get; set; }
  
  [Content]
  public string Text { get; set; }
  
}
```

[read more](https://github.com/metlinskyi/www.metlinskyi.com/tree/master/csharp/html-reader)	

switch
======
Yet another fluent switch
```C#
var _switch =
	new Switch<object>()
		.Case((constant, value) =>
		{
			// do somthing ...
		}, 1)
		.Case((constant, value) =>
		{
			// do somthing ...
		}, "test", "1")
		.Case((constant, value) => 
		{
			// do somthing ...
		}, new object[] {String.Empty, "TEST"})
		.Default((value) =>
		{
			// do somthing ...
		});

_switch.Invoke("TEST");
```

whois...
=============

Do You know who is who in .net? No... I don't know too ... 
The Synthetic Super Tests must know!

![staging-photo](https://raw.github.com/metlinskyi/www.metlinskyi.com/master/csharp/string.jpg "Array of strings")
Array of strings

&nbsp;
============
&copy; [The best software engineer in the Universe!](http://metlinskyi.com/)