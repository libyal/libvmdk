/*
 * Grain table functions
 *
 * Copyright (c) 2009-2013, Joachim Metz <joachim.metz@gmail.com>
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libvmdk_definitions.h"
#include "libvmdk_extent_table.h"
#include "libvmdk_grain_data.h"
#include "libvmdk_grain_table.h"
#include "libvmdk_io_handle.h"
#include "libvmdk_libbfio.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libcnotify.h"
#include "libvmdk_libfcache.h"
#include "libvmdk_libfdata.h"
#include "libvmdk_unused.h"

/* Creates a grain table
 * Make sure the value grain_table is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libvmdk_grain_table_initialize(
     libvmdk_grain_table_t **grain_table,
     libvmdk_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function    = "libvmdk_grain_table_initialize";
	size_t grain_offset_size = 0;

	if( grain_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain table.",
		 function );

		return( -1 );
	}
	if( *grain_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid grain table value already set.",
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
	*grain_table = memory_allocate_structure(
	                libvmdk_grain_table_t );

	if( *grain_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create grain table.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *grain_table,
	     0,
	     sizeof( libvmdk_grain_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear grain table.",
		 function );

		goto on_error;
	}
	( *grain_table )->io_handle = io_handle;

	return( 1 );

on_error:
	if( *grain_table != NULL )
	{
		memory_free(
		 *grain_table );

		*grain_table = NULL;
	}
	return( -1 );
}

/* Frees a grain table
 * Returns 1 if successful or -1 on error
 */
int libvmdk_grain_table_free(
     libvmdk_grain_table_t **grain_table,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_grain_table_free";

	if( grain_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain table.",
		 function );

		return( -1 );
	}
	if( *grain_table != NULL )
	{
		/* The IO handle reference is freed elsewhere
		 */
		memory_free(
		 *grain_table );

		*grain_table = NULL;
	}
	return( 1 );
}

/* Clones the grain table
 * Returns 1 if successful or -1 on error
 */
int libvmdk_grain_table_clone(
     libvmdk_grain_table_t **destination_grain_table,
     libvmdk_grain_table_t *source_grain_table,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_grain_table_clone";

	if( destination_grain_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination grain table.",
		 function );

		return( -1 );
	}
	if( *destination_grain_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination grain table value already set.",
		 function );

		return( -1 );
	}
	if( source_grain_table == NULL )
	{
		*destination_grain_table = NULL;

		return( 1 );
	}
	*destination_grain_table = memory_allocate_structure(
	                            libvmdk_grain_table_t );

	if( *destination_grain_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination grain table.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_grain_table,
	     source_grain_table,
	     sizeof( libvmdk_grain_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination grain table.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_grain_table != NULL )
	{
		memory_free(
		 *destination_grain_table );

		*destination_grain_table = NULL;
	}
	return( -1 );
}

/* Reads a grain
 * Callback function for the grain table list
 * Returns 1 if successful or -1 on error
 */
