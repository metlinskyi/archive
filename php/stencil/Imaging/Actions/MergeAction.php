<?php

/**
 * MergeAction - bitmap source
 *
 * @author Roman Metlinskyi
 * @link https://github.com/interreto/.php/tree/master/stencil
 */

namespace Imaging\Actions;

use Imaging\{IImage, IImageAction};

class MergeAction implements IImageAction {

    protected $image;
    protected $buckground;
    public function __construct(IImage $image, $buckground = true) {
        $this->image = $image;
        $this->buckground = $buckground;
 
        imagesavealpha($this->image->getBitmap(), true);
    }

    public function applyTo(IImage $image) : IImage {
        
        $bitmap = imagecreatetruecolor($image->getWidth(), $image->getHeight());
        $fill = imagecolorallocatealpha($bitmap, 0, 0, 0, 127);
        imagefill($bitmap, 0, 0, $fill);
        imagesavealpha($bitmap, true); 
        
        
        $sources = $this->buckground 
                ? [$this->image, $image]
                : [$image,$this->image];
        
        foreach ($sources as $img){
            imagecopy($bitmap, $img->getBitmap(), 0, 0, 0, 0, $image->getWidth(), $image->getHeight());
        }
        
        return $image->fromBitmap($bitmap);  
    }
}
