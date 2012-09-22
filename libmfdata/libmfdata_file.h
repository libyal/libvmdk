/*
 * The file functions
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

#if !defined( _LIBMFDATA_INTERNAL_FILE_H )
#define _LIBMFDATA_INTERNAL_FILE_H

#include <common.h>
#include <types.h>

#include "libmfdata_extern.h"
#include "libmfdata_libbfio.h"
#include "libmfdata_libcerror.h"
#include "libmfdata_libmfcache.h"
#include "libmfdata_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libmfdata_internal_file libmfdata_internal_file_t;

struct libmfdata_internal_file
{
	/* The (file) list reference
	 */
	libmfdata_file_list_t *list;

	/* The (file list) element index
	 */
	int element_index;

	/* The file IO pool entry
	 */
	int file_io_pool_entry;

	/* The time stamp
	 */
	time_t timestamp;
};

int libmfdata_file_initialize(
     libmfdata_file_t **file,
     libmfdata_file_list_t *list,
     int element_index,
     libcerror_error_t **error );

int libmfdata_file_free(
     libmfdata_file_t **file,
     libcerror_error_t **error );

int libmfdata_file_get_element_index(
     libmfdata_file_t *file,
     int *element_index,
     libcerror_error_t **error );

int libmfdata_file_set_element_index(
     libmfdata_file_t *file,
     int element_index,
     libcerror_error_t **error );

int libmfdata_file_get_timestamp(
     libmfdata_file_t *file,
     time_t *timestamp,
     libcerror_error_t **error );

/* Data range functions
 */
LIBMFDATA_EXTERN \
int libmfdata_file_get_data_range(
     libmfdata_file_t *file,
     int *file_io_pool_entry,
     libcerror_error_t **error );

int libmfdata_file_set_data_range(
     libmfdata_file_t *file,
     int file_io_pool_entry,
     libcerror_error_t **error );

/* File value functions
 */
LIBMFDATA_EXTERN \
int libmfdata_file_get_file_value(
     libmfdata_file_t *file,
     libbfio_pool_t *file_io_pool,
     libmfcache_cache_t *cache,
     intptr_t **file_value,
     uint8_t read_flags,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_file_set_file_value(
     libmfdata_file_t *file,
     libmfcache_cache_t *cache,
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

