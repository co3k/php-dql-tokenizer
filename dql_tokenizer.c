/**
 *  Copyright 2010 Kousuke Ebihara
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/* {{{ includes */
#include "php_dql_tokenizer.h"

#include "ext/standard/php_string.h"
#include "ext/standard/php_array.h"
#include "ext/standard/php_smart_str.h"

#include "ext/standard/php_var.h"
/* }}} */

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

/* {{{ void _dql_merge_bracket_terms()
 */
static void _dql_merge_bracket_terms(zval *return_value, zval *terms)
{
	zval **val, **data;
	int i = 0, string_key_len;;
	char *string_key;
	ulong key;
	HashPosition pos;

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
/* }}} */

/* {{{ void _dql_clause_explode_non_quoted()
 */
static void _dql_clause_explode_non_quoted(zval *this_ptr, zval *return_value, char *str, char *regexp)
{
	TSRMLS_FETCH();

	zval *pattern, *input, *limit, *flags, *tmp;
	MAKE_STD_ZVAL(pattern); ZVAL_STRING(pattern, regexp, 1);
	MAKE_STD_ZVAL(input); ZVAL_STRING(input, str, 1);
	MAKE_STD_ZVAL(limit); ZVAL_LONG(limit, -1);
	MAKE_STD_ZVAL(flags); ZVAL_LONG(flags, PHP_DQL_PREG_SPLIT_DELIM_CAPTURE);
	MAKE_STD_ZVAL(tmp);

	zend_function *func; PHP_DQL_FUNC4(preg_split, tmp, this_ptr, pattern, input, limit, flags);

	char  *string_key;
	int i = 0, string_key_len;
	zval **val;
	ulong key;
	HashPosition pos;

	zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(tmp), &pos);
	while (zend_hash_get_current_data_ex(Z_ARRVAL_P(tmp), (void **)&val, &pos) == SUCCESS) {
		convert_to_string_ex(val);

		zend_hash_get_current_key_ex(Z_ARRVAL_P(tmp), &string_key, &string_key_len, &key, 0, &pos);

		if (!(key & 1)) {
			zval *tmp_arr;

			MAKE_STD_ZVAL(tmp_arr);
			array_init(tmp_arr);

			add_next_index_stringl(tmp_arr, Z_STRVAL_PP(val), Z_STRLEN_PP(val), 1);
			add_next_index_stringl(tmp_arr, "", 0, 1);

			add_index_zval(return_value, i, tmp_arr);
		} else {
			zval **subarray;
			zend_hash_index_find(Z_ARRVAL_P(return_value), i++, (void **)&subarray);
			add_index_stringl(*subarray, 1, Z_STRVAL_PP(val), Z_STRLEN_PP(val), 1);
		}
		zend_hash_move_forward_ex(Z_ARRVAL_P(tmp), &pos);
	}

	zval_ptr_dtor(&pattern);
	zval_ptr_dtor(&input);
	zval_ptr_dtor(&limit);
	zval_ptr_dtor(&flags);
	zval_ptr_dtor(&tmp);
}
/* }}} */

/* {{{ void _dql_quoted_string_explode()
 */
static void _dql_quoted_string_explode(zval *this_ptr, zval *return_value, char *str)
{
	TSRMLS_FETCH();

	zval *pattern, *input, *limit, *flags, *tmp;
	MAKE_STD_ZVAL(pattern); ZVAL_STRING(pattern, "#(\\\\'|''|'|\\\"|\"\"|\")#", 1);

	MAKE_STD_ZVAL(input); ZVAL_STRING(input, str, 1);
	MAKE_STD_ZVAL(limit); ZVAL_LONG(limit, -1);
	MAKE_STD_ZVAL(flags); ZVAL_LONG(flags, PHP_DQL_PREG_SPLIT_DELIM_CAPTURE);
	MAKE_STD_ZVAL(tmp);

	zend_function *func; PHP_DQL_FUNC4(preg_split, tmp, this_ptr, pattern, input, limit, flags);

	char *mode = '\0', *string_key;
	int i = 0, string_key_len;
	ulong key;
	zval **val, **data;
	HashPosition pos;

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
/* }}} */

/* {{{ char *_dql_get_split_regexp_from_array()
 */
char *_dql_get_split_regexp_from_array(zval *this_ptr, HashTable *ht)
{
	char *retval;

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
		const char *__s = (result.c);
		retval = estrndup(__s, result.len);
	}

	smart_str_free(&result);

	return retval;
}
/* }}} */

/* {{{ void _dql_clause_explode_count_brackets()
 */
