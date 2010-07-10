PHP_ARG_ENABLE(dql-tokenizer, for dql_tokenizer support,
[  --enable-dql-tokenizer  Include dql_tokenizer support])

if test $PHP_DQL_TOKENIZER != "no"; then
  PHP_NEW_EXTENSION(dql_tokenizer, dql_tokenizer.c, $ext_shared)
  PHP_SUBST(DQL_TOKENIZER_SHARED_LIBADD)
fi
