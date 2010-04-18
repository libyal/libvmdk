/*
 * List type functions
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBVMDK_LIST_TYPE_H )
#define _LIBVMDK_LIST_TYPE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( __cplusplus )
extern "C" {
#endif

enum LIBVMDK_LIST_COMPARE_DEFINITIONS
{
	LIBVMDK_LIST_COMPARE_LESS,
	LIBVMDK_LIST_COMPARE_EQUAL,
	LIBVMDK_LIST_COMPARE_GREATER
};

typedef struct libvmdk_list_element libvmdk_list_element_t;

struct libvmdk_list_element
{
	/* The previous element
	 */
	libvmdk_list_element_t *previous;

	/* The next element
	 */
	libvmdk_list_element_t *next;

	/* The list element value
	 */
	intptr_t *value;
};

typedef struct libvmdk_list libvmdk_list_t;

struct libvmdk_list
{
	/* The amount of elements
	 */
	int amount_of_elements;

	/* The first list element
	 */
	libvmdk_list_element_t *first;

	/* The last list element
	 */
	libvmdk_list_element_t *last;
};

int libvmdk_list_element_initialize(
     libvmdk_list_element_t **list_element,
     liberror_error_t **error );

int libvmdk_list_element_free(
     libvmdk_list_element_t **list_element,
     int (*value_free_function)(
            intptr_t *value,
            liberror_error_t **error ),
     liberror_error_t **error );

int libvmdk_list_initialize(
     libvmdk_list_t **list,
     liberror_error_t **error );

int libvmdk_list_free(
     libvmdk_list_t **list,
     int (*value_free_function)(
            intptr_t *value,
            liberror_error_t **error ),
     liberror_error_t **error );

int libvmdk_list_empty(
     libvmdk_list_t *list,
     int (*value_free_function)(
            intptr_t *value,
            liberror_error_t **error ),
     liberror_error_t **error );

int libvmdk_list_clone(
     libvmdk_list_t **destination,
     libvmdk_list_t *source,
     int (*value_clone_function)(
            intptr_t **destination,
            intptr_t *source,
            liberror_error_t **error ),
     liberror_error_t **error );

int libvmdk_list_get_amount_of_elements(
     libvmdk_list_t *list,
     int *amount_of_elements,
     liberror_error_t **error );

int libvmdk_list_get_element(
     libvmdk_list_t *list,
     int element_index,
     libvmdk_list_element_t **element,
     liberror_error_t **error );

int libvmdk_list_get_value(
     libvmdk_list_t *list,
     int element_index,
     intptr_t **value,
     liberror_error_t **error );

int libvmdk_list_prepend_element(
     libvmdk_list_t *list,
     libvmdk_list_element_t *element,
     liberror_error_t **error );

int libvmdk_list_prepend_value(
     libvmdk_list_t *list,
     intptr_t *value,
     liberror_error_t **error );

int libvmdk_list_append_element(
     libvmdk_list_t *list,
     libvmdk_list_element_t *element,
     liberror_error_t **error );

int libvmdk_list_append_value(
     libvmdk_list_t *list,
     intptr_t *value,
     liberror_error_t **error );

int libvmdk_list_insert_element(
     libvmdk_list_t *list,
     libvmdk_list_element_t *element,
     int (*value_compare_function)(
            intptr_t *first,
            intptr_t *second,
            liberror_error_t **error ),
     liberror_error_t **error );

int libvmdk_list_insert_value(
     libvmdk_list_t *list,
     intptr_t *value,
     int (*value_compare_function)(
            intptr_t *first,
            intptr_t *second,
            liberror_error_t **error ),
     liberror_error_t **error );

int libvmdk_list_remove_element(
     libvmdk_list_t *list,
     libvmdk_list_element_t *element,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

