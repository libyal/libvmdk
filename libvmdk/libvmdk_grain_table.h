/*
 * Grain table functions
 *
 * Copyright (c) 2009-2013, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBVMDK_GRAIN_TABLE_H )
#define _LIBVMDK_GRAIN_TABLE_H

#include <common.h>
#include <types.h>

#include "libvmdk_extent_table.h"
#include "libvmdk_grain_data.h"
#include "libvmdk_io_handle.h"
#include "libvmdk_libbfio.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libfcache.h"
#include "libvmdk_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libvmdk_grain_table libvmdk_grain_table_t;

struct libvmdk_grain_table
{
	/* The IO handle
	 */
	libvmdk_io_handle_t *io_handle;

	/* The previous last grain offset that was filled
	 */
	int previous_last_grain_offset_filled;

	/* The last grain offset that was filled
	 */
	int last_grain_offset_filled;

	/* The last grain offset that was compared
	 */
	int last_grain_offset_compared;
};

int libvmdk_grain_table_initialize(
     libvmdk_grain_table_t **grain_table,
     libvmdk_io_handle_t *io_handle,
     libcerror_error_t **error );

int libvmdk_grain_table_free(
     libvmdk_grain_table_t **grain_table,
     libcerror_error_t **error );

int libvmdk_grain_table_clone(
     libvmdk_grain_table_t **destination_grain_table,
     libvmdk_grain_table_t *source_grain_table,
     libcerror_error_t **error );

int libvmdk_grain_table_read_grain(
     intptr_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libfdata_list_element_t *list_element,
     libfcache_cache_t *cache,
     int file_io_pool_entry,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint32_t element_data_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

int libvmdk_grain_table_read_offsets(
     intptr_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libfdata_list_t *grain_table_list,
     int element_index,
     int number_of_elements,
     libfcache_cache_t *cache,
     int file_io_pool_entry,
     off64_t element_group_offset,
     size64_t element_group_size,
     uint32_t element_group_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

int libvmdk_grain_table_fill(
     libvmdk_grain_table_t *grain_table,
     libfdata_list_t *grain_table_list,
     int grain_index,
     int file_io_pool_entry,
     const uint8_t *grain_table_data,
     size_t grain_table_data_size,
     int number_of_grain_table_entries,
     libcerror_error_t **error );

int libvmdk_grain_table_correct(
     libvmdk_grain_table_t *grain_table,
     libfdata_list_t *grain_table_list,
     int grain_index,
     int file_io_pool_entry,
     const uint8_t *grain_table_data,
     size_t grain_table_data_size,
     int number_of_grain_table_entries,
     libcerror_error_t **error );

/* TODO refactor */

int libvmdk_grain_table_get_extent_file_grain_group_by_offset(
     libvmdk_grain_table_t *grain_table,
     libbfio_pool_t *file_io_pool,
     libvmdk_extent_table_t *extent_table,
     off64_t offset,
     uint32_t *extent_number,
     off64_t *extent_file_data_offset,
     libvmdk_extent_file_t **extent_file,
     int *grain_groups_list_index,
     off64_t *grain_group_data_offset,
     libfdata_list_t **grains_list,
     libcerror_error_t **error );

int libvmdk_grain_table_get_grain_data_by_offset(
     libvmdk_grain_table_t *grain_table,
     uint64_t grain_index,
     libbfio_pool_t *file_io_pool,
     libvmdk_extent_table_t *extent_table,
     libfcache_cache_t *grains_cache,
     off64_t offset,
     libvmdk_grain_data_t **grain_data,
     off64_t *grain_data_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

