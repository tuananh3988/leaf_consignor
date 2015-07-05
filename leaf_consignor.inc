<?php

function form_collection_contract($form, &$form_state, $no_js_use = FALSE) {
    //select payment method
    $payment = db_select('leaf_payment', 'v')
            ->fields('v')
            ->execute()
            ->fetchAll();
    $optionpayment = array();
    foreach ($payment as $v) {
        $optionpayment[$v->id] = $v->name;
    }

    $form = array();
    $form['#tree'] = TRUE;
    $formNewContract = form_new_contract(true);
    $form = array_merge($form, $formNewContract);
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Submit',
        '#weight' => '10',
    );

    if ($no_js_use) {
        // Remove the #ajax from the above, so ajax.js won't be loaded.
        if (!empty($form['names_fieldset']['remove_name']['#ajax'])) {
            unset($form['names_fieldset']['remove_name']['#ajax']);
        }
        unset($form['names_fieldset']['add_name']['#ajax']);
    }
    return $form;
}

function form_collection_consignor($form, &$form_state, $no_js_use = FALSE) {
    $form = array();
    $form['#tree'] = TRUE;
    $formNewVendor = form_new_vendor();
    $formNewContract = form_new_contract();
    $form = array_merge($form, $formNewVendor, $formNewContract);

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Submit',
        '#weight' => '10',
    );

    if ($no_js_use) {
        // Remove the #ajax from the above, so ajax.js won't be loaded.
        if (!empty($form['names_fieldset']['remove_name']['#ajax'])) {
            unset($form['names_fieldset']['remove_name']['#ajax']);
        }
        unset($form['names_fieldset']['add_name']['#ajax']);
    }
    return $form;
}