int libvmdk_grain_table_read_grain(
     intptr_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libfdata_list_element_t *list_element,
     libfcache_cache_t *cache,
     int file_io_pool_entry,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint32_t element_data_flags,
     uint8_t read_flags LIBVMDK_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libvmdk_grain_data_t *grain_data   = NULL;
	libvmdk_grain_table_t *grain_table = NULL;
	static char *function              = "libvmdk_grain_table_read_grain";
	ssize_t read_count                 = 0;
	int element_index                  = 0;

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
	grain_table = (libvmdk_grain_table_t *) io_handle;

	if( grain_table->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid grain table - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( element_data_size == (size64_t) 0 )
	 || ( element_data_size > (size64_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid element data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( element_data_flags & LIBVMDK_RANGE_FLAG_IS_COMPRESSED ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported element data flags.",
		 function );

		return( -1 );
	}
	if( libvmdk_grain_data_initialize(
	     &grain_data,
	     (size_t) element_data_size,
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
	if( ( element_data_flags & LIBVMDK_RANGE_FLAG_IS_SPARSE ) != 0 )
	{
		if( grain_table->io_handle->parent_handle == NULL )
		{
			if( memory_set(
			     grain_data->data,
			     0,
			     (size_t) element_data_size ) == NULL )
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
			     list_element,
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
			element_data_offset = element_index * element_data_size;

			if( libvmdk_handle_seek_offset(
			     grain_table->io_handle->parent_handle,
			     element_data_offset,
			     SEEK_SET,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_SEEK_FAILED,
				 "%s: unable to seek grain offset: %" PRIi64 " in parent.",
				 function,
				 element_data_offset );

				goto on_error;
			}
			read_count = libvmdk_handle_read_buffer(
				      grain_table->io_handle->parent_handle,
				      grain_data->data,
				      (size_t) element_data_size,
				      error );

			if( read_count != (ssize_t) element_data_size )
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
	}
	else
	{
		if( libbfio_pool_seek_offset(
		     file_io_pool,
		     file_io_pool_entry,
		     element_data_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek grain offset: %" PRIi64 " in file IO pool entry: %d.",
			 function,
			 element_data_offset,
			 file_io_pool_entry );

			goto on_error;
		}
		read_count = libbfio_pool_read_buffer(
			      file_io_pool,
			      file_io_pool_entry,
			      grain_data->data,
			      (size_t) element_data_size,
			      error );

		if( read_count != (ssize_t) element_data_size )
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
	}
	if( libfdata_list_element_set_element_value(
	     list_element,
	     cache,
	     (intptr_t *) grain_data,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libvmdk_grain_data_free,
	     LIBMFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
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

/* Reads a grain table for the grain offsets
 * Callback function for the grain table list
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libvmdk_grain_table_read_offsets(
     intptr_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libfdata_list_t *grain_table_list,
     int element_index,
     int number_of_elements,
     libfcache_cache_t *cache LIBVMDK_ATTRIBUTE_UNUSED,
     int file_io_pool_entry,
     off64_t element_group_offset,
     size64_t element_group_size,
     uint32_t element_group_flags,
     uint8_t read_flags,
     libcerror_error_t **error )
{
	libvmdk_grain_table_t *grain_table = NULL;
	uint8_t *grain_table_data          = NULL;
	static char *function              = "libvmdk_grain_table_read_offsets";
	ssize_t read_count                 = 0;
	int result                         = 0;

	LIBVMDK_UNREFERENCED_PARAMETER( cache )

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
	grain_table = (libvmdk_grain_table_t *) io_handle;

	if( grain_table->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid grain table - missing IO handle.",
		 function );

		return( -1 );
	}
	if( element_group_size > (size64_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid element group size value exceeds maximum.",
		 function );

		goto on_error;
	}
	grain_table_data = (uint8_t *) memory_allocate(
	                                sizeof( uint8_t ) * (size_t) element_group_size );

	if( grain_table_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create grain table data.",
		 function );

		goto on_error;
	}
	if( ( element_group_flags & LIBVMDK_RANGE_FLAG_IS_SPARSE ) != 0 )
	{
		if( memory_set(
		     grain_table_data,
		     0,
		     (size_t) element_group_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear grain table data.",
			 function );

			goto on_error;
		}
	}
	else
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: reading grain table at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
			 function,
			 element_group_offset,
			 element_group_offset );
		}
#endif
		if( libbfio_pool_seek_offset(
		     file_io_pool,
		     file_io_pool_entry,
		     element_group_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek grain table offset: %" PRIi64 ".",
			 function,
			 element_group_offset );

			goto on_error;
		}
		read_count = libbfio_pool_read_buffer(
			      file_io_pool,
			      file_io_pool_entry,
			      grain_table_data,
			      (size_t) element_group_size,
			      error );

		if( read_count != (ssize_t) element_group_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read grain table data.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: grain table data:\n",
		 function );
		libcnotify_print_data(
		 grain_table_data,
		 (size_t) element_group_size,
		 0 );
	}
#endif
	if( ( read_flags & LIBMFDATA_READ_FLAG_IS_BACKUP_RANGE ) == 0 )
	{
		if( libvmdk_grain_table_fill(
		     grain_table,
		     grain_table_list,
		     element_index,
		     file_io_pool_entry,
		     grain_table_data,
		     (size_t) element_group_size,
		     number_of_elements,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to fill grain table.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libvmdk_grain_table_correct(
		     grain_table,
		     grain_table_list,
		     element_index,
		     file_io_pool_entry,
		     grain_table_data,
		     (size_t) element_group_size,
		     number_of_elements,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to correct grain table.",
			 function );

			goto on_error;
		}
	}
/* TODO check if remainder of sector block is emtpy */

	memory_free(
	 grain_table_data );

	grain_table_data = NULL;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );

on_error:
	if( grain_table_data != NULL )
	{
		memory_free(
		 grain_table_data );
	}
	return( -1 );
}

/* Fills the grain offset from the grain table
 * Returns 1 if successful or -1 on error
 */
int libvmdk_grain_table_fill(
     libvmdk_grain_table_t *grain_table,
     libfdata_list_t *grain_table_list,
     int grain_index,
     int file_io_pool_entry,
     const uint8_t *grain_table_data,
     size_t grain_table_data_size,
     int number_of_grain_table_entries,
     libcerror_error_t **error )
{
	const uint8_t *grain_table_entry = NULL;
	static char *function            = "libvmdk_grain_table_fill";
	off64_t grain_data_offset        = 0;
	uint32_t range_flags             = 0;
	int grain_table_entry_index      = 0;
	int result                       = 0;

	if( grain_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain table.",
		 function );

		return( -1 );
	}
	if( grain_table->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid grain table - missing IO handle.",
		 function );

		return( -1 );
	}
	if( grain_table->io_handle->grain_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid grain table - invalid IO handle - missing grain size.",
		 function );

		return( -1 );
	}
	if( grain_table_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain table data.",
		 function );

		return( -1 );
	}
	if( grain_table_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid grain table size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( (size_t) number_of_grain_table_entries * 4 ) > grain_table_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of grain table entries value out of bounds.",
		 function );

		return( -1 );
	}
	grain_table_entry = grain_table_data;

	for( grain_table_entry_index = 0;
	     grain_table_entry_index < number_of_grain_table_entries;
	     grain_table_entry_index++ )
	{
		byte_stream_copy_to_uint32_little_endian(
		 grain_table_entry,
		 grain_data_offset );

#if defined( HAVE_VERBOSE_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: grain: %d data sector number: %" PRIi64 " (offset: 0x%08" PRIx64 " and size: %" PRIu32 ") file IO pool entry: %d\n",
			 function,
			 grain_index,
			 grain_data_offset,
			 grain_data_offset * 512,
			 grain_table->io_handle->grain_size,
			 file_io_pool_entry );
		}
#endif
		if( grain_data_offset == 0 )
		{
			range_flags = LIBVMDK_RANGE_FLAG_IS_SPARSE;
		}
		else
		{
			grain_data_offset *= 512;
			range_flags        = 0;
		}
		result = libfdata_list_is_group(
		          grain_table_list,
		          grain_index,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if grain: %d is a group.",
			 function,
			 grain_index );

			return( -1 );
		}
		else if( result != 0 )
		{
			if( libfdata_list_set_element_by_index(
			     grain_table_list,
			     grain_index,
			     file_io_pool_entry,
			     grain_data_offset,
			     (size64_t) grain_table->io_handle->grain_size,
			     range_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set grain: %d.",
				 function,
				 grain_index );

				return( -1 );
			}
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: unable to set data range of grain: %d value already set.",
			 function,
			 grain_index );

			return( -1 );
		}
		grain_table_entry += sizeof( uint32_t );

		grain_index++;
	}
	return( 1 );
}

