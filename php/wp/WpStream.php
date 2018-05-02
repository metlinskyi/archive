<?php
/**
 * WpStream - current context
 *
 * @author Roman Metlinskyi
 */
class WpStream {
    
    private static $PageNumber;
    private static $Category;

    public static $Categories = [];
    public static $Tags = [];
    public static $Html = [];
    
    public static $Post = null;
    public static $Attachment = null;

    public static function GetPageNumber(){
        return self::$PageNumber 
                ? self::$PageNumber 
                : (self::$PageNumber = (get_query_var('paged')) ? get_query_var('paged') : 1);
    }
       
    private $wp;
        
    public function __construct($wp) {
        $this->wp = $wp;
    }
}
