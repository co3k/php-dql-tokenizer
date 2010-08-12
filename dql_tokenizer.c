#include "php_dql_tokenizer.h"

#include "ext/standard/php_string.h"
#include "ext/standard/php_array.h"
#include "ext/standard/php_smart_str.h"

#include "ext/standard/php_var.h"

#ifdef COMPILE_DL_DQL_TOKENIZER
ZEND_GET_MODULE(dql_tokenizer)
#endif

/* {{{ PHP_MINFO_FUNCTION(dql_tokenizer) */
static PHP_MINFO_FUNCTION(dql_tokenizer)
{
  php_info_print_table_start();
  php_info_print_table_row(2, "Version", PHP_DQL_TOKENIZER_EXTVAR);
  php_info_print_table_row(2, "Build Date", __DATE__ " " __TIME__);
  php_info_print_table_end();
  DISPLAY_INI_ENTRIES();
}
/* }}} */


static void _dql_merge_bracket_terms(zval *return_value, zval *terms)
{
  zval **val, **data;
  int i = 0, string_key_len;;
  char *string_key;
  ulong key;
  HashPosition pos;

  array_init(return_value);

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

      smart_str_free(&s);
    }

    if (0 == count) {
      i++;
    }

    zend_hash_move_forward_ex(Z_ARRVAL_P(terms), &pos);
  }
}

static void _dql_clause_explode_non_quoted(zval *this_ptr, zval *return_value, char *str, char *regexp)
{
  TSRMLS_FETCH();

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

static void _dql_quoted_string_explode(zval *this_ptr, zval *return_value, char *str)
{
  TSRMLS_FETCH();

  zval *pattern, *input, *limit, *flags, *tmp;
  MAKE_STD_ZVAL(pattern); ZVAL_STRING(pattern, "#(\\\\'|''|'|\\\"|\"\"|\")#", 1);

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

          smart_str_free(&s);
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

      smart_str_free(&s);
    }

    zend_hash_move_forward_ex(Z_ARRVAL_P(tmp), &pos);
  }

  zval_ptr_dtor(&pattern);
  zval_ptr_dtor(&input);
  zval_ptr_dtor(&limit);
  zval_ptr_dtor(&flags);
  zval_ptr_dtor(&tmp);
}

char *_dql_get_split_regexp_from_array(zval *this_ptr, HashTable *ht)
{
  zval **entry;
  HashPosition pos;
  TSRMLS_FETCH();

  smart_str result = {0};
  smart_str_appendl(&result, "#(", 2);

  zend_hash_internal_pointer_reset_ex(ht, &pos);
  while (zend_hash_get_current_data_ex(ht, (void **)&entry, &pos) == SUCCESS) {
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
      zend_function *func;
      PHP_DQL_FUNC1(preg_quote, filtered, this_ptr, pre);

      smart_str_appendl(&result, Z_STRVAL_P(filtered), Z_STRLEN_P(filtered));

      zval_ptr_dtor(&pre);
      zval_ptr_dtor(&filtered);
    }

    zend_hash_move_forward_ex(ht, &pos);
  }

  smart_str_appendl(&result, ")#", 2);

  smart_str_0(&result);

  if (result.len) {
    return result.c;
  } else {
    smart_str_free(&result);
    return NULL;
  }
}

static void _dql_clause_explode_count_brackets(zval *this_ptr, zval *return_value, char *str, char *regexp, char *e1, char *e2)
{
  TSRMLS_FETCH();

  if (e1 == NULL) {
    e1 = "(";
  }
  if (e2 == NULL) {
    e2 = ")";
  }

  array_init(return_value);

  zval *quoteTerms; MAKE_STD_ZVAL(quoteTerms);
  _dql_quoted_string_explode(this_ptr, quoteTerms, str);

  char  *string_key;
  int i = 0, string_key_len;
  zval **val, *tmpE1, *tmpE2;
  ulong key;
  HashPosition pos;

  MAKE_STD_ZVAL(tmpE1); ZVAL_STRING(tmpE1, e1, 1);
  MAKE_STD_ZVAL(tmpE2); ZVAL_STRING(tmpE2, e2, 1);

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

            smart_str_free(&s);
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
      zval *subterms, *_subterms;
      MAKE_STD_ZVAL(subterms); array_init(subterms);
      MAKE_STD_ZVAL(_subterms);
      _dql_clause_explode_non_quoted(this_ptr, _subterms, Z_STRVAL_PP(val), regexp);

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

            smart_str_free(&s);
          }
        }
      }

      php_array_merge(Z_ARRVAL_P(return_value), Z_ARRVAL_P(subterms), 0 TSRMLS_CC);
      i += zend_hash_num_elements(Z_ARRVAL_P(subterms));
    }

    zend_hash_move_forward_ex(Z_ARRVAL_P(quoteTerms), &pos);
  }
}

