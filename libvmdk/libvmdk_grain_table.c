/*
 * Grain table functions
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
	static char *function = "libvmdk_grain_table_initialize";

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

/* Determines if the grain at a specific offset is sparse
 * Returns 1 if the grain is sparse, 0 if not or -1 on error
 */
int libvmdk_grain_table_grain_is_sparse_at_offset(
     libvmdk_grain_table_t *grain_table,
     uint64_t grain_index,
     libbfio_pool_t *file_io_pool,
     libvmdk_extent_table_t *extent_table,
     off64_t offset,
     libcerror_error_t **error )
{
	libvmdk_extent_file_t *extent_file = NULL;
	libfdata_list_t *grains_list       = NULL;
	static char *function              = "libvmdk_grain_table_grain_is_sparse_at_offset";
	off64_t extent_file_data_offset    = 0;
	off64_t grain_data_offset          = 0;
	off64_t grain_group_data_offset    = 0;
	off64_t grain_offset               = 0;
	size64_t grain_size                = 0;
	uint32_t grain_flags               = 0;
	int extent_number                  = 0;
	int grain_file_index               = 0;
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
	result = libvmdk_extent_table_get_extent_file_at_offset(
	          extent_table,
	          offset,
	          file_io_pool,
	          &extent_number,
	          &extent_file_data_offset,
	          &extent_file,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve extent file at offset: %" PRIi64 " from extent table.",
		 function,
		 offset );

		return( -1 );
	}
	if( extent_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing extent file.",
		 function );

		return( -1 );
	}
	result = libvmdk_extent_file_grain_group_is_sparse_at_offset(
	          extent_file,
	          extent_file_data_offset,
	          &grain_groups_list_index,
	          &grain_group_data_offset,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve grain group from extent file: %d at offset: %" PRIi64 ".",
		 function,
		 extent_number,
		 extent_file_data_offset );

		return( -1 );
	}
	else if( result != 0 )
	{
		return( 1 );
	}
	result = libvmdk_extent_file_get_grain_group_at_offset(
		  extent_file,
		  file_io_pool,
		  extent_file_data_offset,
		  &grain_groups_list_index,
		  &grain_group_data_offset,
		  &grains_list,
		  error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve grain group from extent file: %d at offset: %" PRIi64 ".",
		 function,
		 extent_number,
		 extent_file_data_offset );

		return( -1 );
	}
	result = libfdata_list_get_element_at_offset(
		  grains_list,
		  grain_group_data_offset,
		  &grains_list_index,
		  &grain_data_offset,
		  &grain_file_index,
		  &grain_offset,
		  &grain_size,
		  &grain_flags,
		  error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve grain: %" PRIu64 " from grain group: %d in extent file: %d at offset: %" PRIi64 ".",
		 function,
		 grain_index,
		 grain_groups_list_index,
		 extent_number,
		 extent_file_data_offset );

		return( -1 );
	}
	if( ( grain_flags & LIBVMDK_RANGE_FLAG_IS_SPARSE ) != 0 )
	{
		return( 1 );
	}
	return( 0 );
}

/* Retrieves the grain data of a grain at a specific offset
 * Returns 1 if successful or -1 on error
 */
int libvmdk_grain_table_get_grain_data_at_offset(
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
	static char *function              = "libvmdk_grain_table_get_grain_data_at_offset";
	off64_t grain_group_data_offset    = 0;
	off64_t extent_file_data_offset    = 0;
	int extent_number                  = 0;
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
	result = libvmdk_extent_table_get_extent_file_at_offset(
	          extent_table,
	          offset,
	          file_io_pool,
	          &extent_number,
	          &extent_file_data_offset,
	          &extent_file,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve extent file at offset: %" PRIi64 " from extent table.",
		 function,
		 offset );

		return( -1 );
	}
	if( extent_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing extent file.",
		 function );

		return( -1 );
	}
	result = libvmdk_extent_file_get_grain_group_at_offset(
		  extent_file,
		  file_io_pool,
		  extent_file_data_offset,
		  &grain_groups_list_index,
		  &grain_group_data_offset,
		  &grains_list,
		  error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve grain group from extent file: %d at offset: %" PRIi64 ".",
		 function,
		 extent_number,
		 extent_file_data_offset );

		return( -1 );
	}
	result = libfdata_list_get_element_value_at_offset(
		  grains_list,
		  (intptr_t *) file_io_pool,
		  (libfdata_cache_t *) grains_cache,
		  grain_group_data_offset,
		  &grains_list_index,
		  grain_data_offset,
		  (intptr_t **) grain_data,
		  0,
		  error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve grain: %" PRIu64 " data from grain group: %d in extent file: %d at offset: %" PRIi64 ".",
		 function,
		 grain_index,
		 grain_groups_list_index,
		 extent_number,
		 extent_file_data_offset );

		return( -1 );
	}
	return( 1 );
}

