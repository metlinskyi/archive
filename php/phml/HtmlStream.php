<?php

function Phml($version = null) {
    if (HtmlTag::$Stream === null){
        return new Html($version);
    }
    return HtmlTag::$Stream;
}