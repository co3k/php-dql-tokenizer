--TEST--
Test for dql_clause_explode_regexp()
--FILE--
<?php
$str = "dctrn_find.id IN (?)";
$regexp = '#( |\s)#';
$e1 = '(';
$e2 = ')';

$result = dql_clause_explode_regexp($str, $regexp, $e1, $e2);
var_dump($result);
--EXPECT--
array(3) {
  [0]=>
  array(2) {
    [0]=>
    string(13) "dctrn_find.id"
    [1]=>
    string(1) " "
  }
  [1]=>
  array(2) {
    [0]=>
    string(2) "IN"
    [1]=>
    string(1) " "
  }
  [2]=>
  array(2) {
    [0]=>
    string(3) "(?)"
    [1]=>
    string(0) ""
  }
}
