<?php
/**
 * WpImages - image factory
 *
 * @author Roman Metlinskyi
 */
class WpImages {

    public function __construct() {

    }

    public function imageSize($sizes){
        foreach ($sizes as $name=>$size) {
            add_image_size($name, $size['width'],  $size['height'], $size['crop']);
        }
        
        return $this;
    }
}