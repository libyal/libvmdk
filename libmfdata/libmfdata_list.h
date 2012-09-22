/*
 * The list functions
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

#if !defined( _LIBMFDATA_INTERNAL_LIST_H )
#define _LIBMFDATA_INTERNAL_LIST_H

#include <common.h>
#include <types.h>

#include "libmfdata_array_type.h"
#include "libmfdata_extern.h"
#include "libmfdata_libbfio.h"
#include "libmfdata_libcerror.h"
#include "libmfdata_libmfcache.h"
#include "libmfdata_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libmfdata_internal_list libmfdata_internal_list_t;

struct libmfdata_internal_list
{
	/* The value size
	 */
	size64_t value_size;

	/* The (list) elements (array)
	 */
	libmfdata_array_t *elements;

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

	/* The read element data function
	 */
	int (*read_element_data)(
	       intptr_t *io_handle,
	       libbfio_pool_t *file_io_pool,
	       libmfdata_list_element_t *list_element,
	       libmfcache_cache_t *cache,
	       int file_io_pool_entry,
	       off64_t element_data_offset,
	       size64_t element_data_size,
	       uint32_t element_data_flags,
               uint8_t read_flags,
	       libcerror_error_t **error );

	/* The read element group function
	 */
	int (*read_element_group)(
	       intptr_t *io_handle,
	       libbfio_pool_t *file_io_pool,
	       libmfdata_list_t *list,
	       int element_index,
	       int number_of_elements,
	       libmfcache_cache_t *cache,
	       int file_io_pool_entry,
	       off64_t element_group_offset,
	       size64_t element_group_size,
               uint8_t read_flags,
	       libcerror_error_t **error );
};

