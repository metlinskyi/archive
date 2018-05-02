<?php

$dir = __DIR__; 

spl_autoload_register(function ($class) use ($dir)  {
    require $dir 
    . DIRECTORY_SEPARATOR 
    . str_replace('\\', DIRECTORY_SEPARATOR, $class) 
    . '.php';
});

$folder = __DIR__ . '/images/';

$marker = new Marker($folder . 'mask.png', $folder . 'blank.png');

$marker->applyTo([$folder . 'src.jpg']);