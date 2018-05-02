![alt text][html-reader-logo] html-reader 
===========
[html-reader-logo]: https://raw.github.com/Interreto/.net/master/html-reader.png "HTML Reader is html parser and tag mapping"

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
###### Usage
You can use the LINQ request
```C#
var allLinks = new HtmlReader<Link>().Read(source);
var withHash = new HtmlReader<Link>().Read(source).Where(l=>l.Url.StartsWith("#"));
```

###### Future
Develop web-spider

    \|/
    (((8:
    /|\


&nbsp;
============
&copy; [Software Engineer](http://metlinskyi.com/)
