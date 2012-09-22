/*
 * Extent file functions
 *
 * Copyright (c) 2009-2012 Joachim Metz <joachim.metz@gmail.com>
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
#include "libvmdk_extent_file.h"
#include "libvmdk_libbfio.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libcstring.h"

#include "cowd_sparse_file_header.h"
#include "vmdk_sparse_file_header.h"

const char cowd_sparse_file_signature[ 4 ] = "DWOC";
const char vmdk_sparse_file_signature[ 4 ] = "KDMV";

/* Initialize the extent file
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_file_initialize(
     libvmdk_extent_file_t **extent_file,
     int file_io_pool_entry,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_file_initialize";

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
	if( *extent_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extent file value already set.",
		 function );

		return( -1 );
	}
	*extent_file = memory_allocate_structure(
	                libvmdk_extent_file_t );

	if( *extent_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create extent file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *extent_file,
	     0,
	     sizeof( libvmdk_extent_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear extent file.",
		 function );

		goto on_error;
	}
	( *extent_file )->file_io_pool_entry = file_io_pool_entry;

	return( 1 );

on_error:
	if( *extent_file != NULL )
	{
		memory_free(
		 *extent_file );

		*extent_file = NULL;
	}
	return( -1 );
}

/* Frees the extent file including elements
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_file_free(
     libvmdk_extent_file_t **extent_file,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_file_free";

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
	if( *extent_file != NULL )
	{
		memory_free(
		 *extent_file );

		*extent_file = NULL;
	}
	return( 1 );
}

/* Reads the file header from an extent file
 * Returns the number of bytes read if successful, or -1 on errror
 */
ssize_t libvmdk_extent_file_read_file_header(
         libvmdk_extent_file_t *extent_file,
         libbfio_pool_t *file_io_pool,
         libcerror_error_t **error )
{
	uint8_t *file_header  = NULL;
	static char *function = "libvmdk_extent_file_read_file_header";
	size_t read_size      = 0;
	ssize_t read_count    = 0;

#if defined( HAVE_VERBOSE_OUTPUT )
	uint64_t value_64bit  = 0;
#endif

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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	"%s: reading file header at offset: 0 (0x00000000)\n",
		 function );
	}
