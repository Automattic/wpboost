/* koloktos extension for PHP */

#ifndef PHP_KOLOKTOS_H
# define PHP_KOLOKTOS_H

extern zend_module_entry koloktos_module_entry;
# define phpext_koloktos_ptr &koloktos_module_entry

# define PHP_KOLOKTOS_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_KOLOKTOS)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

ZEND_BEGIN_MODULE_GLOBALS(koloktos)
	zend_long scale;
ZEND_END_MODULE_GLOBALS(koloktos)

ZEND_EXTERN_MODULE_GLOBALS(koloktos)

#define ZEND_HASH_FOREACH_VAL_FROM(ht, _val, _from) \
       ZEND_HASH_FOREACH_FROM(ht, 0, _from); \
       _val = _z;

#define KOLOKTOS_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(koloktos, v)

#endif	/* PHP_KOLOKTOS_H */
