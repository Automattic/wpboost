<?php
function custom_wp_array_get( array $input_array, array $path, $default_value = null ) {
	// Confirm $path is valid.
	if ( ! is_array( $input_array ) || 0 === count( $path ) ) {
		return $default_value;
	}

	foreach ( $path as $path_element ) {
		if ( ! isset( $input_array[ $path_element ] ) ) {
			return $default_value;
		}
		$input_array = $input_array[ $path_element ];
	}

	return $input_array;
}

$input_array = array(
    'a' => array(
        'b' => array(
            'c' => 1,
        ),
    ),
);
$path = array( 'a', 'b', 'c' );

$start = microtime( true );
custom_wp_array_get( $input_array, $path, 123 );
$end = microtime( true );

$total_custom = $end - $start;

printf( "PHP implementation of _wp_array_get takes %.10f\n", $total_custom );

$start = microtime( true );
_wp_array_get( $input_array, $path, 123 );
$end = microtime( true );

$total = $end - $start;

printf( "C implementation of _wp_array_get takes %.10f\n", $total );

printf( "Improvement of %f%%\n", 100 * ( $total_custom - $total ) / $total_custom );

