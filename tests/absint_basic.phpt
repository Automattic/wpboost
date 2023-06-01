--TEST--
Test absint() function : basic functionality
--FILE--
<?php
echo "*** Testing absint() : basic functionality ***\n";

$tests = array(
	-1,
	0,
	1,
	'-1',
	'1',
	1.0,
	-1.0,
	'123a',
);

foreach ( $tests as $arg ) {
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
Testing absint(123a) = 123
Done
