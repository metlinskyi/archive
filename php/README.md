PHP Dealer - helpers, tips and tricks
============

![alt text][phml-logo] phml
=============
[phml-logo]: https://raw.github.com/metlinskyi/www.metlinskyi.com/master/php/phml/logo.png "Php html templating"

PHML is new way in templating html for php.

###### Usage
```php
Phml(5)
    ->doctype()
    ->html(function($e){
        
        $e->class = "no-js";
        
        $e->head( wp_head )
  
            ->body(function($e){

                $e->class = get_body_class();
                
                $e->nav(function($e){

                })
                ->header(function($e){

                })
                ->main(function($e){
                    
                });
            });
    });
```

[read more](https://github.com/metlinskyi/www.metlinskyi.com/tree/master/php/phml)	

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

[read more](https://github.com/metlinskyi/www.metlinskyi.com/tree/master/php/stencil)	

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

[read more](https://github.com/metlinskyi/www.metlinskyi.com/tree/master/php/wp)

&nbsp;
============
&copy; [The best software engineer in the Universe!](http://metlinskyi.com/)
