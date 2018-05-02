<?php
/**
 * HtmlTag - html tag object
 *
 * @author Roman Metlinskyi
 */
class HtmlTag {
    
    public static $ClosedTags = array('img', 'hr'); 
    public static $Stream;
    
    protected $parent;
    
    protected $name;
    protected $attributes = [];
    protected $innerHtml = null;
    protected $childs = [];
    
    protected $isOpen = false;
    protected $isClosed = false;

    function __construct($parent, $name, $arguments = null) {
        
        self::$Stream = $this; // bad boy
               
        $this->parent = $parent;
        
        $this->name = $name;
        $this->isClosed = in_array($this->name, self::$ClosedTags);

        if(!empty($arguments)){
            foreach ( $arguments as $value) {
                if (is_array($value)) {
                    $this->is='array';
                    $this->attributes($value);
                } else {
                    $this->innerHtml($value);
                }
            }
        } else {
            $this->is = 'empty'; 
            $this->isOpen = true;
        }
    }

    public function __get($name) {
        return $this->__call($name, null);
    }

    public function __set($name, $value) {
        $this->attr(str_replace('_', "-", $name), $value);
    }

    public function __call($name, $arguments) {
        $tag = new HtmlTag($this, $name, $arguments);
        return $this->child($tag);
    }

    public function __toString() {
        foreach ((array) $this->childs as $tag) {
            $this->innerHtml .= $tag->render();
        }
        return $this->renderOpen() . $this->innerHtml . $this->renderClose();
    }

    public function innerHtml($value) {
        
        if (is_callable($value)) {
            
            $this->is='callable';
            
            $this->isOpen = true;
            
            ob_start();

            $value($this);

            $this->innerHtml = ob_get_contents();

            ob_end_clean();
            
        } elseif (is_string($value)) {
            
            $this->is='string';
            $this->innerHtml = $value;
            
        }
        
        return $this;
    }
    
    public function attributes($attributes) {
        $this->attributes = $attributes;
    }

    public function attr($name, $values) {
        $this->attributes[$name] = $values;
    }

    public function child($tag) {
        $this->childs[] = $tag;
        return $this;
    }

    public function flush() {
        $first = $this->firstTag();
        $first->render();
        flush();
    }

    public function firstTag() {
        return $this->parent === null ? $this : $this->parent->firstTag();
    }

    public function renderOpen($attributes = '') {
        foreach ((array) $this->attributes as $name => $value) {
            $attrValue = is_array($value) ? implode(" ", $value) : $value;
            $attributes.= " {$name}=\"{$attrValue}\"";
        }
        return "<{$this->name}{$attributes}" . ($this->isClosed ? '/>': '>');
    }

    public function renderClose() {
        return $this->isClosed ? '' : "</{$this->name}>";
    }
    
    public function render() {

        echo $this->renderOpen();
        
        echo $this->innerHtml;

        foreach ((array) $this->childs as $tag) {
            if ($tag->render()) {
                return $this;
            }
        }
        
        echo $this->renderClose();

        return null;
    }
}
