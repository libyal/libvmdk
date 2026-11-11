/*
 * OSS-Fuzz target for libvmdk extent descriptor type
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

#endif /* !defined( LIBVMDK_HAVE_BFIO ) */

int LLVMFuzzerTestOneInput(
      const uint8_t *data,
      size_t size )
{
	uint8_t utf8_string[ 64 ];
	uint16_t utf16_string[ 64 ];

	libbfio_handle_t *file_io_handle            = NULL;
	libvmdk_handle_t *handle                    = NULL;
	libvmdk_extent_descriptor_t *extent_descriptor = NULL;
	off64_t extent_offset                       = 0;
	size64_t extent_size                        = 0;
	size_t string_size                          = 0;
	int extent_index                            = 0;
	int extent_type                             = 0;
	int number_of_extents                       = 0;

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
	if( libvmdk_handle_get_number_of_extents(
	     handle,
	     &number_of_extents,
	     NULL ) != 1 )
	{
		goto on_error_libvmdk;
	}
	for( extent_index = 0;
	     extent_index < number_of_extents;
	     extent_index++ )
	{
		if( libvmdk_handle_get_extent_descriptor(
		     handle,
		     extent_index,
		     &extent_descriptor,
		     NULL ) != 1 )
		{
			goto on_error_libvmdk;
		}
		if( libvmdk_extent_descriptor_get_type(
		     extent_descriptor,
		     &extent_type,
		     NULL ) != 1 )
		{
			goto on_error_extent_descriptor;
		}
		if( libvmdk_extent_descriptor_get_range(
		     extent_descriptor,
		     &extent_offset,
		     &extent_size,
		     NULL ) != 1 )
		{
			goto on_error_extent_descriptor;
		}
		if( libvmdk_extent_descriptor_get_utf8_filename_size(
		     extent_descriptor,
		     &string_size,
		     NULL ) == -1 )
		{
			goto on_error_extent_descriptor;
		}
		if( libvmdk_extent_descriptor_get_utf8_filename(
		     extent_descriptor,
		     utf8_string,
		     64,
		     NULL ) == -1 )
		{
			goto on_error_extent_descriptor;
		}
		if( libvmdk_extent_descriptor_get_utf16_filename_size(
		     extent_descriptor,
		     &string_size,
		     NULL ) == -1 )
		{
			goto on_error_extent_descriptor;
		}
		if( libvmdk_extent_descriptor_get_utf16_filename(
		     extent_descriptor,
		     utf16_string,
		     64,
		     NULL ) == -1 )
		{
			goto on_error_extent_descriptor;
		}
 on_error_extent_descriptor:
		libvmdk_extent_descriptor_free(
		 &extent_descriptor,
		 NULL );
	}
	libvmdk_handle_close(
	 handle,
	 NULL );

on_error_libvmdk:
	libvmdk_handle_free(
	 &handle,
	 NULL );

on_error_libbfio:
	libbfio_handle_free(
	 &file_io_handle,
	 NULL );

	return( 0 );
}

} /* extern "C" */
