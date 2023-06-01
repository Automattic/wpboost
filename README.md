# koloktos

koloktos extension. Repository for the http://pecl.php.net/koloktos releases.

Contains various utility functions. Some of these may potentially go into core at some point. Related discussion: https://externals.io/message/120451

The most basic idea with adding these functions is to:

1. Improve the developer experience, having these functions directly accessible for usage rather than relying on PHP libraries
2. Performance improvement. Some brief benchmarks are available here: https://gist.github.com/bor0/3fa539263335fa415faa67606a469f2e

## Functions

```php
function array_group(array $array, callable $callback): array {}
function array_group_pair(array $array, callable $callback): array {}
function array_every(array $array, callable $callback): bool {}
function array_some(array $array, callable $callback): bool {}
function array_starts_with(array $array1, array $array2): bool {}
function array_ends_with(array $array1, array $array2): bool {}
function array_take_while(array $array, callable $callback): array {}
function array_drop_while(array $array, callable $callback): array {}
function zeroise(int $number, int $threshold): string {}
function absint(int|double $maybeint): int {}
function wp_slash(mixed $value): mixed {}
```

The filename `koloktos.php` contains polyfills in PHP.

## Building

To build:
1. Run `phpize`
2. Run `./configure`
3. Run `make`
4. Run `make test`

## Contributing

If you have a suggestion to add a function, it's best to approach by creating a pull request.

While doing so, make sure:

1. You have a solid reason why this function needs to be added - how frequently is it used, what problem does it solve, and why should it be included in this extension.
2. You add test cases for the added function.
3. There's both a C version of the function, and a PHP version of the function (polyfill)

## TODO

- See what can be brought from https://codex.wordpress.org/Function_Reference into here
- Add CI on GitHub, etc.
