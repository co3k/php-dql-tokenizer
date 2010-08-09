--TEST--
Test for dql_tokenize_query()
--FILE--
<?php
$query = "SELECT u.* FROM User u WHERE u.name LIKE ?";
$result = dql_tokenize_query($query);
var_dump($result);
--EXPECT--
array(3) {
  ["SELECT"]=>
  string(4) "u.* "
  ["FROM"]=>
  string(7) "User u "
  ["WHERE"]=>
  string(14) "u.name LIKE ? "
}
