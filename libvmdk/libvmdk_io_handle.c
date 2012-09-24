/*
 * Input/Output (IO) handle
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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libvmdk_debug.h"
#include "libvmdk_definitions.h"
#include "libvmdk_io_handle.h"
#include "libvmdk_libbfio.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libcnotify.h"

/* Initialize an io handle
 * Make sure the value io_handle is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libvmdk_io_handle_initialize(
     libvmdk_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_io_handle_initialize";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid io handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle value already set.",
		 function );

		return( -1 );
	}
	*io_handle = memory_allocate_structure(
	              libvmdk_io_handle_t );

	if( *io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create io handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *io_handle,
	     0,
	     sizeof( libvmdk_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( -1 );
}

/* Frees an IO handle
 * Returns 1 if successful or -1 on error
 */
int libvmdk_io_handle_free(
     libvmdk_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_io_handle_free";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid io handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( 1 );
}

#ifdef TODO

/* Reads a grain table
 * Returns 1 if successful or -1 on error
 */
int libvmdk_io_handle_read_grain_table(
     libvmdk_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     libvmdk_offset_table_t *offset_table,
     off64_t grain_table_offset,
     uint32_t number_of_grain_table_entries,
     size64_t grain_size,
     uint8_t is_secondary_grain_directory,
     libcerror_error_t **error )
{
	uint8_t *sector_blocks_data    = NULL;
	static char *function          = "libvmdk_io_handle_read_grain_table";
	size_t grain_table_data_size   = 0;
	size_t sector_blocks_data_size = 0;
	ssize_t read_count             = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid io handle.",
		 function );

		return( -1 );
	}
	if( offset_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
		 function );

		return( -1 );
	}
	grain_table_data_size = number_of_grain_table_entries * sizeof( uint32_t );

	if( grain_table_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid grain table size value exceeds maximum.",
		 function );

		goto on_error;
	}
	sector_blocks_data_size = grain_table_data_size / 512;

	if( ( grain_table_data_size % 512 ) != 0 )
	{
		sector_blocks_data_size += 1;
	}
	sector_blocks_data_size *= 512;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading grain table at offset: %" PRIu64 " (0x%08" PRIx64 ")\n",
		 function,
		 grain_table_offset,
		 grain_table_offset );
	}
#endif
	if( libbfio_pool_seek_offset(
	     file_io_pool,
	     file_io_pool_entry,
	     grain_table_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek grain table offset: %" PRIu64 ".",
		 function,
		 grain_table_offset );

		goto on_error;
	}
	sector_blocks_data = (uint8_t *) memory_allocate(
	                                  sizeof( uint8_t ) * sector_blocks_data_size );

	if( sector_blocks_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create sector blocks data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              sector_blocks_data,
	              sector_blocks_data_size,
	              error );

	if( read_count != (ssize_t) sector_blocks_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read grain table data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: grain table data:\n",
		 function );
		libcnotify_print_data(
		 sector_blocks_data,
		 sector_blocks_data_size,
		 0 );
	}
#endif
	if( is_secondary_grain_directory ==  0 )
	{
		if( libvmdk_offset_table_fill(
		     offset_table,
		     sector_blocks_data,
		     grain_table_data_size,
		     number_of_grain_table_entries,
		     grain_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to fill offset table.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libvmdk_offset_table_compare(
		     offset_table,
		     sector_blocks_data,
		     grain_table_data_size,
		     number_of_grain_table_entries,
		     grain_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to compare offset table.",
			 function );

			goto on_error;
		}
	}
/* TODO check if remainder of sector block is emtpy */

	memory_free(
	 sector_blocks_data );

	sector_blocks_data = NULL;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );

on_error:
	if( sector_blocks_data != NULL )
	{
		memory_free(
		 sector_blocks_data );
	}
	return( -1 );
}

#endif

