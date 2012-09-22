/*
 * The segment functions
 *
 * Copyright (c) 2010-2012, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBMFDATA_INTERNAL_SEGMENT_H )
#define _LIBMFDATA_INTERNAL_SEGMENT_H

#include <common.h>
#include <types.h>

#include "libmfdata_extern.h"
#include "libmfdata_libcerror.h"
#include "libmfdata_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libmfdata_internal_segment libmfdata_internal_segment_t;

struct libmfdata_internal_segment
{
	/* The segment table reference
	 */
	libmfdata_segment_table_t *segment_table;

	/* The segment (table) index
	 */
	int segment_index;

	/* The file IO pool entry
	 */
	int file_io_pool_entry;

	/* The (segment) size
	 */
	size64_t size;

	/* The flags
	 */
	uint8_t flags;

	/* The (segment) value offset
	 */
	off64_t value_offset;
};

int libmfdata_segment_initialize(
     libmfdata_segment_t **segment,
     libmfdata_segment_table_t *segment_table,
     int segment_index,
     libcerror_error_t **error );

int libmfdata_segment_free(
     libmfdata_segment_t **segment,
     libcerror_error_t **error );

int libmfdata_segment_get_range(
     libmfdata_segment_t *segment,
     int *file_io_pool_entry,
     size64_t *segment_size,
     libcerror_error_t **error );

int libmfdata_segment_set_range(
     libmfdata_segment_t *segment,
     int file_io_pool_entry,
     size64_t segment_size,
     libcerror_error_t **error );

int libmfdata_segment_get_segment_index(
     libmfdata_segment_t *segment,
     int *segment_index,
     libcerror_error_t **error );

int libmfdata_segment_set_segment_index(
     libmfdata_segment_t *segment,
     int segment_index,
     libcerror_error_t **error );

int libmfdata_segment_get_value_offset(
     libmfdata_segment_t *segment,
     off64_t *value_offset,
     libcerror_error_t **error );

int libmfdata_segment_set_value_offset(
     libmfdata_segment_t *segment,
     off64_t value_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

