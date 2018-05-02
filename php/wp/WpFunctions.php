<?php
/**
 * WpFunctions - wordpress functions.php helper
 *
 * @author Roman Metlinskyi
 */
class WpFunctions {
  
    public function themeSupport($support){
        foreach ($support as $feature => $args) {
            if(is_string($feature)){
                add_theme_support($feature, $args);
            }else{
                add_theme_support($args);
            }
        }
        
        return $this;
    }
    
    public function postTypeSupport($support){
        foreach ($support as $type => $feature) {
            add_post_type_support($type, $feature);
        }
        
        return $this;
    }
    
    public function removeAction($list){
        foreach ($list as $tag => $actions) {
            foreach ($actions as $name => $priority) {
                remove_action($tag, $name, $priority);
            }
        }
        
        return $this;
    }
}