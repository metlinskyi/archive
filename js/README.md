![alt text][curl-logo] JavaScript
============
[curl-logo]: https://raw.github.com/metlinskyi/js/master/smart.png "Widgets for UI"
Smart js widgets for your sexy UI

Enum
============
###### Usage
```JS

    var x = new Enum({ A: 1, B: 2, C: 'Z' });

    console.log(x.A.toString());    // = A
    console.log(x.A());             // = 1
    
```

Hash
============
###### Usage
```JS

    var x = new Hash("Hello, World!");
    
    console.log(x.toString());    	// = -284335608
  
```

Smart.MediaQuerie
============
###### Usage
```JS
$(window).on('resize',function() {

    var width = $( window ).width();
    var height = $( window ).height();
	
    Smart.MediaQuerie.Resize(width, height, function(prev, current){
        $('body')
                .removeClass(prev.Size)
                .removeClass(prev.Ratio)
                .removeClass(prev.Orientation)
                .addClass(current.Size)
                .addClass(current.Ratio)
                .addClass(current.Orientation)
        ;
    });
});
```

###### Result
```HTML
<body class="medium widescreen landscape"></body>
```

&nbsp;
============
&copy; [The best software engineer in the Universe!](http://metlinskyi.com/)

