<?php
function custom_wp_slash( $value ) {
	if ( is_array( $value ) ) {
		$value = array_map( 'wp_slash', $value );
	}

	if ( is_string( $value ) ) {
		return addslashes( $value );
	}

	return $value;
}

$start = microtime( true );
custom_wp_slash( 'hi' );
custom_wp_slash( 123 );
custom_wp_slash( [1,2,3] );
$end = microtime( true );

$total_custom = $end - $start;

printf( "PHP implementation of wp_slash takes %.10f\n", $total_custom );

$start = microtime( true );
wp_slash( 'hi' );
wp_slash( 123 );
wp_slash( [1,2,3] );
$end = microtime( true );

$total = $end - $start;

printf( "C implementation of wp_slash takes %.10f\n", $total );

printf( "Improvement of %f%%\n", 100 * ( $total_custom - $total ) / $total_custom );
