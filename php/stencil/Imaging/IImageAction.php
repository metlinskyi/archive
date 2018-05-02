<?php

/**
 * IImageAction - image action interface
 *
 * @author Roman Metlinskyi
 * @link https://github.com/interreto/.php/tree/master/stencil
 */

namespace Imaging;

interface IImageAction {
    public function applyTo(IImage $image) : IImage;
}
