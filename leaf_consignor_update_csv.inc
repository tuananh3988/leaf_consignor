<?php
function form_update_csv($form, &$form_state, $no_js_use = FALSE) {
    $form['vendor_update'] = array(
        '#type' => 'submit',
        '#value' => 'vendor',
        '#weight' => '1',
    );

    $form['contract_update'] = array(
        '#type' => 'submit',
        '#value' => 'contract',
        '#weight' => '2',
    );
    
    $form['product_update'] = array(
        '#type' => 'submit',
        '#value' => 'product',
        '#weight' => '3',
    );
    
    $form['other_update'] = array(
        '#type' => 'submit',
        '#value' => 'other',
        '#weight' => '4',
    );

    return $form;
}

function form_update_csv_submit($form, &$form_state) {
    $v = $form_state['values'];
    if($v['op'] == 'vendor') {
        $i = 1;
        $file = fopen(drupal_get_path('module', 'leaf_consignor') . '/csv/vendor.csv', 'r');
        while(! feof($file))
        {
            $data = fgetcsv($file);
            if($i > 1 && !empty($data[0])) {
                $nid = db_insert('leaf_vendor') // Table name no longer needs {}
                ->fields(array(
                    'vendor_uuid' => $data[0],
                    'name' => $data[1],
                    'address' => $data[2],
                    'city' => $data[3],
                    'state' => $data[4],
                    'zip' => $data[5],
                    //'email_address' => $data[6],
                    'email_address' => 'jspencer@teachitinc.com',
                    'phone_number' => $data[7],
                    'alternate_phone_number' => $data[8],
                    'payment_id' => $data[9],
                    'user_id' => 1,
                    'delete_flag' => $data[11],
                    'created_date' => $data[12],
                ))
                ->execute();
            }

            $i++;

        }
    }
    //contract
    if($v['op'] == 'contract') {
        $i = 1;
        $file = fopen(drupal_get_path('module', 'leaf_consignor') . '\csv\contract.csv', 'r');
        while(! feof($file))
        {
            $data = fgetcsv($file);
            if($i > 1 && !empty($data[0])) {
                $vendor = db_select('leaf_vendor', 'v')
                ->fields('v')
                ->condition('v.vendor_uuid', $data[1], '=')
                ->execute()
                ->fetchAssoc();
                if($vendor) {
                    $nid = db_insert('leaf_contract') // Table name no longer needs {}
                    ->fields(array(
                        'contract_uuid' => $data[0],
                        'vendor_id' => $vendor['vendor_id'],
                        'user_id' => 1,
                        'shop_store_id' => 1,
                        'from_date' => strtotime($data[4]),
                        'close_date' => strtotime($data[5]),
                        'claim_date' => strtotime($data[5]) + 24*60*60,
                        'vendor_category' => $data[6],
                        'revenue_sharing' => $data[7],
                        'offer_price' => $data[8],
                        'number_items_off' => $data[9],
                        'number_items_accepeted' => $data[10],
                        'number_items_rejected' => $data[11],
                        'delete_flag' => $data[12],
                        'created_date' => $data[13],
                    ))
                    ->execute();
                }
                else {
                    watchdog('csv_import', 'contract uuid %contract_uuid have vendor_uuid: %vendor_uuid not exift', array('%contract_uuid' => $data[0], '%vendor_uuid' => $data[1]), WATCHDOG_DEBUG, $link = NULL);
                }

            }

            $i++;

        }
    }
    
    //product
    if($v['op'] == 'product') {
        $i = 1;
        $file = fopen(drupal_get_path('module', 'leaf_consignor') . '\csv\product.csv', 'r');
        while(! feof($file))
        {
            $data = fgetcsv($file);
            
            if($i > 1 && !empty($data[0])) {
                $contract = db_select('leaf_contract', 'v')
                ->fields('v')
                ->condition('v.contract_uuid', $data[2], '=')
                ->execute()
                ->fetchAssoc();
                
                $sizeMagento = db_select('leaf_size_magento', 'v')
                ->fields('v')
                ->condition('v.id', $data[8], '=')
                ->execute()
                ->fetchAssoc();
                
                if($contract) {
                    $nid = db_insert('leaf_product') // Table name no longer needs {}
                    ->fields(array(
                        'sku' => $data[1],
                        'contract_id' => $contract['contract_id'],
                        'name' => $data[3],
                        //'item_category_id' => 1,
                        'description' => $data[5],
                        'price' => $data[6],
                        'color_id' => !empty($data[7]) ? $data[7] : NULL,
                        'size' => $sizeMagento['name'],
                        'brand_id' => !empty($data[9]) ? $data[9] : NULL,
                        'consignment_price' => floatval($data[10]),
                        //'purchase_price' => $data[10],
                        'quantity' => $data[11],
                        'status' => $data[12],
                        'cop' => ($data[13] == 'false') ? 0 : $data[13],
                        //'created_date' => $data[13],
                    ))
                    ->execute();
                }
                else {
                    watchdog('csv_import', 'contract uuid %contract_uuid have vendor_uuid: %vendor_uuid not exift', array('%contract_uuid' => $data[0], '%vendor_uuid' => $data[1]), WATCHDOG_DEBUG, $link = NULL);
                }

            }

            $i++;

        }
    }
    
    if($v['op'] == 'other') {
        //size
        $i = 1;
        $file = fopen(drupal_get_path('module', 'leaf_consignor') . '/csv/size.csv', 'r');
        while(! feof($file))
        {
            $data = fgetcsv($file);
            if($i > 1 && !empty($data[0])) {
                $nid = db_insert('leaf_size_magento') // Table name no longer needs {}
                ->fields(array(
                    'id' => $data[0],
                    'name' => $data[1],
                ))
                ->execute();
            }

            $i++;
        }
        //color
        $i = 1;
        $file = fopen(drupal_get_path('module', 'leaf_consignor') . '/csv/color.csv', 'r');
        while(! feof($file))
        {
            $data = fgetcsv($file);
            if($i > 1 && !empty($data[0])) {
                $nid = db_insert('leaf_color') // Table name no longer needs {}
                ->fields(array(
                    'id' => $data[0],
                    'name' => $data[1],
                ))
                ->execute();
            }

            $i++;
        }
        
        //brand
        $i = 1;
        $file = fopen(drupal_get_path('module', 'leaf_consignor') . '/csv/brand.csv', 'r');
        while(! feof($file))
        {
            $data = fgetcsv($file);
            if($i > 1 && !empty($data[0])) {
                $nid = db_insert('leaf_brand') // Table name no longer needs {}
                ->fields(array(
                    'brand_id' => $data[0],
                    'brand_name' => $data[1],
                ))
                ->execute();
            }

            $i++;
        }
    }
}