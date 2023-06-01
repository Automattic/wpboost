<?php
function custom_array_some( array $array, callable $callable ) {
	if ( empty( $array ) ) {
		return array();
	}

	foreach ( $array as $element ) {
		if ( $callable( $element ) ) {
			return true;
		}
	}

	return false;
}

function even( $x ) {
	return $x % 2 == 0;
}

$start = microtime( true );
custom_array_some( [ 1, 5, 7, 9, 123, 456, 789 ], 'even' );
$end = microtime( true );

$total_custom = $end - $start;

printf( "PHP implementation of array_some takes %.10f\n", $total_custom );

$start = microtime( true );
array_some( [ 1, 5, 7, 9, 123, 456, 789 ], 'even' );
$end = microtime( true );

$total = $end - $start;

printf( "C implementation of array_some takes %.10f\n", $total );

printf( "Improvement of %f%%\n", 100 * ( $total_custom - $total ) / $total_custom );
