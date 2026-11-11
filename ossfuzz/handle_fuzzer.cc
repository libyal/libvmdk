/*
 * OSS-Fuzz target for libvmdk handle type
 *
 * Copyright (C) 2009-2026, Joachim Metz <joachim.metz@gmail.com>
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

#include <stddef.h>
#include <stdint.h>

/* Note that some of the OSS-Fuzz engines use C++
 */
extern "C" {

#include "ossfuzz_libbfio.h"
#include "ossfuzz_libvmdk.h"

#if !defined( LIBVMDK_HAVE_BFIO )

/* Opens a handle using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_open_file_io_handle(
      libvmdk_handle_t *handle,
      libbfio_handle_t *file_io_handle,
      int access_flags,
      libvmdk_error_t **error );

/* Opens the extent data files using a Basic File IO (bfio) pool
 * This function assumes the extent data files are in the same order as defined by the descriptor file
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_open_extent_data_files_file_io_pool(
      libvmdk_handle_t *handle,
      libbfio_pool_t *file_io_pool,
      libvmdk_error_t **error );

#endif /* !defined( LIBVMDK_HAVE_BFIO ) */

int LLVMFuzzerTestOneInput(
      const uint8_t *data,
      size_t size )
{
	uint8_t buffer[ 512 ];
	uint8_t utf8_string[ 64 ];
	uint16_t utf16_string[ 64 ];

	libbfio_handle_t *file_io_handle = NULL;
	libbfio_pool_t *file_io_pool     = NULL;
	libvmdk_handle_t *handle         = NULL;
	off64_t media_offset             = 0;
	size64_t media_size              = 0;
	size_t string_size               = 0;
	ssize_t read_count               = 0;
	uint32_t value_32bit             = 0;
	int disk_type                    = 0;
	int entry_index                  = 0;
	int number_of_extents            = 0;
	int read_iterator                = 0;

	if( libbfio_memory_range_initialize(
	     &file_io_handle,
	     NULL ) != 1 )
	{
		return( 0 );
	}
	if( libbfio_memory_range_set(
	     file_io_handle,
	     (uint8_t *) data,
	     size,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libbfio_pool_initialize(
	     &file_io_pool,
	     0,
	     0,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libvmdk_handle_initialize(
	     &handle,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libvmdk_handle_open_file_io_handle(
	     handle,
	     file_io_handle,
	     LIBVMDK_OPEN_READ,
	     NULL ) != 1 )
	{
		goto on_error_libvmdk;
	}
	if( libbfio_pool_append_handle(
	     file_io_pool,
	     &entry_index,
	     file_io_handle,
	     LIBBFIO_OPEN_READ,
	     NULL ) != 1 )
	{
		goto on_error_libvmdk;
	}
	/* The file IO pool takes over management of the file IO handle
	 */
	file_io_handle = NULL;

	/* Note that the extent data files can only be opened if the number
	 * of file IO handles in the pool matches the number of extents
	 * defined in the descriptor file.
	 */
	libvmdk_handle_open_extent_data_files_file_io_pool(
	 handle,
	 file_io_pool,
	 NULL );

	if( libvmdk_handle_set_maximum_number_of_open_handles(
	     handle,
	     8,
	     NULL ) != 1 )
	{
		goto on_error_libvmdk;
	}
	if( libvmdk_handle_get_disk_type(
	     handle,
	     &disk_type,
	     NULL ) != 1 )
	{
		goto on_error_libvmdk;
	}
	if( libvmdk_handle_get_media_size(
	     handle,
	     &media_size,
	     NULL ) != 1 )
	{
		goto on_error_libvmdk;
	}
	if( libvmdk_handle_get_content_identifier(
	     handle,
	     &value_32bit,
	     NULL ) != 1 )
	{
		goto on_error_libvmdk;
	}
	if( libvmdk_handle_get_parent_content_identifier(
	     handle,
	     &value_32bit,
	     NULL ) == -1 )
	{
		goto on_error_libvmdk;
	}
	if( libvmdk_handle_get_utf8_parent_filename_size(
	     handle,
	     &string_size,
	     NULL ) == -1 )
	{
		goto on_error_libvmdk;
	}
	if( libvmdk_handle_get_utf8_parent_filename(
	     handle,
	     utf8_string,
	     64,
	     NULL ) == -1 )
	{
		goto on_error_libvmdk;
	}
	if( libvmdk_handle_get_utf16_parent_filename_size(
	     handle,
	     &string_size,
	     NULL ) == -1 )
	{
		goto on_error_libvmdk;
	}
	if( libvmdk_handle_get_utf16_parent_filename(
	     handle,
	     utf16_string,
	     64,
	     NULL ) == -1 )
	{
		goto on_error_libvmdk;
	}
	if( libvmdk_handle_get_number_of_extents(
	     handle,
	     &number_of_extents,
	     NULL ) != 1 )
	{
		goto on_error_libvmdk;
	}
	if( libvmdk_handle_seek_offset(
	     handle,
	     0,
	     SEEK_SET,
	     NULL ) == -1 )
	{
		goto on_error_libvmdk;
	}
	for( read_iterator = 0;
	     read_iterator < 128;
	     read_iterator++ )
	{
		if( media_offset >= media_size )
		{
			break;
		}
		read_count = libvmdk_handle_read_buffer(
		              handle,
		              buffer,
		              497,
		              NULL );

		if( read_count == -1 )
		{
			goto on_error_libvmdk;
		}
		if( read_count == 0 )
		{
			break;
		}
		media_offset += 497;
	}
	if( media_size > 0 )
	{
		if( libvmdk_handle_seek_offset(
		     handle,
		     (off64_t) ( media_size / 2 ),
		     SEEK_SET,
		     NULL ) == -1 )
		{
			goto on_error_libvmdk;
		}
	}
	if( libvmdk_handle_seek_offset(
	     handle,
	     512,
	     SEEK_CUR,
	     NULL ) == -1 )
	{
		goto on_error_libvmdk;
	}
	if( libvmdk_handle_seek_offset(
	     handle,
	     0,
	     SEEK_END,
	     NULL ) == -1 )
	{
		goto on_error_libvmdk;
	}
	if( libvmdk_handle_get_offset(
	     handle,
	     &media_offset,
	     NULL ) != 1 )
	{
		goto on_error_libvmdk;
	}
	if( libvmdk_handle_seek_offset(
	     handle,
	     0,
	     SEEK_SET,
	     NULL ) == -1 )
	{
		goto on_error_libvmdk;
	}
	media_offset = 0;

	for( read_iterator = 0;
	     read_iterator < 128;
	     read_iterator++ )
	{
		if( media_offset >= media_size )
		{
			break;
		}
		if( libvmdk_handle_read_buffer_at_offset(
		     handle,
		     buffer,
		     497,
		     media_offset,
		     NULL ) == -1 )
		{
			goto on_error_libvmdk;
		}
		media_offset += 497;
	}
	if( libvmdk_handle_signal_abort(
	     handle,
	     NULL ) != 1 )
	{
		goto on_error_libvmdk;
	}
	libvmdk_handle_close(
	 handle,
	 NULL );

on_error_libvmdk:
	libvmdk_handle_free(
	 &handle,
	 NULL );

on_error_libbfio:
	/* Note that on error the handle still has a reference to file_io_pool
	 * that will be closed. Therefore the file IO pool and handle need to
	 * be freed after closing or freeing the handle.
	 */
	if( file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &file_io_pool,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( 0 );
}

} /* extern "C" */
