/*
 * Grain table functions
 *
 * Copyright (c) 2009-2012, Joachim Metz <joachim.metz@gmail.com>
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

#include "libvmdk_io_handle.h"
#include "libvmdk_libbfio.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libmfcache.h"
#include "libvmdk_libmfdata.h"

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
     libmfdata_list_element_t *list_element,
     libmfcache_cache_t *cache,
     int file_io_pool_entry,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint32_t element_data_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

int libvmdk_grain_table_read_offsets(
     intptr_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libmfdata_list_t *grain_table_list,
     int element_index,
     int number_of_elements,
     libmfcache_cache_t *cache,
     int file_io_pool_entry,
     off64_t element_group_offset,
     size64_t element_group_size,
     uint32_t element_group_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

int libvmdk_grain_table_fill(
     libvmdk_grain_table_t *grain_table,
     libmfdata_list_t *grain_table_list,
     int grain_index,
     int file_io_pool_entry,
     const uint8_t *grain_table_data,
     size_t grain_table_data_size,
     int number_of_grain_table_entries,
     libcerror_error_t **error );

int libvmdk_grain_table_correct(
     libvmdk_grain_table_t *grain_table,
     libmfdata_list_t *grain_table_list,
     int grain_index,
     int file_io_pool_entry,
     const uint8_t *grain_table_data,
     size_t grain_table_data_size,
     int number_of_grain_table_entries,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

