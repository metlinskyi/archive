<?php

/**
 * PngFormat - support png format
 *
 * @author Roman Metlinskyi
 * @link https://github.com/interreto/.php/tree/master/stencil
 */

namespace Imaging\Formats;

use Imaging\{ImageFormat};

class PngFormat extends ImageFormat  {

    public function fromFile($filename) {
        return imagecreatefrompng($filename);
    }

    public function saveAs($bitmap, $filename) : bool {
        return imagepng($bitmap, $filename, 9);
    }

    public function extension(): string {
        return '.png';
    }
}  
