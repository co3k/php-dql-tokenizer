======
README
======

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

Credits
=======

Kousuke Ebihara
  ebihara@php.net
  http://co3k.org/

TODO
====

* Add license descriptions and copyright notifications (The license for this extension will be Apache License 2.0)
* Test performance
* There are too allocation of zvals in current functions. It will be pitfall of memory-leak
* There are many duplicated codes (e.g. iterating hash tables)
* Some functions are tooooooooooo long
* Kill compiler warnings...
