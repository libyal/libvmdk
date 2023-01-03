/*
 * Grain table functions
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

int libvmdk_grain_table_grain_is_sparse_at_offset(
     libvmdk_grain_table_t *grain_table,
     uint64_t grain_index,
     libbfio_pool_t *file_io_pool,
     libvmdk_extent_table_t *extent_table,
     off64_t offset,
     libcerror_error_t **error );

int libvmdk_grain_table_get_grain_data_at_offset(
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

#endif /* !defined( _LIBVMDK_GRAIN_TABLE_H ) */

