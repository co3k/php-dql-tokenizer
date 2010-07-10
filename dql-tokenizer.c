#include "php_dql-tokenizer.h"

zend_module_entry dql_tokenizer_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
  STANDARD_MODULE_HEADER,
#endif
  PHP_DQL_TOKENIZER_EXTNAME,
  NULL,  /* functions */
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

#ifdef COMPILE_DL_DQL_TOKENIZER
ZEND_GET_MODULE(sample)
#endif
