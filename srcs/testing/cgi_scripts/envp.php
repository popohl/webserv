<?php
$arr=getenv();
foreach ($arr as $key=>$val)
echo "$key=>$val
";
?>

</br>
<p>And now, the POST data:</p>
<?php var_dump($_POST); ?>
<?php var_dump(file_get_contents('php://input')); ?>
