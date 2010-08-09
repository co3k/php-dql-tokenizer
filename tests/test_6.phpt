--TEST--
Test for dql_clause_explode_count_brackets()
--FILE--
<?php
$str = "'a' AND name = 'John O\'Connor'";
$regexp = '#(\s)#';
$result = dql_clause_explode_count_brackets($str, $regexp);
var_dump($result);
--EXPECT--
array(5) {
  [0]=>
  array(3) {
    [0]=>
    string(3) "'a'"
    [1]=>
    string(1) " "
    [2]=>
    int(0)
  }
  [1]=>
  array(3) {
    [0]=>
    string(3) "AND"
    [1]=>
    string(1) " "
    [2]=>
    int(0)
  }
  [2]=>
  array(3) {
    [0]=>
    string(4) "name"
    [1]=>
    string(1) " "
    [2]=>
    int(0)
  }
  [3]=>
  array(3) {
    [0]=>
    string(1) "="
    [1]=>
    string(1) " "
    [2]=>
    int(0)
  }
  [4]=>
  array(3) {
    [0]=>
    string(16) "'John O\'Connor'"
    [1]=>
    string(0) ""
    [2]=>
    int(0)
  }
}
