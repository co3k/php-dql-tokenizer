===============================
How to execute performance test
===============================

Get Doctrine 1
==============

execute following::

  $ svn co http://svn.doctrine-project.org/tags/1.2.2 /path/to/Doctrine-1.2.2

Run test script
===============

execute following (path to your Doctrine 1.2 must end with "/")::

  $ cd /path/to/dql-tokenizer-root-dir
  $ DQL_TOKENIZER_TEST_DOCTRINE_ROOT_DIR="/path/to/Doctrine-1.2.2/" sh test-performance/execute-dql.sh
