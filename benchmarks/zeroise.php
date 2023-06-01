<?php
function custom_zeroise( $number, $threshold ) {
	return sprintf( '%0' . $threshold . 's', $number );
}

$start = microtime( true );
custom_zeroise( 123, 456 );
$end = microtime( true );

$total_custom = $end - $start;

printf( "PHP implementation of zeroise takes %10f\n", $total_custom );

$start = microtime( true );
zeroise( 123, 456 );
$end = microtime( true );

$total = $end - $start;

printf( "C implementation of zeroise takes %10f\n", $total );

printf( "Improvement of %f%%\n", 100 * ( $total_custom - $total ) / $total_custom );
