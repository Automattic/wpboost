<?php
function custom_wp_filter_build_unique_id( $hook_name, $callback, $priority ) {
	if ( is_string( $callback ) ) {
		return $callback;
	}

	if ( is_object( $callback ) ) {
		// Closures are currently implemented as objects.
		$callback = array( $callback, '' );
	} else {
		$callback = (array) $callback;
	}

	if ( is_object( $callback[0] ) ) {
		// Object class calling.
		return spl_object_hash( $callback[0] ) . $callback[1];
	} elseif ( is_string( $callback[0] ) ) {
		// Static calling.
		return $callback[0] . '::' . $callback[1];
	}
}

$input_object = (object)array(
    'a' => 1,
	'b' => 2,
);

$tests = array(
	[ '', 'foobar', '' ],
	[ '', $input_object, '' ],
	[ '', array( $input_object, 'bar' ), '' ],
	[ '', array( 'foo', 'bar' ), '' ],
);

$start = microtime( true );

foreach ( $tests as $args ) {
	list( $arg1, $arg2, $arg3 ) = $args;

	custom_wp_filter_build_unique_id( $arg1, $arg2, $arg3 );
}

$end = microtime( true );

$total_custom = $end - $start;

printf( "PHP implementation of _wp_filter_build_unique_id takes %.10f\n", $total_custom );

$start = microtime( true );
foreach ( $tests as $args ) {
	list( $arg1, $arg2, $arg3 ) = $args;

	_wp_filter_build_unique_id( $arg1, $arg2, $arg3 );
}
$end = microtime( true );

$total = $end - $start;

printf( "C implementation of _wp_filter_build_unique_id takes %.10f\n", $total );

printf( "Improvement of %f%%\n", 100 * ( $total_custom - $total ) / $total_custom );

