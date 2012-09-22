/*
 * The segment table functions
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

#if !defined( _LIBMFDATA_INTERNAL_SEGMENT_TABLE_H )
#define _LIBMFDATA_INTERNAL_SEGMENT_TABLE_H

#include <common.h>
#include <types.h>

#include "libmfdata_array_type.h"
#include "libmfdata_extern.h"
#include "libmfdata_libbfio.h"
#include "libmfdata_libcerror.h"
#include "libmfdata_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libmfdata_internal_segment_table libmfdata_internal_segment_table_t;

struct libmfdata_internal_segment_table
{
	/* The current segment index
	 */
	int segment_index;

	/* The current value offset
	 */
	off64_t value_offset;

	/* The value size
	 */
	size64_t value_size;

	/* The maximum segment size
	 */
	size64_t maximum_segment_size;

	/* The segments (array)
	 */
	libmfdata_array_t *segments;

	/* The flags
	 */
	uint8_t flags;

	/* The io handle
	 */
	intptr_t *io_handle;

	/* The free io handle function
	 */
	int (*free_io_handle)(
	       intptr_t **io_handle,
	       libcerror_error_t **error );

	/* The clone (duplicate) io handle function
	 */
	int (*clone_io_handle)(
	       intptr_t **destination_io_handle,
	       intptr_t *source_io_handle,
	       libcerror_error_t **error );

	/* The set segment name function
	 */
	int (*set_segment_name)(
	       intptr_t *io_handle,
	       libbfio_handle_t *file_io_handle,
	       int segment_index,
	       libcerror_error_t **error );

	/* The read segment data function
	 */
	ssize_t (*read_segment_data)(
	           intptr_t *io_handle,
	           libbfio_handle_t *file_io_handle,
	           int segment_index,
	           uint8_t *segment_data,
	           size_t segment_data_size,
	           libcerror_error_t **error );

	/* The write segment data function
	 */
	ssize_t (*write_segment_data)(
	           intptr_t *io_handle,
	           libbfio_handle_t *file_io_handle,
	           int segment_index,
	           const uint8_t *segment_data,
	           size_t segment_data_size,
	           libcerror_error_t **error );

	/* The seek segment offset function
	 */
	off64_t (*seek_segment_offset)(
	           intptr_t *io_handle,
	           libbfio_handle_t *file_io_handle,
	           int segment_index,
	           off64_t segment_offset,
	           int whence,
	           libcerror_error_t **error );
};

LIBMFDATA_EXTERN \
int libmfdata_segment_table_initialize(
     libmfdata_segment_table_t **segment_table,
     intptr_t *io_handle,
     int (*free_io_handle)(
            intptr_t **io_handle,
            libcerror_error_t **error ),
     int (*clone_io_handle)(
            intptr_t **destination_io_handle,
            intptr_t *source_io_handle,
            libcerror_error_t **error ),
     int (*set_segment_name)(
            intptr_t *io_handle,
            libbfio_handle_t *file_io_handle,
            int segment_index,
            libcerror_error_t **error ),
     ssize_t (*read_segment_data)(
                intptr_t *io_handle,
                libbfio_handle_t *file_io_handle,
                int segment_index,
                uint8_t *segment_data,
                size_t segment_data_size,
                libcerror_error_t **error ),
     ssize_t (*write_segment_data)(
                intptr_t *io_handle,
                libbfio_handle_t *file_io_handle,
                int segment_index,
                const uint8_t *segment_data,
                size_t segment_data_size,
                libcerror_error_t **error ),
     off64_t (*seek_segment_offset)(
                intptr_t *io_handle,
                libbfio_handle_t *file_io_handle,
                int segment_index,
                off64_t segment_offset,
                int whence,
                libcerror_error_t **error ),
     uint8_t flags,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_segment_table_free(
     libmfdata_segment_table_t **segment_table,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_segment_table_clone(
     libmfdata_segment_table_t **destination_segment_table,
     libmfdata_segment_table_t *source_segment_table,
     libcerror_error_t **error );

int libmfdata_segment_table_clone_segments(
     libmfdata_segment_table_t *destination_segment_table,
     libmfdata_segment_table_t *source_segment_table,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_segment_table_empty(
     libmfdata_segment_table_t *segment_table,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_segment_table_get_value_offset(
     libmfdata_segment_table_t *segment_table,
     off64_t *value_offset,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_segment_table_get_value_size(
     libmfdata_segment_table_t *segment_table,
     size64_t *value_size,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_segment_table_get_maximum_segment_size(
     libmfdata_segment_table_t *segment_table,
     size64_t *maximum_segment_size,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_segment_table_set_maximum_segment_size(
     libmfdata_segment_table_t *segment_table,
     size64_t maximum_segment_size,
     libcerror_error_t **error );

/* Segment functions
 */
LIBMFDATA_EXTERN \
int libmfdata_segment_table_resize(
     libmfdata_segment_table_t *segment_table,
     int number_of_segments,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_segment_table_get_number_of_segments(
     libmfdata_segment_table_t *segment_table,
     int *number_of_segments,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_segment_table_get_segment_by_index(
     libmfdata_segment_table_t *segment_table,
     int segment_index,
     libmfdata_segment_t **segment,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_segment_table_get_segment_at_value_offset(
     libmfdata_segment_table_t *segment_table,
     off64_t value_offset,
     int *segment_index,
     libmfdata_segment_t **segment,
     off64_t *segment_offset,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_segment_table_set_segment_by_index(
     libmfdata_segment_table_t *segment_table,
     int segment_index,
     int file_io_pool_entry,
     size64_t segment_size,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_segment_table_append_segment(
     libmfdata_segment_table_t *segment_table,
     int *segment_index,
     int file_io_pool_entry,
     size64_t segment_size,
     libcerror_error_t **error );

/* Input/Output functions
 */
LIBMFDATA_EXTERN \
ssize_t libmfdata_segment_table_read_segment_data(
         intptr_t *io_handle,
         libbfio_handle_t *file_io_handle,
         int segment_index,
         uint8_t *segment_data,
         size_t segment_data_size,
         libcerror_error_t **error );

LIBMFDATA_EXTERN \
ssize_t libmfdata_segment_table_write_segment_data(
         intptr_t *io_handle,
         libbfio_handle_t *file_io_handle,
         int segment_index,
         const uint8_t *segment_data,
         size_t segment_data_size,
         libcerror_error_t **error );

LIBMFDATA_EXTERN \
off64_t libmfdata_segment_table_seek_segment_offset(
         intptr_t *io_handle,
         libbfio_handle_t *file_io_handle,
         int segment_index,
         off64_t segment_offset,
         int whence,
         libcerror_error_t **error );

LIBMFDATA_EXTERN \
ssize_t libmfdata_segment_table_read_buffer(
         libmfdata_segment_table_t *segment_table,
         libbfio_pool_t *file_io_pool,
         uint8_t *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBMFDATA_EXTERN \
ssize_t libmfdata_segment_table_write_buffer(
         libmfdata_segment_table_t *segment_table,
         libbfio_pool_t *file_io_pool,
         const uint8_t *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBMFDATA_EXTERN \
off64_t libmfdata_segment_table_seek_offset(
         libmfdata_segment_table_t *segment_table,
         libbfio_pool_t *file_io_pool,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_segment_table_get_file_io_handle(
     libmfdata_segment_table_t *segment_table,
     libbfio_pool_t *file_io_pool,
     libbfio_handle_t **file_io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

