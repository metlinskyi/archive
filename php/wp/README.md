![alt text][wp-logo] wp
=============
[wp-logo]: https://raw.github.com/metlinskyi/www.metlinskyi.com/master/php/wp/logo.png "Wordpress helper"

Wp is a wordpress helper 

###### Usage
```php

// Scripts settings

add_action('wp_enqueue_scripts', function () {

    WpApp::assets()->style(array(
        'style' => 'style.css'
    ));

    WpApp::assets()->script(array(
        'jquery' => 'js/jquery.js'
    ));
});
```

&nbsp;
============
&copy; [The best software engineer in the Universe!](http://metlinskyi.com/)
