<?php
function custom_wp_array_get( $input_array, $path, $default_value = null ) {
	// Confirm $path is valid.
	if ( ! is_array( $path ) || 0 === count( $path ) ) {
		return $default_value;
	}

	foreach ( $path as $path_element ) {
		if ( ! is_array( $input_array ) ) {
			return $default_value;
		}

		if ( is_string( $path_element )
			|| is_integer( $path_element )
			|| null === $path_element
		) {
			/*
			 * Check if the path element exists in the input array.
			 * We check with `isset()` first, as it is a lot faster
			 * than `array_key_exists()`.
			 */
			if ( isset( $input_array[ $path_element ] ) ) {
				$input_array = $input_array[ $path_element ];
				continue;
			}

			/*
			 * If `isset()` returns false, we check with `array_key_exists()`,
			 * which also checks for `null` values.
			 */
			if ( array_key_exists( $path_element, $input_array ) ) {
				$input_array = $input_array[ $path_element ];
				continue;
			}
		}

		return $default_value;
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

