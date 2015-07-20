<?php

include_once DRUPAL_ROOT . '/sites/all/libraries/dropbox-sdk/Dropbox/autoload.php';

use \Dropbox as dbx;

function form_intergrate_dropbox($form, &$form_state, $no_js_use = FALSE) {
    $appInfo = dbx\AppInfo::loadFromJsonFile(drupal_get_path('module', 'leaf_consignor') . "/config/config.json");
    $webAuth = new dbx\WebAuthNoRedirect($appInfo, "PHP-Example/1.0");

    $authorizeUrl = $webAuth->start();


    $form['dropbox'] = array(
        '#type' => 'fieldset',
        '#title' => t('Intergrate Dropbox'),
        '#weight' => 1,
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
    );

    $form['dropbox']['token'] = array(
        '#type' => 'textfield',
        '#title' => t('Authorization code'),
        '#default_value' => variable_get('leaf_dropbox_access_token', ''),
        '#size' => 128,
        '#maxlength' => 256,
        '#required' => TRUE,
    );

    $form['dropbox']['markup'] = array(
        '#markup' => "1. Go to: " . l($authorizeUrl, $authorizeUrl, ['attributes' => ['target' => '_blank']]) . "<br>" . "2. Click \"Allow\" (you might have to log in first).<br>" . "3. Copy the authorization code.",
        '#prefix' => '<p>',
        '#suffix' => '</p>'
    );

    $form['dropbox']['submit'] = array(
        '#type' => 'submit',
        '#value' => 'submit',
        '#weight' => '1',
    );

    return $form;
}

function form_intergrate_dropbox_submit($form, &$form_state) {
    $appInfo = dbx\AppInfo::loadFromJsonFile(DRUPAL_ROOT . "/sites/all/libraries/dropbox-sdk/config.json");
    $webAuth = new dbx\WebAuthNoRedirect($appInfo, "PHP-Example/1.0");
    $message = '';
    try {
        list($accessToken, $dropboxUserId) = $webAuth->finish($form_state['values']['token']);
    } catch (Exception $exc) {
        $message = $exc->getMessage();
    }

    if (empty($message)) {
        variable_set('leaf_dropbox_access_token', $accessToken);
        drupal_set_message('Update successfully!');
    } else {
        $message = json_decode($message);
        drupal_set_message($message->error_description, 'warning');
    }
}

function uploadToDropbox($path) {
    $accessToken = variable_get('leaf_dropbox_access_token', '');

    if (empty($accessToken)) {
        //log
        watchdog('leaf_consignor', 'Authorization code dont setup!', [], WATCHDOG_WARNING);
    } else {
        $dbxClient = new dbx\Client($accessToken, "PHP-Example/1.0");
        $f = fopen($path, "rb");
        $folder = '/leaf export' . date('/Y/m');
        $dbxClient->createFolder($folder);
        $name = $folder . '/' . date('Ymd') . '-leaf-export.xls';
        $result = $dbxClient->uploadFile($name, dbx\WriteMode::force(), $f);
    }
}

function downloadFromDropbox() {
    $accessToken = variable_get('leaf_dropbox_access_token', '');
    $file = NULL;
    if (empty($accessToken)) {
        //log
        watchdog('leaf_consignor', 'Authorization code dont setup!', [], WATCHDOG_WARNING);
    } else {
        $dbxClient = new dbx\Client($accessToken, "PHP-Example/1.0");
        $folder = '/POS' . date('/Y/m');
        $dbxClient->createFolder($folder);
        $name = date('Ymd', strtotime("yesterday")) . '-POS.xls';
        $f = fopen($name, "w+b");
        $fileMetadata = $dbxClient->getFile($folder . '/' . $name, $f);
        $f = file_get_contents($name);
        $file = file_save_data($f, 'public://' . $name);
        $file->status = FILE_STATUS_PERMANENT;
        file_save($file);
    }
    
    return $file;
}
