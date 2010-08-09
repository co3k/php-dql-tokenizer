--TEST--
Test for dql_bracket_explode()
--FILE--
<?php
$result = dql_bracket_explode("(age < 20 AND age > 18) AND email LIKE 'John@example.com'");
var_dump($result);
--EXPECT--
array(5) {
  [0]=>
  string(23) "(age < 20 AND age > 18)"
  [1]=>
  string(3) "AND"
  [2]=>
  string(5) "email"
  [3]=>
  string(4) "LIKE"
  [4]=>
  string(18) "'John@example.com'"
}
