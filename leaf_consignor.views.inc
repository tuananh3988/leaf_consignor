<?php

/* Define Views interface for custom data tables */

function leaf_consignor_views_data() {
    $data = array();

    $data['Leaf Consignor/Contract']['moved to'] = 'leaf_contract';
    $data['leaf_contract']['table']['group'] = t('Contract');

    $data['leaf_contract']['table']['join'] = array(
        // Link ourself to the {file_managed} table so we can provide file->entity relationships.
        'leaf_vendor' => array(
            'field' => 'vendor_id',
            'left_field' => 'vendor_id',
        ),
    );

    // Define this as a base table – a table that can be described in itself by
    // views (and not just being brought in as a relationship). In reality this
    // is not very useful for this table, as it isn't really a distinct object of
    // its own, but it makes a good example.
    $data['leaf_contract']['table']['base'] = array(
        'field' => 'contract_id', // This is the identifier field for the view.
        'title' => t('Contract'),
        'help' => t('Contract.'),
        'weight' => -10,
    );


    $data['leaf_contract']['contract_id'] = array(
        'title' => t('Contract Id field'),
        'help' => t('Just a numeric field.'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
    );

    $data['leaf_contract']['vendor_id'] = array(
        'title' => t('vendor Id field'),
        'help' => t('Just a numeric field.'),
        'relationship' => array(
            'base' => 'leaf_vendor', // The name of the table to join with.
            'base field' => 'vendor_id', // The name of the field on the joined table.
            // 'field' => 'nid' -- see hook_views_data_alter(); not needed here.
            'handler' => 'views_handler_relationship',
            'label' => t('Default label for the relationship'),
            'title' => t('leaf_vendor relationship'),
            'help' => t('More information on this relationship'),
        ),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
    );

    // Example plain text field.
    $data['leaf_contract']['revenue_sharing'] = array(
        'title' => t('revenue sharing field'),
        'help' => t('revenue_sharing.'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE, // This is use by the table display plugin.
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );

    // Example plain text field.
    $data['leaf_contract']['contract_uuid'] = array(
        'title' => t('Contract uuid field'),
        'help' => t('Contract uuid'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE, // This is use by the table display plugin.
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );
    
    $data['leaf_contract']['total_price'] = array(
        'title' => t('Total Price'),
        'help' => t('Total Price'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE, // This is use by the table display plugin.
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );
    
    $data['leaf_contract']['total_payout'] = array(
        'title' => t('Total Payout'),
        'help' => t('Total Payout'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE, // This is use by the table display plugin.
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );

    $data['leaf_contract']['number_of_product'] = array(
        'title' => t('Numbers of product'),
        'help' => t('Numbers of product.'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
    );

    $data['leaf_contract']['from_date'] = array(
        'title' => t('From date field'),
        'help' => t('Just a timestamp field.'),
        'field' => array(
            'handler' => 'views_handler_field_date',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort_date',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_date',
        ),
    );

    $data['leaf_contract']['close_date'] = array(
        'title' => t('Close date field'),
        'help' => t('Just a timestamp field.'),
        'field' => array(
            'handler' => 'views_handler_field_date',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort_date',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_date',
        ),
    );
    
    $data['leaf_contract']['claim_date'] = array(
        'title' => t('Claim date field'),
        'help' => t('Just a timestamp field.'),
        'field' => array(
            'handler' => 'views_handler_field_date',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort_date',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_date',
        ),
    );
    
    $data['leaf_contract']['delete_flag'] = array(
        'title' => t('Delete flag'),
        'help' => t('Just an on/off field.'),
        'field' => array(
            'handler' => 'views_handler_field_boolean',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_boolean_operator',
            // Note that you can override the field-wide label:
            'label' => t('Published'),
            // This setting is used by the boolean filter handler, as possible option.
            'type' => 'yes-no',
            // use boolean_field = 1 instead of boolean_field <> 0 in WHERE statment.
            'use equal' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
    );

    $data['leaf_vendor']['table']['group'] = t('Vendor');

    // Define this as a base table – a table that can be described in itself by
    // views (and not just being brought in as a relationship). In reality this
    // is not very useful for this table, as it isn't really a distinct object of
    // its own, but it makes a good example.
    $data['leaf_vendor']['table']['base'] = array(
        'field' => 'vendor_id', // This is the identifier field for the view.
        'title' => t('Vendor'),
        'help' => t('Vendor.'),
        'weight' => -10,
    );

    // Example plain text field.
    $data['leaf_vendor']['name'] = array(
        'title' => t('Vendor name field'),
        'help' => t('Vendor name.'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE, // This is use by the table display plugin.
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );

    $data['leaf_vendor']['vendor_id'] = array(
        'title' => t('Vendor Id field'),
        'help' => t('Just a numeric field.'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_numeric',
        ),
    );
    
    $data['leaf_vendor']['number_of_contract'] = array(
        'title' => t('Numbers of contract'),
        'help' => t('Numbers of contract.'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
    );

    $data['leaf_vendor']['vendor_uuid'] = array(
        'title' => t('Vendor uuid field'),
        'help' => t('Vendor uuid.'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE, // This is use by the table display plugin.
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );

    // Example plain text field.
    $data['leaf_vendor']['address'] = array(
        'title' => t('Address field'),
        'help' => t('Address.'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE, // This is use by the table display plugin.
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );

    $data['leaf_vendor']['created_date'] = array(
        'title' => t('Created date field'),
        'help' => t('Created date.'),
        'field' => array(
            'handler' => 'views_handler_field_date',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort_date',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_date',
        ),
    );
    $data['leaf_vendor']['delete_flag'] = array(
        'title' => t('Delete Flag'),
        'help' => t('Just an on/off field.'),
        'field' => array(
            'handler' => 'views_handler_field_boolean',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_boolean_operator',
            // Note that you can override the field-wide label:
            'label' => t('Published'),
            // This setting is used by the boolean filter handler, as possible option.
            'type' => 'yes-no',
            // use boolean_field = 1 instead of boolean_field <> 0 in WHERE statment.
            'use equal' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
    );
    
    
    $data['leaf_product']['table']['group'] = t('Product');
    
    $data['leaf_product']['table']['base'] = array(
        'field' => 'product_id', // This is the identifier field for the view.
        'title' => t('Product'),
        'help' => t('Product.'),
        'weight' => -11,
    );

    $data['leaf_product']['table']['join'] = array(
        // Link ourself to the {file_managed} table so we can provide file->entity relationships.
        'leaf_contract' => array(
            'field' => 'contract_id',
            'left_field' => 'contract_id',
        ),
    );
    
    $data['leaf_product']['product_id'] = array(
        'title' => t('Product Id field'),
        'help' => t('Just a numeric field.'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_numeric',
        ),
    );
    
    $data['leaf_product']['sku'] = array(
        'title' => t('sku field'),
        'help' => t('Just a numeric field.'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_numeric',
        ),
    );

    
    $data['leaf_product']['contract_id'] = array(
        'title' => t('Contract Id field'),
        'help' => t('Just a numeric field.'),
        'relationship' => array(
            'base' => 'leaf_contract', // The name of the table to join with.
            'base field' => 'contract_id', // The name of the field on the joined table.
            // 'field' => 'nid' -- see hook_views_data_alter(); not needed here.
            'handler' => 'views_handler_relationship',
            'label' => t('Default label for the relationship'),
            'title' => t('leaf_contract relationship'),
            'help' => t('More information on this relationship'),
        ),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
    );
    
    $data['leaf_product']['name'] = array(
        'title' => t('Name field'),
        'help' => t('Name.'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE, // This is use by the table display plugin.
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );
    
    $data['leaf_product']['item_category_id'] = array(
        'title' => t('item_category_id field'),
        'help' => t('Just a numeric field.'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_numeric',
        ),
    );
    
    $data['leaf_product']['description'] = array(
        'title' => t('description field'),
        'help' => t('description.'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE, // This is use by the table display plugin.
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );
    
    $data['leaf_product']['price'] = array(
        'title' => t('price field'),
        'help' => t('price.'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE, // This is use by the table display plugin.
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );
    
    $data['leaf_product']['color_id'] = array(
        'title' => t('color_id field'),
        'help' => t('Just a numeric field.'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_numeric',
        ),
    );
    
    $data['leaf_product']['size'] = array(
        'title' => t('size field'),
        'help' => t('size.'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE, // This is use by the table display plugin.
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );
    
    $data['leaf_product']['brand_id'] = array(
        'title' => t('brand_id field'),
        'help' => t('Just a numeric field.'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_numeric',
        ),
    );
    
    $data['leaf_product']['consignment_price'] = array(
        'title' => t('consignment_price field'),
        'help' => t('consignment_price.'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE, // This is use by the table display plugin.
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );
    
    $data['leaf_product']['purchase_price'] = array(
        'title' => t('purchase_price field'),
        'help' => t('purchase_price.'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE, // This is use by the table display plugin.
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );
    
    $data['leaf_product']['quantity'] = array(
        'title' => t('quantity field'),
        'help' => t('quantity.'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE, // This is use by the table display plugin.
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );
    
    $data['leaf_product']['status'] = array(
        'title' => t('status field'),
        'help' => t('Just a numeric field.'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_numeric',
        ),
    );
    
    $data['leaf_product']['created_date'] = array(
        'title' => t('created_date field'),
        'help' => t('created_date.'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE, // This is use by the table display plugin.
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );
    
    
    
    $data['leaf_item_category']['table']['group'] = t('Product');
    
    $data['leaf_item_category']['table']['base'] = array(
        'field' => 'id', // This is the identifier field for the view.
        'title' => t('leaf_item_category id'),
        'help' => t('leaf_item_category id.'),
        'weight' => -11,
    );

    $data['leaf_item_category']['table']['join'] = array(
        // Link ourself to the {file_managed} table so we can provide file->entity relationships.
        'leaf_product' => array(
            'field' => 'id',
            'left_field' => 'item_category_id',
        ),
    );
    
    $data['leaf_item_category']['id'] = array(
        'title' => t('leaf_item_category Id field'),
        'help' => t('Just a numeric field.'),
        'relationship' => array(
            'base' => 'leaf_product', // The name of the table to join with.
            'base field' => 'item_category_id', // The name of the field on the joined table.
            // 'field' => 'nid' -- see hook_views_data_alter(); not needed here.
            'handler' => 'views_handler_relationship',
            'label' => t('leaf_item_category relationship'),
            'title' => t('leaf_item_category category relationship'),
            'help' => t('More information on this relationship'),
        ),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
    );
    
    $data['leaf_item_category']['name'] = array(
        'title' => t('leaf_item_category Name field'),
        'help' => t('leaf_item_category Name.'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE, // This is use by the table display plugin.
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );

    
    $data['leaf_color']['table']['group'] = t('Product');
    
    $data['leaf_color']['table']['base'] = array(
        'field' => 'id', // This is the identifier field for the view.
        'title' => t('leaf_color id'),
        'help' => t('leaf_color id.'),
        'weight' => -11,
    );

    $data['leaf_color']['table']['join'] = array(
        // Link ourself to the {file_managed} table so we can provide file->entity relationships.
        'leaf_product' => array(
            'field' => 'id',
            'left_field' => 'color_id',
        ),
    );
    
    $data['leaf_color']['id'] = array(
        'title' => t('leaf_color Id field'),
        'help' => t('Just a numeric field.'),
        'relationship' => array(
            'base' => 'leaf_product', // The name of the table to join with.
            'base field' => 'color_id', // The name of the field on the joined table.
            // 'field' => 'nid' -- see hook_views_data_alter(); not needed here.
            'handler' => 'views_handler_relationship',
            'label' => t('leaf_color for the relationship'),
            'title' => t('leaf_color category relationship'),
            'help' => t('More information on this relationship'),
        ),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
    );
    
    $data['leaf_color']['name'] = array(
        'title' => t('leaf_color Name field'),
        'help' => t('leaf_color Name.'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE, // This is use by the table display plugin.
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );
    
    
    $data['leaf_brand']['table']['group'] = t('Product');
    
    $data['leaf_brand']['table']['base'] = array(
        'field' => 'brand_id', // This is the identifier field for the view.
        'title' => t('leaf_brand id'),
        'help' => t('leaf_brand id.'),
        'weight' => -11,
    );

    $data['leaf_brand']['table']['join'] = array(
        // Link ourself to the {file_managed} table so we can provide file->entity relationships.
        'leaf_product' => array(
            'field' => 'brand_id',
            'left_field' => 'brand_id',
        ),
    );
    
    $data['leaf_brand']['brand_id'] = array(
        'title' => t('leaf_brand Id field'),
        'help' => t('Just a numeric field.'),
        'relationship' => array(
            'base' => 'leaf_product', // The name of the table to join with.
            'base field' => 'brand_id', // The name of the field on the joined table.
            // 'field' => 'nid' -- see hook_views_data_alter(); not needed here.
            'handler' => 'views_handler_relationship',
            'label' => t('leaf_brand for the relationship'),
            'title' => t('leaf_brand category relationship'),
            'help' => t('More information on this relationship'),
        ),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
    );
    
    $data['leaf_brand']['brand_name'] = array(
        'title' => t('leaf_brand Name field'),
        'help' => t('leaf_brand Name.'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE, // This is use by the table display plugin.
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );

    return $data;
}

;