static void _dql_clause_explode_count_brackets(zval *this_ptr, zval *return_value, char *str, char *regexp, char *e1, char *e2)
{
	TSRMLS_FETCH();

	if (e1 == NULL) {
		e1 = "(";
	}
	if (e2 == NULL) {
		e2 = ")";
	}

	zval *quote_terms; MAKE_STD_ZVAL(quote_terms); array_init(quote_terms);
	_dql_quoted_string_explode(this_ptr, quote_terms, str);

	char  *string_key;
	int i = 0, string_key_len;
	zval **val, *tmp_e1, *tmp_e2;
	ulong key;
	HashPosition pos;

	MAKE_STD_ZVAL(tmp_e1); ZVAL_STRING(tmp_e1, e1, 1);
	MAKE_STD_ZVAL(tmp_e2); ZVAL_STRING(tmp_e2, e2, 1);

	zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(quote_terms), &pos);
	while (zend_hash_get_current_data_ex(Z_ARRVAL_P(quote_terms), (void **)&val, &pos) == SUCCESS) {
		convert_to_string_ex(val);
		zend_hash_get_current_key_ex(Z_ARRVAL_P(quote_terms), &string_key, &string_key_len, &key, 0, &pos);

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
					zval *tmp_arr;
					MAKE_STD_ZVAL(tmp_arr);
					array_init(tmp_arr);

					add_next_index_stringl(tmp_arr, Z_STRVAL_PP(val), Z_STRLEN_PP(val), 1);
					add_next_index_stringl(tmp_arr, "", 0, 1);
					add_next_index_long(tmp_arr, 0);

					add_index_zval(*pterms, i++, tmp_arr);
				}
			}
		} else {
			zval *subterms, *_subterms;
			MAKE_STD_ZVAL(subterms); array_init(subterms);
			MAKE_STD_ZVAL(_subterms); array_init(_subterms);
			_dql_clause_explode_non_quoted(this_ptr, _subterms, Z_STRVAL_PP(val), regexp);

			char  *sub_string_key;
			int sub_string_key_len;
			zval **sub;
			ulong sub_key;
			HashPosition sub_pos;

			zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(_subterms), &sub_pos);
			while (zend_hash_get_current_data_ex(Z_ARRVAL_P(_subterms), (void **)&sub, &sub_pos) == SUCCESS) {
				zval *c1, *c2, *ts, **tmp_sub0, **tmp_sub1;
				MAKE_STD_ZVAL(c1); MAKE_STD_ZVAL(c2);

				zend_hash_index_find(Z_ARRVAL_PP(sub), 0, (void **)&tmp_sub0);
				zend_hash_index_find(Z_ARRVAL_PP(sub), 1, (void **)&tmp_sub1);
				MAKE_STD_ZVAL(ts); ZVAL_STRING(ts, Z_STRVAL_PP(tmp_sub0), 1);

				zend_function *func;
				PHP_DQL_FUNC2(substr_count, c1, this_ptr, ts, tmp_e1);
				PHP_DQL_FUNC2(substr_count, c2, this_ptr, ts, tmp_e2);

				zval *_v; MAKE_STD_ZVAL(_v); array_init(_v);
				add_next_index_string(_v, Z_STRVAL_PP(tmp_sub0), 1);
				add_next_index_string(_v, Z_STRVAL_PP(tmp_sub1), 1);
				add_next_index_long(_v, (Z_LVAL_P(c1) - Z_LVAL_P(c2)));
				add_next_index_zval(subterms, _v);

				zend_hash_move_forward_ex(Z_ARRVAL_P(_subterms), &sub_pos);
				zval_ptr_dtor(&c1);
				zval_ptr_dtor(&c2);
				zval_ptr_dtor(&ts);
			}

			if (i > 0) {
				zval **pterms, **pt0, **pt1, **pt2;
				if (zend_hash_index_find(Z_ARRVAL_P(return_value), i - 1, (void **)&pterms) == SUCCESS) {
					zend_hash_index_find(Z_ARRVAL_PP(pterms), 1, (void **)&pt1);
					convert_to_string_ex(pt1);
					if (0 == Z_STRLEN_PP(pt1)) {
						zval *first, **f0, **f1, **f2;
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
						zval_ptr_dtor(&first);
					}
				}
			}

			php_array_merge(Z_ARRVAL_P(return_value), Z_ARRVAL_P(subterms), 0 TSRMLS_CC);
			i += zend_hash_num_elements(Z_ARRVAL_P(subterms));

			zval_ptr_dtor(&subterms);
			zval_ptr_dtor(&_subterms);
		}

		zend_hash_move_forward_ex(Z_ARRVAL_P(quote_terms), &pos);
	}

	zval_ptr_dtor(&quote_terms);
	zval_ptr_dtor(&tmp_e1);
	zval_ptr_dtor(&tmp_e2);
}
/* }}} */

