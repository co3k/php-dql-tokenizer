#include "php_dql_tokenizer.h"

#include "ext/standard/php_string.h"
#include "ext/standard/php_array.h"
#include "ext/standard/php_smart_str.h"

#include "ext/standard/php_var.h"

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
      zend_function *func; PHP_DQL_FUNC1(preg_quote, filtered, this_ptr, pre);
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
  char *str, *regexp, *e1 = "(", *e2 = ")";
  int str_len, regexp_len, e1_len, e2_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|ss", &str, &str_len, &regexp, &regexp_len, &e1, &e1_len, &e2, &e2_len)) {
    return;
  }

  array_init(return_value);

  zval *tmpStr, *tmpRegexp, *tmpE1, *tmpE2, *tmpTerms;
  MAKE_STD_ZVAL(tmpStr); ZVAL_STRING(tmpStr, str, 1);
  MAKE_STD_ZVAL(tmpRegexp); ZVAL_STRING(tmpRegexp, regexp, 1);
  MAKE_STD_ZVAL(tmpE1); ZVAL_STRING(tmpE1, e1, 1);
  MAKE_STD_ZVAL(tmpE2); ZVAL_STRING(tmpE2, e2, 1);
  MAKE_STD_ZVAL(tmpTerms); array_init(tmpTerms);

  zend_function *func;
  PHP_DQL_FUNC4(dql_clause_explode_count_brackets, tmpTerms, this_ptr, tmpStr, tmpRegexp, tmpE1, tmpE2);
  PHP_DQL_FUNC1(dql_merge_bracket_terms, return_value, this_ptr, tmpTerms);

  zval **val;
  HashPosition pos;

  zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(return_value), &pos);
  while (zend_hash_get_current_data_ex(Z_ARRVAL_P(return_value), (void **)&val, &pos) == SUCCESS) {
    zend_hash_index_del(Z_ARRVAL_PP(val), 2);
    zend_hash_move_forward_ex(Z_ARRVAL_P(return_value), &pos);
  }
}

