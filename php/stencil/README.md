![alt text][stencil-logo] stencil
=============
[stencil-logo]: https://raw.github.com/interreto/.php/master/stencil/img/logo.png "Marker's generator"

Stencil is a simple image cutter for markers or icons

###### Usage

```php

    // get image 
    $stencil = new Stencil($filename);
        
    $stencil
            ->resize(64, 64)
            ->cut($blank, $mask)
            ->save($filename);

```

$filename is a source picture

![alt text][img-src]

$blank is a background image

![img-blank]

$mask is a stencil image

![img-marker]

Tadam!

![img-result]

[img-src]: https://raw.github.com/interreto/.php/master/stencil/img/src.jpg "Eiffel tower"

[img-blank]: https://raw.github.com/interreto/.php/master/stencil/img/blank.png

[img-marker]: https://raw.github.com/interreto/.php/master/stencil/img/marker.png

[img-result]: https://raw.github.com/interreto/.php/master/stencil/img/result.png

You can see more results on our blog [juliet.life](https://juliet.life/mexico/) 

&nbsp;
============
&copy; [The best software engineer in the Universe!](http://metlinskyi.com/)
