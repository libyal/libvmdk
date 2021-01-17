/*
 * Grain data functions
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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libvmdk_compression.h"
#include "libvmdk_definitions.h"
#include "libvmdk_grain_data.h"
#include "libvmdk_handle.h"
#include "libvmdk_io_handle.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libcnotify.h"
#include "libvmdk_libfdata.h"
#include "libvmdk_unused.h"

/* Creates grain data
 * Make sure the value grain_data is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libvmdk_grain_data_initialize(
     libvmdk_grain_data_t **grain_data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_grain_data_initialize";

	if( grain_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain data.",
		 function );

		return( -1 );
	}
	if( *grain_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid grain data value already set.",
		 function );

		return( -1 );
	}
	if( ( data_size == 0 )
	 || ( data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	*grain_data = memory_allocate_structure(
	               libvmdk_grain_data_t );

	if( *grain_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create grain data.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *grain_data,
	     0,
	     sizeof( libvmdk_grain_data_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear grain data.",
		 function );

		goto on_error;
	}
	( *grain_data )->data = (uint8_t *) memory_allocate(
	                                     sizeof( uint8_t ) * data_size );

	if( ( *grain_data )->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data.",
		 function );

		goto on_error;
	}
	( *grain_data )->data_size = data_size;

	return( 1 );

on_error:
	if( *grain_data != NULL )
	{
		memory_free(
		 *grain_data );

		*grain_data = NULL;
	}
	return( -1 );
}

/* Frees grain data
 * Returns 1 if successful or -1 on error
 */
int libvmdk_grain_data_free(
     libvmdk_grain_data_t **grain_data,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_grain_data_free";

	if( grain_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain data.",
		 function );

		return( -1 );
	}
	if( *grain_data != NULL )
	{
		if( ( *grain_data )->data != NULL )
		{
			memory_free(
			 ( *grain_data )->data );
		}
		memory_free(
		 *grain_data );

		*grain_data = NULL;
	}
	return( 1 );
}

/* Reads a compressed grain data header
 * The number of bytes read or -1 on error
 */
ssize_t libvmdk_grain_data_read_compressed_header(
         libvmdk_grain_data_t *grain_data,
         libvmdk_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libcerror_error_t **error )
{
	uint8_t compressed_data_header[ 12 ];

	static char *function = "libvmdk_grain_data_read_compressed_header";
	ssize_t read_count    = 0;

	if( grain_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain data.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	read_count = libbfio_pool_read_buffer(
		      file_io_pool,
		      file_io_pool_entry,
		      compressed_data_header,
		      12,
		      error );

	if( read_count != (ssize_t) 12 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read compressed grain data header.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: compressed grain data header:\n",
		 function );
		libcnotify_print_data(
		 compressed_data_header,
		 12,
		 0 );
	}
#endif
	byte_stream_copy_to_uint64_little_endian(
	 compressed_data_header,
	 grain_data->uncompressed_data_offset );

	byte_stream_copy_to_uint32_little_endian(
	 &( compressed_data_header[ 8 ] ),
	 grain_data->compressed_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: uncompressed data offset\t: %" PRIu64 " grains (0x%08" PRIx64 ")\n",
		 function,
		 grain_data->uncompressed_data_offset,
		 grain_data->uncompressed_data_offset * io_handle->grain_size );

		libcnotify_printf(
		 "%s: compressed data size\t\t: %" PRIu32 "\n",
		 function,
		 grain_data->compressed_data_size );

		libcnotify_printf(
		 "\n" );
	}
#endif
	grain_data->uncompressed_data_offset *= io_handle->grain_size;

	return( read_count );
}

/* Reads a grain
 * Callback function for the grains list
 * Returns 1 if successful or -1 on error
 */
int libvmdk_grain_data_read_element_data(
     libvmdk_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libfdata_list_element_t *element,
     libfdata_cache_t *cache,
     int file_io_pool_entry,
     off64_t grain_data_offset,
     size64_t grain_data_size,
     uint32_t grain_data_flags,
     uint8_t read_flags LIBVMDK_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libvmdk_grain_data_t *grain_data = NULL;
	uint8_t *compressed_data         = NULL;
	static char *function            = "libvmdk_grain_data_read_element_data";
	ssize_t read_count               = 0;

	LIBVMDK_UNREFERENCED_PARAMETER( read_flags )

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( ( grain_data_size == (size64_t) 0 )
	 || ( grain_data_size > (size64_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid grain data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( grain_data_flags & LIBVMDK_RANGE_FLAG_IS_SPARSE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: sparse grain not supported.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_seek_offset(
	     file_io_pool,
	     file_io_pool_entry,
	     grain_data_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek grain offset: %" PRIi64 " in file IO pool entry: %d.",
		 function,
		 grain_data_offset,
		 file_io_pool_entry );

		goto on_error;
	}
	if( ( grain_data_flags & LIBVMDK_RANGE_FLAG_IS_COMPRESSED ) != 0 )
	{
		if( io_handle->grain_size > (size64_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid IO handle - grain size value exceeds maximum.",
			 function );

			goto on_error;
		}
		if( libvmdk_grain_data_initialize(
		     &grain_data,
		     (size_t) io_handle->grain_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create grain data.",
			 function );

			goto on_error;
		}
		read_count = libvmdk_grain_data_read_compressed_header(
		              grain_data,
		              io_handle,
		              file_io_pool,
		              file_io_pool_entry,
		              error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read compressed grain data header.",
			 function );

			goto on_error;
		}
		if( ( grain_data->compressed_data_size == 0 )
		 || ( grain_data->compressed_data_size > (uint32_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid grain data - compressed data size value out of bounds.",
			 function );

			goto on_error;
		}
		compressed_data = (uint8_t *) memory_allocate(
		                               sizeof( uint8_t ) * (size_t) grain_data->compressed_data_size );

		if( compressed_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create compressed data.",
			 function );

			goto on_error;
		}
		read_count = libbfio_pool_read_buffer(
			      file_io_pool,
			      file_io_pool_entry,
			      compressed_data,
			      (size_t) grain_data->compressed_data_size,
			      error );

		if( read_count != (ssize_t) grain_data->compressed_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read compressed grain data.",
			 function );

			goto on_error;
		}
		if( libvmdk_decompress_data(
		     compressed_data,
		     (size_t) grain_data->compressed_data_size,
		     LIBVMDK_COMPRESSION_METHOD_DEFLATE,
		     grain_data->data,
		     &( grain_data->data_size ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
			 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
			 "%s: unable to decompress grain data.",
			 function );

			goto on_error;
		}
		memory_free(
		 compressed_data );

		compressed_data = NULL;
	}
	else
	{
		if( libvmdk_grain_data_initialize(
		     &grain_data,
		     (size_t) grain_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create grain data.",
			 function );

			goto on_error;
		}
		if( grain_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing grain data.",
			 function );

			goto on_error;
		}
		read_count = libbfio_pool_read_buffer(
			      file_io_pool,
			      file_io_pool_entry,
			      grain_data->data,
			      (size_t) grain_data_size,
			      error );

		if( read_count != (ssize_t) grain_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read grain data.",
			 function );

			goto on_error;
		}
	}
	if( libfdata_list_element_set_element_value(
	     element,
	     (intptr_t *) file_io_pool,
	     cache,
	     (intptr_t *) grain_data,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libvmdk_grain_data_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set grain data as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( compressed_data != NULL )
	{
		memory_free(
		 compressed_data );
	}
	if( grain_data != NULL )
	{
		libvmdk_grain_data_free(
		 &grain_data,
		 NULL );
	}
	return( -1 );
}

