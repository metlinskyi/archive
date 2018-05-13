![alt text][stencil-logo] stencil
=============
[stencil-logo]: https://raw.github.com/metlinskyi/www.metlinskyi.com/master/php/stencil/images/logo.png "Marker's generator"

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

[img-src]: https://raw.github.com/metlinskyi/www.metlinskyi.com/master/php/stencil/images/src.jpg "Eiffel tower"

[img-blank]: https://raw.github.com/metlinskyi/www.metlinskyi.com/master/php/stencil/images/blank.png

[img-marker]: https://raw.github.com/metlinskyi/www.metlinskyi.com/master/php/stencil/images/marker.png

[img-result]: https://raw.github.com/metlinskyi/www.metlinskyi.com/master/php/stencil/images/result.png

You can see more results on our blog [juliet.life](http://www.juliet.life/sitemap/) 

&nbsp;
============
&copy; [The best software engineer in the Universe!](http://metlinskyi.com/)
