<?php

/**
 * WpAssets - wordpress resource manager
 *
 * @author Roman Metlinskyi
 */
class WpAssets {

    private $assetsDirectory = '';
    private $assetsDirectoryUri = '';
    private $templateDirectory = '';
    private $templateDirectoryUri = '';
    public $debug = false;
    public $version = false;

    public function __construct($directory) {

        $this->debug = WP_DEBUG;

        $this->assetsDirectory = $directory ? ($directory . DIRECTORY_SEPARATOR) : $directory;
        $this->assetsDirectoryUri = $directory ? ($directory . '/') : $directory;

        $this->templateDirectory = $this->dirFix(get_template_directory()) . DIRECTORY_SEPARATOR;
        $this->templateDirectoryUri = get_template_directory_uri() . '/';
    }

    public function version($version = null) {
        if ($version) {
            $this->version = $version;
        }

        return $this->version;
    }

    public function uri($url = '') {
        return $this->templateDirectoryUri
                . $this->assetsDirectoryUri
                . $url;
    }

    public function dir($path = '') {
        return $this->templateDirectory
                . $this->assetsDirectory
                . $path;
    }

    public function style($styles) {
        foreach ($styles as $handle => $src) {
            wp_deregister_style($handle);
            if ($src !== null) {
                wp_enqueue_style($handle, $src, array(), $this->version);
            }
        }
    }
    
    public function styleTag($path) {
        echo '<style type="text/css">';
        require_once ( $this->dir($this->dirFix($this->css($path))) );
        echo '</style>';
    }
    
    public function script($scripts, $to_footer = true) {
        foreach ($scripts as $handle => $src) {
            wp_deregister_script($handle);
            if ($src !== null) {
                wp_enqueue_script($handle, $src, array(), $this->version, $to_footer);
            }
        }
    }

    public function scriptTag($path) {
        echo '<script type="text/javascript">';
        require_once ( $this->dir($this->dirFix($this->js($path))) );
        echo '</script>';
    }

    public function loadThemeTextdomain($name, $path = DIRECTORY_SEPARATOR) {
        load_theme_textdomain($name, $this->templateDirectory . $path);
    }

    private function dirFix($path) {
        return str_replace('/', DIRECTORY_SEPARATOR, $path);
    }

    private function js($url) {
        return $this->debug ? $url : str_replace('.js', '.min.js', $url);
    }
    
    private function css($url) {
        return $this->debug ? $url : str_replace('.css', '.min.css', $url);
    }
}
