/*
 * Grain data functions
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

#if !defined( _LIBVMDK_GRAIN_DATA_H )
#define _LIBVMDK_GRAIN_DATA_H

#include <common.h>
#include <types.h>

#include "libvmdk_io_handle.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libvmdk_grain_data libvmdk_grain_data_t;

struct libvmdk_grain_data
{
	/* The uncompressed data offset
	 */
	off64_t uncompressed_data_offset;

	/* The compressed data size
	 */
	uint32_t compressed_data_size;

	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	size_t data_size;
};

int libvmdk_grain_data_initialize(
     libvmdk_grain_data_t **grain_data,
     size_t data_size,
     libcerror_error_t **error );

int libvmdk_grain_data_free(
     libvmdk_grain_data_t **grain_data,
     libcerror_error_t **error );

ssize_t libvmdk_grain_data_read_compressed_header(
         libvmdk_grain_data_t *grain_data,
         libvmdk_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libcerror_error_t **error );

int libvmdk_grain_data_read_element_data(
     libvmdk_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libfdata_list_element_t *element,
     libfdata_cache_t *cache,
     int file_io_pool_entry,
     off64_t grain_data_offset,
     size64_t grain_data_size,
     uint32_t grain_data_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBVMDK_GRAIN_DATA_H ) */

