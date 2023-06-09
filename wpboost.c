/* wpboost extension for PHP */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"
#include "ext/standard/php_var.h" // php_var_dump
#include "ext/spl/php_spl.h"

#include "wpboost.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

ZEND_DECLARE_MODULE_GLOBALS(wpboost)

/* {{{ Return a formatted string */
PHP_FUNCTION(zeroise)
{
	zend_long number, threshold;
	char *format, *format2;
	zend_string *result;
	size_t format_len;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(number)
		Z_PARAM_LONG(threshold)
	ZEND_PARSE_PARAMETERS_END();

	zend_spprintf(&format, 0, "%%0%lldd", threshold);
	format_len = zend_spprintf(&format2, 0, format, number);

	result = zend_string_init_fast(format2, format_len);

	efree(format);
	efree(format2);

	if (result == NULL) {
		RETURN_THROWS();
	}
	RETVAL_STR(result);
}
/* }}} */

/* {{{ Return the absolute value of a number cast to integer */
PHP_FUNCTION(absint)
{
	zval *value;
	zend_long retval;

	ZEND_PARSE_PARAMETERS_START(1, -1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();

	retval = zval_get_long(value);

	RETVAL_LONG(retval < 0 ? -retval : retval);
}
/* }}} */

/* {{{ Adds slashes to a string or recursively adds slashes to strings within an array */
PHP_FUNCTION(wp_slash)
{
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache = empty_fcall_info_cache;

	zval retval;
	zval *value;
	zval args[2];

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(value) == IS_ARRAY) {
		// TODO: ZVAL_STRING(&a, "foo")
		zend_string *array_map = zend_string_init_fast("array_map", sizeof("array_map")-1);
		zend_string *wp_slash  = zend_string_init_fast("wp_slash", sizeof("wp_slash")-1);

		ZVAL_STR(&args[0], wp_slash);
		ZVAL_COPY(&args[1], value);

		fci.size = sizeof(fci);
		fci.retval = &retval;
		fci.param_count = 2;
		ZVAL_STR(&fci.function_name, array_map);
		fci.params = args;
		fci.named_params = NULL;
		fci.object = NULL;

		if (zend_call_function(&fci, &fci_cache) == SUCCESS && Z_TYPE(retval) != IS_UNDEF) {
			zval_ptr_dtor(&args[0]);
			zval_ptr_dtor(&args[1]);
			zval_ptr_dtor(&fci.function_name);

			ZVAL_COPY(return_value, &retval);

			zval_ptr_dtor(&retval);
		} else {
			zval_ptr_dtor(&args[0]);
			zval_ptr_dtor(&args[1]);
			zval_ptr_dtor(&fci.function_name);
			RETURN_NULL();
		}
	} else if (Z_TYPE_P(value) == IS_STRING) {
		zend_string *buf = php_addslashes(Z_STR_P(value));
		RETVAL_STR(buf);
	} else {
		RETVAL_COPY(value);
	}
}
/* }}} */

/* {{{ Accesses an array in depth based on a path of keys */
PHP_FUNCTION(_wp_array_get)
{
	zval *input_array;
	zval *path;
	zval *default_value = NULL;

	zval *path_element;

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_ZVAL(input_array)
		Z_PARAM_ZVAL(path)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL(default_value)
	ZEND_PARSE_PARAMETERS_END();

	if (default_value != NULL) {
		ZVAL_COPY(return_value, default_value);
	} else {
		ZVAL_NULL(return_value);
	}

	if (Z_TYPE_P(path) != IS_ARRAY || zend_hash_num_elements(Z_ARRVAL_P(path)) == 0) {
		return;
	}

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(path), path_element) {
		ZVAL_DEREF(input_array);

		if (Z_TYPE_P(input_array) != IS_ARRAY) {
			return;
		}

		switch (Z_TYPE_P(path_element)) {
			case IS_STRING:
				input_array = zend_symtable_find(Z_ARRVAL_P(input_array), Z_STR_P(path_element));
				break;
			case IS_LONG:
				input_array = zend_hash_index_find(Z_ARRVAL_P(input_array), Z_LVAL_P(path_element));
				break;
			case IS_NULL:
				input_array = zend_hash_find(Z_ARRVAL_P(input_array), ZSTR_EMPTY_ALLOC());
				break;
			default:
				input_array = NULL;
		}

		if (!input_array) {
			return;
		}
	} ZEND_HASH_FOREACH_END();
	RETVAL_COPY(input_array);
}
/* }}} */

