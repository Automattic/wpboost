--TEST--
Test array_starts_with() function : basic functionality
--FILE--
<?php
echo "*** Testing array_starts_with() : basic functionality ***\n";

$arr1 = array(1, 2, 3);
$arr2 = array(1, 2, 3, 4);
$arr3 = array(1, 2, 3, 4, 5);
$arr4 = array(2, 3, 4);

echo "-- With the first parameter empty --\n";
var_dump( array_starts_with( [], $arr1 ) );
echo "-- With the second parameter empty --\n";
var_dump( array_starts_with( $arr1, [] ) );

echo "-- With first array being smaller than the second --\n";
var_dump( array_starts_with( $arr1, $arr2 ) );
var_dump( array_starts_with( $arr1, $arr3 ) );

echo "-- With first array being larger than the second --\n";
var_dump( array_starts_with( $arr2, $arr1 ) );
var_dump( array_starts_with( $arr3, $arr1 ) );
var_dump( array_starts_with( $arr3, $arr2 ) );
var_dump( array_starts_with( $arr2, $arr4 ) );

echo "Done";
?>
--EXPECT--
*** Testing array_starts_with() : basic functionality ***
-- With the first parameter empty --
bool(false)
-- With the second parameter empty --
bool(true)
-- With first array being smaller than the second --
bool(false)
bool(false)
-- With first array being larger than the second --
bool(true)
bool(true)
bool(true)
bool(false)
Done
