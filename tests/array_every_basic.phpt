--TEST--
Test array_every() function : basic functionality
--FILE--
<?php
echo "*** Testing array_every() : basic functionality ***\n";

$arr1 = array('one', 'two', 'three');
$arr2 = array('one', 'two');

echo "-- With an array of three elements (unsat) --\n";
var_dump( array_every($arr1, function( $x ) {
	return 3 == strlen( $x );
} ) );

echo "-- With an array of two elements (sat) --\n";
var_dump( array_every($arr2, function( $x ) {
	return 3 == strlen( $x );
} ) );

echo "-- With an empty array --\n";
var_dump( array_every(array(), function( $x ) {
	return true;
} ) );

$obj1 = (object)array('id'=>3,'name'=>'foo');
$obj2 = (object)array('id'=>4,'name'=>'foo');
$obj3 = (object)array('id'=>5,'name'=>'baz');

echo "-- With an array of objects on a specific field --\n";
var_dump( array_every(array($obj1, $obj2, $obj3), function( $obj ) {
	return $obj->name;
} ) );

echo "Done";
?>
--EXPECT--
*** Testing array_every() : basic functionality ***
-- With an array of three elements (unsat) --
bool(false)
-- With an array of two elements (sat) --
bool(true)
-- With an empty array --
bool(true)
-- With an array of objects on a specific field --
bool(true)
Done
