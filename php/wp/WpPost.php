<?php
/**
 * WpPost - current wordpress post
 *
 * @author Roman Metlinskyi
 */
class WpPost {
    
    private $post;
    private $author;
    private $thumbnail;
    private $microdata;
    
    public function __construct($post, $sizes) {
        $this->post = $post;
        $this->thumbnail = WpImage::GetThumbnail($this->post->ID, get_the_title($this->post->ID), $sizes);
        $this->author = (object) array(
            '@type'             =>  'Person',
            'name'              =>  get_the_author_meta( 'description' ),
            'additionalName'    =>  get_the_author_meta( 'nicename' ),
            'sameAs'            =>  array_map( function($x){ 
                                        return $x->link_url; 
                                    }, get_bookmarks())
        );
    }

    public function thumbnail($sizes = null){
        if($sizes !== null){
            $this->thumbnail = WpImage::GetThumbnail($this->post->ID, get_the_title($this->post->ID), $sizes);
        }
        return $this->thumbnail;
    }
    
    public function author(){
        return $this->author;
    }
    
    public function image($size){
        $image = $this->thumbnail->getSrc($size);
        return (object) array(
            '@context'          =>  'http://schema.org',
            '@type'             =>  'ImageObject',
            'url'               =>  $image->src,
            'height'            =>  $image->height,
            'width'             =>  $image->width
        );
    }
    
    public function logo(){
        return (object) array(
            '@context'          =>  'http://schema.org',
            '@type'             =>  'ImageObject',
            'url'               =>  'http://juliett.co/logo.png',
            'height'            =>  60,
            'width'             =>  600   
        );
    }
    
    public function publisher(){
        return (object) array(
            '@context'          =>  'http://schema.org',
            '@type'             =>  'Organization', 
            'name'              =>  'Juliett.co',
            'logo'              =>  $this->logo()
        );
    }
    
    public function microdata($size){
        return (object) array(
            '@context'          =>  'http://schema.org',
            '@type'             =>  'BlogPosting',
            'image'             =>  $this->image($size),
            'publisher'         =>  $this->publisher(),
            'author'            =>  $this->author(),
            'headline'          =>  get_the_title($this->post->ID),
            'datePublished'     =>  get_the_time(DATE_ISO8601, $this->post->ID),
            'dateModified '     =>  get_post_modified_time(DATE_ISO8601, true, $this->post->ID),
            'description'       =>  get_the_excerpt($this->post->ID),
            'mainEntityOfPage'  =>  get_the_permalink($this->post->ID)
        );
    }
}
