/*
 * Offset table functions
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

#if !defined( _LIBEWF_OFFSET_TABLE_H )
#define _LIBEWF_OFFSET_TABLE_H

#include <common.h>
#include <types.h>

#include "libvmdk_grain_offset.h"
#include "libvmdk_libbfio.h"
#include "libvmdk_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libvmdk_offset_table libvmdk_offset_table_t;

struct libvmdk_offset_table
{
	/* Stores the number of grains in the table
	 * There is an offset per grain in the table
	 */
	uint32_t number_of_grain_offsets;

	/* The last grain offset that was filled
	 */
	uint32_t last_grain_offset_filled;

	/* The last grain offset that was compared
	 */
	uint32_t last_grain_offset_compared;

	/* Dynamic array of grain offsets
	 */
	libvmdk_grain_offset_t *grain_offset;
};

int libvmdk_offset_table_initialize(
     libvmdk_offset_table_t **offset_table,
     uint32_t number_of_grain_offsets,
     libcerror_error_t **error );

int libvmdk_offset_table_free(
     libvmdk_offset_table_t **offset_table,
     libcerror_error_t **error );

int libvmdk_offset_table_resize(
     libvmdk_offset_table_t *offset_table,
     uint32_t number_of_grain_offsets,
     libcerror_error_t **error );

int libvmdk_offset_table_fill(
     libvmdk_offset_table_t *offset_table,
     uint8_t *grain_table,
     size_t grain_table_size,
     uint32_t number_of_grain_table_entries,
     size64_t grain_size,
     libcerror_error_t **error );

int libvmdk_offset_table_compare(
     libvmdk_offset_table_t *offset_table,
     uint8_t *grain_table,
     size_t grain_table_size,
     uint32_t number_of_grain_table_entries,
     size64_t grain_size,
     libcerror_error_t **error );

off64_t libvmdk_offset_table_seek_grain_offset(
         libvmdk_offset_table_t *offset_table,
         uint32_t grain,
         libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

