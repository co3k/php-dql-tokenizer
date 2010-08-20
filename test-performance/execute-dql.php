<?php



if (!isset($_SERVER['DQL_TOKENIZER_TEST_DOCTRINE_ROOT_DIR']))
{
  echo 'Set $DQL_TOKENIZER_TEST_DOCTRINE_ROOT_DIR before executing this script'.PHP_EOL;

  exit;
}

define('DOCTRINE_DIR', $_SERVER['DQL_TOKENIZER_TEST_DOCTRINE_ROOT_DIR']);
define('LOOP_COUNT', (isset($argv[1])) ? $argv[1] : 100);

require_once DOCTRINE_DIR.'/lib/Doctrine.php';

spl_autoload_register(array('Doctrine', 'autoload'));

$a = new Doctrine_Adapter_Mock('Mysql');
$c = Doctrine_Manager::getInstance()->openConnection($a, 'Mysql');
$c->setAttribute(Doctrine_Core::ATTR_TABLE_CLASS, 'Doctrine_Table');
$c->setAttribute(Doctrine_Core::ATTR_DEFAULT_COLUMN_OPTIONS, array());
$c->setAttribute(Doctrine_Core::ATTR_LISTENER, new Doctrine_EventListener());

class ExampleRecord extends Doctrine_Record
{
  public function setTableDefinition()
  {
    $this->setTableName('example_record');
    $this->hasColumn('id', 'integer', 4, array('primary' => true));
  }

  public function setUp()
  {
    parent::setUp();

    $this->hasOne('ExampleRecord', array(
      'local' => 'id',
      'foreign' => 'id',
      'onDelete' => 'cascade',
    ));
  }
}

require_once dirname(__FILE__).'/../example/myQueryTokenizer.class.php';
class myQuery extends Doctrine_Query
{
  public function __construct(Doctrine_Connection $connection = null, Doctrine_Hydrator_Abstract $hydrator = null)
  {
    parent::__construct($connection, $hydrator);

    $this->_tokenizer = new myDqlTokenizer();
  }
}

function execute_string_query($c, $str)
{
  $query = new Doctrine_Query($c);
  $my_query = new myQuery($c);

  echo $str."\t";

  $begin = microtime(true);
  for ($i = 0; $i < LOOP_COUNT; $i++)
  {
    $query->query($str);
  }
  echo (microtime(true) - $begin)."\t";

  $begin = microtime(true);
  for ($i = 0; $i < LOOP_COUNT; $i++)
  {
    $my_query->query($str);
  }
  echo (microtime(true) - $begin)."\t";

  echo PHP_EOL;
}

function execute_constructed_dql($c, $parts)
{
  $query = new Doctrine_Query($c);
  foreach ($parts as $p)
  {
    $query->$p[0]($p[1]);
  }

  $my_query = new myQuery($c);
  foreach ($parts as $p)
  {
    $my_query->$p[0]($p[1]);
  }

  echo '[constructed]'.$query->getDql()."\t";

  $begin = microtime(true);
  for ($i = 0; $i < LOOP_COUNT; $i++)
  {
    $query->execute();
  }
  echo (microtime(true) - $begin)."\t";

  $begin = microtime(true);
  for ($i = 0; $i < LOOP_COUNT; $i++)
  {
    $my_query->execute();
  }
  echo (microtime(true) - $begin)."\t";

  echo PHP_EOL;
}

// ---------------------------------------------------------

echo "query\tnormal-tokenizer\tphp-dql-tokenizer".PHP_EOL;

execute_string_query($c, 'SELECT * FROM ExampleRecord');
execute_string_query($c, 'SELECT id, id, id, id, id, id, id FROM ExampleRecord');
execute_string_query($c, 'SELECT a.id, a.id, a.id, a.id, a.id, a.id, a.id FROM ExampleRecord AS a');
execute_string_query($c, 'SELECT id, id, id, id, id, id, id FROM ExampleRecord WHERE id = ?');
execute_string_query($c, 'SELECT id, id, id, id, id, id, id FROM ExampleRecord WHERE id = ? AND id IN (?, ?, ?, ?)');
execute_string_query($c, 'SELECT id, id, id, id, id, id, id FROM ExampleRecord WHERE id = ? AND id IN (?, ?, ?, ?) AND id = (SELECT s.id FROM ExampleRecord.ExampleRecord AS s WHERE s.id = ?)');

execute_string_query($c, 'DELETE FROM ExampleRecord');
execute_string_query($c, 'DELETE FROM ExampleRecord WHERE id = ? AND id > 10 AND id < 10');

// too slow (both of Doctrine_Query and myQuery)
/*
execute_string_query($c, 'UPDATE ExampleRecord SET id = ?, id = ? WHERE id = ?');
execute_string_query($c, 'UPDATE ExampleRecord SET id = ?, id = ? WHERE id IN (?)');
*/

execute_constructed_dql($c, array(
  array('from', 'ExampleRecord'),
));

execute_constructed_dql($c, array(
  array('from', 'ExampleRecord'),
  array('select', 'id, id'),
));

execute_constructed_dql($c, array(
  array('from', 'ExampleRecord'),
  array('select', 'id, id, id, id, id'),
  array('where', 'id = ?'),
  array('andWhere', 'id = ?'),
  array('andWhere', 'id IN ?'),
));

execute_constructed_dql($c, array(
  array('from', 'ExampleRecord'),
  array('delete', ''),
  array('where', 'id = ?'),
  array('andWhere', 'id = ?'),
  array('andWhere', 'id IN ?'),
));

Doctrine_Manager::resetInstance(); unset($a, $c);
