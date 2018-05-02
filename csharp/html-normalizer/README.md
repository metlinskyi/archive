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

###### Usage

```C#

	// General
	
	new HtmlNormalizer().Process("<html content>");
	
	new HtmlStream("<some html stream>");
	
	// Web Form
	
	HtmlNormalizerAdapter
	
	// Mvc
	public class HomeController : Controller
	{
		[HtmlNormalizer]
		public ActionResult Index()
		{
			// ...
```		


&nbsp;
============
&copy; [Software Engineer](http://metlinskyi.com/)
