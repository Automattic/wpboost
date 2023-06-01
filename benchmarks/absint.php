<?php
function custom_absint( $maybeint ) {
	return abs( (int) $maybeint );
}

$start = microtime( true );
custom_absint( 123 );
$end = microtime( true );

$total_custom = $end - $start;

printf( "PHP implementation of absint takes %.10f\n", $total_custom );

$start = microtime( true );
absint( 123 );
$end = microtime( true );

$total = $end - $start;

printf( "C implementation of absint takes %.10f\n", $total );

printf( "Improvement of %f%%\n", 100 * ( $total_custom - $total ) / $total_custom );
