#ifndef PHP_DQL_TOKENIZER_H

#define PHP_DQL_TOKENIZER_H

#define PHP_DQL_TOKENIZER_EXTNAME  "dql_tokenizer"
#define PHP_DQL_TOKENIZER_EXTVAR   "0.1"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"

extern zend_module_entry dql_tokenizer_module_entry;
#define phpext_dql_tokenizer_ptr &dql_tokenizer_module_entry

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
  zend_function *func;  \
  zend_hash_find(CG(function_table), #funcname, sizeof(#funcname), (void **)&func); \
  DQL_PUSH_PARAM(param); DQL_PUSH_PARAM((void*)num);        \
  DQL_PUSH_EO_PARAM();              \
  func->internal_function.handler(num, retval, NULL, thisptr, 0 TSRMLS_CC); \
  DQL_POP_EO_PARAM();     \
  DQL_POP_PARAM(); DQL_POP_PARAM();

/* push parameters, call function, pop parameters */
#define PHP_DQL_FUNC1(funcname, retval, thisptr, param1)   \
  PHP_DQL_FUNC_HELPER(funcname, retval, thisptr, 1, param1);

#define PHP_DQL_FUNC4(funcname, retval, thisptr, param1, param2, param3, param4)   \
  DQL_PUSH_PARAM(param1); DQL_PUSH_PARAM(param2); DQL_PUSH_PARAM(param3); \
  PHP_DQL_FUNC_HELPER(funcname, retval, thisptr, 4, param4);  \
  DQL_POP_PARAM(); DQL_POP_PARAM(); DQL_POP_PARAM();

#endif  /* end of "ifndef  PHP_DQL_TOKENIZER_H" */
