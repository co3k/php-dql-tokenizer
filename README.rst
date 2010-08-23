======
README
======

What is php-dql-tokenizer?
==========================

This extension is for improving performance of Doctrine_Query_Tokenizer.

INSTALL
=======

Execute the following commands::

  $ phpize
  $ ./configure
  $ make
  # make install

Then add the following to your php.ini::

  [dql_tokenizer]
  extension=dql_tokenizer.so

How To Use
==========

This extension provides functions that corresponds to the methods of the Doctrine_Query_Tokenizer class.

So you need to define a new tokenizer classes to use this extension.

First, you write the following code in your query class::

  if (function_exists('dql_tokenize_query'))
  {
    $this->_tokenizer = new myQueryTokenizer();
  }

Next, you define a new tokenizer class. You can get the example class file from "example/myQueryTokenizer.class.php".

Credits
=======

Kousuke Ebihara
  ebihara@php.net
  http://co3k.org/

TODO
====

* There are many duplicated codes (e.g. iterating hash tables)
* Some functions are tooooooooooo long
