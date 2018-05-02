<?php

/**
 * IImage - interface
 *
 * @author Roman Metlinskyi
 * @link https://github.com/interreto/.php/tree/master/stencil
 */

namespace Imaging;

interface IImage {

    public function getBitmap();

    public function getWidth();

    public function getHeight();

    public function fromBitmap($bitmap) : IImage;
}
