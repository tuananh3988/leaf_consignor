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
        '#weight' => '1',
    );

    return $form;
}

function form_update_csv_submit($form, &$form_state) {
    $v = $form_state['values'];
    if($v['op'] == 'vendor') {
        $i = 1;
        $file = fopen('D:\xampp\htdocs\drupal-7.34\sites\all\modules\leaf_consignor\csv\vendor.csv', 'r');
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
                    'email_address' => $data[6],
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

    if($v['op'] == 'contract') {
        $i = 1;
        $file = fopen('D:\xampp\htdocs\drupal-7.34\sites\all\modules\leaf_consignor\csv\contract.csv', 'r');
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
}