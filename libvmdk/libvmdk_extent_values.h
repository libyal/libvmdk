/*
 * Extent values functions
 *
 * Copyright (C) 2009-2023, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _LIBVMDK_EXTENT_VALUES_H )
#define _LIBVMDK_EXTENT_VALUES_H

#include <common.h>
#include <types.h>

#include "libvmdk_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libvmdk_extent_values libvmdk_extent_values_t;

struct libvmdk_extent_values
{
	/* The filename
	 */
	uint8_t *filename;

	/* The filename size
	 */
	size_t filename_size;

	/* The filename encoding
	 */
	int filename_encoding;

	/* The alternate filename
	 */
	system_character_t *alternate_filename;

	/* The alternate filename size
	 */
	size_t alternate_filename_size;

	/* The extent offset
	 */
	off64_t offset;

	/* The extent size
	 */
	size64_t size;

	/* The type
	 */
	int type;

	/* The access
	 */
	int access;
};

int libvmdk_extent_values_initialize(
     libvmdk_extent_values_t **extent_values,
     libcerror_error_t **error );

int libvmdk_extent_values_free(
     libvmdk_extent_values_t **extent_values,
     libcerror_error_t **error );

int libvmdk_extent_values_read(
     libvmdk_extent_values_t *extent_values,
     const char *value_string,
     size_t value_string_size,
     int encoding,
     libcerror_error_t **error );

int libvmdk_extent_values_set_filename(
     libvmdk_extent_values_t *extent_values,
     const char *filename,
     size_t filename_length,
     int encoding,
     libcerror_error_t **error );

int libvmdk_extent_values_set_alternate_filename(
     libvmdk_extent_values_t *extent_values,
     const char *filename,
     size_t filename_length,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

int libvmdk_extent_values_set_alternate_filename_wide(
     libvmdk_extent_values_t *extent_values,
     const wchar_t *filename,
     size_t filename_length,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

int libvmdk_extent_values_get_type(
     libvmdk_extent_values_t *extent_values,
     int *type,
     libcerror_error_t **error );

int libvmdk_extent_values_get_range(
     libvmdk_extent_values_t *extent_values,
     off64_t *offset,
     size64_t *size,
     libcerror_error_t **error );

int libvmdk_extent_values_get_utf8_filename_size(
     libvmdk_extent_values_t *extent_values,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libvmdk_extent_values_get_utf8_filename(
     libvmdk_extent_values_t *extent_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libvmdk_extent_values_get_utf16_filename_size(
     libvmdk_extent_values_t *extent_values,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libvmdk_extent_values_get_utf16_filename(
     libvmdk_extent_values_t *extent_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBVMDK_EXTENT_VALUES_H ) */

