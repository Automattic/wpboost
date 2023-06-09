--TEST--
Test _wp_filter_build_unique_id() function : basic functionality
--FILE--
<?php
echo "*** Testing _wp_filter_build_unique_id() : basic functionality ***\n";

$input_object = (object)array(
    'a' => 1,
	'b' => 2,
);

$tests = array(
	[ '', 'foobar', '' ],
	[ '', $input_object, '' ],
	[ '', array( $input_object, 'bar' ), '' ],
	[ '', array( 'foo', 'bar' ), '' ],
	[ '', array( $input_object, $input_object ), '' ],
);

foreach ( $tests as $args ) {
	list( $arg1, $arg2, $arg3 ) = $args;
	printf( "Testing _wp_filter_build_unique_id(%s, %s, %s) = %s\n", json_encode( $arg1 ), json_encode( $arg2 ), json_encode( $arg3 ), json_encode( _wp_filter_build_unique_id( $arg1, $arg2, $arg3 ) ) );
}

echo "Done";
?>
--EXPECT--
*** Testing _wp_filter_build_unique_id() : basic functionality ***
Testing _wp_filter_build_unique_id("", "foobar", "") = "foobar"
Testing _wp_filter_build_unique_id("", {"a":1,"b":2}, "") = "00000000000000010000000000000000"
Testing _wp_filter_build_unique_id("", [{"a":1,"b":2},"bar"], "") = "00000000000000010000000000000000bar"
Testing _wp_filter_build_unique_id("", ["foo","bar"], "") = "foo::bar"
Testing _wp_filter_build_unique_id("", [{"a":1,"b":2},{"a":1,"b":2}], "") = null
Done
