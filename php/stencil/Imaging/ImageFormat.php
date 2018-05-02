<?php

/**
 * ImageFormat - format factory
 *
 * @author Roman Metlinskyi
 * @link https://github.com/interreto/.php/tree/master/stencil
 */

namespace Imaging;

use Imaging\Formats\{JpegFormat, PngFormat};

abstract class ImageFormat {

    private static $formats;

    private static function Formats()
    {
        if(self::$formats){
            return self::$formats;
        }

        $jpg = new JpegFormat();
        $png = new PngFormat();

        return self::$formats = [
            'jpg'           => $jpg,
            'jpeg'          => $jpg,
            'png'           => $png,
            'image\png'     => $png,
            'image\jpeg'    => $jpg, 
        ];
    }

    public static function Gif() : ImageFormat {
        throw new Exception('Not supported');
    }

    public static function Png() : ImageFormat {
        return self::Formats()['png'];
    }

    public static function Jpeg() : ImageFormat {
        return self::Formats()['jpeg'];
    }

    public static function Get($alias) : ImageFormat {
        return self::Formats()[$alias];
    }        
    
    abstract function extension() : string;   

    abstract function fromFile($filename);

    abstract function saveAs($bitmap, $filename) : bool;   
}
