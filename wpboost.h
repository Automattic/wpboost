/* wpboost extension for PHP */

#ifndef PHP_WPBOOST_H
# define PHP_WPBOOST_H

extern zend_module_entry wpboost_module_entry;
# define phpext_wpboost_ptr &wpboost_module_entry

# define PHP_WPBOOST_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_WPBOOST)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

ZEND_BEGIN_MODULE_GLOBALS(wpboost)
	zend_long scale;
ZEND_END_MODULE_GLOBALS(wpboost)

ZEND_EXTERN_MODULE_GLOBALS(wpboost)

#define WPBOOST_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(wpboost, v)

#endif	/* PHP_WPBOOST_H */
