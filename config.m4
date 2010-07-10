PHP_ARG_ENABLE (dql-tokenizer,
  [Whether to enable the "dql-tokenizer" extension],
  [  --enable-dql-tokenizer   Enable "dql-tokenizer" extension support])

if test $PHP_DQL_TOKENIZER != "no"; then
  PHP_NEW_EXTENSION(dql-tokenizer, dql-tokenizer.c, $ext_shared)
  PHP_SUBST(DQL_TOKENIZER_SHARED_LIBADD)
fi
