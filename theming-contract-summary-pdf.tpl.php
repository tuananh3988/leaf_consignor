<div id="page:main-container">
        <table style="width: 100%;clear:both;">
            <tbody><tr>
                    <td style="width: 30%;text-align: left;font-size:14px;"><strong><?= date('m-d-Y'); ?></strong>
                        <div style="width:300px;margin-top:30px;" class="contract_information">
                            <div class="row">
                                <label>Consign Date</label>
                                <span><?= date('m-d-Y', ($contract['from_date'])); ?></span>
                            </div>
                            <div class="row">
                                <label>Claim Date</label>
                                <span><?= date('m-d-Y', ($contract['close_date'])); ?></span>
                            </div>
                            <div class="row">
                                <label>Total Items</label>
                                <span><?= $productsCount ?></span>
                            </div>
                            <div class="row">
                                <label>Tag Color</label>
                                <span></span>
                            </div>
                            <div class="row">
                                <label>Consign Fee</label>
                                <span>$0.00</span>
                            </div>
                        </div>


                        <div style="clear:both;width:300px;margin-top:10px;float:left;" class="inventory_information">
                            <div class="row">
                                <label>Items Sold</label>
                                <span>0</span>
                            </div>
                            <div class="row">
                                <label>Total Sold</label>
                                <span>$0.00</span>
                            </div>
                            <div class="row">
                                <label>Total Due</label>
                                <span>$0.00</span>
                            </div>
                            <div class="row">
                                <label>Total Paid</label>
                                <span>$0.00</span>
                            </div>
                        </div>
                    </td>
                    <td style="width: 30%;text-align: center;">
                        <strong style="font-size:18px;font-weight:bold;">greenestreet.com
                            <!--Greene Street Consignment<br />
                            Manayunk<br />
                            4313 Main Street<br />
                            Manayunk, PA 19127<br />
                            267-335-5478
                            --></strong>
                    </td>
                    <td style="width: 40%;text-align: right;font-size:14px;">
                        <strong>CONSIGNMENT<br>
                            CONTRACT
                        </strong>
                        <p style="text-align: right;">#<?= $vendor['vendor_uuid']; ?></p>
                        <div style="clear:both;" class="vendor_information">
                            <div class="row">
                                <strong><?= $vendor['name']; ?></strong>
                            </div>
                            <div class="row">
                                <strong><?= $vendor['address']; ?></strong>
                            </div>
                            <div class="row">
                                <strong><?= $vendor['city']; ?></strong>
                            </div>
                            <div class="row">
                                <strong><?= $vendor['state']; ?></strong>
                            </div>
                            <div class="row">
                                <strong><?= $vendor['zip']; ?></strong>
                            </div>
                            <div class="row">
                                <label>Phone Number</label>
                                <span><?= $vendor['phone_number']; ?></span>
                            </div>
                            <div class="row">
                                <label>Payment Method</label>
                                <span>
                                    <?= $vendor['payment_name']; ?>
                                </span>
                            </div>
                        </div>

                    </td>
                </tr>
            </tbody></table>
        <div style="height: 15px;"></div>

        <table class="ajaxview_vendor" border="1" style="margin:0 auto;width:100%;border-collapse:collapse;border:1px solid gray;text-align: center;">
            <tbody><tr>
                    <th>Item number</th>
                    <th>Category</th>
                    <th>Size</th>
                    <th>Color</th>
                    <th>Name</th>
                    <th>Description</th>
                    <th>Brand</th>
                    <?php if(isset($form['email_type'])): ?>
                        <?php if($form['email_type'] == 0): ?>
                        <th class="asis_col" style="display: none;">Consignment Price</th>
                        <th class="asis_col" style="display: none;">Purchase Price</th>
                        <th class="asis_col" style="display: none;">Price</th>
                        <th class="asis_col" style="display: none;">Quantity</th>
                        <th class="asis_col" style="display: none;">Status</th>
                        <?php else : ?>
                        <th class="cop_col" style="display: table-cell;">COP*</th>
                        <?php endif; ?>
                    <?php else : ?>
                    <th class="asis_col" style="display: none;">Consignment Price</th>
                    <th class="asis_col" style="display: none;">Purchase Price</th>
                    <th class="asis_col" style="display: none;">Price</th>
                    <th class="asis_col" style="display: none;">Quantity</th>
                    <th class="asis_col" style="display: none;">Status</th>
                    <th class="cop_col" style="display: table-cell;">COP*</th>
                    <?php endif; ?>
                </tr>
                <?php foreach ($products as $v) : ?>
                <tr>
                    <td><?= $v->sku; ?></td>
                    <td><?= $v->i_name; ?></td>
                    <td><?= $v->size; ?></td>
                    <td><?= $v->c_name; ?></td>
                    <td><?= $v->name; ?></td>
                    <td><?= $v->description; ?></td>
                    <td><?= $v->brand_name; ?></td>
                    <?php if(isset($form['email_type'])): ?>
                        <?php if($form['email_type'] == 0): ?>
                        <td class="asis_col" style="display: none;"><?= $v->consignment_price; ?></td>
                        <td class="asis_col" style="display: none;"><?= $v->purchase_price; ?></td>
                        <td class="asis_col" style="display: none;"><?= $v->price; ?></td>
                        <td class="asis_col" style="display: none;"><?= $v->quantity; ?></td>
                        <td class="asis_col" style="display: none;"><?= $v->status == 0 ? 'in stock' : 'sold'; ?></td>
                        <?php else : ?>
                        <td class="cop_col" style="display: table-cell;"><?= $v->cop; ?></td>
                        <?php endif; ?>
                    <?php else : ?>
                        <td class="asis_col" style="display: none;"><?= $v->consignment_price; ?></td>
                        <td class="asis_col" style="display: none;"><?= $v->purchase_price; ?></td>
                        <td class="asis_col" style="display: none;"><?= $v->price; ?></td>
                        <td class="asis_col" style="display: none;"><?= $v->quantity; ?></td>
                        <td class="asis_col" style="display: none;"><?= $v->status == 0 ? 'in stock' : 'sold'; ?></td>
                        <td class="cop_col" style="display: table-cell;">
                            <?= drupal_render($form["cop-{$v->product_id}"]); ?>
                        </td>
                    <?php endif; ?>

                </tr>
                <?php endforeach; ?>
               <?php if($products->rowCount() > 0):?>
                <tr class="total-row">
                    <td></td>
                    <td></td>
                    <td></td>
                    <td></td>
                    <td></td>
                    <td></td>
                    <td></td>
                    <?php if(isset($form['email_type'])): ?>
                        <?php if($form['email_type'] == 0): ?>
                        <td class="asis_col" style="display: none;"><?= $totalCP;?></td>
                        <td class="asis_col" style="display: none;"><?= $totalPP;?></td>
                        <td class="asis_col" style="display: none;"><?= $totalP;?></td>
                        <td class="asis_col" style="display: none;"><?= $totalQ;?></td>
                        <td class="asis_col" style="display: none;"></td>
                        <?php else : ?>
                        <td class="cop_col" style="display: table-cell;"></td>
                        <?php endif; ?>
                    <?php else : ?>
                    <td class="asis_col" style="display: none;"><?= $totalCP;?></td>
                    <td class="asis_col" style="display: none;"></td>
                    <td class="asis_col" style="display: none;"><?= $totalP;?></td>
                    <td class="asis_col" style="display: none;"></td>
                    <td class="asis_col" style="display: none;"></td>
                    <td class="cop_col" style="display: table-cell;"></td>
                    <?php endif; ?>
                </tr>
                <?php endif; ?>
            </tbody></table>


    <div style="border-top: 2px solid gray;margin-top:10px;clear:both;padding-top:5px;">
        <p style="font-weight: bold;">TERMS: (I)   <?= $contract['revenue_sharing']; ?>% BASIC</p>

        <p style="text-align: center;font-weight: normal;">
            You mailed us &lt;<?= $contract['number_items_off']; ?>&gt; pieces for online consignment.
        </p>
        <p style="text-align: center;font-weight: normal;">
            We have accepted &lt;<?= $contract['number_items_accepeted']; ?>&gt; pieces.
        </p>
        <p style="text-align: center;font-weight: normal;">
            We have passed on &lt;<?= $contract['number_items_rejected']; ?>&gt; pieces.
        </p>

        <br>

        <p style="text-align: left;font-weight: normal;">
            <b>Passed-On Items:</b><br>
            Please email <a href="mailto:onlinecustomerservice@greenestreet.com">onlinecustomerservice@greenestreet.com</a> within 7 days of the enclosed emailed contract to reclaim any items that
            were not accepted, and we will return those items to you for a flat fee of $12.95.
            <br><br>
        </p>

        <p style="text-align: left;font-weight: normal;">
            <b>Claim Date and Commission:</b><br>
            Greene Street will email updates 5 days prior to Claim Date and ON the Claim Date.<br><br>

            The Claim Date is the <b style="text-decoration: underline;">last day</b> of your contract term.  You may request your unsold items to be shipped back to you for a flat
            fee of $12.95 within 5 days <b style="text-decoration: underline;">prior to</b> the Claim Date.  This fee will be PayPal invoiced.  Items become property of
            Greene Street immediately after the Claim Date.<br><br>

            Commission checks for sold items are mailed immediately after your Claim Date.  Please allow time for delivery.<br><br>

            Address changes must be made prior to your Claim Date to ensure proper delivery of your commission check.<br><br>

            Thank you for consigning with Greene Street.<br>
        </p>

        <br><br>
        <p style="text-align: center;font-weight: bold;">
            Visit our website: <a href="http://www.greenestreet.com">www.greenestreet.com</a>
        </p>
        <p style="text-align: center;font-weight: bold;">
            <a href="mailto:onlinecustomerservice@greenestreet.com">onlinecustomerservice@greenestreet.com</a>
        </p>

    </div>

</div>