/* {{{ Builds Unique ID for storage and retrieval */
PHP_FUNCTION(_wp_filter_build_unique_id)
{
	zval retval;
	zend_string *hook_name, *priority;
	zval *callback;
	zval *prop1, *prop2;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_STR(hook_name)
		Z_PARAM_ZVAL(callback)
		Z_PARAM_STR(priority)
	ZEND_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(callback) == IS_STRING) {
		RETURN_COPY_VALUE(callback);
	} else if (Z_TYPE_P(callback) == IS_OBJECT) {
		RETURN_STR(php_spl_object_hash(Z_OBJ_P(callback)));
	} else if (Z_TYPE_P(callback) == IS_ARRAY) {
		prop1 = zend_hash_index_find(Z_ARRVAL_P(callback), 0);
		if (prop1) {
			prop2 = zend_hash_index_find(Z_ARRVAL_P(callback), 1);
			if (Z_TYPE_P(prop2) == IS_STRING) {
				if (Z_TYPE_P(prop1) == IS_OBJECT) {
					zend_string *prop1hash = php_spl_object_hash(Z_OBJ_P(callback));
					zend_string *prop12 = zend_string_concat2(
						ZSTR_VAL(prop1hash), ZSTR_LEN(prop1hash),
						ZSTR_VAL(Z_STR_P(prop2)), ZSTR_LEN(Z_STR_P(prop2)));

					zend_string_release(prop1hash);

					RETURN_STR(prop12);
				} else if (Z_TYPE_P(prop1) == IS_STRING) {
					zend_string *prop12 = zend_string_concat3(
						ZSTR_VAL(Z_STR_P(prop1)), ZSTR_LEN(Z_STR_P(prop1)),
						"::", sizeof("::")-1,
						ZSTR_VAL(Z_STR_P(prop2)), ZSTR_LEN(Z_STR_P(prop2)));

					RETURN_STR(prop12);
				}
			}
		}
	}
}
/* }}} */

static PHP_GINIT_FUNCTION(wpboost)
{
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(wpboost)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(wpboost)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "wpboost support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_zeroise, 0, 2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, number, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, threshold, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_absint, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_wp_slash, 0, 1, IS_MIXED, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo__wp_array_get, 0, 2, IS_MIXED, 1)
	ZEND_ARG_TYPE_INFO(0, input_array, IS_MIXED, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_MIXED, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, default_value, IS_MIXED, 0, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo__wp_filter_build_unique_id, 0, 3, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, hook_name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, callback, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, priority, IS_LONG, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ wpboost_functions[]
 */
static const zend_function_entry wpboost_functions[] = {
	PHP_FE(zeroise,                    arginfo_zeroise)
	PHP_FE(absint,                     arginfo_absint)
	PHP_FE(wp_slash,                   arginfo_wp_slash)
	PHP_FE(_wp_array_get,              arginfo__wp_array_get)
	PHP_FE(_wp_filter_build_unique_id, arginfo__wp_filter_build_unique_id)
	PHP_FE_END
};
/* }}} */

/* {{{ wpboost_module_entry
 */
zend_module_entry wpboost_module_entry = {
	STANDARD_MODULE_HEADER,
	"wpboost",                   /* Extension name */
	wpboost_functions,           /* zend_function_entry */
	PHP_MINIT(wpboost),          /* PHP_MINIT - Module initialization */
	NULL,                        /* PHP_MSHUTDOWN - Module shutdown */
	NULL,                        /* PHP_RINIT - Request initialization */
	NULL,                        /* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(wpboost),          /* PHP_MINFO - Module info */
	PHP_WPBOOST_VERSION,         /* Version */
	PHP_MODULE_GLOBALS(wpboost), /* Module globals */
	PHP_GINIT(wpboost),          /* PHP_GINIT - Globals initialization */
	NULL,                        /* PHP_GSHUTDOWN - Globals shutdown */
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_WPBOOST
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(wpboost)
#endif
