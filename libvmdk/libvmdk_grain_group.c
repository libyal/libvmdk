/*
 * Grain group functions
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

/* Fills the grain offset from the grain group
 * Returns 1 if successful or -1 on error
 */
int libvmdk_grain_group_fill(
     libfdata_list_t *grains_list,
     int grain_index,
     size64_t grain_size,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     const uint8_t *grain_group_data,
     size_t grain_group_data_size,
     int number_of_grain_group_entries,
     uint32_t extent_file_flags,
     libcerror_error_t **error )
{
	uint8_t compressed_data_header[ 12 ];

	const uint8_t *grain_group_entry = NULL;
	static char *function            = "libvmdk_grain_group_fill";
	off64_t grain_data_offset        = 0;
	size64_t grain_data_size         = 0;
	ssize_t read_count               = 0;
	uint32_t range_flags             = 0;
	int element_index                = 0;
	int grain_group_entry_index      = 0;

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

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: grain table entry: %03" PRIu32 " sector number\t\t: %" PRIi64 "\n",
			 function,
			 grain_group_entry_index,
			 grain_data_offset );
		}
#endif
		if( grain_data_offset != 0 )
		{
			if( ( extent_file_flags & LIBVMDK_FLAG_HAS_GRAIN_COMPRESSION ) != 0 )
			{
				range_flags = LIBVMDK_RANGE_FLAG_IS_COMPRESSED;
			}
			else
			{
				range_flags = 0;
			}
			grain_data_offset *= 512;
		}
		else
		{
			range_flags = LIBVMDK_RANGE_FLAG_IS_SPARSE;
		}
		if( ( extent_file_flags & LIBVMDK_FLAG_HAS_GRAIN_COMPRESSION ) != 0 )
		{
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
			byte_stream_copy_to_uint32_little_endian(
			 &( compressed_data_header[ 8 ] ),
			 grain_data_size );
		}
		else
		{
			grain_data_size = grain_size;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: grain table entry: %03" PRIu32 " offset\t\t\t: %" PRIi64 " (0x%08" PRIx64 ")\n",
			 function,
			 grain_group_entry_index,
			 grain_data_offset * 512,
			 grain_data_offset * 512 );

			libcnotify_printf(
			 "%s: grain table entry: %03" PRIu32 " size\t\t\t: %" PRIu64 "\n",
			 function,
			 grain_group_entry_index,
			 grain_data_size );

			libcnotify_printf(
			 "%s: grain table entry: %03" PRIu32 " file IO pool entry\t: %d\n",
			 function,
			 grain_group_entry_index,
			 file_io_pool_entry );

			libcnotify_printf(
			 "%s: grain table entry: %03" PRIu32 " range flags\t\t: 0x%08" PRIx64 "\n",
			 function,
			 grain_group_entry_index,
			 range_flags );

			if( ( range_flags & LIBVMDK_RANGE_FLAG_IS_SPARSE ) != 0 )
			{
				libcnotify_printf(
				 "\tIs sparse.\n" );
			}
			if( ( range_flags & LIBVMDK_RANGE_FLAG_IS_COMPRESSED ) != 0 )
			{
				libcnotify_printf(
				 "\tIs compressed.\n" );
			}
			libcnotify_printf(
			 "\n" );
		}
#endif
		if( libfdata_list_append_element_with_mapped_size(
		     grains_list,
		     &element_index,
		     file_io_pool_entry,
		     grain_data_offset,
		     grain_data_size,
		     range_flags,
		     grain_size,
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

