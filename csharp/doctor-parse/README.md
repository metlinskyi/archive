![alt text][doctor-parse-logo] doctor-parse 
============
[doctor-parse-logo]: https://raw.github.com/Interreto/.net/master/doctor-parser.png "Dr.Parse is batch processing of text data"

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
Execute console application

    drparser.exe parsinst.yaml


&nbsp;
============
&copy; [Software Engineer](http://metlinskyi.com/)
