/*
 * The file list functions
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

#if !defined( _LIBMFDATA_INTERNAL_FILE_LIST_H )
#define _LIBMFDATA_INTERNAL_FILE_LIST_H

#include <common.h>
#include <types.h>

#include "libmfdata_extern.h"
#include "libmfdata_libbfio.h"
#include "libmfdata_libcdata.h"
#include "libmfdata_libcerror.h"
#include "libmfdata_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libmfdata_internal_file_list libmfdata_internal_file_list_t;

struct libmfdata_internal_file_list
{
	/* The files (array)
	 */
	libcdata_array_t *files;

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

	/* The read file data function
	 */
	int (*read_file_data)(
	       intptr_t *io_handle,
	       libbfio_pool_t *file_io_pool,
	       int file_io_pool_entry,
	       libmfdata_file_t *file,
	       libmfcache_cache_t *cache,
	       uint8_t read_flags,
	       libcerror_error_t **error );
};

LIBMFDATA_EXTERN \
int libmfdata_file_list_initialize(
     libmfdata_file_list_t **file_list,
     intptr_t *io_handle,
     int (*free_io_handle)(
            intptr_t **io_handle,
            libcerror_error_t **error ),
     int (*clone_io_handle)(
            intptr_t **destination_io_handle,
            intptr_t *source_io_handle,
            libcerror_error_t **error ),
     int (*read_file_data)(
            intptr_t *io_handle,
            libbfio_pool_t *file_io_pool,
            int file_io_pool_entry,
            libmfdata_file_t *file,
            libmfcache_cache_t *cache,
            uint8_t read_flags,
            libcerror_error_t **error ),
     uint8_t flags,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_file_list_free(
     libmfdata_file_list_t **file_list,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_file_list_clone(
     libmfdata_file_list_t **destination_list,
     libmfdata_file_list_t *source_list,
     libcerror_error_t **error );

int libmfdata_file_list_clone_files(
     libmfdata_file_list_t *destination_file_list,
     libmfdata_file_list_t *source_file_list,
     libcerror_error_t **error );

/* File list element functions
 */
LIBMFDATA_EXTERN \
int libmfdata_file_list_resize(
     libmfdata_file_list_t *file_list,
     int number_of_files,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_file_list_get_number_of_files(
     libmfdata_file_list_t *file_list,
     int *number_of_files,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_file_list_get_file_item_by_index(
     libmfdata_file_list_t *file_list,
     int file_index,
     libmfdata_file_t **file,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_file_list_get_file_by_index(
     libmfdata_file_list_t *file_list,
     int file_index,
     int *file_io_pool_entry,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_file_list_set_file_by_index(
     libmfdata_file_list_t *file_list,
     int file_index,
     int file_io_pool_entry,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_file_list_append_file(
     libmfdata_file_list_t *file_list,
     int *file_index,
     int file_io_pool_entry,
     libcerror_error_t **error );

/* File value functions
 */
int libmfdata_file_list_get_file_value(
     libmfdata_file_list_t *file_list,
     libbfio_pool_t *file_io_pool,
     libmfcache_cache_t *cache,
     libmfdata_file_t *file,
     intptr_t **file_value,
     uint8_t read_flags,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_file_list_get_file_value_by_index(
     libmfdata_file_list_t *file_list,
     libbfio_pool_t *file_io_pool,
     libmfcache_cache_t *cache,
     int file_index,
     intptr_t **file_value,
     uint8_t read_flags,
     libcerror_error_t **error );

int libmfdata_file_list_set_file_value(
     libmfdata_file_list_t *file_list,
     libmfcache_cache_t *cache,
     libmfdata_file_t *file,
     intptr_t *file_value,
     int (*free_file_value)(
            intptr_t **file_value,
            libcerror_error_t **error ),
     uint8_t flags,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_file_list_set_file_value_by_index(
     libmfdata_file_list_t *file_list,
     libmfcache_cache_t *cache,
     int file_index,
     intptr_t *file_value,
     int (*free_file_value)(
            intptr_t **file_value,
            libcerror_error_t **error ),
     uint8_t flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

