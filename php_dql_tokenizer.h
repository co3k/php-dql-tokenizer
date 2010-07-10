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

#endif  /* end of "ifndef  PHP_DQL_TOKENIZER_H" */
