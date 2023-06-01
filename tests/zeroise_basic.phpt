--TEST--
Test zeroise() function : basic functionality
--FILE--
<?php
echo "*** Testing zeroise() : basic functionality ***\n";

$tests = array(
	array( 3, 3, '003' ),
	array( 3, 5, '00003' ),
	array( 3, 0, '3' ),
	array( 3, -1, '3' ),
	array( 0, 5, '00000' ),
);

foreach ( $tests as $test ) {
	list( $arg1, $arg2, $expected ) = $test;
	printf( "Testing zeroise(%s,%s) = %s\n", $arg1, $arg2, zeroise($arg1, $arg2));
}

echo "Done";
?>
--EXPECT--
*** Testing zeroise() : basic functionality ***
Testing zeroise(3,3) = 003
Testing zeroise(3,5) = 00003
Testing zeroise(3,0) = 3
Testing zeroise(3,-1) = 3
Testing zeroise(0,5) = 00000
Done
