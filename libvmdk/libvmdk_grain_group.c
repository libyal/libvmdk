/*
 * Grain group functions
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
#include "libvmdk_grain_group.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libcnotify.h"
#include "libvmdk_libfdata.h"

/* Creates a grain group
 * Make sure the value grain_group is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libvmdk_grain_group_initialize(
     libvmdk_grain_group_t **grain_group,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_grain_group_initialize";

	if( grain_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain group.",
		 function );

		return( -1 );
	}
	if( *grain_group != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid grain group value already set.",
		 function );

		return( -1 );
	}
	*grain_group = memory_allocate_structure(
	                libvmdk_grain_group_t );

	if( *grain_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create grain group.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *grain_group,
	     0,
	     sizeof( libvmdk_grain_group_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear grain group.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *grain_group != NULL )
	{
		memory_free(
		 *grain_group );

		*grain_group = NULL;
	}
	return( -1 );
}

/* Frees a grain group
 * Returns 1 if successful or -1 on error
 */
int libvmdk_grain_group_free(
     libvmdk_grain_group_t **grain_group,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_grain_group_free";

	if( grain_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain group.",
		 function );

		return( -1 );
	}
	if( *grain_group != NULL )
	{
		memory_free(
		 *grain_group );

		*grain_group = NULL;
	}
	return( 1 );
}

/* Clones the grain group
 * Returns 1 if successful or -1 on error
 */
int libvmdk_grain_group_clone(
     libvmdk_grain_group_t **destination_grain_group,
     libvmdk_grain_group_t *source_grain_group,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_grain_group_clone";

	if( destination_grain_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination grain group.",
		 function );

		return( -1 );
	}
	if( *destination_grain_group != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination grain group value already set.",
		 function );

		return( -1 );
	}
	if( source_grain_group == NULL )
	{
		*destination_grain_group = NULL;

		return( 1 );
	}
	*destination_grain_group = memory_allocate_structure(
	                            libvmdk_grain_group_t );

	if( *destination_grain_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination grain group.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_grain_group,
	     source_grain_group,
	     sizeof( libvmdk_grain_group_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination grain group.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_grain_group != NULL )
	{
		memory_free(
		 *destination_grain_group );

		*destination_grain_group = NULL;
	}
	return( -1 );
}

/* Fills the grain offset from the grain group
 * Returns 1 if successful or -1 on error
 */
int libvmdk_grain_group_fill(
     libfdata_list_t *grains_list,
     int grain_index,
     size64_t grain_size,
     int file_io_pool_entry,
     const uint8_t *grain_group_data,
     size_t grain_group_data_size,
     int number_of_grain_group_entries,
     libcerror_error_t **error )
{
	const uint8_t *grain_group_entry = NULL;
	static char *function            = "libvmdk_grain_group_fill";
	off64_t grain_data_offset        = 0;
	uint32_t range_flags             = 0;
	int element_index                = 0;
	int grain_group_entry_index      = 0;
	int result                       = 0;

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
	if( grain_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid grain size.",
		 function );

		return( -1 );
	}
	if( grain_group_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain group data.",
		 function );

		return( -1 );
	}
	if( grain_group_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid grain group size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( (size_t) number_of_grain_group_entries * 4 ) > grain_group_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of grain group entries value out of bounds.",
		 function );

		return( -1 );
	}
	grain_group_entry = grain_group_data;

	for( grain_group_entry_index = 0;
	     grain_group_entry_index < number_of_grain_group_entries;
	     grain_group_entry_index++ )
	{
		byte_stream_copy_to_uint32_little_endian(
		 grain_group_entry,
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
			 grain_size,
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
		if( libfdata_list_append_element(
		     grains_list,
		     &element_index,
		     file_io_pool_entry,
		     grain_data_offset,
		     grain_size,
		     range_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append grain: %d to grains list.",
			 function,
			 grain_index );

			return( -1 );
		}
		grain_group_entry += sizeof( uint32_t );

		grain_index++;
	}
	return( 1 );
}

/* Compares the grain offset with the ones in the grain group and makes corrections if necessary
 * Returns 1 if successful or -1 on error
 */
int libvmdk_grain_group_correct(
     libfdata_list_t *grains_list,
     int grain_index,
     size64_t grain_size,
     int file_io_pool_entry,
     const uint8_t *grain_group_data,
     size_t grain_group_data_size,
     int number_of_grain_group_entries,
     libcerror_error_t **error )
{
	const uint8_t *grain_group_entry = NULL;
	static char *function            = "libvmdk_grain_group_correct";
	off64_t grain_data_offset        = 0;
	uint32_t range_flags             = 0;
	int grain_group_entry_index      = 0;
	int result                       = 0;

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
	if( grain_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid grain size.",
		 function );

		return( -1 );
	}
	if( grain_group_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain group data.",
		 function );

		return( -1 );
	}
	if( grain_group_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid grain group size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( (size_t) number_of_grain_group_entries * 4 ) > grain_group_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of grain group entries value out of bounds.",
		 function );

		return( -1 );
	}
	grain_group_entry = grain_group_data;

	for( grain_group_entry_index = 0;
	     grain_group_entry_index < number_of_grain_group_entries;
	     grain_group_entry_index++ )
	{
		byte_stream_copy_to_uint32_little_endian(
		 grain_group_entry,
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
			 grain_size,
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
/* TODO */
		grain_group_entry += sizeof( uint32_t );

		grain_index++;
	}
	return( 1 );
}

