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


        $name = 'QB_POS_' . date('Ymd') . '.xls';

        $path = export_consignor_data($name);

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

function export_consignor_data($name) {
    $path = 'public://xls/';
    if (!is_dir($path)) {
        mkdir($path, 0777);
    }

    $options = [
        'template' => drupal_get_path('module', 'leaf_consignor') . '/templates/excel/QB POS Import Template.xls',
        'format' => 'xls',
        'ignore_headers' => TRUE,
    ];

    $path .= $name;
    $library = libraries_load('PHPExcel');
    $xls_reader = PHPExcel_IOFactory::createReaderForFile($options['template']);
    $xls = $xls_reader->load($options['template']);
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
    $products->addField('ic', 'type', 'type');
    $products = $products->execute()
            ->fetchAll();

    $startRow = 3;

    foreach ($products as $v) {
        $sheet->setCellValueByColumnAndRow(0, $startRow, $v->sku);
        $sheet->setCellValueByColumnAndRow(1, $startRow, $v->name);
        $sheet->setCellValueByColumnAndRow(2, $startRow, $v->type);
        $sheet->setCellValueByColumnAndRow(3, $startRow, $v->item_category_name);
        $sheet->setCellValueByColumnAndRow(4, $startRow, $v->size);
        $sheet->setCellValueByColumnAndRow(5, $startRow, $v->price);
        //$sheet->setCellValueByColumnAndRow(6, $startRow, $v->tax);
        $sheet->setCellValueByColumnAndRow(7, $startRow, $v->vendor_name);
        $sheet->setCellValueByColumnAndRow(8, $startRow, $v->remain_quantity);
        $sheet->setCellValueByColumnAndRow(9, $startRow, $v->vendor_uuid);
        $sheet->setCellValueByColumnAndRow(10, $startRow, $v->contract_uuid);
        $sheet->setCellValueByColumnAndRow(11, $startRow, date('Y-m-d', $v->from_date));
        $sheet->setCellValueByColumnAndRow(12, $startRow, date('Y-m-d', $v->close_date));
        $sheet->setCellValueByColumnAndRow(13, $startRow, date('Y-m-d', $v->claim_date));
        $sheet->setCellValueByColumnAndRow(14, $startRow, 'Yes');

        $startRow++;
    }

    $writer = new PHPExcel_Writer_Excel5($xls);
    $writer->save($path);

    return $path;
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
        import_consignor_data($file);
    }
}

function import_consignor_data($file) {
    $library = libraries_load('PHPExcel');
    $xls_reader = PHPExcel_IOFactory::createReaderForFile($file->uri);

    $xls = $xls_reader->load($file->uri);

    $sheet = $xls->setActiveSheetIndex(0);
    $k = 1;
    foreach ($sheet->getRowIterator() as $row) {
        if ($k > 2) {
            $cellIterator = $row->getCellIterator();
            $cellIterator->setIterateOnlyExistingCells(false);
            $i = 1;
            $v = [];
            foreach ($cellIterator as $cell) {
                if (!is_null($cell)) {
                    $v[$i] = $cell->getCalculatedValue();
                }

                $i++;
            }
            
            $product = db_select('leaf_product', 'p')
                    ->fields('p')
                    ->condition('p.sku', $v[1], '=')
                    ->execute()
                    ->fetchAllAssoc('product_id');

            if (count($product) > 1) {
                watchdog('leaf_consignor', 'sku duplicated with sku id: %id', ['%id' => $v[1]], WATCHDOG_WARNING);
            } else {
                if (!empty($product)) {
                    $product = array_values($product)[0];
                    if ($v[9] < $product->remain_quantity) {
                        db_update('leaf_product')
                                ->fields(array(
                                    'remain_quantity' => $v[9],
                                    'status' => 1,
                                ))
                                ->condition('product_id', $product->product_id, '=')
                                ->execute();

                        drupal_set_message('update product sku ' . $v[1]);
                    }
                }
            }
        }

        $k++;
    }
    
    drupal_set_message('Update success!');
}
