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
```C#
	Curl
		.Get("http://metlinskyi.com/api/")
		.Json()
		.Content<success>();
```

###### Future
Parser for a curl command
```C#
	Curl.Parse(@"
				curl -X POST -u username:password http://metlinskyi.com/api/
					-d grant_type=client_credentials
					-d scope=basic+user
					-p 127.0.0.1:8888 
			")
			.Responce()
			.Content();
```


&nbsp;
============
&copy; [Software Engineer](http://metlinskyi.com/)
