#include "php_dql_tokenizer.h"

#include "ext/standard/php_string.h"
#include "ext/standard/php_smart_str.h"

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
  zval *array, **entry;
  HashPosition pos;

  smart_str result = {0};
  smart_str_appendl(&result, "#(", 2);

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &array)) {
    return;
  }

  zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(array), &pos);
  while (zend_hash_get_current_data_ex(Z_ARRVAL_P(array), (void **)&entry, &pos) == SUCCESS) {
    convert_to_string_ex(entry);

    if (1 <= pos->h) {
      smart_str_appendl(&result, "|", 1);
    }

    if (strcmp(" ", Z_STRVAL_PP(entry)) == 0) {
      smart_str_appendl(&result, "\\s", 2);
    } else {
      zval *pre, *filtered;
      MAKE_STD_ZVAL(pre);
      ZVAL_STRING(pre, Z_STRVAL_PP(entry), 1);
      MAKE_STD_ZVAL(filtered);

      /* filtering by zif_preg_quote() */
      PHP_DQL_FUNC1(preg_quote, filtered, this_ptr, pre);
      smart_str_appendl(&result, Z_STRVAL_P(filtered), Z_STRLEN_P(filtered));

      zval_ptr_dtor(&pre);
      zval_ptr_dtor(&filtered);
    }

    zend_hash_move_forward_ex(Z_ARRVAL_P(array), &pos);
  }

  smart_str_appendl(&result, ")#", 2);

  smart_str_0(&result);

  if (result.len) {
    RETURN_STRINGL(result.c, result.len, 0);
  } else {
    smart_str_free(&result);
    RETURN_EMPTY_STRING();
  }
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
  char *str, *regexp;
  int len1, len2;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &str, &len1, &regexp, &len2)) {
    return;
  }

  zval *pattern, *input, *limit, *flags, *tmp;
  MAKE_STD_ZVAL(pattern); ZVAL_STRING(pattern, regexp, 1);
  MAKE_STD_ZVAL(input); ZVAL_STRING(input, str, 1);
  MAKE_STD_ZVAL(limit); ZVAL_LONG(limit, -1);
  MAKE_STD_ZVAL(flags); ZVAL_LONG(flags, 2); /* PREG_SPLIT_DELIM_CAPTURE */
  MAKE_STD_ZVAL(tmp);

  PHP_DQL_FUNC4(preg_split, tmp, this_ptr, pattern, input, limit, flags);

  char  *string_key;
  int i = 0, string_key_len;
  zval **val;
  ulong key;
  HashPosition pos;

  array_init(return_value);

  zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(tmp), &pos);
  while (zend_hash_get_current_data_ex(Z_ARRVAL_P(tmp), (void **)&val, &pos) == SUCCESS) {
    convert_to_string_ex(val);

    zend_hash_get_current_key_ex(Z_ARRVAL_P(tmp), &string_key, &string_key_len, &key, 0, &pos);

    if (!(key & 1)) {
      zval *tmpArr;

      MAKE_STD_ZVAL(tmpArr);
      array_init(tmpArr);

      add_next_index_stringl(tmpArr, Z_STRVAL_PP(val), Z_STRLEN_PP(val), 1);
      add_next_index_stringl(tmpArr, "", 0, 1);

      add_index_zval(return_value, i, tmpArr);
    } else {
      zval *subarray;

      zend_hash_index_find(Z_ARRVAL_P(return_value), i++, (void **)&subarray);
      add_index_stringl(Z_ARRVAL_P(subarray), 1, Z_STRVAL_PP(val), Z_STRLEN_PP(val), 1);

      add_index_zval(return_value, i, subarray);
      zval_ptr_dtor(&subarray);
    }
    zend_hash_move_forward_ex(Z_ARRVAL_P(tmp), &pos);
  }

  zval_ptr_dtor(&pattern);
  zval_ptr_dtor(&input);
  zval_ptr_dtor(&limit);
  zval_ptr_dtor(&flags);
  zval_ptr_dtor(&tmp);
}

static PHP_FUNCTION(dql_merge_bracket_terms)
{
  php_error_docref(NULL TSRMLS_CC, E_ERROR, "This function is not implemented. Call Doctrine_Query_Tokenizer::mergeBracketTerms() instead.");
}

static PHP_FUNCTION(dql_quoted_string_explode)
{
  char *str;
  int len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &len)) {
    return;
  }

  zval *pattern, *input, *limit, *flags, *tmp;
  MAKE_STD_ZVAL(pattern); ZVAL_STRING(pattern, "#(\\'|''|'|\\\"|\"\"|\")#", 1);
  MAKE_STD_ZVAL(input); ZVAL_STRING(input, str, 1);
  MAKE_STD_ZVAL(limit); ZVAL_LONG(limit, -1);
  MAKE_STD_ZVAL(flags); ZVAL_LONG(flags, 2); /* PREG_SPLIT_DELIM_CAPTURE */
  MAKE_STD_ZVAL(tmp);

  PHP_DQL_FUNC4(preg_split, tmp, this_ptr, pattern, input, limit, flags);

  char *mode = '\0', *string_key;
  int i = 0, string_key_len;
  ulong key;
  zval **val, **data;
  HashPosition pos;

  array_init(return_value);

  zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(tmp), &pos);
  while (zend_hash_get_current_data_ex(Z_ARRVAL_P(tmp), (void **)&val, &pos) == SUCCESS) {
    convert_to_string_ex(val);

    zend_hash_get_current_key_ex(Z_ARRVAL_P(tmp), &string_key, &string_key_len, &key, 0, &pos);

    if (key & 1) {
      if (mode == '\0') {
        if (strcmp("'", Z_STRVAL_PP(val)) == 0 || strcmp("\"", Z_STRVAL_PP(val)) == 0) {
          mode = Z_STRVAL_PP(val);
          i++;
        }
      } else if (strcmp(mode, Z_STRVAL_PP(val)) == 0) {
        if (zend_hash_index_find(Z_ARRVAL_P(return_value), i, (void **)&data) == FAILURE) {
          add_index_stringl(return_value, i, Z_STRVAL_PP(val), Z_STRLEN_PP(val), 1);
        } else {
          smart_str s = {0};

          smart_str_appendl(&s, Z_STRVAL_PP(data), Z_STRLEN_PP(data));
          smart_str_appendl(&s, Z_STRVAL_PP(val), Z_STRLEN_PP(val));

          smart_str_0(&s);

          add_index_stringl(return_value, i, s.c, s.len, 1);
        }

        mode = '\0';
        i++;

        zend_hash_move_forward_ex(Z_ARRVAL_P(tmp), &pos);
        continue;
      }
    }

    if (zend_hash_index_find(Z_ARRVAL_P(return_value), i, (void **)&data) == FAILURE) {
      add_index_stringl(return_value, i, Z_STRVAL_PP(val), Z_STRLEN_PP(val), 1);
    } else {
      smart_str s = {0};
      smart_str_appendl(&s, Z_STRVAL_PP(data), Z_STRLEN_PP(data));
      smart_str_appendl(&s, Z_STRVAL_PP(val), Z_STRLEN_PP(val));

      smart_str_0(&s);

      add_index_stringl(return_value, i, s.c, s.len, 1);
    }

    zend_hash_move_forward_ex(Z_ARRVAL_P(tmp), &pos);
  }

  zval_ptr_dtor(&pattern);
  zval_ptr_dtor(&input);
  zval_ptr_dtor(&limit);
  zval_ptr_dtor(&flags);
  zval_ptr_dtor(&tmp);
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