static void _dql_clause_explode_regexp(zval *this_ptr, zval *return_value, char *str, char *regexp, char *e1, char *e2)
{
  if (e1 == NULL)
  {
    e1 = "(";
  }
  if (e2 == NULL)
  {
    e2 = ")";
  }

  array_init(return_value);

  zval *tmpTerms; MAKE_STD_ZVAL(tmpTerms);
  _dql_clause_explode_count_brackets(this_ptr, tmpTerms, str, regexp, e1, e2);
  _dql_merge_bracket_terms(return_value, tmpTerms);

  zval **val;
  HashPosition pos;

  zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(return_value), &pos);
  while (zend_hash_get_current_data_ex(Z_ARRVAL_P(return_value), (void **)&val, &pos) == SUCCESS) {
    zend_hash_index_del(Z_ARRVAL_PP(val), 2);
    zend_hash_move_forward_ex(Z_ARRVAL_P(return_value), &pos);
  }
}

static void _dql_clause_explode(zval *this_ptr, zval *return_value, char *str, zval *d, char *e1, char *e2)
{
  if (e1 == NULL) {
    e1 = "(";
  }

  if (e2 == NULL) {
    e2 = ")";
  }

  char *regexp;
  regexp = _dql_get_split_regexp_from_array(this_ptr, Z_ARRVAL_P(d));
  _dql_clause_explode_regexp(this_ptr, return_value, str, regexp, e1, e2);
}

static void _dql_sql_explode(zval *this_ptr, zval *return_value, char *str, zval *d, char *e1, char *e2)
{
  zval *_d;

  if (e1 == NULL) {
    e1 = "(";
  }

  if (e2 == NULL) {
    e2 = ")";
  }

  if (d == NULL)
  {
    MAKE_STD_ZVAL(d);
    ZVAL_STRING(d, " ", 1);
  }

  MAKE_STD_ZVAL(_d);
  if (IS_STRING == Z_TYPE(*d))
  {
    array_init(_d);

    add_next_index_stringl(_d, Z_STRVAL_P(d), Z_STRLEN_P(d), 1);
  }
  else
  {
    ZVAL_ZVAL(_d, d, 1, 1);
  }

  zval *terms; MAKE_STD_ZVAL(terms);
  _dql_clause_explode(this_ptr, terms, str, _d, e1, e2);

  zval **val;
  HashPosition pos;

  array_init(return_value);

  zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(terms), &pos);
  while (zend_hash_get_current_data_ex(Z_ARRVAL_P(terms), (void **)&val, &pos) == SUCCESS) {
    zval **vs;
    zend_hash_index_find(Z_ARRVAL_PP(val), 0, (void **)&vs); convert_to_string_ex(vs);

    add_next_index_stringl(return_value, Z_STRVAL_PP(vs), Z_STRLEN_PP(vs), 1);

    zend_hash_move_forward_ex(Z_ARRVAL_P(terms), &pos);
  }
}

