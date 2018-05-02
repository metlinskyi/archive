<?php

/**
 * StencilAction - bitmap action
 *
 * @author Roman Metlinskyi
 * @link https://github.com/interreto/.php/tree/master/stencil
 */

namespace Imaging\Actions;

use Imaging\{IImage, IImageAction};

class StencilAction implements IImageAction {

    protected $stencil;
    protected $transparent;
    
    public function __construct(IImage $stencil) {
        $this->stencil = $stencil;
        imagesavealpha($this->stencil->getBitmap(), true);
        $this->transparent = imagecolorat($this->stencil->getBitmap(), 0, 0);
    }

    public function applyTo(IImage $image) : IImage {

        $bitmap = imagecreatetruecolor($image->getWidth(), $image->getHeight());
        $fill = imagecolorallocatealpha($bitmap, 0, 0, 0, 127);
        imagefill($bitmap, 0, 0, $fill);
        imagesavealpha($bitmap, true);      

        $source = $image->getBitmap();

        // cut
        for ($x = 0; $x < $image->getWidth(); $x++) {
            for ($y = 0; $y < $image->getHeight(); $y++) {
                $mask = imagecolorat($this->stencil->getBitmap(), $x, $y);
                $alpha = imagecolorsforindex($this->stencil->getBitmap(), $mask)['alpha'];
                $pixel = imagecolorsforindex($source, imagecolorat($source, $x, $y));
                $rgba = imagecolorallocatealpha($source, $pixel['red'], $pixel['green'], $pixel['blue'], $alpha);

                imagesetpixel($bitmap, $x, $y, $rgba);
            }
        }

        return $image->fromBitmap($bitmap);
    }
}