/* Compares the grain offset with the ones in the grain table and makes corrections if necessary
 * Returns 1 if successful or -1 on error
 */
int libvmdk_grain_table_correct(
     libvmdk_grain_table_t *grain_table,
     libfdata_list_t *grain_table_list,
     int grain_index,
     int file_io_pool_entry,
     const uint8_t *grain_table_data,
     size_t grain_table_data_size,
     int number_of_grain_table_entries,
     libcerror_error_t **error )
{
	const uint8_t *grain_table_entry = NULL;
	static char *function            = "libvmdk_grain_table_correct";
	off64_t grain_data_offset        = 0;
	uint32_t range_flags             = 0;
	int grain_table_entry_index      = 0;
	int result                       = 0;

	if( grain_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain table.",
		 function );

		return( -1 );
	}
	if( grain_table->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid grain table - missing IO handle.",
		 function );

		return( -1 );
	}
	if( grain_table->io_handle->grain_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid grain table - invalid IO handle - missing grain size.",
		 function );

		return( -1 );
	}
	if( grain_table_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain table data.",
		 function );

		return( -1 );
	}
	if( grain_table_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid grain table size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( (size_t) number_of_grain_table_entries * 4 ) > grain_table_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of grain table entries value out of bounds.",
		 function );

		return( -1 );
	}
	grain_table_entry = grain_table_data;

	for( grain_table_entry_index = 0;
	     grain_table_entry_index < number_of_grain_table_entries;
	     grain_table_entry_index++ )
	{
		byte_stream_copy_to_uint32_little_endian(
		 grain_table_entry,
		 grain_data_offset );

#if defined( HAVE_VERBOSE_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: grain: %d data sector number: %" PRIi64 " (offset: 0x%08" PRIx64 " and size: %" PRIu32 ") file IO pool entry: %d\n",
			 function,
			 grain_index,
			 grain_data_offset,
			 grain_data_offset * 512,
			 grain_table->io_handle->grain_size,
			 file_io_pool_entry );
		}