function ajax_example_add_more_callback($form, $form_state) {
    return $form['wrap-product'];
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function ajax_example_add_more_add_one($form, &$form_state) {
    $form_state['num_names'] ++;
    $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "remove one" button.
 *
 * Decrements the max counter and causes a form rebuild.
 */
function ajax_example_add_more_remove_one($form, &$form_state) {
    if ($form_state['num_names'] > 1) {
        $form_state['num_names'] --;
    }
    $form_state['rebuild'] = TRUE;
}

function form_collection_consignor_validate($form, &$form_state) {

}

/**
 * Submit function for form_example_tutorial_9().
 *
 * @ingroup form_example
 */
function form_collection_consignor_submit($form, &$form_state) {
    GLOBAL $user;

    $vendor = $form_state['values']['new_1422973868224'];
    //add to table vendor
    $vendorId = db_insert('leaf_vendor') // Table name no longer needs {}
            ->fields(array(
                'vendor_uuid' => str_replace('.','',microtime(true)), // use microtime instead time() + vendor_id
                'name' => $vendor['new_1422973901058'],
                'address' => $vendor['new_1422973944836'],
                'city' => $vendor['new_1422973972442'],
                'state' => $vendor['new_1422974065842'],
                'zip' => $vendor['new_1422974068093'],
                'email_address' => $vendor['new_1422974070252'],
                'phone_number' => $vendor['new_1422974138424'],
                'alternate_phone_number' => $vendor['new_1422974164604'],
                'payment_id' => $vendor['new_1422974198240'],
                'user_id' => $user->uid,
                'created_date' => time(),
            ))
            ->execute();


    $contractId = save_contract_products($form_state, $vendorId);
    drupal_goto('/contract/summary/' . $contractId);
}

function form_collection_contract_submit($form, &$form_state) {
    $contractId = save_contract_products($form_state);
    drupal_goto('contract/summary/' . $contractId);
}

function update_number_of_contract($vendorId) {
    db_query("UPDATE leaf_vendor "
            . "SET number_of_contract = "
            . "(SELECT COUNT(leaf_contract.contract_id) "
            . "FROM leaf_contract "
            . "WHERE leaf_contract.vendor_id = :vendorId "
            . "AND leaf_contract.delete_flag = 0) "
            . "WHERE leaf_vendor.vendor_id = :vendorId", [':vendorId' => $vendorId]);
}


function save_contract_products($form_state, $vendorId = null) {
    GLOBAL $user;
    $products = $form_state['values']['wrap-product']['new_1423153824585'];
    $count = 0;
    $contract = $form_state['values']['new_1422974499389'];
    if ($vendorId == null) {
        $pieces = explode(" - ", $contract['new_1423064090214']);

        $vendorUuid = substr($pieces[0], 1);

        $result = db_select('leaf_vendor', 'n')
            ->fields('n')
            ->condition('vendor_uuid', $vendorUuid, '=')
            ->execute()
            ->fetchAssoc();

        $vendorId = $result['vendor_id'];


    }
    //add to table contract
    $closeDate = strtotime($contract['new_142297464756']['year'] . '-' . $contract['new_142297464756']['month'] . '-' . $contract['new_142297464756']['day']);
    $contractId = db_insert('leaf_contract') // Table name no longer needs {}
            ->fields(array(
                'contract_uuid' => str_replace('.','',microtime(true)),
                'from_date' => strtotime($contract['new_1422974647565']['year'] . '-' . $contract['new_1422974647565']['month'] . '-' . $contract['new_1422974647565']['day']),
                'close_date' => $closeDate,
                'claim_date' => $closeDate + 24 * 60 * 60,
                'vendor_id' => $vendorId,
                'vendor_category' => $contract['new_1422974647564'],
                'revenue_sharing' => $contract['new_1422974650384'],
                'offer_price' => $contract['new_1422974652868'],
                'number_items_off' => $contract['new_1422974693681'],
                'number_items_accepeted' => $contract['new_1422974696174'],
                'number_items_rejected' => $contract['new_1422974699525'],
                'shop_store_id' => $contract['new_1422974647567'],
                'user_id' => $user->uid,
                'created_date' => time(),
            ))
            ->execute();

    update_number_of_contract($vendorId);
    //add to table product
    $totalPayout = 0;
    $totalPrice = 0;
    foreach ($products as $record) {
        if (!empty($record['new_1423157371520'])) {
            $data = [
                'contract_id' => $contractId,
                'sku' => str_replace('.','',microtime(true)),
                'name' => $record['new_1423157371520'],
                'item_category_id' => $record['new_1423157401928'],
                'description' => $record['new_1423157454597'],
                'price' => $record['new_1423157470669'],
                'color_id' => $record['new_1423157485434'],
                'size' => $record['new_1423157529185'],
                'brand_id' => $record['new_1423157540495'],
                'consignment_price' => $record['new_1423157549492'],
                'purchase_price' => $record['new_1423157549494'],
                'quantity' => $record['new_1423157549495'],
                'remain_quantity' => $record['new_1423157549495'],
                'status' => $record['new_1423157549496'],
            ];

            $productId = db_insert('leaf_product')->fields($data)->execute();
            $count++;
            //total payout and total price
            if($data['status'] == 1) {
                $totalPayout += $data['consignment_price'];
            }

            $totalPrice += $data['price'] * $data['quantity'];
        }
    }

    $totalPayout = ($contract['new_1422974650384'] * $totalPayout) / 100;

    //update contract
    db_update('leaf_contract') // Table name no longer needs {}
        ->fields(array(
            'number_of_product' => $count,
            'total_price' => $totalPrice,
            'total_payout' => $totalPayout,
        ))
        ->condition('contract_id', $contractId, '=')
        ->execute();

    return $contractId;
}

function form_new_vendor() {
    //select payment method
    $payment = db_select('leaf_payment', 'v')
            ->fields('v')
            ->execute()
            ->fetchAll();
    $optionpayment = array();
    foreach ($payment as $v) {
        $optionpayment[$v->id] = $v->name;
    }

    $form['new_1422973868224'] = array(
        '#weight' => '0',
        '#title' => t('New Vendor'),
        '#collapsed' => '0',
        '#type' => 'fieldset',
        '#collapsible' => '0',
    );
    $form['new_1422973868224']['new_1422973901058'] = array(
        '#weight' => '0',
        '#required' => '1',
        '#type' => 'textfield',
        '#title' => t('Name'),
    );
    $form['new_1422973868224']['new_1422973944836'] = array(
        '#weight' => '1',
        '#required' => '1',
        '#type' => 'textfield',
        '#title' => t('Address'),
    );
    $form['new_1422973868224']['new_1422973972442'] = array(
        '#weight' => '2',
        '#required' => '1',
        '#type' => 'textfield',
        '#title' => t('City'),
    );
    $form['new_1422973868224']['new_1422974065842'] = array(
        '#weight' => '3',
        '#required' => '1',
        '#type' => 'textfield',
        '#title' => t('State'),
    );
    $form['new_1422973868224']['new_1422974068093'] = array(
        '#weight' => '4',
        '#required' => '1',
        '#type' => 'textfield',
        '#title' => t('Zip'),
    );
    $form['new_1422973868224']['new_1422974070252'] = array(
        '#weight' => '5',
        '#required' => '1',
        '#type' => 'textfield',
        '#title' => t('Email Address'),
    );
    $form['new_1422973868224']['new_1422974138424'] = array(
        '#weight' => '6',
        '#required' => '1',
        '#type' => 'textfield',
        '#title' => t('Phone Number'),
    );
    $form['new_1422973868224']['new_1422974164604'] = array(
        '#weight' => '7',
        '#required' => '0',
        '#type' => 'textfield',
        '#title' => t('Alternate Phone Number'),
    );
    $form['new_1422973868224']['new_1422974198240'] = array(
        '#weight' => '8',
        '#required' => '1',
        '#multiple' => '0',
        '#key_type_toggled' => '1',
        '#type' => 'select',
        '#options' => $optionpayment,
        '#multiple_toggle' => '1',
        '#title' => t('Payment Methor'),
    );

    return $form;
}

function form_new_contract($vendor = false) {

    //select Stores location
    $stores = db_select('shop_stores', 'v')
            ->fields('v')
            ->execute()
            ->fetchAll();
    $optionStore = array();
    foreach ($stores as $v) {
        $optionStore[$v->id] = '#' . $v->shop_code . ' - ' .$v->name;
    }

    //select color
    $color = db_select('leaf_color', 'v')
            ->fields('v')
            ->execute()
            ->fetchAll();
    $optionColor = array();
    foreach ($color as $v) {
        $optionColor[$v->id] = $v->name;
    }

    //select item catefory
    $items = db_select('leaf_item_category', 'v')
            ->fields('v')
            ->execute()
            ->fetchAll();
    $optionItemCategory = array();
    foreach ($items as $v) {
        $optionItemCategory[$v->id] = $v->name;
    }

    //select brand list
    $items = db_select('leaf_brand', 'v')
            ->fields('v')
            ->execute()
            ->fetchAll();
    $optionBrand = array();
    foreach ($items as $v) {
        $optionBrand[$v->brand_id] = $v->brand_name;
    }

    $form['new_1422974499389'] = array(
        '#title' => t('New Contract'),
        '#weight' => '1',
        '#type' => 'fieldset',
        '#collapsible' => '0',
        '#collapsed' => '0',
        '#attributes' => array(
            'class' => array('contract-fieldset'),
        )
    );

    if ($vendor) {
        //select item catefory
        $items = db_select('leaf_vendor', 'v')
                ->fields('v')
                ->execute()
                ->fetchAll();
        $optionVendor = array();
        foreach ($items as $v) {
            $optionVendor[$v->vendor_id] = '#' . $v->vendor_uuid . ' - ' . $v->name;
        }

        $form['new_1422974499389']['new_1423064090214'] = array(
            '#weight' => '-2',
            '#required' => '1',
            //'#multiple' => '0',
            '#key_type_toggled' => '1',
            '#type' => 'textfield',
            //'#options' => $optionVendor,
            '#autocomplete_path' => 'vendor/autocomplete',
            '#multiple_toggle' => '1',
            '#title' => t('Vendor id'),
        );

//        $vd = arg(1);
//        if (!empty($vd)) {
//            $form['new_1422974499389']['new_1423064090214']['#default_value'] = $vd;
//        }
    }


    $form['new_1422974499389']['new_1422974647564'] = array(
        '#weight' => '-1',
        '#required' => '0',
        '#required' => '0',
        '#type' => 'textfield',
        '#title' => t('Vendor Category'),
    );

    $form['new_1422974499389']['new_1422974647565'] = array(
        '#weight' => '0',
        '#required' => '1',
        '#type' => 'date',
        '#title' => t('From date'),
    );
    $form['new_1422974499389']['new_142297464756'] = array(
        '#weight' => '1',
        '#required' => '1',
        '#type' => 'date',
        '#default_value' => ['year' => date('Y'), 'month' => date('n', strtotime("+2 month")), 'day' => date('j')],
        '#title' => t('Close date'),
    );
    $form['new_1422974499389']['new_1422974647567'] = array(
        '#weight' => '2',
        '#required' => '1',
        '#multiple' => '0',
        '#key_type_toggled' => '1',
        '#type' => 'select',
        '#options' => $optionStore,
        '#multiple_toggle' => '1',
        '#title' => t('Store locations'),
    );
    $form['new_1422974499389']['new_1422974650384'] = array(
        '#weight' => '3',
        '#required' => '1',
        '#type' => 'textfield',
        '#title' => t('Revenue Sharing (%) '),
    );
    $form['new_1422974499389']['new_1422974652868'] = array(
        '#weight' => '4',
        '#required' => '0',
        '#type' => 'textfield',
        '#title' => t('Offer Price'),
    );
    $form['new_1422974499389']['new_1422974693681'] = array(
        '#weight' => '5',
        '#required' => '0',
        '#type' => 'textfield',
        '#title' => t('Number of items DROPPED OFF'),
    );
    $form['new_1422974499389']['new_1422974696174'] = array(
        '#weight' => '6',
        '#required' => '0',
        '#type' => 'textfield',
        '#title' => t('Number of items accepeted'),
    );
    $form['new_1422974499389']['new_1422974699525'] = array(
        '#weight' => '7',
        '#required' => '0',
        '#type' => 'textfield',
        '#title' => t('Number of items rejected'),
    );




    $form['wrap-product'] = array(
        '#weight' => '3',
        '#title' => t('New product'),
        '#type' => 'fieldset',
        '#collapsible' => '1',
        '#collapsed' => '0',
        '#prefix' => '<div id="product-fieldset-wrapper">',
        '#suffix' => '</div>',
    );

    if (empty($form_state['num_names'])) {
        $form_state['num_names'] = 10;
    }
    for ($i = 0; $i < $form_state['num_names']; $i++) {
        $form['wrap-product']['new_1423153824585'][$i] = array(
            '#weight' => $i,
            //'#title' => t('New product'),
            '#type' => 'fieldset',
            '#collapsible' => '1',
            '#collapsed' => '0',
            '#attributes' => array(
                'class' => array(
                    'product-fiedset',
                ),
            ),
        );
        $form['wrap-product']['new_1423153824585'][$i]['new_1423157371520'] = array(
            '#weight' => '0',
            '#required' => '0',
            '#type' => 'textfield',
            '#size' => 7,
        );
        $form['wrap-product']['new_1423153824585'][$i]['new_1423157401928'] = array(
            '#weight' => '1',
            '#required' => '0',
            '#multiple' => '0',
            '#key_type_toggled' => '0',
            '#type' => 'select',
            '#options' => $optionItemCategory,
            '#multiple_toggle' => '1',
        );
        $form['wrap-product']['new_1423153824585'][$i]['new_1423157454597'] = array(
            '#weight' => '2',
            '#required' => '0',
            '#type' => 'textfield',
            '#size' => 7,
        );
        $form['wrap-product']['new_1423153824585'][$i]['new_1423157470669'] = array(
            '#weight' => '3',
            '#required' => '0',
            '#type' => 'textfield',
            '#size' => 7,
        );
        $form['wrap-product']['new_1423153824585'][$i]['new_1423157485434'] = array(
            '#weight' => '4',
            '#required' => '0',
            '#multiple' => '0',
            '#key_type_toggled' => '0',
            '#type' => 'select',
            '#options' => $optionColor,
            '#multiple_toggle' => '1',
        );
        $form['wrap-product']['new_1423153824585'][$i]['new_1423157529185'] = array(
            '#weight' => '5',
            '#required' => '0',
            '#type' => 'textfield',
            '#size' => 7,
        );
        $form['wrap-product']['new_1423153824585'][$i]['new_1423157540495'] = array(
            '#weight' => '6',
            '#required' => '0',
            '#multiple' => '0',
            '#key_type_toggled' => '0',
            '#type' => 'select',
            '#options' => $optionBrand,
            '#multiple_toggle' => '1',
            '#attributes' => array(
                'class' => array('brand_list')
            ),
        );

        $form['wrap-product']['new_1423153824585'][$i]['new_1423157549492'] = array(
            '#weight' => '7',
            '#required' => '0',
            '#type' => 'textfield',
            '#size' => 7,
        );

        $form['wrap-product']['new_1423153824585'][$i]['new_1423157549494'] = array(
            '#weight' => '8',
            '#required' => '0',
            '#type' => 'textfield',
            '#size' => 7,
        );

        $form['wrap-product']['new_1423153824585'][$i]['new_1423157549495'] = array(
            '#weight' => '9',
            '#required' => '0',
            '#type' => 'textfield',
            '#size' => 7,
        );

        $form['wrap-product']['new_1423153824585'][$i]['new_1423157549496'] = array(
            '#weight' => '10',
            '#required' => '0',
            '#multiple' => '0',
            '#key_type_toggled' => '0',
            '#type' => 'select',
            '#options' => [0 => 'in stock', 1 => 'sold'],
        );

        if ($i == 0) {
            $form['wrap-product']['new_1423153824585'][$i]['new_1423157371520']['#title'] = t('Name');
            $form['wrap-product']['new_1423153824585'][$i]['new_1423157401928']['#title'] = t('Item category');
            $form['wrap-product']['new_1423153824585'][$i]['new_1423157454597']['#title'] = t('Description');
            $form['wrap-product']['new_1423153824585'][$i]['new_1423157470669']['#title'] = t('Price');
            $form['wrap-product']['new_1423153824585'][$i]['new_1423157485434']['#title'] = t('Color');
            $form['wrap-product']['new_1423153824585'][$i]['new_1423157529185']['#title'] = t('Size');
            $form['wrap-product']['new_1423153824585'][$i]['new_1423157540495']['#title'] = t('Brands list');
            $form['wrap-product']['new_1423153824585'][$i]['new_1423157549492']['#title'] = t('Consignment price');
            $form['wrap-product']['new_1423153824585'][$i]['new_1423157549494']['#title'] = t('Purchase price');
            $form['wrap-product']['new_1423153824585'][$i]['new_1423157549495']['#title'] = t('Quantity');
            $form['wrap-product']['new_1423153824585'][$i]['new_1423157549496']['#title'] = t('Status');
        }
    }


    $form['wrap-product']['add_name'] = array(
        '#type' => 'submit',
        '#value' => t('Add one more'),
        '#submit' => array('ajax_example_add_more_add_one'),
        '#weight' => '8',
        // See the examples in ajax_example.module for more details on the
        // properties of #ajax.
        '#ajax' => array(
            'callback' => 'ajax_example_add_more_callback',
            'wrapper' => 'product-fieldset-wrapper',
            'prevent' => 'submit click mousedown',
        ),
        '#limit_validation_errors' => array(),
    );
    if ($form_state['num_names'] > 1) {
        $form['wrap-product']['remove_name'] = array(
            '#type' => 'submit',
            '#weight' => '9',
            '#value' => t('Remove one'),
            '#submit' => array('ajax_example_add_more_remove_one'),
            '#ajax' => array(
                'callback' => 'ajax_example_add_more_callback',
                'wrapper' => 'product-fieldset-wrapper',
                'prevent' => 'submit click mousedown',
            ),
            '#limit_validation_errors' => array(),
        );
    }

    return $form;
}