LIBMFDATA_EXTERN \
int libmfdata_list_initialize(
     libmfdata_list_t **list,
     intptr_t *io_handle,
     int (*free_io_handle)(
            intptr_t **io_handle,
            libcerror_error_t **error ),
     int (*clone_io_handle)(
            intptr_t **destination_io_handle,
            intptr_t *source_io_handle,
            libcerror_error_t **error ),
     int (*read_element_data)(
            intptr_t *io_handle,
            libbfio_pool_t *file_io_pool,
            libmfdata_list_element_t *list_element,
            libmfcache_cache_t *cache,
            int file_io_pool_entry,
            off64_t element_data_offset,
            size64_t element_data_size,
            uint32_t element_data_flags,
            uint8_t read_flags,
            libcerror_error_t **error ),
     int (*read_element_group)(
            intptr_t *io_handle,
            libbfio_pool_t *file_io_pool,
            libmfdata_list_t *list,
            int element_index,
            int number_of_elements,
            libmfcache_cache_t *cache,
            int file_io_pool_entry,
            off64_t element_group_offset,
            size64_t element_group_size,
            uint8_t read_flags,
            libcerror_error_t **error ),
     uint8_t flags,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_list_free(
     libmfdata_list_t **list,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_list_clone(
     libmfdata_list_t **destination_list,
     libmfdata_list_t *source_list,
     libcerror_error_t **error );

int libmfdata_list_clone_elements(
     libmfdata_list_t *destination_list,
     libmfdata_list_t *source_list,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_list_resize(
     libmfdata_list_t *list,
     int number_of_elements,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_list_get_number_of_elements(
     libmfdata_list_t *list,
     int *number_of_elements,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_list_is_set(
     libmfdata_list_t *list,
     int element_index,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_list_is_group(
     libmfdata_list_t *list,
     int element_index,
     libcerror_error_t **error );

/* List element functions
 */
LIBMFDATA_EXTERN \
int libmfdata_list_get_element_by_index(
     libmfdata_list_t *list,
     int element_index,
     libmfdata_list_element_t **element,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_list_get_element_by_offset(
     libmfdata_list_t *list,
     off64_t value_offset,
     int *element_index,
     libmfdata_list_element_t **element,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_list_set_element_by_index(
     libmfdata_list_t *list,
     int element_index,
     int file_io_pool_entry,
     off64_t offset,
     size64_t size,
     uint32_t flags,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_list_append_element(
     libmfdata_list_t *list,
     int *element_index,
     int file_io_pool_entry,
     off64_t offset,
     size64_t size,
     uint32_t flags,
     libcerror_error_t **error );

/* Group functions
 */
LIBMFDATA_EXTERN \
int libmfdata_list_get_group_by_index(
     libmfdata_list_t *list,
     int element_index,
     int *number_of_elements,
     int *file_io_pool_entry,
     off64_t *offset,
     size64_t *size,
     uint32_t *flags,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_list_set_group_by_index(
     libmfdata_list_t *list,
     int element_index,
     int number_of_elements,
     int file_io_pool_entry,
     off64_t offset,
     size64_t size,
     uint32_t flags,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_list_append_group(
     libmfdata_list_t *list,
     int *element_index,
     int number_of_elements,
     int file_io_pool_entry,
     off64_t offset,
     size64_t size,
     uint32_t flags,
     libcerror_error_t **error );

int libmfdata_list_expand_group(
     libmfdata_list_t *list,
     libbfio_pool_t *file_io_pool,
     libmfcache_cache_t *cache,
     libmfdata_list_element_t *group_list_element,
     int element_index,
     uint8_t read_flags,
     libcerror_error_t **error );

/* Data range functions
 */
LIBMFDATA_EXTERN \
int libmfdata_list_get_data_range_by_index(
     libmfdata_list_t *list,
     int element_index,
     int *file_io_pool_entry,
     off64_t *offset,
     size64_t *size,
     uint32_t *flags,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_list_get_data_range_by_offset(
     libmfdata_list_t *list,
     off64_t value_offset,
     int *element_index,
     int *file_io_pool_entry,
     off64_t *offset,
     size64_t *size,
     uint32_t *flags,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_list_set_data_range_by_index(
     libmfdata_list_t *list,
     int element_index,
     int file_io_pool_entry,
     off64_t offset,
     size64_t size,
     uint32_t flags,
     libcerror_error_t **error );

/* Backup data range functions
 */
LIBMFDATA_EXTERN \
int libmfdata_list_get_backup_data_range_by_index(
     libmfdata_list_t *list,
     int element_index,
     int *file_io_pool_entry,
     off64_t *offset,
     size64_t *size,
     uint32_t *flags,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_list_set_backup_data_range_by_index(
     libmfdata_list_t *list,
     int element_index,
     int file_io_pool_entry,
     off64_t offset,
     size64_t size,
     uint32_t flags,
     libcerror_error_t **error );

/* List element value functions
 */
int libmfdata_list_get_element_index_at_value_offset(
     libmfdata_list_t *list,
     libbfio_pool_t *file_io_pool,
     libmfcache_cache_t *cache,
     off64_t value_offset,
     int *element_index,
     size_t *element_offset,
     libcerror_error_t **error );

int libmfdata_list_get_element_index_at_value_index(
     libmfdata_list_t *list,
     int value_index,
     size_t value_size,
     int *element_index,
     size_t *element_offset,
     libcerror_error_t **error );

int libmfdata_list_get_element_value(
     libmfdata_list_t *list,
     libbfio_pool_t *file_io_pool,
     libmfcache_cache_t *cache,
     libmfdata_list_element_t *element,
     intptr_t **element_value,
     uint8_t read_flags,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_list_get_element_value_by_index(
     libmfdata_list_t *list,
     libbfio_pool_t *file_io_pool,
     libmfcache_cache_t *cache,
     int element_index,
     intptr_t **element_value,
     uint8_t read_flags,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_list_get_element_value_at_offset(
     libmfdata_list_t *list,
     libbfio_pool_t *file_io_pool,
     libmfcache_cache_t *cache,
     off64_t value_offset,
     intptr_t **element_value,
     uint8_t read_flags,
     libcerror_error_t **error );

int libmfdata_list_set_element_value(
     libmfdata_list_t *list,
     libmfcache_cache_t *cache,
     libmfdata_list_element_t *element,
     intptr_t *element_value,
     int (*free_element_value)(
            intptr_t **element_value,
            libcerror_error_t **error ),
     uint8_t flags,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_list_set_element_value_by_index(
     libmfdata_list_t *list,
     libmfcache_cache_t *cache,
     int element_index,
     intptr_t *element_value,
     int (*free_element_value)(
            intptr_t **element_value,
            libcerror_error_t **error ),
     uint8_t flags,
     libcerror_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_list_get_value_size(
     libmfdata_list_t *list,
     size64_t *value_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

