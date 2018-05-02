<?php
/**
 * WpImage - image factory
 *
 * @author Roman Metlinskyi
 */
class WpImage {
    
    private $id; 
    private $srcset;
    private $first;

    public $alt;
 
    public function __construct($id) {
        $this->id = (int) $id;
        $this->srcset = [];
        
        self::SetImage($this->id);
    }
       
    public function __toString() {
        return '' . $this->simple();
    }
     
    public function addSrc($size, $src) {
        if(!$this->first){
            $this->first = $size;
        }
        $this->srcset[$size] = $src;
    }
    
    public function getSrc($size) {
        if(empty($this->srcset)){
            return null;
        }
        return $size ? $this->srcset[$size] : $this->srcset[$this->first];  
    }
    
    public function getSrcSet() {
        foreach ($this->srcset as $image) {
            $srcset[] = $image->src . ' ' . $image->width . 'w'; 
        }
        return $srcset;
    }
    
    public function responsive($size = null, $class = 'image'){
        
        $img = $this->getSrc($size);
        if($img == null){
            return null; 
        }
        
        $orientation = self::GetOrientation($img->width, $img->height);
        
        return '<img '
                . 'src="' . $img->src . '" '
                . 'width="' . $img->width . '" '
                . 'height="' . $img->height . '" '
                . 'alt="' . $this->alt . '" '
                . 'class="responsive ' . $orientation . ' ' . $size . ' ' . $class . '" '
                . 'srcset="' . implode(',', $this->getSrcSet()) . '" '
                . 'itemprop="image"'
                . '/>';
    }
    
    public function simple($size = null, $class = 'image'){
        
        $img = $this->getSrc($size);        
        if($img == null){
            return null; 
        }
        
        $orientation = self::GetOrientation($img->width, $img->height);
        
        return '<img ' 
                . 'src="' . $img->src . '" '
                . 'width="' . $img->width . '" '
                . 'height="' . $img->height . '" '
                . 'alt="' . $this->alt . '" '
                . 'class="' . $size . ' ' . $orientation . ' ' . $class . '" ' 
                . 'data-width="' . $img->width . '" '
                . 'data-height="' . $img->height . '" ' 
                . '/>';
    }

    private static $Stream = [];
    
    // default sizes
    public static $Sizes = array(
        'thumbnail',
        'medium',
        'large',
    );
    
    public static function GetImages(){
        return self::$Stream;
    }
    
    public static function SetImage($attachment_id){
        return self::$Stream[] = $attachment_id;
    }

    public static function GetOrientation($width, $height){
        $dif = $width - $height;
        $rate = abs($dif > 0 ? $width / $height : $height / $width);

        if($rate < 1.2 ){
            $orientation = 'square';
        }
        else{
            $orientation = ($dif > 0) ? 'landscape' : 'portrait';
        }  
        return $orientation;
    }
    
    public static function GetThumbnail($post_id, $alt = '', $sizes = null){
        if((bool)($post_thumbnail_id = get_post_thumbnail_id($post_id))){
            return self::GetImage($post_thumbnail_id, $alt, $sizes);
        }
        return new WpImage(0);
    }
     
    public static function GetImage($attachment_id, $alt = '', $sizes = null){
        $image = new WpImage($attachment_id);
        $image->id = $attachment_id;
        $image->alt = $alt;
        
        foreach ($sizes ? $sizes : self::$Sizes as $size) {
            if (($img = self::GetImageSrc($attachment_id, $size, $alt))){
                $image->addSrc($size, $img);
            }
        }
        return $image;
    }
    
    public static function GetImageSrc($attachment_id, $size){
        if (($image = wp_get_attachment_image_src($attachment_id, $size))){
            list($src, $width, $height) = $image;
            return (object) array(
                'src'       => $src,
                'width'     => $width,
                'height'    => $height,
            );
        }
        return null;
    }
}