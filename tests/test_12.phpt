--TEST--
Test for dql_bracket_trim()
--FILE--
<?php
$query = "(SELECT u.* FROM User u WHERE u.name LIKE ?)";
$result = dql_bracket_trim($query);
var_dump($result);
--EXPECT--
string(42) "SELECT u.* FROM User u WHERE u.name LIKE ?"