/* {{{ void _dql_clause_explode_regexp()
 */
static void _dql_clause_explode_regexp(zval *this_ptr, zval *return_value, char *str, char *regexp, char *e1, char *e2)
{
	if (e1 == NULL) {
		e1 = "(";
	}
	if (e2 == NULL) {
		e2 = ")";
	}

	zval *tmp_terms; MAKE_STD_ZVAL(tmp_terms); array_init(tmp_terms);
	_dql_clause_explode_count_brackets(this_ptr, tmp_terms, str, regexp, e1, e2);
	_dql_merge_bracket_terms(return_value, tmp_terms);

	zval **val;
	HashPosition pos;

	zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(return_value), &pos);
	while (zend_hash_get_current_data_ex(Z_ARRVAL_P(return_value), (void **)&val, &pos) == SUCCESS) {
		zend_hash_index_del(Z_ARRVAL_PP(val), 2);
		zend_hash_move_forward_ex(Z_ARRVAL_P(return_value), &pos);
	}

	zval_ptr_dtor(&tmp_terms);
}
/* }}} */

/* {{{ void _dql_clause_explode()
 */
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

	efree(regexp);
}
/* }}} */

/* {{{ void _dql_sql_explode()
 */
static void _dql_sql_explode(zval *this_ptr, zval *return_value, char *str, zval *d, char *e1, char *e2)
{
	zval *_d, *terms;
	MAKE_STD_ZVAL(_d); MAKE_STD_ZVAL(terms);

	if (e1 == NULL) {
		e1 = "(";
	}

	if (e2 == NULL) {
		e2 = ")";
	}

	if (d == NULL || IS_STRING == Z_TYPE(*d)) {
		array_init(_d);

		if (d == NULL) {
			add_next_index_stringl(_d, " ", 1, 1);
		} else {
			add_next_index_stringl(_d, Z_STRVAL_P(d), Z_STRLEN_P(d), 1);
		}
	} else {
		ZVAL_ZVAL(_d, d, 1, 1);
	}

	array_init(terms);
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

	zval_ptr_dtor(&_d); zval_ptr_dtor(&terms);
}
/* }}} */

/* {{{ proto array dql_tokenize_query(string query)
   Splits the given dql query into an array where keys represent different query part names and values are arrays splitted using sqlExplode method */
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

		int type = DQL_TOKEN_UNDEFINED;
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

		char *_p = p;
		switch (type) {
			case DQL_TOKEN_BASE:
				efree(_p);

				p = estrndup(t, strlen(t));
				add_assoc_stringl(return_value, t, "", 0, 1);
				break;
			case DQL_TOKEN_BY:
				if (zend_hash_index_find(Z_ARRVAL_P(tokens), index + 1, (void **)&next) == SUCCESS && strcasecmp("by", Z_STRVAL_PP(next)) == 0) {
					efree(_p);

					p = estrndup(t, strlen(t));
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
				efree(t);
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

		efree(t);
		zend_hash_move_forward_ex(Z_ARRVAL_P(tokens), &pos);
	}

	efree(p);
	zval_ptr_dtor(&tokens);
}
/* }}} */

/* {{{ proto string dql_bracket_trim(string str [, string e1, string e2])
   Trims brackets from string */
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
/* }}} */

/* {{{ proto array dql_bracket_explode(string str [, mixed d, string e1, string e2])
   Explodes a sql expression respecting bracket placement. */
static PHP_FUNCTION(dql_bracket_explode)
{
	zval *d = NULL, *_d;
	char *str, *e1 = "(", *e2 = ")";
	int str_len, e1_len, e2_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|zss", &str, &str_len, &d, &e1, &e1_len, &e2, &e2_len)) {
		return;
	}

	MAKE_STD_ZVAL(_d);
	if (d == NULL) {
		array_init(_d);
		add_next_index_stringl(_d, " ", 1, 1);
	} else if (IS_STRING == Z_TYPE(*d)) {
		array_init(_d);
		add_next_index_stringl(_d, Z_STRVAL_P(d), Z_STRLEN_P(d), 1);
	} else {
		ZVAL_ZVAL(_d, d, 1, 0);
	}

	char *regexp;
	regexp = _dql_get_split_regexp_from_array(this_ptr, Z_ARRVAL_P(_d));

	smart_str s = {0};
	smart_str_appendl(&s, regexp, strlen(regexp));
	smart_str_appendl(&s, "i", 1);
	smart_str_0(&s);

	zval *terms; MAKE_STD_ZVAL(terms); array_init(terms);
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
	zval_ptr_dtor(&_d);
	zval_ptr_dtor(&terms);
	efree(regexp);
}
/* }}} */

