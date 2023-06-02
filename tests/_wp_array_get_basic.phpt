--TEST--
Test _wp_array_get() function : basic functionality
--FILE--
<?php
echo "*** Testing _wp_array_get() : basic functionality ***\n";

$input_array = array(
    'a' => array(
        'b' => array(
            'c' => 1,
        ),
    ),
);

$tests = array(
	[ $input_array, [ 'a' ] ],
	[ $input_array, [ 'a', 'b' ] ],
	[ $input_array, [ 'a', 'b', 'c' ] ],
	[ $input_array, [ 'aa' ] ],
	[ [], [ 'a' ] ],
	[ $input_array, [ 'a' ], 123 ],
	[ $input_array, [ 'aa' ], 123 ],
	[ [], [ 'a' ], 123 ],
	[ [ 'a' ], [] ],
	[ [ 'a' ], [], 123 ],
	[ true, [], 123 ],
);

foreach ( $tests as $args ) {
	list( $arg1, $arg2 ) = $args;
	$arg3 = isset( $args[2] ) ? $args[2] : null;
	printf( "Testing _wp_array_get(%s, %s, %s) = %s\n", json_encode( $arg1 ), json_encode( $arg2 ), json_encode( $arg3 ), json_encode( _wp_array_get( $arg1, $arg2, $arg3 ) ) );
}

foreach ( $tests as $args ) {
	list( $arg1, $arg2 ) = $args;
	printf( "Testing _wp_array_get(%s, %s) = %s\n", json_encode( $arg1 ), json_encode( $arg2 ), json_encode( _wp_array_get( $arg1, $arg2 ) ) );
}

echo "Done";
?>
--EXPECT--
*** Testing _wp_array_get() : basic functionality ***
Testing _wp_array_get({"a":{"b":{"c":1}}}, ["a"], null) = {"b":{"c":1}}
Testing _wp_array_get({"a":{"b":{"c":1}}}, ["a","b"], null) = {"c":1}
Testing _wp_array_get({"a":{"b":{"c":1}}}, ["a","b","c"], null) = 1
Testing _wp_array_get({"a":{"b":{"c":1}}}, ["aa"], null) = null
Testing _wp_array_get([], ["a"], null) = null
Testing _wp_array_get({"a":{"b":{"c":1}}}, ["a"], 123) = {"b":{"c":1}}
Testing _wp_array_get({"a":{"b":{"c":1}}}, ["aa"], 123) = 123
Testing _wp_array_get([], ["a"], 123) = 123
Testing _wp_array_get(["a"], [], null) = ["a"]
Testing _wp_array_get(["a"], [], 123) = ["a"]
Testing _wp_array_get(true, [], 123) = 123
Testing _wp_array_get({"a":{"b":{"c":1}}}, ["a"]) = {"b":{"c":1}}
Testing _wp_array_get({"a":{"b":{"c":1}}}, ["a","b"]) = {"c":1}
Testing _wp_array_get({"a":{"b":{"c":1}}}, ["a","b","c"]) = 1
Testing _wp_array_get({"a":{"b":{"c":1}}}, ["aa"]) = null
Testing _wp_array_get([], ["a"]) = null
Testing _wp_array_get({"a":{"b":{"c":1}}}, ["a"]) = {"b":{"c":1}}
Testing _wp_array_get({"a":{"b":{"c":1}}}, ["aa"]) = null
Testing _wp_array_get([], ["a"]) = null
Testing _wp_array_get(["a"], []) = ["a"]
Testing _wp_array_get(["a"], []) = ["a"]
Testing _wp_array_get(true, []) = null
Done
