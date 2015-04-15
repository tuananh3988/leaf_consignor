<?php

function form_export_consignor_data($form, &$form_state, $no_js_use = FALSE) {

    $form['xls'] = array(
        '#type' => 'fieldset',
        '#title' => t('Export xls.'),
        '#weight' => 1,
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
    );

    $form['xls']['export_xls'] = array(
        '#type' => 'submit',
        '#value' => 'Export xls',
        '#weight' => '1',
    );

    return $form;
}

function form_export_consignor_data_submit($form, &$form_state) {
    $v = $form_state['values'];
    if ($v['op'] == 'Export xls') {
        module_load_include('inc', 'phpexcel');


        $headers = array();
        $options = [
            'template' => drupal_get_path('module', 'leaf_consignor') . '/templates/excel/QB POS Import Template.xls',
            'format' => 'xls',
            'ignore_headers' => TRUE,
        ];

        $path = 'public://xls/';
        if (!is_dir($path)) {
            mkdir($path, 0777);
        }

        $name = 'QB_POS_' . date('Ymd') . '.xls';

        $path .= $name;

        $library = libraries_load('PHPExcel');
        $xls_reader = PHPExcel_IOFactory::createReaderForFile($options['template']);

        $xls = $xls_reader->load($options['template']);

        //export vendor
//        $sheet = $xls->setActiveSheetIndex(2);
//        $vendor = db_select('leaf_vendor', 'v');
//        $vendor->leftJoin('leaf_payment', 'p', 'p.id = v.payment_id');
//        $vendor->fields('v')
//                ->addField('p', 'name', 'pname');
//        $vendor = $vendor->execute()
//                ->fetchAll();
//
//        $startRow = 9;
//
//        foreach ($vendor as $v) {
//            $contracts = db_select('leaf_contract', 'c')
//                    ->fields('c')
//                    ->condition('vendor_id', $v->vendor_id, '=')
//                    ->execute()
//                    ->fetchAll();
//
//            $contractStr = [];
//            foreach ($contracts as $c) {
//                $contractStr[] = $c->contract_uuid;
//            }
//
//            $sheet->setCellValueByColumnAndRow(0, $startRow, $v->name);
//            $sheet->setCellValueByColumnAndRow(4, $startRow, $v->address);
//            $sheet->setCellValueByColumnAndRow(5, $startRow, $v->city);
//            $sheet->setCellValueByColumnAndRow(6, $startRow, $v->state);
//            $sheet->setCellValueByColumnAndRow(7, $startRow, $v->zip);
//            $sheet->setCellValueByColumnAndRow(8, $startRow, $v->phone_number);
//            $sheet->setCellValueByColumnAndRow(9, $startRow, $v->alternate_phone_number);
//            $sheet->setCellValueByColumnAndRow(12, $startRow, $v->vendor_uuid);
//            $sheet->setCellValueByColumnAndRow(13, $startRow, $v->vendor_uuid);
//            $sheet->setCellValueByColumnAndRow(17, $startRow, $v->email_address);
//            $sheet->setCellValueByColumnAndRow(19, $startRow, $v->pname);
//            $sheet->setCellValueByColumnAndRow(26, $startRow, implode(', ', $contractStr));
//            $startRow++;
//        }

        //export product
        $sheet = $xls->getActiveSheet();
        $products = db_select('leaf_product', 'p');
        $products->leftJoin('leaf_contract', 'c', 'c.contract_id = p.contract_id');
        $products->leftJoin('leaf_vendor', 'v', 'v.vendor_id = c.vendor_id');
        $products->leftJoin('leaf_item_category', 'ic', 'ic.id = p.item_category_id');
        $products->fields('p')
                ->fields('c', ['contract_uuid', 'from_date', 'close_date', 'claim_date']);
        $products->addField('v', 'name', 'vendor_name');
        $products->addField('v', 'vendor_uuid', 'vendor_uuid');
        $products->addField('ic', 'name', 'item_category_name');
        $products = $products->execute()
                ->fetchAll();

        $startRow = 11;

        foreach ($products as $v) {
            $sheet->setCellValueByColumnAndRow(0, $startRow, $v->sku);
            $sheet->setCellValueByColumnAndRow(1, $startRow, $v->name);
            $sheet->setCellValueByColumnAndRow(2, $startRow, $v->item_category_name);
            $sheet->setCellValueByColumnAndRow(3, $startRow, $v->item_category_name);
            $sheet->setCellValueByColumnAndRow(4, $startRow, $v->description);
            $sheet->setCellValueByColumnAndRow(5, $startRow, $v->size);
            $sheet->setCellValueByColumnAndRow(6, $startRow, $v->vendor_name);
            $sheet->setCellValueByColumnAndRow(7, $startRow, $v->vendor_uuid);
            $sheet->setCellValueByColumnAndRow(8, $startRow, $v->price);
            //$sheet->setCellValueByColumnAndRow(20, $startRow, $v->tax);
            $sheet->setCellValueByColumnAndRow(10, $startRow, $v->quantity);
            $sheet->setCellValueByColumnAndRow(11, $startRow, $v->contract_id);
            $sheet->setCellValueByColumnAndRow(12, $startRow, date('Y-m-d', $v->from_date));
            $sheet->setCellValueByColumnAndRow(13, $startRow, date('Y-m-d', $v->close_date));
            $sheet->setCellValueByColumnAndRow(14, $startRow, date('Y-m-d', $v->claim_date));
            $sheet->setCellValueByColumnAndRow(14, $startRow, date('Y-m-d', $v->claim_date));
            $startRow++;
        }

        $writer = new PHPExcel_Writer_Excel5($xls);
        $writer->save($path);

        $http_headers = array(
            'Content-Type' => 'text/csv',
            'Content-Disposition' => 'attachment; filename="' . $name . '"',
            'Content-Length' => filesize($path),
        );
        if (strpos($_SERVER['HTTP_USER_AGENT'], 'MSIE')) {
            $http_headers['Cache-Control'] = 'must-revalidate, post-check=0, pre-check=0';
            $http_headers['Pragma'] = 'public';
        } else {
            $http_headers['Pragma'] = 'no-cache';
        }


        file_transfer($path, $http_headers);
    }
}

function form_import_consignor_data($form, &$form_state, $no_js_use = FALSE) {

    $form['xls'] = array(
        '#type' => 'fieldset',
        '#title' => t('Import Pos xls.'),
        '#weight' => 1,
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
    );

    $form['#attributes']['enctype'] = "multipart/form-data";

    $form['xls']['upload'] = array(
        '#type' => 'managed_file',
        '#name' => 'xls_pos',
        '#title' => t('Import xls'),
        '#size' => 40,
        '#description' => t("File should xls format."),
        '#upload_location' => 'public://xls/'
    );

    $form['xls']['import_xls'] = array(
        '#type' => 'submit',
        '#value' => 'Import xls',
        '#weight' => '1',
    );

    return $form;
}

function form_import_consignor_data_submit($form, &$form_state) {
    if (isset($form_state['values']['upload'])) {
        $file = file_load($form_state['values']['upload']);
        $library = libraries_load('PHPExcel');
        $xls_reader = PHPExcel_IOFactory::createReaderForFile($file->uri);

        $xls = $xls_reader->load($file->uri);
        
        $sheet = $xls->setActiveSheetIndex(2);
        
        $a = $sheet->getCellByColumnAndRow(0,9)->getValue();
        
        var_dump($a);die;
    }
}
