<?php

/**
 * ResizeAction - bitmap source
 *
 * @author Roman Metlinskyi
 * @link https://github.com/interreto/.php/tree/master/stencil
 */

namespace Imaging\Actions;

use Imaging\{IImage, IImageAction};

class ResizeAction implements IImageAction {

    protected $width;
    protected $height;
    protected $crop;

    public function __construct($width, $height, $crop = true) {
        $this->width = $width;
        $this->height = $height;
        $this->crop = $crop;
    }

    public function applyTo(IImage $image) : IImage {
        $bitmap = imagecreatetruecolor($this->width, $this->height);
        imagecopyresized($bitmap, $image->getBitmap(), 0, 0, 0, 0, $this->width, $this->height, $image->getWidth(), $image->getHeight());
        return $image->fromBitmap($bitmap);  
    }
}
