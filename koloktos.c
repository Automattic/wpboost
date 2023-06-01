/* koloktos extension for PHP */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"

#include "koloktos.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

ZEND_DECLARE_MODULE_GLOBALS(koloktos)

/* {{{ Groups elements from the array via the callback. */
PHP_FUNCTION(array_group)
{
	zval *array;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache = empty_fcall_info_cache;

	zval args[1];
	zval *curr_val;
	zval chunk;
	zval retval;
	zval *group;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY(array)
		Z_PARAM_FUNC(fci, fci_cache)
	ZEND_PARSE_PARAMETERS_END();

	if (zend_hash_num_elements(Z_ARRVAL_P(array)) == 0) {
		RETVAL_EMPTY_ARRAY();
		return;
	}

	array_init(return_value);
	fci.retval = &retval;
	fci.param_count = 1;

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(array), curr_val) {
		ZVAL_COPY(&args[0], curr_val);
		fci.params = args;

		if (zend_call_function(&fci, &fci_cache) == SUCCESS && Z_TYPE(retval) != IS_UNDEF) {
			if (Z_TYPE_P(&retval) != IS_STRING) {
				php_error_docref(NULL, E_WARNING, "Argument #1 must be string, %s given", zend_zval_type_name(&retval));
				RETVAL_EMPTY_ARRAY();
				return;
			}
			group = zend_hash_find(Z_ARRVAL_P(return_value), Z_STR_P(&retval));
			if (group == NULL) {
				array_init(&chunk);
				group = &chunk;
				zend_hash_add_new(Z_ARRVAL_P(return_value), Z_STR_P(&retval), group);
			}

			zend_hash_next_index_insert(Z_ARRVAL_P(group), curr_val);
			zval_ptr_dtor(&args[0]);
			zval_ptr_dtor(&retval);

		} else {
			zval_ptr_dtor(&args[0]);
			RETURN_NULL();
		}
	} ZEND_HASH_FOREACH_END();
}
/* }}} */

/* {{{ Groups consecutive pairs of elements from the array via the callback. */
PHP_FUNCTION(array_group_pair)
{
	zval *array;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache = empty_fcall_info_cache;

	zval args[2];
	zval *curr_val;
	zval *prev_val;
	zval chunk;
	zval retval;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY(array)
		Z_PARAM_FUNC(fci, fci_cache)
		Z_PARAM_OPTIONAL
	ZEND_PARSE_PARAMETERS_END();

	if (zend_hash_num_elements(Z_ARRVAL_P(array)) == 0) {
		RETVAL_EMPTY_ARRAY();
		return;
	}

	array_init(return_value);
	fci.retval = &retval;
	fci.param_count = 2;

	// The array is guaranteed to have at least one element.
	prev_val = ZEND_HASH_ELEMENT(Z_ARRVAL_P(array), 0);

	// Generate the initial group.
	array_init(&chunk);
	zend_hash_next_index_insert_new(Z_ARRVAL_P(&chunk), prev_val);

	ZEND_HASH_FOREACH_VAL_FROM(Z_ARRVAL_P(array), curr_val, 1) {
		ZVAL_COPY(&args[0], prev_val);
		ZVAL_COPY(&args[1], curr_val);
		fci.params = args;

		if (zend_call_function(&fci, &fci_cache) == SUCCESS && Z_TYPE(retval) != IS_UNDEF) {
			int retval_true;

			zval_ptr_dtor(&args[1]);
			zval_ptr_dtor(&args[0]);

			retval_true = zend_is_true(&retval);

			zval_ptr_dtor(&retval);

			// Perform grouping - add the current group and create a new one.
			if (!retval_true) {
				zend_hash_next_index_insert_new(Z_ARRVAL_P(return_value), &chunk);
				array_init(&chunk);
			}

			zend_hash_next_index_insert_new(Z_ARRVAL_P(&chunk), curr_val);

			prev_val = curr_val;
		} else {
			zval_ptr_dtor(&args[1]);
			zval_ptr_dtor(&args[0]);
			RETURN_NULL();
		}
	} ZEND_HASH_FOREACH_END();

	// Add the last group.
	zend_hash_next_index_insert_new(Z_ARRVAL_P(return_value), &chunk);
}
/* }}} */

