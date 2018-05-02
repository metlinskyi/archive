![alt text][phml-logo] phml
=============
[phml-logo]: https://raw.github.com/interreto/.php/master/phml/logo.png "Php html templating"

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

&nbsp;
============
&copy; [The best software engineer in the Universe!](http://metlinskyi.com/)