#endif
		if( grain_data_offset == 0 )
		{
			range_flags = LIBVMDK_RANGE_FLAG_IS_SPARSE;
		}
		else
		{
			grain_data_offset *= 512;
			range_flags        = 0;
		}
		result = libfdata_list_is_group(
		          grain_table_list,
		          grain_index,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if grain: %d is a group.",
			 function,
			 grain_index );

			return( -1 );
		}
		else if( result != 0 )
		{
			if( libfdata_list_set_element_by_index(
			     grain_table_list,
			     grain_index,
			     file_io_pool_entry,
			     grain_data_offset,
			     (size64_t) grain_table->io_handle->grain_size,
			     range_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set grain: %d.",
				 function,
				 grain_index );

				return( -1 );
			}
		}
		else
		{
/* TODO */
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: unable to set data range of grain: %d value already set.",
			 function,
			 grain_index );

			return( -1 );
		}
		grain_table_entry += sizeof( uint32_t );

		grain_index++;
	}
	return( 1 );
}

/* TODO refactor */

/* Retrieves the grains group in a extent file at a specific offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libvmdk_grain_table_get_extent_file_grain_group_by_offset(
     libvmdk_grain_table_t *grain_table,
     libbfio_pool_t *file_io_pool,
     libvmdk_extent_table_t *extent_table,
     off64_t offset,
     uint32_t *extent_number,
     off64_t *extent_file_data_offset,
     libvmdk_extent_file_t **extent_file,
     int *grain_groups_list_index,
     off64_t *grain_group_data_offset,
     libfdata_list_t **grains_list,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_grain_table_get_extent_file_grain_group_by_offset";
	int result            = 0;

	if( grain_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain table.",
		 function );

		return( -1 );
	}
	if( extent_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent number.",
		 function );

		return( -1 );
	}
	if( extent_file_data_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent file data offset.",
		 function );

		return( -1 );
	}
	if( extent_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent file.",
		 function );

		return( -1 );
	}
	if( grain_groups_list_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain groups list index.",
		 function );

		return( -1 );
	}
	if( grain_group_data_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain group data offset.",
		 function );

		return( -1 );
	}
	if( grains_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grains list.",
		 function );

		return( -1 );
	}
	result = libvmdk_extent_table_get_extent_file_at_offset(
	          extent_table,
	          offset,
	          file_io_pool,
	          extent_number,
	          extent_file_data_offset,
	          extent_file,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve extent file at offset: %" PRIi64 " from extent files list.",
		 function,
		 offset );

		return( -1 );
	}
	if( result != 0 )
	{
		result = libvmdk_extent_file_get_grain_group_by_offset(
			  *extent_file,
			  file_io_pool,
			  *extent_file_data_offset,
			  grain_groups_list_index,
			  grain_group_data_offset,
			  grains_list,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve grain group from extent file: %" PRIu32 " at offset: %" PRIi64 ".",
			 function,
			 *extent_number,
			 *extent_file_data_offset );

			return( -1 );
		}
	}
	if( result != 0 )
	{
		if( *grains_list == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing grains list: %d.",
			 function,
			 *grain_groups_list_index );

			return( -1 );
		}
	}
/* TODO handle corrupt grains groups */
	return( result );
}

