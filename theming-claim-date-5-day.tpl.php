<div style="background:#f6f6f6;font-family:Verdana,Arial,Helvetica,sans-serif;font-size:12px;margin:0;padding:0">
Hello <?= $vendor['name']; ?>!<br><br>
<p>Your contract with Greene Street online will be expiring in 5 days!</p>
<p>Please reply to this email before the end of your contract should you wish to have any unsold items returned to you. We have a $12.95 return shipping fee for the return of unsold items, which will be deducted from your commission check.</p>
<p>Your commission check for sold items will be printed upon the expiration of your contract. Any address changes must be made prior to the end of your contract to ensure proper delivery.</p>
<p>Here are the details:</p>
<p>Contract ID: #<?= $contract['contract_uuid']; ?></p>
<p>Start Date: <span class="aBn" data-term="goog_1636969087" tabindex="0"><span class="aQJ"><?= date('m-d-Y', ($contract['from_date'])); ?></span></span></p>
<p>End Claim Date: <span class="aBn" data-term="goog_1636969088" tabindex="0"><span class="aQJ"><?= date('m-d-Y', ($contract['close_date'])); ?></span></span></p>
<p>Commission %: <?= $contract['revenue_sharing']; ?>%</p>

<p>Thank you for consigning with Greene Street!</p>
<p>The Greene Street Online Team</p>
<p><a href="http://www.shopgreenestreet.com" target="_blank">www.shopgreenestreet.com</a></p>
<p>Any questions? You can reach us at:</p>
<p><a href="mailto:onlinecustomerservice@greenestreet.com" style="color:blue" target="_blank">onlinecustomerservice@<wbr>greenestreet.com </a></p>
</div>