--TEST--
Test for dql_clause_explode()
--FILE--
<?php
$str = "email LIKE 'John@example.com'";
$d = ' LIKE ';
$result = dql_quote_explode($str, $d);
var_dump($result);
--EXPECT--
array(2) {
  [0]=>
  string(5) "email"
  [1]=>
  string(18) "'John@example.com'"
}