/* {{{ Checks if every element from the array satisfies a property. */
PHP_FUNCTION(array_every)
{
	zval *array;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache = empty_fcall_info_cache;

	zval args[1];
	zval *curr_val;
	zval retval;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY(array)
		Z_PARAM_FUNC(fci, fci_cache)
	ZEND_PARSE_PARAMETERS_END();

	if (zend_hash_num_elements(Z_ARRVAL_P(array)) == 0) {
		RETURN_TRUE;
	}

	fci.retval = &retval;
	fci.param_count = 1;

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(array), curr_val) {
		ZVAL_COPY(&args[0], curr_val);
		fci.params = args;

		if (zend_call_function(&fci, &fci_cache) == SUCCESS && Z_TYPE(retval) != IS_UNDEF) {
			int retval_true;

			zval_ptr_dtor(&args[0]);
			retval_true = zend_is_true(&retval);
			zval_ptr_dtor(&retval);

			if (!retval_true) {
				RETURN_FALSE;
			}
		} else {
			zval_ptr_dtor(&args[0]);
			RETURN_NULL();
		}
	} ZEND_HASH_FOREACH_END();

	RETURN_TRUE;
}
/* }}} */

/* {{{ Checks if at least one element from the array satisfies a property. */
PHP_FUNCTION(array_some)
{
	zval *array;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache = empty_fcall_info_cache;

	zval args[1];
	zval *curr_val;
	zval retval;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY(array)
		Z_PARAM_FUNC(fci, fci_cache)
	ZEND_PARSE_PARAMETERS_END();

	if (zend_hash_num_elements(Z_ARRVAL_P(array)) == 0) {
		RETURN_FALSE;
	}

	fci.retval = &retval;
	fci.param_count = 1;

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(array), curr_val) {
		ZVAL_COPY(&args[0], curr_val);
		fci.params = args;

		if (zend_call_function(&fci, &fci_cache) == SUCCESS && Z_TYPE(retval) != IS_UNDEF) {
			int retval_true;

			zval_ptr_dtor(&args[0]);
			retval_true = zend_is_true(&retval);
			zval_ptr_dtor(&retval);

			if (retval_true) {
				RETURN_TRUE;
			}
		} else {
			zval_ptr_dtor(&args[0]);
			RETURN_NULL();
		}
	} ZEND_HASH_FOREACH_END();

	RETURN_FALSE;
}
/* }}} */

/* {{{ Checks if an array starts with another array. */
PHP_FUNCTION(array_starts_with)
{
	zval *array1, *array2;
	zval *array1_val, *array2_val;
	uint32_t index = 0;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY(array1)
		Z_PARAM_ARRAY(array2)
	ZEND_PARSE_PARAMETERS_END();

	if (zend_hash_num_elements(Z_ARRVAL_P(array2)) == 0) {
		RETURN_TRUE;
	}

	if (zend_hash_num_elements(Z_ARRVAL_P(array2)) > zend_hash_num_elements(Z_ARRVAL_P(array1))) {
		RETURN_FALSE;
	}

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(array2), array2_val) {
		// Index is guaranteed to exist because of ZEND_HASH_ELEMENT_SIZE check earlier.
		array1_val = ZEND_HASH_ELEMENT(Z_ARRVAL_P(array1), index++);
		if (zend_compare(array1_val, array2_val)) {
			RETURN_FALSE;
		}
	} ZEND_HASH_FOREACH_END();

	RETURN_TRUE;
}
/* }}} */

/* {{{ Checks if an array ends with another array. */
PHP_FUNCTION(array_ends_with)
{
	zval *array1, *array2;
	zval *array1_val, *array2_val;
	uint32_t index;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY(array1)
		Z_PARAM_ARRAY(array2)
	ZEND_PARSE_PARAMETERS_END();

	index = zend_hash_num_elements(Z_ARRVAL_P(array2));

	if (index == 0) {
		RETURN_TRUE;
	}

	if (zend_hash_num_elements(Z_ARRVAL_P(array2)) > zend_hash_num_elements(Z_ARRVAL_P(array1))) {
		RETURN_FALSE;
	}

	ZEND_HASH_REVERSE_FOREACH_VAL(Z_ARRVAL_P(array2), array2_val) {
		// Index is guaranteed to exist because of ZEND_HASH_ELEMENT_SIZE check earlier.
		array1_val = ZEND_HASH_ELEMENT(Z_ARRVAL_P(array1), index--);
		if (zend_compare(array1_val, array2_val)) {
			RETURN_FALSE;
		}
	} ZEND_HASH_FOREACH_END();

	RETURN_TRUE;
}
/* }}} */

/* {{{ Takes the elements until a property is satisfied. */
PHP_FUNCTION(array_take_while)
{
	zval *array;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache = empty_fcall_info_cache;

	zval args[1];
	zval *curr_val;
	zval retval;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY(array)
		Z_PARAM_FUNC(fci, fci_cache)
	ZEND_PARSE_PARAMETERS_END();

	if (zend_hash_num_elements(Z_ARRVAL_P(array)) == 0) {
		RETVAL_EMPTY_ARRAY();
		return;
	}

	array_init(return_value);

	fci.retval = &retval;
	fci.param_count = 1;

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(array), curr_val) {
		ZVAL_COPY(&args[0], curr_val);
		fci.params = args;

		if (zend_call_function(&fci, &fci_cache) == SUCCESS && Z_TYPE(retval) != IS_UNDEF) {
			int retval_true;

			zval_ptr_dtor(&args[0]);
			retval_true = zend_is_true(&retval);
			zval_ptr_dtor(&retval);

			if (retval_true) {
				return;
			}

			zend_hash_next_index_insert_new(Z_ARRVAL_P(return_value), curr_val);
		} else {
			zval_ptr_dtor(&args[0]);
			RETURN_NULL();
		}
	} ZEND_HASH_FOREACH_END();
}
/* }}} */

