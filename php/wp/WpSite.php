<?php

/**
 * WpSite - site info
 *
 * @author Roman Metlinskyi
 */
class WpSite {

    public function api() {

        $method = filter_input(INPUT_SERVER, 'REQUEST_METHOD');

        $action = filter_input(INPUT_GET, 'action');

        try {
            get_template_part('api/' . strtolower($method) . '-' . strtolower($action));
        } catch (Exception $e) {
            if (WP_DEBUG) {
                echo json_encode($e);
            }
        }
        exit;
    }

    public function json($model) {
        header('Content-Type: application/json');
        echo json_encode($model);
    }

}
