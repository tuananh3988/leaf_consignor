<?php

function form_export_consignor_data($form, &$form_state, $no_js_use = FALSE)
{

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

function form_export_consignor_data_submit($form, &$form_state)
{
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
        $sheet = $xls->setActiveSheetIndex(2);
        $vendor = db_select('leaf_vendor', 'v')
                ->fields('v')
                ->execute()
                ->fetchAll();

        $startRow = 9;
        foreach($vendor as $v) {
            $sheet->setCellValueByColumnAndRow(0, $startRow, $v->name);
            $sheet->setCellValueByColumnAndRow(1, $startRow, $v->name);
            $sheet->setCellValueByColumnAndRow(0, $startRow, $v->name);
        }
        var_dump($vendor);die;
        $sheet->setCellValueByColumnAndRow(0, 9, 'abcxyz');

        $writer = new PHPExcel_Writer_Excel5($xls);
        $writer->save($path);

        //$result = phpexcel_export($headers, $data, $path, $options);

        //var_dump($result);die;
    }
}