#endif
	if( libbfio_pool_seek_offset(
	     file_io_pool,
	     extent_file->file_io_pool_entry,
	     0,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek file header offset: %" PRIu64 ".",
		 function,
		 0 );

		goto on_error;
	}
	file_header = (uint8_t *) memory_allocate(
	                           sizeof( uint8_t ) * 2048 );

	if( file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file header.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read(
	              file_io_pool,
	              extent_file->file_io_pool_entry,
	              file_header,
	              4,
	              error );

	if( read_count != (ssize_t) 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header.",
		 function );

		goto on_error;
	}
	if( memory_compare(
	     file_header,
	     cowd_sparse_file_signature,
	     4 ) == 0 )
	{
		extent_file->file_type = LIBVMDK_FILE_TYPE_COWD_SPARSE_DATA;
		read_size              = sizeof( cowd_sparse_file_header_t );
	}
	else if( memory_compare(
	          file_header,
	          vmdk_sparse_file_signature,
	          4 ) == 0 )
	{
		extent_file->file_type = LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA;
		read_size              = sizeof( vmdk_sparse_file_header_t );
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file signature.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read(
	              file_io_pool,
	              extent_file->file_io_pool_entry,
	              &( file_header[ 4 ] ),
	              read_size - 4,
	              error );

	if( read_count != (ssize_t) ( read_size - 4 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: file header:\n",
		 function );
		libcnotify_print_data(
		 file_header,
		 read_size );
	}
#endif
	if( extent_file->file_type == LIBVMDK_FILE_TYPE_COWD_SPARSE_DATA )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header )->version,
		 extent_file->format_version );

		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header )->flags,
		 extent_file->flags );

		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header )->maximum_data_size,
		 extent_file->maximum_data_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header )->grain_size,
		 extent_file->grain_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header )->grain_directory_offset,
		 extent_file->grain_directory_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header )->number_of_grain_directory_entries,
		 extent_file->number_of_grain_directory_entries );

		/* TODO */
	}
	else if( extent_file->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->version,
		 extent_file->format_version );

		byte_stream_copy_to_uint32_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->flags,
		 extent_file->flags );

		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->maximum_data_size,
		 extent_file->maximum_data_size );

		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->grain_size,
		 extent_file->grain_size );

		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->descriptor_offset,
		 extent_file->descriptor_offset );

		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->descriptor_size,
		 extent_file->descriptor_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->number_of_grain_table_entries,
		 extent_file->number_of_grain_table_entries );

		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->secondary_grain_directory_offset,
		 extent_file->secondary_grain_directory_offset );

		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->grain_directory_offset,
		 extent_file->grain_directory_offset );

		byte_stream_copy_to_uint16_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->compression_method,
		 extent_file->compression_method );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c%c%c\n",
		 function,
		 (char) file_header[ 0 ],
		 (char) file_header[ 1 ],
		 (char) file_header[ 2 ],
		 (char) file_header[ 3 ] );

		libcnotify_printf(
		 "%s: format version\t\t\t\t: %" PRIu32 "\n",
		 function,
		 extent_file->format_version );

		libcnotify_printf(
		 "%s: flags\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 extent_file->flags );

		libcnotify_printf(
		 "%s: maximum data size\t\t\t: %" PRIu64 " sectors\n",
		 function,
		 extent_file->maximum_data_size );

		libcnotify_printf(
		 "%s: grain size\t\t\t\t: %" PRIu64 " sectors\n",
		 function,
		 extent_file->grain_size );

		if( extent_file->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
		{
			libcnotify_printf(
			 "%s: descriptor offset\t\t\t: %" PRIu64 "\n",
			 function,
			 extent_file->descriptor_offset );

			libcnotify_printf(
			 "%s: descriptor size\t\t\t\t: %" PRIu64 " sectors\n",
			 function,
			 extent_file->descriptor_size );

			libcnotify_printf(
			 "%s: number of grain table entries\t\t: %" PRIu32 "\n",
			 function,
			 extent_file->number_of_grain_table_entries );

			libcnotify_printf(
			 "%s: secondary grain directory offset\t\t: %" PRIu64 "\n",
			 function,
			 extent_file->secondary_grain_directory_offset );
		}
		libcnotify_printf(
		 "%s: grain directory offset\t\t\t: %" PRIu64 "\n",
		 function,
		 extent_file->grain_directory_offset );

		if( extent_file->file_type == LIBVMDK_FILE_TYPE_COWD_SPARSE_DATA )
		{
			libcnotify_printf(
			 "%s: padding:\n",
			 function );
			libcnotify_print_data(
			 (uint8_t *) ( (vmdk_sparse_file_header_t *) file_header )->padding,
			 433 );
		}
		else if( extent_file->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (vmdk_sparse_file_header_t *) file_header )->metadata_size,
			 value_64bit );
			libcnotify_printf(
			 "%s: metadata size\t\t\t\t: %" PRIu64 " sectors\n",
			 function,
			 value_64bit );

			libcnotify_printf(
			 "%s: single end of line character\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (vmdk_sparse_file_header_t *) file_header )->single_end_of_line_character );

			libcnotify_printf(
			 "%s: non end of line character\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (vmdk_sparse_file_header_t *) file_header )->non_end_of_line_character );

			libcnotify_printf(
			 "%s: first double end of line character\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (vmdk_sparse_file_header_t *) file_header )->first_double_end_of_line_character );

			libcnotify_printf(
			 "%s: second double end of line character\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (vmdk_sparse_file_header_t *) file_header )->second_double_end_of_line_character );

			libcnotify_printf(
			 "%s: compression method\t\t\t: %" PRIu16 "\n",
			 function,
			 extent_file->compression_method );

			libcnotify_printf(
			 "%s: padding:\n",
			 function );
			libcnotify_print_data(
			 ( (vmdk_sparse_file_header_t *) file_header )->padding,
			 433 );
		}
	}
#endif
	if( extent_file->grain_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported grain size value is 0.",
		 function );

		goto on_error;
	}
	if( extent_file->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
	{
		if( extent_file->grain_size <= 8 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported grain size value is less than or equal to 8.",
			 function );

			goto on_error;
		}
		if( ( extent_file->grain_size % 2 ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported grain size value is not a power of 2.",
			 function );

			goto on_error;
		}
		if( extent_file->number_of_grain_table_entries == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported number of grain table entries value is 0.",
			 function );

			goto on_error;
		}
	}
	if( ( extent_file->maximum_data_size % extent_file->grain_size ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported maximum data size not a multide of the grain size.",
		 function );

		goto on_error;
	}
	if( extent_file->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
	{
		if( ( (vmdk_sparse_file_header_t *) file_header )->single_end_of_line_character != (uint8_t) '\n' )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported single end of line character.",
			 function );

			goto on_error;
		}
		if( ( (vmdk_sparse_file_header_t *) file_header )->non_end_of_line_character != (uint8_t) ' ' )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported non end of line character.",
			 function );

			goto on_error;
		}
		if( ( (vmdk_sparse_file_header_t *) file_header )->first_double_end_of_line_character != (uint8_t) '\r' )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported first double end of line character.",
			 function );

			goto on_error;
		}
		if( ( (vmdk_sparse_file_header_t *) file_header )->second_double_end_of_line_character != (uint8_t) '\n' )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported second double end of line character.",
			 function );

			goto on_error;
		}
	}
	memory_free(
	 file_header );

	file_header = NULL;

	if( ( extent_file->compression_method != LIBVMDK_COMPRESSION_METHOD_NONE )
	 && ( extent_file->compression_method != LIBVMDK_COMPRESSION_METHOD_DEFLATE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compression method: %" PRIu16 ".",
		 function,
		 extent_file->compression_method );

		goto on_error;
	}
	/* Change all sector values to byte values
	 */
	extent_file->maximum_data_size      *= LIBVMDK_SECTOR_SIZE;
	extent_file->grain_size             *= LIBVMDK_SECTOR_SIZE;
	extent_file->grain_directory_offset *= LIBVMDK_SECTOR_SIZE;

	if( extent_file->file_type == LIBVMDK_FILE_TYPE_COWD_SPARSE_DATA )
	{
		extent_file->number_of_grain_table_entries = 4096;
	}
	else if( extent_file->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
	{
		extent_file->number_of_grain_directory_entries = extent_file->maximum_data_size
		                                               / ( extent_file->number_of_grain_table_entries * extent_file->grain_size );

		if( ( extent_file->maximum_data_size % ( extent_file->number_of_grain_table_entries * extent_file->grain_size ) ) != 0 )
		{
			extent_file->number_of_grain_directory_entries += 1;
		}
		extent_file->descriptor_offset                *= LIBVMDK_SECTOR_SIZE;
		extent_file->descriptor_size                  *= LIBVMDK_SECTOR_SIZE;
		extent_file->secondary_grain_directory_offset *= LIBVMDK_SECTOR_SIZE;
	}
	return( (ssize_t) read_size );

on_error:
	if( file_header != NULL )
	{
		memory_free(
		 file_header );
	}
	return( -1 );
}