/* Retrieves the grain data of a grain at a specific offset
 * Returns 1 if successful or -1 on error
 */
int libvmdk_grain_table_get_grain_data_by_offset(
     libvmdk_grain_table_t *grain_table,
     uint64_t grain_index,
     libbfio_pool_t *file_io_pool,
     libvmdk_extent_table_t *extent_table,
     libfcache_cache_t *grains_cache,
     off64_t offset,
     libvmdk_grain_data_t **grain_data,
     off64_t *grain_data_offset,
     libcerror_error_t **error )
{
	libvmdk_extent_file_t *extent_file = NULL;
	libfdata_list_t *grains_list       = NULL;
	static char *function              = "libvmdk_grain_table_get_grain_data_by_offset";
	off64_t grain_offset               = 0;
	off64_t grain_group_data_offset    = 0;
	off64_t extent_file_data_offset    = 0;
	size_t grain_data_size             = 0;
	uint64_t start_sector              = 0;
	uint64_t number_of_sectors         = 0;
	uint32_t extent_number             = 0;
	int grain_groups_list_index        = 0;
	int grains_list_index              = 0;
	int result                         = 0;

	if( grain_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain table.",
		 function );

		return( -1 );
	}
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
	if( grain_data_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain data offset.",
		 function );

		return( -1 );
	}
	result = libvmdk_grain_table_get_extent_file_grain_group_by_offset(
		  grain_table,
		  file_io_pool,
		  extent_table,
		  offset,
		  &extent_number,
		  &extent_file_data_offset,
		  &extent_file,
		  &grain_groups_list_index,
		  &grain_group_data_offset,
		  &grains_list,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve extent file grain group at offset: %" PRIi64 ".",
		 function,
		 offset );

		return( -1 );
	}
	if( result != 0 )
	{
		if( grains_list == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing grains list: %d.",
			 function,
			 grain_groups_list_index );

			return( -1 );
		}
		result = libfdata_list_get_element_value_at_offset(
			  grains_list,
			  (intptr_t *) file_io_pool,
			  grains_cache,
			  grain_group_data_offset,
			  &grains_list_index,
			  grain_data_offset,
			  (intptr_t **) grain_data,
			  0,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve grain: %" PRIu64 " data from grain group: %d in extent file: %" PRIu32 " at offset: %" PRIi64 ".",
			 function,
			 grain_index,
			 grain_groups_list_index,
			 extent_number,
			 extent_file_data_offset );

			return( -1 );
		}
	}
/* TODO handle corrupt grains */
	return( 1 );
}

