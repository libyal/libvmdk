/*
 * Segment table functions
 *
 * Copyright (c) 2010, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBVMDK_SEGMENT_TABLE_H )
#define _LIBVMDK_SEGMENT_TABLE_H

#include <common.h>
#include <types.h>

#include "libvmdk_libcdata.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libcstring.h"
#include "libvmdk_segment_file_handle.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libvmdk_segment_table libvmdk_segment_table_t;

struct libvmdk_segment_table
{
	/* The basename
	 */
	libcstring_system_character_t *basename;

	/* The basename size
	 */
	size_t basename_size;

	/* The segment file handle array
	 */
	libcdata_array_t *segment_file_handle_array;

	/* The maximum segment size
	 */
	size64_t maximum_segment_size;
};

int libvmdk_segment_table_initialize(
     libvmdk_segment_table_t **segment_table,
     int amount,
     size64_t maximum_segment_size,
     libcerror_error_t **error );

int libvmdk_segment_table_free(
     libvmdk_segment_table_t **segment_table,
     libcerror_error_t **error );

int libvmdk_segment_table_resize(
     libvmdk_segment_table_t *segment_table,
     int amount,
     libcerror_error_t **error );

int libvmdk_segment_table_get_basename_size(
     libvmdk_segment_table_t *segment_table,
     size_t *basename_size,
     libcerror_error_t **error );

int libvmdk_segment_table_get_basename(
     libvmdk_segment_table_t *segment_table,
     char *basename,
     size_t basename_size,
     libcerror_error_t **error );

int libvmdk_segment_table_set_basename(
     libvmdk_segment_table_t *segment_table,
     const char *basename,
     size_t basename_length,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
int libvmdk_segment_table_get_basename_size_wide(
     libvmdk_segment_table_t *segment_table,
     size_t *basename_size,
     libcerror_error_t **error );

int libvmdk_segment_table_get_basename_wide(
     libvmdk_segment_table_t *segment_table,
     wchar_t *basename,
     size_t basename_size,
     libcerror_error_t **error );

int libvmdk_segment_table_set_basename_wide(
     libvmdk_segment_table_t *segment_table,
     const wchar_t *basename,
     size_t basename_length,
     libcerror_error_t **error );
#endif

int libvmdk_segment_table_get_amount_of_handles(
     libvmdk_segment_table_t *segment_table,
     int *amount_of_handles,
     libcerror_error_t **error );

int libvmdk_segment_table_get_handle(
     libvmdk_segment_table_t *segment_table,
     int handle_index,
     libvmdk_segment_file_handle_t **handle,
     libcerror_error_t **error );

int libvmdk_segment_table_set_handle(
     libvmdk_segment_table_t *segment_table,
     int handle_index,
     libvmdk_segment_file_handle_t *handle,
     libcerror_error_t **error );

int libvmdk_segment_table_set_maximum_segment_size(
     libvmdk_segment_table_t *segment_table,
     size64_t maximum_segment_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

