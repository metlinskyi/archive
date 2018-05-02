<?php

/**
 * StencilAction - bitmap source
 *
 * @author Roman Metlinskyi
 * @link https://github.com/interreto/.php/tree/master/stencil
 */

namespace Imaging\Actions;

use Imaging\{IImage, IImageAction, ImageFormat};

class SaveAction implements IImageAction {

    protected $format;
    protected $dir;
    protected $filename;

    public function __construct(ImageFormat $format, $dir) {
        $this->dir = $dir;
        $this->format = $format;
    }

    public function setFilename($filename) : SaveAction {
        $this->filename = $filename;
        return $this;
    }

    public function applyTo(IImage $image) : IImage {
        $this->format->saveAs($image->getBitmap(), $this->getFilename(), 9);
        return $image;
    }

    private function getFilename() {
        if(!isset($this->filename)) {
            $this->filename =  uniqid($this->dir . '/') . $this->format->extension();
        }
        return $this->filename;
    }
}
