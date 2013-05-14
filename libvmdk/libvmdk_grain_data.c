/*
 * Grain data functions
 *
 * Copyright (c) 2006-2013, Joachim Metz <joachim.metz@gmail.com>
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libvmdk_definitions.h"
#include "libvmdk_grain_data.h"
#include "libvmdk_handle.h"
#include "libvmdk_io_handle.h"
#include "libvmdk_libcerror.h"
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
	if( data_size > (size_t) SSIZE_MAX )
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
	if( data_size > 0 )
	{
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
	}
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

/* Reads a grain
 * Callback function for the grains list
 * Returns 1 if successful or -1 on error
 */
int libvmdk_grain_data_read_element_data(
     libvmdk_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libfdata_list_element_t *element,
     libfcache_cache_t *cache,
     int file_io_pool_entry,
     off64_t grain_data_offset,
     size64_t grain_data_size,
     uint32_t grain_data_flags,
     uint8_t read_flags LIBVMDK_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libvmdk_grain_data_t *grain_data = NULL;
	static char *function            = "libvmdk_grain_data_read_element_data";
	ssize_t read_count               = 0;
	int element_index                = 0;

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
	if( ( grain_data_flags & LIBVMDK_RANGE_FLAG_IS_COMPRESSED ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: compressed grain not supported.",
		 function );

		goto on_error;
	}
	if( ( grain_data_flags & LIBVMDK_RANGE_FLAG_IS_SPARSE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: sparse grain not supported.",
		 function );

		goto on_error;
	}
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
/* TODO move
		if( io_handle->parent_handle == NULL )
		{
			if( memory_set(
			     grain_data->data,
			     0,
			     (size_t) grain_data_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear grain data.",
				 function );

				goto on_error;
			}
		}
		else
		{
			if( libfdata_list_element_get_element_index(
			     element,
			     &element_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to retrieve element index.",
				 function );

				goto on_error;
			}
			grain_data_offset = element_index * grain_data_size;

			if( libvmdk_handle_seek_offset(
			     io_handle->parent_handle,
			     grain_data_offset,
			     SEEK_SET,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_SEEK_FAILED,
				 "%s: unable to seek grain offset: %" PRIi64 " in parent.",
				 function,
				 grain_data_offset );

				goto on_error;
			}
			read_count = libvmdk_handle_read_buffer(
				      io_handle->parent_handle,
				      grain_data->data,
				      (size_t) grain_data_size,
				      error );

			if( read_count != (ssize_t) grain_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read grain data from parent.",
				 function );

				goto on_error;
			}
		}
*/
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
/* TODO handle compressed grains
	grain_data->data_size = (size_t) read_count;
*/
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
	if( grain_data != NULL )
	{
		libvmdk_grain_data_free(
		 &grain_data,
		 NULL );
	}
	return( -1 );
}

