/*
 * Values table functions
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

#if !defined( _LIBVMDK_VALUES_TABLE_H )
#define _LIBVMDK_VALUES_TABLE_H

#include <common.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libvmdk_values_table libvmdk_values_table_t;

struct libvmdk_values_table
{
	/* The amount of values
	 */
	int amount_of_values;

	/* The value identifiers
	 */
	libcstring_character_t **identifier;

	/* The value identifier lengths
	 */
	size_t *identifier_length;

	/* The values
	 */
	libcstring_character_t **value;

	/* The value lengths
	 */
	size_t *value_length;
};

int libvmdk_values_table_initialize(
     libvmdk_values_table_t **values_table,
     int amount_of_values,
     liberror_error_t **error );

int libvmdk_values_table_free(
     libvmdk_values_table_t **values_table,
     liberror_error_t **error );

int libvmdk_values_table_resize(
     libvmdk_values_table_t *values_table,
     int amount_of_values,
     liberror_error_t **error );

int libvmdk_values_table_get_amount_of_values(
     libvmdk_values_table_t *values_table,
     int *amount_of_values,
     liberror_error_t **error );

int libvmdk_values_table_get_index(
     libvmdk_values_table_t *values_table,
     const libcstring_character_t *identifier,
     size_t identifier_length,
     int *index,
     liberror_error_t **error );

int libvmdk_values_table_get_identifier_size(
     libvmdk_values_table_t *values_table,
     int index,
     size_t *identifier_size,
     liberror_error_t **error );

int libvmdk_values_table_get_identifier(
     libvmdk_values_table_t *values_table,
     int index,
     libcstring_character_t *identifier,
     size_t identifier_size,
     liberror_error_t **error );

int libvmdk_values_table_set_identifier(
     libvmdk_values_table_t *values_table,
     int index,
     const libcstring_character_t *identifier,
     size_t indentifier_length,
     liberror_error_t **error );

int libvmdk_values_table_get_value_size(
     libvmdk_values_table_t *values_table,
     const libcstring_character_t *identifier,
     size_t identifier_length,
     size_t *value_size,
     liberror_error_t **error );

int libvmdk_values_table_get_value(
     libvmdk_values_table_t *values_table,
     const libcstring_character_t *identifier,
     size_t identifier_length,
     libcstring_character_t *value,
     size_t value_size,
     liberror_error_t **error );

int libvmdk_values_table_set_value(
     libvmdk_values_table_t *values_table,
     const libcstring_character_t *identifier,
     size_t identifier_length,
     const libcstring_character_t *value,
     size_t value_length,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

