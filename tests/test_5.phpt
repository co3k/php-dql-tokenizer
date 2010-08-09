--TEST--
Test for dql_quoted_string_explode()
--FILE--
<?php
$str = "'a' AND name = 'John O\'Connor'";
$result = dql_quoted_string_explode($str);
var_dump($result);
--EXPECT--
array(5) {
  [0]=>
  string(0) ""
  [1]=>
  string(3) "'a'"
  [2]=>
  string(12) " AND name = "
  [3]=>
  string(16) "'John O\'Connor'"
  [4]=>
  string(0) ""
}