static PHP_FUNCTION(dql_tokenize_query)
{
  char *query;
  int query_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &query, &query_len)) {
    return;
  }

  array_init(return_value);

  zval *tokens; MAKE_STD_ZVAL(tokens);
  _dql_sql_explode(this_ptr, tokens, query, NULL, NULL, NULL);

  char *string_key, *p = NULL;
  int i = 0, string_key_len;
  zval **token, **pre, **next;
  ulong index;
  HashPosition pos;

  zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(tokens), &pos);
  while (zend_hash_get_current_data_ex(Z_ARRVAL_P(tokens), (void **)&token, &pos) == SUCCESS) {
    char *t;
    convert_to_string_ex(token);
    zend_hash_get_current_key_ex(Z_ARRVAL_P(tokens), &string_key, &string_key_len, &index, 0, &pos);

    t = php_trim(Z_STRVAL_PP(token), Z_STRLEN_PP(token), NULL, 0, NULL, 3 TSRMLS_CC);

    int type = NULL;
    const char *k;
    php_dql_keyword_map *mapping;
    for (mapping = php_dql_tokens; mapping->keywords != NULL; mapping++) {
      for (k = mapping->keywords; *k != '\0'; k += (strlen(k) + 1)) {
        if (strcasecmp(k, t) == 0) {
          type = mapping->code;
          break;
        }
      }
    }

    switch (type) {
      case DQL_TOKEN_BASE:
        p = t;
        add_assoc_stringl(return_value, t, "", 0, 1);
        break;
      case DQL_TOKEN_BY:
        if (zend_hash_index_find(Z_ARRVAL_P(tokens), index + 1, (void **)&next) == SUCCESS && strcasecmp("by", Z_STRVAL_PP(next)) == 0) {
          p = t;
          add_assoc_stringl(return_value, t, "", 0, 1);
        } else {
          zend_hash_find(Z_ARRVAL_P(return_value), p, strlen(p), (void **)&pre);

          smart_str s = {0};
          if (zend_hash_find(Z_ARRVAL_P(return_value), p, strlen(p) + 1, (void **)&pre) == SUCCESS) {
            smart_str_appendl(&s, Z_STRVAL_PP(pre), Z_STRLEN_PP(pre));
          }
          smart_str_appendl(&s, t, strlen(t));
          smart_str_appendl(&s, " ", 1);
          smart_str_0(&s);

          add_assoc_stringl(return_value, p, s.c, s.len, 1);

          smart_str_free(&s);
        }
        break;
      case DQL_TOKEN_PASS:
        zend_hash_move_forward_ex(Z_ARRVAL_P(tokens), &pos);
        continue;
      default:
        if (p == NULL) {
          php_error_docref(NULL TSRMLS_CC, E_ERROR, "Couldn't tokenize query.");
        }

        smart_str s = {0};
        if (zend_hash_find(Z_ARRVAL_P(return_value), p, strlen(p) + 1, (void **)&pre) == SUCCESS) {
          smart_str_appendl(&s, Z_STRVAL_PP(pre), Z_STRLEN_PP(pre));
        }
        smart_str_appendl(&s, t, strlen(t));
        smart_str_appendl(&s, " ", 1);
        smart_str_0(&s);

        add_assoc_stringl(return_value, p, s.c, s.len, 1);

        smart_str_free(&s);
    }

    zend_hash_move_forward_ex(Z_ARRVAL_P(tokens), &pos);
  }
}

static PHP_FUNCTION(dql_bracket_trim)
{
  char *str, *e1 = "(", *e2 = ")";
  int str_len, e1_len, e2_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|ss", &str, &str_len, &e1, &e1_len, &e2, &e2_len)) {
    return;
  }

  if (str[0] == '(' && str[str_len - 1] == ')') {
    RETURN_STRINGL(str + 1, str_len - 2, 1);
  } else {
    RETURN_STRINGL(str, str_len, 1);
  }
}

static PHP_FUNCTION(dql_bracket_explode)
{
  zval *d = NULL, *_d;
  char *str, *e1 = "(", *e2 = ")";
  int str_len, e1_len, e2_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|zss", &str, &str_len, &d, &e1, &e1_len, &e2, &e2_len)) {
    return;
  }

  if (d == NULL)
  {
    MAKE_STD_ZVAL(d);
    ZVAL_STRING(d, " ", 1);
  }

  MAKE_STD_ZVAL(_d);
  if (IS_STRING == Z_TYPE(*d))
  {
    array_init(_d);

    add_next_index_stringl(_d, Z_STRVAL_P(d), Z_STRLEN_P(d), 1);
  }
  else
  {
    ZVAL_ZVAL(_d, d, 1, 1);
  }

  char *regexp;
  regexp = _dql_get_split_regexp_from_array(this_ptr, Z_ARRVAL_P(_d));

  smart_str s = {0};
  smart_str_appendl(&s, regexp, strlen(regexp));
  smart_str_appendl(&s, "i", 1);
  smart_str_0(&s);

  zval *terms;
  MAKE_STD_ZVAL(terms);
  _dql_clause_explode_regexp(this_ptr, terms, str, s.c, e1, e2);

  zval **val;
  HashPosition pos;

  array_init(return_value);

  zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(terms), &pos);
  while (zend_hash_get_current_data_ex(Z_ARRVAL_P(terms), (void **)&val, &pos) == SUCCESS) {
    zval **vs;
    zend_hash_index_find(Z_ARRVAL_PP(val), 0, (void **)&vs); convert_to_string_ex(vs);

    add_next_index_stringl(return_value, Z_STRVAL_PP(vs), Z_STRLEN_PP(vs), 1);

    zend_hash_move_forward_ex(Z_ARRVAL_P(terms), &pos);
  }

  smart_str_free(&s);
}

