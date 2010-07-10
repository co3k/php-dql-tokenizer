--TEST--
Test for fatal error in not-implemented functions
--FILE--
<?php
dql_tokenize_query();
--EXPECTF--
Fatal error: dql_tokenize_query(): This function is not implemented. Call Doctrine_Query_Tokenizer::tokenizeQuery() instead. in %stest_1.php on line %d
