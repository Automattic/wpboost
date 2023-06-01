--TEST--
Test absint() function : basic functionality
--FILE--
<?php
echo "*** Testing absint() : basic functionality ***\n";

$tests = array(
	array( -1, 1 ),
	array( 0, 0 ),
	array( 1, 1 ),
	array( '-1', 1 ),
	array( '1', 1 ),
	array( 1.0, 1 ),
	array( -1.0, 1 ),
);

foreach ( $tests as $test ) {
	list( $arg, $expected ) = $test;
	printf( "Testing absint(%s) = %s\n", $arg, absint($arg));
}

echo "Done";
?>
--EXPECT--
*** Testing absint() : basic functionality ***
Testing absint(-1) = 1
Testing absint(0) = 0
Testing absint(1) = 1
Testing absint(-1) = 1
Testing absint(1) = 1
Testing absint(1) = 1
Testing absint(-1) = 1
Done