/* {{{ Drops the elements until a property is satisfied. */
PHP_FUNCTION(array_drop_while)
{
	zval *array;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache = empty_fcall_info_cache;

	zval args[1];
	zval *curr_val;
	zval retval;
	int retval_true = 0;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY(array)
		Z_PARAM_FUNC(fci, fci_cache)
	ZEND_PARSE_PARAMETERS_END();

	if (zend_hash_num_elements(Z_ARRVAL_P(array)) == 0) {
		RETVAL_EMPTY_ARRAY();
		return;
	}

	array_init(return_value);

	fci.retval = &retval;
	fci.param_count = 1;

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(array), curr_val) {
		ZVAL_COPY(&args[0], curr_val);
		fci.params = args;

		if (retval_true) {
			zend_hash_next_index_insert_new(Z_ARRVAL_P(return_value), curr_val);
			continue;
		}

		if (zend_call_function(&fci, &fci_cache) == SUCCESS && Z_TYPE(retval) != IS_UNDEF) {

			zval_ptr_dtor(&args[0]);
			retval_true = zend_is_true(&retval);
			zval_ptr_dtor(&retval);

			if (retval_true) {
				zend_hash_next_index_insert_new(Z_ARRVAL_P(return_value), curr_val);
			}
		} else {
			zval_ptr_dtor(&args[0]);
			RETURN_NULL();
		}
	} ZEND_HASH_FOREACH_END();
}
/* }}} */

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

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_NUMBER(value)
	ZEND_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(value) == IS_DOUBLE) {
		ZVAL_LONG(value, zend_dval_to_lval(Z_DVAL_P(value)));
	}

	if (Z_TYPE_P(value) == IS_LONG) {
		RETURN_LONG(Z_LVAL_P(value) < 0 ? -Z_LVAL_P(value) : Z_LVAL_P(value));
	} else {
		ZEND_ASSERT(0 && "Unexpected type");
	}
}
/* }}} */

static PHP_GINIT_FUNCTION(koloktos)
{
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(koloktos)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(koloktos)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "koloktos support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_array_group, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_array_group_pair, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_array_every, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_array_some, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_array_starts_with, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, array1, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array2, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_array_ends_with, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, array1, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array2, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_array_take_while, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_array_drop_while, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_zeroise, 0, 2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, number, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, threshold, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_absint, 0, 1, IS_LONG)
	ZEND_ARG_TYPE_MASK(0, num, MAY_BE_LONG|MAY_BE_DOUBLE, NULL)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ koloktos_functions[]
 */
static const zend_function_entry koloktos_functions[] = {
	PHP_FE(array_group,       arginfo_array_group)
	PHP_FE(array_group_pair,  arginfo_array_group_pair)
	PHP_FE(array_every,       arginfo_array_every)
	PHP_FE(array_some,        arginfo_array_some)
	PHP_FE(array_starts_with, arginfo_array_starts_with)
	PHP_FE(array_ends_with,   arginfo_array_ends_with)
	PHP_FE(array_take_while,  arginfo_array_take_while)
	PHP_FE(array_drop_while,  arginfo_array_drop_while)
	PHP_FE(zeroise,           arginfo_zeroise)
	PHP_FE(absint,            arginfo_absint)
	PHP_FE_END
};
/* }}} */

/* {{{ koloktos_module_entry
 */
zend_module_entry koloktos_module_entry = {
	STANDARD_MODULE_HEADER,
	"koloktos",                   /* Extension name */
	koloktos_functions,           /* zend_function_entry */
	PHP_MINIT(koloktos),          /* PHP_MINIT - Module initialization */
	NULL,                         /* PHP_MSHUTDOWN - Module shutdown */
	NULL,                         /* PHP_RINIT - Request initialization */
	NULL,                         /* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(koloktos),          /* PHP_MINFO - Module info */
	PHP_KOLOKTOS_VERSION,         /* Version */
	PHP_MODULE_GLOBALS(koloktos), /* Module globals */
	PHP_GINIT(koloktos),          /* PHP_GINIT - Globals initialization */
	NULL,                         /* PHP_GSHUTDOWN - Globals shutdown */
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_KOLOKTOS
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(koloktos)
#endif
