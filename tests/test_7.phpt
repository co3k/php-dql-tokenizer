--TEST--
Test for dql_clause_explode_non_quoted()
--FILE--
<?php
$str = "dctrn_find.id = ?";
$regexp = '#( |\s)#';
$result = dql_clause_explode_non_quoted($str, $regexp);
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
    string(1) "="
    [1]=>
    string(1) " "
  }
  [2]=>
  array(2) {
    [0]=>
    string(1) "?"
    [1]=>
    string(0) ""
  }
}
