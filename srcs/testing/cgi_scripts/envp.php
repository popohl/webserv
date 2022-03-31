<?php
$arr=getenv();
foreach ($arr as $key=>$val)
echo "$key=>$val
";
?>

</br>
<p>And now, the POST data:</p>
<?php print_r($_POST); ?>