/* {{{ proto array dql_quote_explode(string str [, mixed d])
   Explode quotes from string */
static PHP_FUNCTION(dql_quote_explode)
{
	zval *d = NULL, *_d;
	char *str;
	int str_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|z", &str, &str_len, &d)) {
		return;
	}

	MAKE_STD_ZVAL(_d);
	if (d == NULL) {
		array_init(_d);
		add_next_index_stringl(_d, " ", 1, 1);
	} else if (IS_STRING == Z_TYPE(*d)) {
		array_init(_d);
		add_next_index_stringl(_d, Z_STRVAL_P(d), Z_STRLEN_P(d), 1);
	} else {
		ZVAL_ZVAL(_d, d, 1, 0);
	}

	char *regexp;
	regexp = _dql_get_split_regexp_from_array(this_ptr, Z_ARRVAL_P(_d));

	smart_str s = {0};
	smart_str_appendl(&s, regexp, strlen(regexp));
	smart_str_appendl(&s, "i", 1);
	smart_str_0(&s);

	zval *terms; MAKE_STD_ZVAL(terms); array_init(terms);
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
	zval_ptr_dtor(&_d);
	zval_ptr_dtor(&terms);
	efree(regexp);
}
/* }}} */

/* {{{ proto array dql_sql_explode(string str [, mixed d, string e1, string e2])
   Explodes a string into array using custom brackets and quote delimeters */
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
/* }}} */

/* {{{ proto array dql_clause_explode(string str, array d [, string e1, string e2])
   Explodes a string into array using custom brackets and quote delimeters */
static PHP_FUNCTION(dql_clause_explode)
{
	zval *d;
	char *str, *e1 = "(", *e2 = ")", *regexp;
	int str_len, e1_len, e2_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sa|ss", &str, &str_len, &d, &e1, &e1_len, &e2, &e2_len)) {
		return;
	}

	array_init(return_value);
	_dql_clause_explode(this_ptr, return_value, str, d, e1, e2);
}
/* }}} */

/* {{{ proto string dql_get_split_regexp_from_array(array array)
   Builds regular expression for split from array */
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
/* }}} */

/* {{{ proto array dql_clause_explode_regexp(string str, string regexp [, string e1, string e2])
   Same as clauseExplode, but you give a regexp, which splits the string */
static PHP_FUNCTION(dql_clause_explode_regexp)
{
	char *str, *regexp, *e1 = "(", *e2 = ")";
	int str_len, regexp_len, e1_len, e2_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|ss", &str, &str_len, &regexp, &regexp_len, &e1, &e1_len, &e2, &e2_len)) {
		return;
	}

	array_init(return_value);
	_dql_clause_explode_regexp(this_ptr, return_value, str, regexp, e1, e2);
}
/* }}} */

/* {{{ proto array dql_clause_explode_count_brackets(string str, string regexp [, string e1, string e2])
   This function is like clauseExplode, but it doesn't merge bracket terms */
static PHP_FUNCTION(dql_clause_explode_count_brackets)
{
	char *str, *regexp, *e1 = "(", *e2 = ")";
	int str_len, regexp_len, e1_len, e2_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|ss", &str, &str_len, &regexp, &regexp_len, &e1, &e1_len, &e2, &e2_len)) {
		return;
	}

	array_init(return_value);
	_dql_clause_explode_count_brackets(this_ptr, return_value, str, regexp, e1, e2);
}
/* }}} */

/* {{{ proto array dql_clause_explode_non_quoted(string str, string regexp)
   Explodes a string by the given delimiters, and counts quotes in every term */
static PHP_FUNCTION(dql_clause_explode_non_quoted)
{
	char *str, *regexp;
	int len1, len2;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &str, &len1, &regexp, &len2)) {
		return;
	}

	array_init(return_value);
	_dql_clause_explode_non_quoted(this_ptr, return_value, str, regexp);
}
/* }}} */

/* {{{ proto array dql_merge_bracket_terms(array terms)
   This expects input from clauseExplodeNonQuoted */
static PHP_FUNCTION(dql_merge_bracket_terms)
{
	zval *terms;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &terms)) {
		return;
	}

	array_init(return_value);
	_dql_merge_bracket_terms(return_value, terms);
}
/* }}} */

/* {{{ proto array dql_quoted_string_explode(string str)
   Explodes the given string by <quoted words> */
static PHP_FUNCTION(dql_quoted_string_explode)
{
	char *str;
	int len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &len)) {
		return;
	}

	array_init(return_value);
	_dql_quoted_string_explode(this_ptr, return_value, str);
}
/* }}} */

/* {{{ entry */
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
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: fdm=marker
 * vim: noet sw=4 ts=4
 */
