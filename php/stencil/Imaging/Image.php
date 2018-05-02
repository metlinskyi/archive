<?php

/**
 * Image
 *
 * @author Roman Metlinskyi
 * @link https://github.com/interreto/.php/tree/master/stencil
 */

namespace Imaging;

class Image implements IImage {

    protected $bitmap;       
    protected $width;
    protected $height;

    public function __construct($bitmap = null) {
        $this->bitmap = $bitmap;
        if($this->bitmap != null){
            $this->width = imagesx($this->bitmap);
            $this->height = imagesy($this->bitmap);
        }
    }

    public function __destruct() {
        imagedestroy($this->bitmap);
    }

    public function getBitmap() {
        return $this->bitmap;
    }

    public function getWidth() {
        return $this->width;
    }

    public function getHeight() {
        return $this->height;
    }

    public function fromBitmap($bitmap) : IImage {
        return new Image($bitmap);
    }

    public function action(IImageAction $action) : IImage {
        return $action->applyTo($this);
    }

    public static function FromGif($filename) : Image {
        throw new Exception('Not supported');
    }

    public static function FromPng($filename) : Image {
        return new Image(ImageFormat::Png()->fromFile($filename));
    }

    public static function FromJpeg($filename) : Image {
        return new Image(ImageFormat::Jpeg()->fromFile($filename));
    }

    public static function FromFile($filename) : Image {
        $pathinfo = pathinfo($filename);
        $format = ImageFormat::Get($pathinfo['extension']);
        if(!$format){
            throw new Exception('Not supported');
        }
        return new Image($format->fromFile($filename));
    }
}
