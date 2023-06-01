--TEST--
Test array_group() function : basic functionality
--FILE--
<?php
echo "*** Testing array_group() : basic functionality ***\n";

$arr1 = array('one', 'two', 'three');

echo "-- With an array of three elements --\n";
var_dump( array_group($arr1, function( $x ) {
	return (string) strlen( $x );
} ) );

echo "-- With an empty array --\n";
var_dump( array_group(array(), function( $x ) {
	return (string) $x;
} ) );

$obj1 = (object)array('id'=>3,'name'=>'foo');
$obj2 = (object)array('id'=>4,'name'=>'foo');
$obj3 = (object)array('id'=>5,'name'=>'baz');

echo "-- With an array of objects on a specific field --\n";
var_dump( array_group(array($obj1, $obj2, $obj3), function( $obj ) {
	return $obj->name;
} ) );

echo "Done";
?>
--EXPECT--
*** Testing array_group() : basic functionality ***
-- With an array of three elements --
array(2) {
  ["3"]=>
  array(2) {
    [0]=>
    string(3) "one"
    [1]=>
    string(3) "two"
  }
  ["5"]=>
  array(1) {
    [0]=>
    string(5) "three"
  }
}
-- With an empty array --
array(0) {
}
-- With an array of objects on a specific field --
array(2) {
  ["foo"]=>
  array(2) {
    [0]=>
    object(stdClass)#1 (2) {
      ["id"]=>
      int(3)
      ["name"]=>
      string(3) "foo"
    }
    [1]=>
    object(stdClass)#2 (2) {
      ["id"]=>
      int(4)
      ["name"]=>
      string(3) "foo"
    }
  }
  ["baz"]=>
  array(1) {
    [0]=>
    object(stdClass)#3 (2) {
      ["id"]=>
      int(5)
      ["name"]=>
      string(3) "baz"
    }
  }
}
Done
