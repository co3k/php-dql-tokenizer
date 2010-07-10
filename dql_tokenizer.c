#include "php_dql_tokenizer.h"

#ifdef COMPILE_DL_DQL_TOKENIZER
ZEND_GET_MODULE(dql_tokenizer)
#endif

static PHP_FUNCTION(dql_tokenize_query)
{
  php_error_docref(NULL TSRMLS_CC, E_ERROR, "This function is not implemented. Call Doctrine_Query_Tokenizer::tokenizeQuery() instead.");
}

static PHP_FUNCTION(dql_bracket_trim)
{
  php_error_docref(NULL TSRMLS_CC, E_ERROR, "This function is not implemented. Call Doctrine_Query_Tokenizer::bracketTrim() instead.");
}

static PHP_FUNCTION(dql_bracket_explode)
{
  php_error_docref(NULL TSRMLS_CC, E_ERROR, "This function is not implemented. Call Doctrine_Query_Tokenizer::bracketExplode() instead.");
}

static PHP_FUNCTION(dql_quote_explode)
{
  php_error_docref(NULL TSRMLS_CC, E_ERROR, "This function is not implemented. Call Doctrine_Query_Tokenizer::quoteExplode() instead.");
}

static PHP_FUNCTION(dql_sql_explode)
{
  php_error_docref(NULL TSRMLS_CC, E_ERROR, "This function is not implemented. Call Doctrine_Query_Tokenizer::sqlExplode() instead.");
}

static PHP_FUNCTION(dql_clause_explode)
{
  php_error_docref(NULL TSRMLS_CC, E_ERROR, "This function is not implemented. Call Doctrine_Query_Tokenizer::clauseExplode() instead.");
}

static PHP_FUNCTION(dql_get_split_regexp_from_array)
{
  php_error_docref(NULL TSRMLS_CC, E_ERROR, "This function is not implemented. Call Doctrine_Query_Tokenizer::getSplitRegExpFromArray() instead.");
}

static PHP_FUNCTION(dql_clause_explode_regexp)
{
  php_error_docref(NULL TSRMLS_CC, E_ERROR, "This function is not implemented. Call Doctrine_Query_Tokenizer::clauseExplodeRegExp() instead.");
}

static PHP_FUNCTION(dql_clause_explode_count_brackets)
{
  php_error_docref(NULL TSRMLS_CC, E_ERROR, "This function is not implemented. Call Doctrine_Query_Tokenizer::clauseExplodeCountBrackets() instead.");
}

static PHP_FUNCTION(dql_clause_explode_non_quoted)
{
  php_error_docref(NULL TSRMLS_CC, E_ERROR, "This function is not implemented. Call Doctrine_Query_Tokenizer::clauseExplodeNonQuoted() instead.");
}

static PHP_FUNCTION(dql_merge_bracket_terms)
{
  php_error_docref(NULL TSRMLS_CC, E_ERROR, "This function is not implemented. Call Doctrine_Query_Tokenizer::mergeBracketTerms() instead.");
}

static PHP_FUNCTION(dql_quoted_string_explode)
{
  php_error_docref(NULL TSRMLS_CC, E_ERROR, "This function is not implemented. Call Doctrine_Query_Tokenizer::quotedStringExplode() instead.");
}

static function_entry dql_tokenizer_functions[] = {
  PHP_FE(dql_tokenize_query, NULL)
  PHP_FE(dql_bracket_trim, NULL)
  PHP_FE(dql_bracket_explode, NULL)
  PHP_FE(dql_quote_explode, NULL)
  PHP_FE(dql_sql_explode, NULL)
  PHP_FE(dql_clause_explode, NULL)
  PHP_FE(dql_get_split_regexp_from_array, NULL)
  PHP_FE(dql_clause_explode_regexp, NULL)
  PHP_FE(dql_clause_explode_count_brackets, NULL)
  PHP_FE(dql_clause_explode_non_quoted, NULL)
  PHP_FE(dql_merge_bracket_terms, NULL)
  PHP_FE(dql_quoted_string_explode, NULL)
  { NULL, NULL, NULL }
};

/* entry */
zend_module_entry dql_tokenizer_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
  STANDARD_MODULE_HEADER,
#endif
  PHP_DQL_TOKENIZER_EXTNAME,
  dql_tokenizer_functions,
  NULL,  /* MINIT */
  NULL,  /* MSHUTDOWN */
  NULL,  /* RINIT */
  NULL,  /* RSHUTDOWN */
  NULL,  /* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
  PHP_DQL_TOKENIZER_EXTVAR,
#endif
  STANDARD_MODULE_PROPERTIES
};
