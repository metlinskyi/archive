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


&nbsp;
============
&copy; [The best software engineer in the Universe!](http://metlinskyi.com/)