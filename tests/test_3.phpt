--TEST--
Test for dql_quote_explode()
--FILE--
<?php
$str = "(age < 20 AND age > 18) AND name LIKE 'John'+' Doe'";
$d   = array(' ', '+');
$result = dql_clause_explode($str, $d);
var_dump($result);
--EXPECT--
array(6) {
  [0]=>
  array(2) {
    [0]=>
    string(23) "(age < 20 AND age > 18)"
    [1]=>
    string(1) " "
  }
  [1]=>
  array(2) {
    [0]=>
    string(3) "AND"
    [1]=>
    string(1) " "
  }
  [2]=>
  array(2) {
    [0]=>
    string(4) "name"
    [1]=>
    string(1) " "
  }
  [3]=>
  array(2) {
    [0]=>
    string(4) "LIKE"
    [1]=>
    string(1) " "
  }
  [4]=>
  array(2) {
    [0]=>
    string(6) "'John'"
    [1]=>
    string(1) "+"
  }
  [5]=>
  array(2) {
    [0]=>
    string(6) "' Doe'"
    [1]=>
    string(0) ""
  }
}
