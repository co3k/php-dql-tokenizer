--TEST--
Test for creating regexp string by dql_get_split_regexp_from_array()
--FILE--
<?php
$result = dql_get_split_regexp_from_array(array(
  's', 'str', ' ', '\\', '|', '(', '*', 0, true,
));

var_dump($result);
--EXPECT--
string(28) "#(s|str|\s|\\|\||\(|\*|0|1)#"
