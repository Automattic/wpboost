--TEST--
Test wp_slash() function : basic functionality
--FILE--
<?php
echo "*** Testing wp_slash() : basic functionality ***\n";

$tests = array(
	'hi',
	'123',
	[1,2,3],
	'"quotes',
	['hi', '"quotes'],
);

foreach ( $tests as $arg ) {
	printf( "Testing wp_slash(%s) = %s\n", json_encode( $arg ), json_encode( wp_slash( $arg ) ) );
}

echo "Done";
?>
--EXPECT--
*** Testing wp_slash() : basic functionality ***
Testing wp_slash("hi") = "hi"
Testing wp_slash("123") = "123"
Testing wp_slash([1,2,3]) = [1,2,3]
Testing wp_slash("\"quotes") = "\\\"quotes"
Testing wp_slash(["hi","\"quotes"]) = ["hi","\\\"quotes"]
Done
