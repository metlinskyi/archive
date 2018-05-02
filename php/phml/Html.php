<?php
/**
 * Html - lib loader
 *
 * @author Roman Metlinskyi
 */

require_once 'Html.php';
require_once 'HtmlTag.php';
require_once 'HtmlDoctype.php';
require_once 'HtmlStream.php';

class Html {
    
    protected $version;
    
    protected $firstTag;

    function __construct($version) {
        $this->version = $version;
    }

    public function __toString() {
        return "$this->firstTag";
    }

    public function __call($name, $arguments) {
        return ($this->firstTag = new HtmlTag(null, $name, $arguments));
    }

    public function doctype() {
        return ($this->firstTag = new HtmlDoctype());
    }
}

class Html5 extends Html {
    function __construct() {
        parent::__construct(5);
    }
}

class Html4 extends Html {
    function __construct() {
        parent::__construct(4);
    }
}
