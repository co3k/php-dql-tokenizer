--TEST--
Test for dql_merge_bracket_terms()
--FILE--
<?php
$terms = array(
    array("'a(b'", '+', 0),
    array('(2', '+', 1),
    array('3)', '-', -1),
    array('5', '' , '0'),
);

$result = dql_merge_bracket_terms($terms);

var_dump($result);
--EXPECT--
array(3) {
  [0]=>
  array(3) {
    [0]=>
    string(5) "'a(b'"
    [1]=>
    string(1) "+"
    [2]=>
    int(0)
  }
  [1]=>
  array(3) {
    [0]=>
    string(5) "(2+3)"
    [1]=>
    string(1) "-"
    [2]=>
    int(0)
  }
  [2]=>
  array(3) {
    [0]=>
    string(1) "5"
    [1]=>
    string(0) ""
    [2]=>
    int(0)
  }
}
