<?php
/**
 * WpApp - singleton
 *
 * @author Roman Metlinskyi
 */
class WpApp {
    
    /**
     * Class loader;
     */
    
    static private $dirname;

    static private function load($name){
        if(!self::$dirname){
            self::$dirname = dirname( __FILE__ ) . DIRECTORY_SEPARATOR;
        }
        require_once( self::$dirname . $name . '.php');  
    }


    /**
     * WpAssets singleton;
     */
    static private $assets;
    
    static public function assets($directory = ''){
        if(!self::$assets){
            self::load('WpAssets');
            self::$assets = new WpAssets($directory);
        }

        return self::$assets;
    }
    
    /**
     * WpFunctions singleton;
     */
    static private $functions;
    
    static public function functions(){
        if(!self::$functions){
            self::load('WpFunctions');
            self::$functions = new WpFunctions();
        }

        return self::$functions;
    }
    
    /**
     * WpStream singleton;
     */
    static private $stream;
    
    static public function stream(){
        if(!self::$stream){
            self::load('WpStream');
            self::$stream = new WpStream();
        }

        return self::$stream;
    }
    
    /**
     * WpImage singleton;
     */
    static private $images;
    
    static public function images(){
        if(!self::$images){
            self::load('WpImages');
            self::$images = new WpImages();
        }

        return self::$images;
    }
    
    /**
     * WpAssets singleton;
     */
    static private $post;
    
    static public function post(){
        if(!self::$post){
            self::load('WpPost');
            self::$post = new WpPost();
        }

        return self::$post;
    }
    
    /**
     * WpAssets singleton;
     */
    static private $site;
    
    static public function site(){
        if(!self::$site){
            self::load('WpSite');
            self::$site = new WpSite();
        }

        return self::$site;
    }
}