static PHP_FUNCTION(dql_clause_explode_count_brackets)
{
  char *str, *regexp, *e1 = "(", *e2 = ")";
  int str_len, regexp_len, e1_len, e2_len;

  array_init(return_value);

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|ss", &str, &str_len, &regexp, &regexp_len, &e1, &e1_len, &e2, &e2_len)) {
    return;
  }

  zval *quoteTerms, *tmpStr, *tmpRegexp, *tmpE1, *tmpE2;
  MAKE_STD_ZVAL(quoteTerms);
  MAKE_STD_ZVAL(tmpStr); ZVAL_STRING(tmpStr, str, 1);
  MAKE_STD_ZVAL(tmpRegexp); ZVAL_STRING(tmpRegexp, regexp, 1);
  MAKE_STD_ZVAL(tmpE1); ZVAL_STRING(tmpE1, e1, 1);
  MAKE_STD_ZVAL(tmpE2); ZVAL_STRING(tmpE2, e2, 1);
  zend_function *func; PHP_DQL_FUNC1(dql_quoted_string_explode, quoteTerms, this_ptr, tmpStr);

  char  *string_key;
  int i = 0, string_key_len;
  zval **val;
  ulong key;
  HashPosition pos;

  zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(quoteTerms), &pos);
  while (zend_hash_get_current_data_ex(Z_ARRVAL_P(quoteTerms), (void **)&val, &pos) == SUCCESS) {
    convert_to_string_ex(val);
    zend_hash_get_current_key_ex(Z_ARRVAL_P(quoteTerms), &string_key, &string_key_len, &key, 0, &pos);

    if (key & 1) {
      zval **pterms, **pt0, **pt1, **pt2;
      if (zend_hash_index_find(Z_ARRVAL_P(return_value), i - 1, (void **)&pterms) == SUCCESS) {
          zend_hash_index_find(Z_ARRVAL_PP(pterms), 1, (void **)&pt1);
          convert_to_string_ex(pt1);
          if (0 == Z_STRLEN_PP(pt1)) {
            zend_hash_index_find(Z_ARRVAL_PP(pterms), 0, (void **)&pt0); convert_to_string_ex(pt0);

            smart_str s = {0};
            smart_str_appendl(&s, Z_STRVAL_PP(pt0), Z_STRLEN_PP(pt0));
            smart_str_appendl(&s, Z_STRVAL_PP(val), Z_STRLEN_PP(val));
            smart_str_0(&s);

            add_index_stringl(*pterms, 0, s.c, s.len, 1);
          } else {
            zval *tmpArr;
            MAKE_STD_ZVAL(tmpArr);
            array_init(tmpArr);

            add_next_index_stringl(tmpArr, Z_STRVAL_PP(val), Z_STRLEN_PP(val), 1);
            add_next_index_stringl(tmpArr, "", 0, 1);
            add_next_index_long(tmpArr, 0);

            add_index_zval(pterms, i++, tmpArr);
          }
      }
    } else {
      zval *subterms, *_subterms, *tmpVal;
      MAKE_STD_ZVAL(subterms); array_init(subterms);
      MAKE_STD_ZVAL(_subterms);
      MAKE_STD_ZVAL(tmpVal); ZVAL_STRING(tmpVal, Z_STRVAL_PP(val), 1);
      zend_function *func; PHP_DQL_FUNC2(dql_clause_explode_non_quoted, _subterms, this_ptr, tmpVal, tmpRegexp);

      char  *sub_string_key;
      int sub_string_key_len;
      zval **sub;
      ulong sub_key;
      HashPosition sub_pos;

      zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(_subterms), &sub_pos);
      while (zend_hash_get_current_data_ex(Z_ARRVAL_P(_subterms), (void **)&sub, &sub_pos) == SUCCESS) {
        zval *c1, *c2, *ts, **tmpSub0, **tmpSub1;
        MAKE_STD_ZVAL(c1); MAKE_STD_ZVAL(c2);

        zend_hash_index_find(Z_ARRVAL_PP(sub), 0, (void **)&tmpSub0);
        zend_hash_index_find(Z_ARRVAL_PP(sub), 1, (void **)&tmpSub1);
        MAKE_STD_ZVAL(ts); ZVAL_STRING(ts, Z_STRVAL_PP(tmpSub0), 1);

        zend_function *func;
        PHP_DQL_FUNC2(substr_count, c1, this_ptr, ts, tmpE1);
        PHP_DQL_FUNC2(substr_count, c2, this_ptr, ts, tmpE2);

        zval *_v; MAKE_STD_ZVAL(_v); array_init(_v);
        add_next_index_string(_v, Z_STRVAL_PP(tmpSub0), 1);
        add_next_index_string(_v, Z_STRVAL_PP(tmpSub1), 1);
        add_next_index_long(_v, (Z_LVAL_P(c1) - Z_LVAL_P(c2)));
        add_next_index_zval(subterms, _v);

        zend_hash_move_forward_ex(Z_ARRVAL_P(_subterms), &sub_pos);
      }

      if (i > 0) {
        zval **pterms, **pt0, **pt1, **pt2;
        if (zend_hash_index_find(Z_ARRVAL_P(return_value), i - 1, (void **)&pterms) == SUCCESS) {
          zend_hash_index_find(Z_ARRVAL_PP(pterms), 1, (void **)&pt1);
          convert_to_string_ex(pt1);
          if (0 == Z_STRLEN_PP(pt1)) {
            zval *first, **f0, **f1, **f2, *uba;
            smart_str s = {0};
            MAKE_STD_ZVAL(first);

            zend_function *func; PHP_DQL_FUNC1(array_shift, first, this_ptr, subterms);

            zend_hash_index_find(Z_ARRVAL_P(first), 0, (void **)&f0); convert_to_string_ex(f0);
            zend_hash_index_find(Z_ARRVAL_P(first), 1, (void **)&f1); convert_to_string_ex(f1);
            zend_hash_index_find(Z_ARRVAL_P(first), 2, (void **)&f2);

            zend_hash_index_find(Z_ARRVAL_PP(pterms), 0, (void **)&pt0); convert_to_string_ex(pt0);
            zend_hash_index_find(Z_ARRVAL_PP(pterms), 2, (void **)&pt2);

            smart_str_appendl(&s, Z_STRVAL_PP(pt0), Z_STRLEN_PP(pt0));
            smart_str_appendl(&s, Z_STRVAL_PP(f0), Z_STRLEN_PP(f0));
            smart_str_0(&s);

            add_index_stringl(*pterms, 0, s.c, s.len, 1);
            add_index_stringl(*pterms, 1, Z_STRVAL_PP(f1), Z_STRLEN_PP(f1), 1);
            add_index_long(*pterms, 2, Z_LVAL_PP(pt2) + Z_LVAL_PP(f2));
          }
        }
      }

      php_array_merge(Z_ARRVAL_P(return_value), Z_ARRVAL_P(subterms), 0 TSRMLS_CC);
      i += zend_hash_num_elements(Z_ARRVAL_P(subterms));
    }

    zend_hash_move_forward_ex(Z_ARRVAL_P(quoteTerms), &pos);
  }
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

  zend_function *func; PHP_DQL_FUNC4(preg_split, tmp, this_ptr, pattern, input, limit, flags);

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
  zval *terms, **val, **data;
  int i = 0, string_key_len;;
  char *string_key;
  ulong key;
  HashPosition pos;

  array_init(return_value);

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &terms)) {
    return;
  }

  zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(terms), &pos);
  while (zend_hash_get_current_data_ex(Z_ARRVAL_P(terms), (void **)&val, &pos) == SUCCESS) {
    zval **v0, **v1, **v2;
    int count = 0;
    zend_hash_index_find(Z_ARRVAL_PP(val), 0, (void **)&v0); convert_to_string_ex(v0);
    zend_hash_index_find(Z_ARRVAL_PP(val), 1, (void **)&v1); convert_to_string_ex(v1);
    zend_hash_index_find(Z_ARRVAL_PP(val), 2, (void **)&v2); convert_to_long_ex(v2);

    if (zend_hash_index_find(Z_ARRVAL_P(return_value), i, (void **)&data) == FAILURE) {
      zval *tmp; MAKE_STD_ZVAL(tmp); array_init(tmp);

      count = Z_LVAL_PP(v2);

      add_next_index_stringl(tmp, Z_STRVAL_PP(v0), Z_STRLEN_PP(v0), 1);
      add_next_index_stringl(tmp, Z_STRVAL_PP(v1), Z_STRLEN_PP(v1), 1);
      add_next_index_long(tmp, count);

      add_index_zval(return_value, i, tmp);
    } else {
      zval **d0, **d1, **d2;
      smart_str s = {0};

      zend_hash_index_find(Z_ARRVAL_PP(data), 0, (void **)&d0); convert_to_string_ex(d0);
      zend_hash_index_find(Z_ARRVAL_PP(data), 1, (void **)&d1); convert_to_string_ex(d1);
      zend_hash_index_find(Z_ARRVAL_PP(data), 2, (void **)&d2); convert_to_long_ex(d2);

      smart_str_appendl(&s, Z_STRVAL_PP(d0), Z_STRLEN_PP(d0));
      smart_str_appendl(&s, Z_STRVAL_PP(d1), Z_STRLEN_PP(d1));
      smart_str_appendl(&s, Z_STRVAL_PP(v0), Z_STRLEN_PP(v0));
      smart_str_0(&s);

      count = Z_LVAL_PP(d2) + Z_LVAL_PP(v2);

      add_index_stringl(*data, 0, s.c, s.len, 1);
      add_index_stringl(*data, 1, Z_STRVAL_PP(v1), Z_STRLEN_PP(v1), 1);
      add_index_long(*data, 2, count);
    }

    if (0 == count) {
      i++;
    }

    zend_hash_move_forward_ex(Z_ARRVAL_P(terms), &pos);
  }
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

  zend_function *func; PHP_DQL_FUNC4(preg_split, tmp, this_ptr, pattern, input, limit, flags);

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