static PHP_FUNCTION(dql_quote_explode)
{
  zval *d = NULL, *_d;
  char *str;
  int str_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|z", &str, &str_len, &d)) {
    return;
  }

  if (d == NULL)
  {
    MAKE_STD_ZVAL(d);
    ZVAL_STRING(d, " ", 1);
  }

  MAKE_STD_ZVAL(_d);
  if (IS_STRING == Z_TYPE(*d))
  {
    array_init(_d);

    add_next_index_stringl(_d, Z_STRVAL_P(d), Z_STRLEN_P(d), 1);
  }
  else
  {
    ZVAL_ZVAL(_d, d, 1, 1);
  }

  char *regexp;
  regexp = _dql_get_split_regexp_from_array(this_ptr, Z_ARRVAL_P(_d));

  smart_str s = {0};
  smart_str_appendl(&s, regexp, strlen(regexp));
  smart_str_appendl(&s, "i", 1);
  smart_str_0(&s);

  zval *terms; MAKE_STD_ZVAL(terms);
  _dql_clause_explode_count_brackets(this_ptr, terms, str, s.c, NULL, NULL);

  zval **val;
  HashPosition pos;

  array_init(return_value);

  zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(terms), &pos);
  while (zend_hash_get_current_data_ex(Z_ARRVAL_P(terms), (void **)&val, &pos) == SUCCESS) {
    zval **vs;
    zend_hash_index_find(Z_ARRVAL_PP(val), 0, (void **)&vs); convert_to_string_ex(vs);

    add_next_index_stringl(return_value, Z_STRVAL_PP(vs), Z_STRLEN_PP(vs), 1);

    zend_hash_move_forward_ex(Z_ARRVAL_P(terms), &pos);
  }

  smart_str_free(&s);
}

static PHP_FUNCTION(dql_sql_explode)
{
  zval *d = NULL;
  char *str, *e1 = "(", *e2 = ")";
  int str_len, e1_len, e2_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|zss", &str, &str_len, &d, &e1, &e1_len, &e2, &e2_len)) {
    return;
  }

  _dql_sql_explode(this_ptr, return_value, str, d, e1, e2);
}

static PHP_FUNCTION(dql_clause_explode)
{
  zval *d;
  char *str, *e1 = "(", *e2 = ")", *regexp;
  int str_len, e1_len, e2_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sa|ss", &str, &str_len, &d, &e1, &e1_len, &e2, &e2_len)) {
    return;
  }

  _dql_clause_explode(this_ptr, return_value, str, d, e1, e2);
}

static PHP_FUNCTION(dql_get_split_regexp_from_array)
{
  char *result;
  zval *array;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &array)) {
    return;
  }

  result = _dql_get_split_regexp_from_array(this_ptr, Z_ARRVAL_P(array));
  if (result) {
    RETURN_STRINGL(result, strlen(result), 0);
  } else {
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

  _dql_clause_explode_regexp(this_ptr, return_value, str, regexp, e1, e2);
}

static PHP_FUNCTION(dql_clause_explode_count_brackets)
{
  char *str, *regexp, *e1 = "(", *e2 = ")";
  int str_len, regexp_len, e1_len, e2_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|ss", &str, &str_len, &regexp, &regexp_len, &e1, &e1_len, &e2, &e2_len)) {
    return;
  }

  _dql_clause_explode_count_brackets(this_ptr, return_value, str, regexp, e1, e2);
}

static PHP_FUNCTION(dql_clause_explode_non_quoted)
{
  char *str, *regexp;
  int len1, len2;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &str, &len1, &regexp, &len2)) {
    return;
  }

  _dql_clause_explode_non_quoted(this_ptr, return_value, str, regexp);
}

static PHP_FUNCTION(dql_merge_bracket_terms)
{
  zval *terms;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &terms)) {
    return;
  }

  _dql_merge_bracket_terms(return_value, terms);
}

static PHP_FUNCTION(dql_quoted_string_explode)
{
  char *str;
  int len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &len)) {
    return;
  }

  _dql_quoted_string_explode(this_ptr, return_value, str);
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
  PHP_MINFO(dql_tokenizer),
#if ZEND_MODULE_API_NO >= 20010901
  PHP_DQL_TOKENIZER_EXTVAR,
#endif
  STANDARD_MODULE_PROPERTIES
};
