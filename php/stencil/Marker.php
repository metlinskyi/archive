<?php

use Imaging\{Image, ImageFormat};
use Imaging\Actions\{ResizeAction, StencilAction, MergeAction, SaveAction};

class Marker {
    
    protected $actions;
    protected $stencil;
    protected $buckground;
    
    public function __construct($stencil, $buckground) {
        
        $this->stencil = Image::FromFile($stencil);
        $this->buckground = Image::FromFile($buckground);

        $this->actions = [
            new ResizeAction($this->stencil->getWidth(), $this->stencil->getHeight()),
            new StencilAction($this->stencil),
            new MergeAction($this->buckground),
            new SaveAction(ImageFormat::Png(), __DIR__),
        ];
    }
    
    public function applyTo($files){
        $images = [];
        foreach ($files as $filename) {
            $images[] = Image::FromFile($filename);    
        }
        foreach ($images as $image) {
            foreach ($this->actions as $action) {
                $image = $action->applyTo($image);
            }
        }    
        
        return $images;
    }
}