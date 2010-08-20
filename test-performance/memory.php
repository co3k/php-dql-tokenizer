<?php

$funcName = ($argc == 1) ? 'default' : $argv[1];
var_dump('Current: '.$funcName);

$time = microtime(true);
$m = memory_get_usage();

for ($i = 0; $i < 100; $i++)
{
  if ('dql_get_split_regexp_from_array' === $funcName)
  {
    dql_get_split_regexp_from_array(array('s', 'str', ' ', '\\', '|', '(', '*', 0, true));
  }

  if ('dql_sql_explode' === $funcName)
  {
    dql_sql_explode("(age < 20 AND age > 18) AND name LIKE 'John Doe'", ' ', '(', ')');
  }

  if ('dql_tokenize_query' === $funcName)
  {
    dql_tokenize_query("SELECT u.* FROM User u WHERE u.name LIKE ?");
  }

  if ('dql_bracket_trim' === $funcName)
  {
    dql_bracket_trim("(SELECT u.* FROM User u WHERE u.name LIKE ?)");
  }

  if ('dql_bracket_explode' === $funcName)
  {
    dql_bracket_explode("(age < 20 AND age > 18) AND email LIKE 'John@example.com'");
  }

  if ('dql_clause_explode' === $funcName)
  {
    dql_clause_explode("(age < 20 AND age > 18) AND name LIKE 'John'+' Doe'", array(' ', '+'));
  }

  if ('dql_quote_explode' === $funcName)
  {
    dql_quote_explode("email LIKE 'John@example.com'", ' LIKE ');
  }

  if ('dql_quoted_string_explode' === $funcName)
  {
    dql_quoted_string_explode("'a' AND name = 'John O\'Connor'");
  }

  if ('dql_clause_explode_count_brackets' === $funcName)
  {
    dql_clause_explode_count_brackets("'a' AND name = 'John O\'Connor'", '#(\s)#');
  }

  if ('dql_clause_explode_non_quoted' === $funcName)
  {
    dql_clause_explode_non_quoted("dctrn_find.id = ?", '#( |\s)#');
  }

  if ('dql_clause_explode_regexp' === $funcName)
  {
    dql_clause_explode_regexp("dctrn_find.id IN (?)", '#( |\s)#', '(', ')');
  }

  if ('dql_merge_bracket_terms' === $funcName)
  {
    dql_merge_bracket_terms(array(array("'a(b'", '+', 0), array('(2', '+', 1), array('3)', '-', -1), array('5', '' , '0')));
  }

  if ('tokenizeA' === $funcName)
  {
    dql_tokenize_query('SELECT * FROM ExampleRecord');
  }

  if ('tokenizeB' === $funcName)
  {
    dql_tokenize_query('SELECT id, id, id, id, id, id, id FROM ExampleRecord');
  }

  if ('tokenizeC' === $funcName)
  {
    dql_tokenize_query('SELECT a.id, a.id, a.id, a.id, a.id, a.id, a.id FROM ExampleRecord AS a');
  }

  if ('tokenizeD' === $funcName)
  {
    dql_tokenize_query('SELECT id, id, id, id, id, id, id FROM ExampleRecord WHERE id = ?');
  }

  if ('tokenizeE' === $funcName)
  {
    dql_tokenize_query('SELECT id, id, id, id, id, id, id FROM ExampleRecord WHERE id = ? AND id IN (?, ?, ?, ?)');
  }

  if ('tokenizeF' === $funcName)
  {
    dql_tokenize_query('SELECT id, id, id, id, id, id, id FROM ExampleRecord WHERE id = ? AND id IN (?, ?, ?, ?) AND id = (SELECT s.id FROM ExampleRecord.ExampleRecord AS s WHERE s.id = ?)');
  }

  if ('tokenizeG' === $funcName)
  {
    dql_tokenize_query('DELETE FROM ExampleRecord');
  }

  if ('tokenizeH' === $funcName)
  {
    dql_tokenize_query('DELETE FROM ExampleRecord WHERE id = ? AND id > 10 AND id < 10');
  }

  if ('tokenizeI' === $funcName)
  {
    dql_tokenize_query('UPDATE ExampleRecord SET id = ?, id = ? WHERE id = ?');
  }

  if ('tokenizeJ' === $funcName)
  {
    dql_tokenize_query('UPDATE ExampleRecord SET id = ?, id = ? WHERE id IN (?)');
  }
}

var_dump(
  'Memory: '.(memory_get_usage() - $m),
  'Time  : '.(microtime(true) - $time),
  round(memory_get_usage() / 1024 / 1024, 2)
);
