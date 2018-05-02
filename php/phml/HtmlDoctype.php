<?php
/**
 * HtmlDoctype - html doctype object
 *
 * @author Roman Metlinskyi
 */
class HtmlDoctype extends HtmlTag {

    function __construct() {
        parent::__construct(null, "!doctype");
    }

    public function __toString() {

        foreach ((array) $this->childs as $tag) {
            $this->innerHtml .= "$tag";
        }

        return $this->renderOpen(' html') . $this->innerHtml;
    }

    public function render() {
        echo $this->renderOpen(' html');
        foreach ((array) $this->childs as $tag) {
            $tag->render();
        }
    }
}