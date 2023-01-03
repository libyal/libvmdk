/*
 * Extent file functions
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

#if !defined( _LIBVMDK_EXTENT_FILE_H )
#define _LIBVMDK_EXTENT_FILE_H

#include <common.h>
#include <types.h>

#include "libvmdk_io_handle.h"
#include "libvmdk_libbfio.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libfcache.h"
#include "libvmdk_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const char *cowd_sparse_file_signature;
extern const char *vmdk_sparse_file_signature;

typedef struct libvmdk_extent_file libvmdk_extent_file_t;

struct libvmdk_extent_file
{
	/* The IO handle
	 */
	libvmdk_io_handle_t *io_handle;

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
	size64_t maximum_data_size;

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

	/* The number of grain table entries
	 */
	uint32_t number_of_grain_table_entries;

	/* The grain table size
	 */
	size_t grain_table_size;

	/* The number of grain directory entries
	 */
	uint32_t number_of_grain_directory_entries;

	/* The grain directory size
	 */
	size_t grain_directory_size;

	/* The primary grain directory offset
	 */
	off64_t primary_grain_directory_offset;

	/* The secondary grain directory offset
	 */
	off64_t secondary_grain_directory_offset;

	/* Value to indicate the extent file is dirty
	 */
	uint8_t is_dirty;

	/* The grain groups list
	 */
	libfdata_list_t *grain_groups_list;

	/* The grain groups cache
	 */
	libfcache_cache_t *grain_groups_cache;

	/* The (current) grain groups index
	 */
	int grain_groups_index;

	/* The storage media size (in the extent file)
	 */
	size64_t storage_media_size;
};

int libvmdk_extent_file_initialize(
     libvmdk_extent_file_t **extent_file,
     libvmdk_io_handle_t *io_handle,
     libcerror_error_t **error );

int libvmdk_extent_file_free(
     libvmdk_extent_file_t **extent_file,
     libcerror_error_t **error );

int libvmdk_extent_file_check_for_empty_block(
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libvmdk_extent_file_read_file_header_file_io_handle(
     libvmdk_extent_file_t *extent_file,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libvmdk_extent_file_read_file_header(
     libvmdk_extent_file_t *extent_file,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     off64_t file_offset,
     libcerror_error_t **error );

int libvmdk_extent_file_read_file_header_data(
     libvmdk_extent_file_t *extent_file,
     const uint8_t *file_header_data,
     size_t file_header_data_size,
     libcerror_error_t **error );

int libvmdk_extent_file_read_descriptor_data_file_io_handle(
     libvmdk_extent_file_t *extent_file,
     libbfio_handle_t *file_io_handle,
     uint8_t *descriptor_data,
     size_t descriptor_data_size,
     libcerror_error_t **error );

int libvmdk_extent_file_read_grain_directories(
     libvmdk_extent_file_t *extent_file,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     libcerror_error_t **error );

int libvmdk_extent_file_read_grain_directory(
     libvmdk_extent_file_t *extent_file,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     off64_t file_offset,
     libcerror_error_t **error );

int libvmdk_extent_file_read_backup_grain_directory(
     libvmdk_extent_file_t *extent_file,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     off64_t file_offset,
     libcerror_error_t **error );

int libvmdk_extent_file_read_grains_scan(
     libvmdk_extent_file_t *extent_file,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     libcerror_error_t **error );

int libvmdk_extent_file_read_element_data(
     libvmdk_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libfdata_list_element_t *element,
     libfdata_cache_t *cache,
     int file_io_pool_entry,
     off64_t element_offset,
     size64_t extent_file_size,
     uint32_t element_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

int libvmdk_extent_file_read_grain_group_element_data(
     libvmdk_extent_file_t *extent_file,
     libbfio_pool_t *file_io_pool,
     libfdata_list_element_t *element,
     libfdata_cache_t *cache,
     int file_io_pool_entry,
     off64_t grain_group_data_offset,
     size64_t grain_group_data_size,
     uint32_t grain_group_data_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

int libvmdk_extent_file_grain_group_is_sparse_at_offset(
     libvmdk_extent_file_t *extent_file,
     off64_t offset,
     int *grain_group_index,
     off64_t *grain_group_data_offset,
     libcerror_error_t **error );

int libvmdk_extent_file_get_grain_group_at_offset(
     libvmdk_extent_file_t *extent_file,
     libbfio_pool_t *file_io_pool,
     off64_t offset,
     int *grain_group_index,
     off64_t *grain_group_data_offset,
     libfdata_list_t **grains_list,
     libcerror_error_t **error );

ssize_t libvmdk_extent_file_read_segment_data(
         intptr_t *data_handle,
         libbfio_pool_t *file_io_pool,
         int segment_index,
         int segment_file_index,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags,
         uint8_t read_flags,
         libcerror_error_t **error );

off64_t libvmdk_extent_file_seek_segment_offset(
         intptr_t *data_handle,
         libbfio_pool_t *file_io_pool,
         int segment_index,
         int segment_file_index,
         off64_t segment_offset,
         libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBVMDK_EXTENT_FILE_H ) */

