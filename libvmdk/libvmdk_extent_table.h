/*
 * Extent table functions
 *
 * Copyright (C) 2009-2021, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBVMDK_EXTENT_TABLE_H )
#define _LIBVMDK_EXTENT_TABLE_H

#include <common.h>
#include <types.h>

#include "libvmdk_extent_descriptor.h"
#include "libvmdk_extent_file.h"
#include "libvmdk_io_handle.h"
#include "libvmdk_libbfio.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libfcache.h"
#include "libvmdk_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libvmdk_extent_table libvmdk_extent_table_t;

struct libvmdk_extent_table
{
	/* The IO handle
	 */
	libvmdk_io_handle_t *io_handle;

	/* The data files path
	 */
	system_character_t *data_files_path;

	/* The data files path size
	 */
	size_t data_files_path_size;

	/* The disk type
	 */
	int disk_type;

	/* The extent type
	 */
	int extent_type;

	/* The number of extents
	 */
	int number_of_extents;

	/* The extent files list used for sparse extent files
	 */
	libfdata_list_t *extent_files_list;

	/* The extent files cache used for sparse extent files
	 */
	libfcache_cache_t *extent_files_cache;

	/* The extent files stream used for flat (non-sparse) extent files
	 */
	libfdata_stream_t *extent_files_stream;

	/* Flags
	 */
	uint8_t flags;
};

int libvmdk_extent_table_initialize(
     libvmdk_extent_table_t **extent_table,
     libvmdk_io_handle_t *io_handle,
     libcerror_error_t **error );

int libvmdk_extent_table_free(
     libvmdk_extent_table_t **extent_table,
     libcerror_error_t **error );

int libvmdk_extent_table_clear(
     libvmdk_extent_table_t *extent_table,
     libcerror_error_t **error );

int libvmdk_extent_table_clone(
     libvmdk_extent_table_t **destination_extent_table,
     libvmdk_extent_table_t *source_extent_table,
     libcerror_error_t **error );

int libvmdk_extent_table_get_data_files_path_size(
     libvmdk_extent_table_t *extent_table,
     size_t *path,
     libcerror_error_t **error );

int libvmdk_extent_table_get_data_files_path(
     libvmdk_extent_table_t *extent_table,
     char *path,
     size_t path_size,
     libcerror_error_t **error );

int libvmdk_extent_table_set_data_files_path(
     libvmdk_extent_table_t *extent_table,
     const char *path,
     size_t path_length,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

int libvmdk_extent_table_get_data_files_path_size_wide(
     libvmdk_extent_table_t *extent_table,
     size_t *path_size,
     libcerror_error_t **error );

int libvmdk_extent_table_get_data_files_path_wide(
     libvmdk_extent_table_t *extent_table,
     wchar_t *path,
     size_t path_size,
     libcerror_error_t **error );

int libvmdk_extent_table_set_data_files_path_wide(
     libvmdk_extent_table_t *extent_table,
     const wchar_t *path,
     size_t path_length,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

int libvmdk_extent_table_initialize_extents(
     libvmdk_extent_table_t *extent_table,
     int number_of_extents,
     int disk_type,
     libcerror_error_t **error );

int libvmdk_extent_table_get_number_of_extents(
     libvmdk_extent_table_t *extent_table,
     int *number_of_extents,
     libcerror_error_t **error );

int libvmdk_extent_table_get_extent_by_index(
     libvmdk_extent_table_t *extent_table,
     int extent_index,
     int *file_io_pool_entry,
     size64_t *extent_file_size,
     libcerror_error_t **error );

int libvmdk_extent_table_get_extent_at_offset(
     libvmdk_extent_table_t *extent_table,
     off64_t offset,
     int *file_io_pool_entry,
     size64_t *extent_file_size,
     libcerror_error_t **error );

int libvmdk_extent_table_set_extent_storage_media_size_by_index(
     libvmdk_extent_table_t *extent_table,
     int extent_index,
     size64_t storage_media_size,
     libcerror_error_t **error );

int libvmdk_extent_table_get_extent_file_by_index(
     libvmdk_extent_table_t *extent_table,
     int extent_index,
     libbfio_pool_t *file_io_pool,
     libvmdk_extent_file_t **extent_file,
     libcerror_error_t **error );

int libvmdk_extent_table_get_extent_file_at_offset(
     libvmdk_extent_table_t *extent_table,
     off64_t offset,
     libbfio_pool_t *file_io_pool,
     int *extent_index,
     off64_t *extent_file_data_offset,
     libvmdk_extent_file_t **extent_file,
     libcerror_error_t **error );

int libvmdk_extent_table_set_extent_file_by_index(
     libvmdk_extent_table_t *extent_table,
     libbfio_pool_t *file_io_pool,
     int extent_index,
     libvmdk_extent_file_t *extent_file,
     libcerror_error_t **error );

int libvmdk_extent_table_set_extent_by_extent_descriptor(
     libvmdk_extent_table_t *extent_table,
     libvmdk_internal_extent_descriptor_t *extent_descriptor,
     int extent_index,
     int file_io_pool_entry,
     size64_t extent_file_size,
     off64_t extent_file_offset,
     size64_t extent_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBVMDK_EXTENT_TABLE_H ) */

