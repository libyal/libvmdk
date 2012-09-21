/*
 * Extent file functions
 *
 * Copyright (c) 2009-2012 Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBVMDK_EXTENT_FILE_H )
#define _LIBVMDK_EXTENT_FILE_H

#include <common.h>
#include <types.h>

#include "libvmdk_libbfio.h"
#include "libvmdk_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const char cowd_sparse_file_signature[ 4 ];
extern const char vmdk_sparse_file_signature[ 4 ];

typedef struct libvmdk_extent_file libvmdk_extent_file_t;

struct libvmdk_extent_file
{
	/* The file io pool entry
	 */
	int file_io_pool_entry;

	/* The extent file type
	 */
	uint8_t file_type;

	/* The extent file format version
	 */
	uint32_t format_version;

	/* The extent file flags
	 */
	uint32_t flags;

	/* The maximum data size
	 */
	uint64_t maximum_data_size;

	/* The compression method
	 */
	uint16_t compression_method;

	/* The descriptor offset
	 */
	off64_t descriptor_offset;

	/* The descriptor size
	 */
	size64_t descriptor_size;

	/* The grain size
	 */
	size64_t grain_size;

	/* The amount of grain table entries
	 */
	uint32_t amount_of_grain_table_entries;

	/* The amount of grain directory entries
	 */
	uint32_t amount_of_grain_directory_entries;

	/* The grain directory offset
	 */
	off64_t grain_directory_offset;

	/* The secondary grain directory offset
	 */
	off64_t secondary_grain_directory_offset;
};

int libvmdk_extent_file_initialize(
     libvmdk_extent_file_t **extent_file,
     int file_io_pool_entry,
     liberror_error_t **error );

int libvmdk_extent_file_free(
     libvmdk_extent_file_t **extent_file,
     liberror_error_t **error );

ssize_t libvmdk_extent_file_read_file_header(
         libvmdk_extent_file_t *extent_file,
         libbfio_pool_t *file_io_pool,
         libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

