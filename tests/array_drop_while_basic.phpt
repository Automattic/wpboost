--TEST--
Test array_drop_while() function : basic functionality
--FILE--
<?php
echo "*** Testing array_drop_while() : basic functionality ***\n";

$arr1 = array(1, 2, 3);

echo "-- With the first parameter empty --\n";
var_dump( array_drop_while( [], function( $x ) {
	return $x != 2;
} ) );

echo "-- With the first array being non-empty --\n";
var_dump( array_drop_while( $arr1, function( $x ) {
	return $x != 2;
} ) );
var_dump( array_drop_while( $arr1, function( $x ) {
	return $x == 2;
} ) );
var_dump( array_drop_while( $arr1, function( $x ) {
	return false;
} ) );
var_dump( array_drop_while( $arr1, function( $x ) {
	return true;
} ) );

echo "Done";
?>
--EXPECT--
*** Testing array_drop_while() : basic functionality ***
-- With the first parameter empty --
array(0) {
}
-- With the first array being non-empty --
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
array(2) {
  [0]=>
  int(2)
  [1]=>
  int(3)
}
array(0) {
}
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
Done
