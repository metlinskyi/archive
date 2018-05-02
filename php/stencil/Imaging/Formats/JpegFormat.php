<?php

/**
 * JpegFormat - support jpeg format
 *
 * @author Roman Metlinskyi
 * @link https://github.com/interreto/.php/tree/master/stencil
 */

namespace Imaging\Formats;

use Imaging\{ImageFormat};

class JpegFormat extends ImageFormat  {

    public function fromFile($filename) {
        return imagecreatefromjpeg($filename);
    }

    public function saveAs($bitmap, $filename) : bool {
        return imagejpeg($bitmap, $filename, 100);
    }

    public function extension(): string {
        return '.jpg';    
    }
}
