<?php
if ( ! function_exists( 'array_group' ) ) {
function array_group(array $array, callable $callback): array {
}
}

if ( ! function_exists( 'array_group_pair' ) ) {
function array_group_pair(array $array, callable $callback): array {
}
}

if ( ! function_exists( 'array_every' ) ) {
function array_every(array $array, callable $callback): bool {
}
}

if ( ! function_exists( 'array_some' ) ) {
	function array_some( array $array, callable $callable ) {
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
}

if ( ! function_exists( 'array_starts_with' ) ) {
function array_starts_with(array $array1, array $array2): bool {
}
}

if ( ! function_exists( 'array_ends_with' ) ) {
function array_ends_with(array $array1, array $array2): bool {
}
}

if ( ! function_exists( 'array_take_while' ) ) {
function array_take_while(array $array, callable $callback): array {
}
}

if ( ! function_exists( 'array_drop_while' ) ) {
function array_drop_while(array $array, callable $callback): array {
}
}

if ( ! function_exists( 'zeroise' ) ) {
	function zeroise( $number, $threshold ) {
		return sprintf( '%0' . $threshold . 's', $number );
	}
}

if ( ! function_exists( 'absint' ) ) {
	function absint( $number, $threshold ) {
		return abs( (int) $maybeint );
	}
}

if ( ! function_exists( 'wp_slash' ) ) {
	function wp_slash( $value ) {
		if ( is_array( $value ) ) {
			$value = array_map( 'wp_slash', $value );
		}

		if ( is_string( $value ) ) {
			return addslashes( $value );
		}

		return $value;
	}
}
