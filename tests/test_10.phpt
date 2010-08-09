--TEST--
Test for dql_sql_explode()
--FILE--
<?php

$str = "(age < 20 AND age > 18) AND name LIKE 'John Doe'";
$d   = ' ';
$e1  = '(';
$e2  = ')';

$result = dql_sql_explode($str, $d, $e1, $e2);
var_dump($result);
--EXPECT--
array(5) {
  [0]=>
  string(23) "(age < 20 AND age > 18)"
  [1]=>
  string(3) "AND"
  [2]=>
  string(4) "name"
  [3]=>
  string(4) "LIKE"
  [4]=>
  string(10) "'John Doe'"
}
