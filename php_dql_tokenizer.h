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

#ifndef PHP_DQL_TOKENIZER_H

#define PHP_DQL_TOKENIZER_H

#define PHP_DQL_TOKENIZER_EXTNAME  "dql_tokenizer"
#define PHP_DQL_TOKENIZER_EXTVAR   "0.5.0"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"

extern zend_module_entry dql_tokenizer_module_entry;
#define phpext_dql_tokenizer_ptr &dql_tokenizer_module_entry

/* See ext/pcre/php_pcre.c */
#define PHP_DQL_PREG_SPLIT_DELIM_CAPTURE	(1<<1)

/*
 For keeping compatible with PHP 5.2
*/
#ifndef Z_REFCOUNT_P

#define Z_REFCOUNT_PP(ppz)             ((*(ppz))->refcount)
#define Z_SET_REFCOUNT_PP(ppz, rc)     ((*(ppz))->refcount = rc)
#define Z_ADDREF_PP(ppz)             (++(*(ppz))->refcount)
#define Z_DELREF_PP(ppz)             (--(*(ppz))->refcount)
#define Z_ISREF_PP(ppz)                ((*(ppz))->is_ref)
#define Z_SET_ISREF_PP(ppz)            ((*(ppz))->is_ref = 1)
#define Z_UNSET_ISREF_PP(ppz)          ((*(ppz))->is_ref = 0)
#define Z_SET_ISREF_TO_PP(ppz, isref)  ((*(ppz))->is_ref = isref)

#define Z_REFCOUNT_P(pz)            ((pz)->refcount)
#define Z_SET_REFCOUNT_P(z, rc)     ((pz)->refcount = rc)
#define Z_ADDREF_P(pz)            (++(pz)->refcount)
#define Z_DELREF_P(pz)            (--(pz)->refcount)
#define Z_ISREF_P(pz)               ((pz)->is_ref)
#define Z_SET_ISREF_P(pz)           ((pz)->is_ref = 1)
#define Z_UNSET_ISREF_P(pz)         ((pz)->is_ref = 0)
#define Z_SET_ISREF_TO_P(z, isref)  ((pz)->is_ref = isref)

#define Z_REFCOUNT(z)             ((z).refcount)
#define Z_SET_REFCOUNT(z, rc)     ((z).refcount = rc)
#define Z_ADDREF(z)             (++(z).refcount)
#define Z_DELREF(z)             (--(z).refcount)
#define Z_ISREF(z)                ((z).is_ref)
#define Z_SET_ISREF(z)            ((z).is_ref = 1)
#define Z_UNSET_ISREF(z)          ((z).is_ref = 0)
#define Z_SET_ISREF_TO(z, isref)  ((z).is_ref = isref)

#endif

/**
 * The following calling internal function helper is based some useful macros in mongo-php-driver.
 * http://github.com/mongodb/mongo-php-driver
 */
#if ZEND_MODULE_API_NO >= 20090115
# define DQL_PUSH_PARAM(arg) zend_vm_stack_push(arg TSRMLS_CC)
# define DQL_POP_PARAM() (void)zend_vm_stack_pop(TSRMLS_C)
# define DQL_PUSH_EO_PARAM()
# define DQL_POP_EO_PARAM()
#else
# define DQL_PUSH_PARAM(arg) zend_ptr_stack_push(&EG(argument_stack), arg)
# define DQL_POP_PARAM() (void)zend_ptr_stack_pop(&EG(argument_stack))
# define DQL_PUSH_EO_PARAM() zend_ptr_stack_push(&EG(argument_stack), NULL)
# define DQL_POP_EO_PARAM() (void)zend_ptr_stack_pop(&EG(argument_stack))
#endif

#define PHP_DQL_FUNC_HELPER(funcname, retval, thisptr, num, param) \
  zend_hash_find(CG(function_table), #funcname, sizeof(#funcname), (void **)&func); \
  DQL_PUSH_PARAM(param); DQL_PUSH_PARAM((void*)num);        \
  DQL_PUSH_EO_PARAM();              \
  func->internal_function.handler(num, retval, NULL, thisptr, 0 TSRMLS_CC); \
  DQL_POP_EO_PARAM();     \
  DQL_POP_PARAM(); DQL_POP_PARAM(); \

/* push parameters, call function, pop parameters */
#define PHP_DQL_FUNC1(funcname, retval, thisptr, param1)   \
  PHP_DQL_FUNC_HELPER(funcname, retval, thisptr, 1, param1);

#define PHP_DQL_FUNC2(funcname, retval, thisptr, param1, param2)   \
  DQL_PUSH_PARAM(param1); \
  PHP_DQL_FUNC_HELPER(funcname, retval, thisptr, 2, param2);  \
  DQL_POP_PARAM();

#define PHP_DQL_FUNC4(funcname, retval, thisptr, param1, param2, param3, param4)   \
  DQL_PUSH_PARAM(param1); DQL_PUSH_PARAM(param2); DQL_PUSH_PARAM(param3); \
  PHP_DQL_FUNC_HELPER(funcname, retval, thisptr, 4, param4);  \
  DQL_POP_PARAM(); DQL_POP_PARAM(); DQL_POP_PARAM();

/* DQL_TOKEN_* */

#define DQL_TOKEN_BASE (1<< 0)
#define DQL_TOKEN_BY (1<< 1)
#define DQL_TOKEN_PASS (1<< 2)
#define DQL_TOKEN_UNDEFINED (1<< 3)

typedef struct _php_dql_keyword_map {
	const char *keywords;
	int code;
} php_dql_keyword_map;

php_dql_keyword_map php_dql_tokens[] = {
	{
		"delete\0update\0select\0set\0from\0where\0limit\0offset\0having\0",
		DQL_TOKEN_BASE
	},
	{
		"order\0group\0",
		DQL_TOKEN_BY
	},
	{
		"by\0",
		DQL_TOKEN_PASS
	},
	{
		NULL,
		DQL_TOKEN_UNDEFINED
	}
};

#endif  /* end of "ifndef  PHP_DQL_TOKENIZER_H" */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: fdm=marker
 * vim: noet sw=4 ts=4
 */
