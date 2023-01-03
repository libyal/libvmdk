/*
 * Grain group functions
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

#if !defined( _LIBVMDK_GRAIN_GROUP_H )
#define _LIBVMDK_GRAIN_GROUP_H

#include <common.h>
#include <types.h>

#include "libvmdk_libbfio.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libvmdk_grain_group libvmdk_grain_group_t;

struct libvmdk_grain_group
{
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

int libvmdk_grain_group_initialize(
     libvmdk_grain_group_t **grain_group,
     libcerror_error_t **error );

int libvmdk_grain_group_free(
     libvmdk_grain_group_t **grain_group,
     libcerror_error_t **error );

int libvmdk_grain_group_fill(
     libfdata_list_t *grains_list,
     int grain_index,
     size64_t grain_size,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     const uint8_t *grain_group_data,
     size_t grain_group_data_size,
     int number_of_grain_group_entries,
     uint32_t extent_file_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBVMDK_GRAIN_GROUP_H ) */

