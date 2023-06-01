--TEST--
Test array_some() function : basic functionality
--FILE--
<?php
echo "*** Testing array_some() : basic functionality ***\n";

$arr1 = array('one', 'two', 'three');
$arr2 = array('one', 'two');

echo "-- With an array of three elements (unsat) --\n";
var_dump( array_some($arr1, function( $x ) {
	return 4 == strlen( $x );
} ) );

echo "-- With an array of two elements (sat) --\n";
var_dump( array_some($arr2, function( $x ) {
	return 3 == strlen( $x );
} ) );

echo "-- With an empty array --\n";
var_dump( array_some(array(), function( $x ) {
	return true;
} ) );

$obj1 = (object)array('id'=>3,'name'=>'foo');
$obj2 = (object)array('id'=>4,'name'=>'foo');
$obj3 = (object)array('id'=>5,'name'=>'baz');

echo "-- With an array of objects on a specific field --\n";
var_dump( array_some(array($obj1, $obj2, $obj3), function( $obj ) {
	return $obj->name;
} ) );

echo "Done";
?>
--EXPECT--
*** Testing array_some() : basic functionality ***
-- With an array of three elements (unsat) --
bool(false)
-- With an array of two elements (sat) --
bool(true)
-- With an empty array --
bool(false)
-- With an array of objects on a specific field --
bool(true)
Done
