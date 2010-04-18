/*
 * Segment file reading/writing functions
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <liberror.h>
#include <libnotify.h>

#include "libvmdk_definitions.h"
#include "libvmdk_libbfio.h"
#include "libvmdk_segment_file.h"
#include "libvmdk_segment_file_handle.h"

#include "cowd_sparse_file_header.h"
#include "vmdk_sparse_file_header.h"

const char cowd_sparse_file_signature[ 4 ] = "DWOC";
const char vmdk_sparse_file_signature[ 4 ] = "KDMV";

/* Reads the file header from a segment file
 * Returns the amount of bytes read if successful, or -1 on errror
 */
ssize_t libvmdk_segment_file_read_file_header(
         libvmdk_segment_file_handle_t *segment_file_handle,
         libbfio_pool_t *file_io_pool,
         liberror_error_t **error )
{
	uint8_t *file_header  = NULL;
	void *reallocation    = NULL;
	static char *function = "libvmdk_segment_file_read_file_header";
	size_t read_size      = 0;
	ssize_t read_count    = 0;

#if defined( HAVE_VERBOSE_OUTPUT )
	uint64_t value_64bit  = 0;
#endif

	if( segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
	 	"%s: reading file header at offset: 0 (0x00000000)\n",
		 function );
	}
#endif

	if( libbfio_pool_seek_offset(
	     file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     0,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek file header offset: %" PRIu64 ".",
		 function,
		 0 );

		return( -1 );
	}
	file_header = (uint8_t *) memory_allocate(
	                           sizeof( uint8_t ) * 4 );

	if( file_header == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file header.",
		 function );

		return( -1 );
	}
	read_count = libbfio_pool_read(
	              file_io_pool,
	              segment_file_handle->file_io_pool_entry,
	              file_header,
	              4,
	              error );

	if( read_count != (ssize_t) 4 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header.",
		 function );

		memory_free(
		 file_header );

		return( -1 );
	}
	if( memory_compare(
	     file_header,
	     cowd_sparse_file_signature,
	     4 ) == 0 )
	{
		segment_file_handle->file_type = LIBVMDK_FILE_TYPE_COWD_SPARSE_DATA;
		read_size                      = sizeof( cowd_sparse_file_header_t );
	}
	else if( memory_compare(
	          file_header,
	          vmdk_sparse_file_signature,
	          4 ) == 0 )
	{
		segment_file_handle->file_type = LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA;
		read_size                      = sizeof( vmdk_sparse_file_header_t );
	}
	else
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file signature.",
		 function );

		memory_free(
		 file_header );

		return( -1 );
	}
	reallocation = memory_reallocate(
	                file_header,
	                sizeof( uint8_t ) * read_size );

	if( reallocation == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to resize file header.",
		 function );

		memory_free(
		 file_header );

		return( -1 );
	}
	file_header = (uint8_t *) reallocation;

	read_count = libbfio_pool_read(
	              file_io_pool,
	              segment_file_handle->file_io_pool_entry,
	              &( file_header[ 4 ] ),
	              read_size - 4,
	              error );

	if( read_count != (ssize_t) ( read_size - 4 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header.",
		 function );

		memory_free(
		 file_header );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: file header:\n",
		 function );
		libnotify_print_data(
		 file_header,
		 read_size );
	}
#endif

	if( segment_file_handle->file_type == LIBVMDK_FILE_TYPE_COWD_SPARSE_DATA )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header )->version,
		 segment_file_handle->format_version );
		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header )->flags,
		 segment_file_handle->flags );

		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header )->maximum_data_size,
		 segment_file_handle->maximum_data_size );
		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header )->grain_size,
		 segment_file_handle->grain_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header )->grain_directory_offset,
		 segment_file_handle->grain_directory_offset );
		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header )->amount_of_grain_directory_entries,
		 segment_file_handle->amount_of_grain_directory_entries );

		/* TODO */
	}
	else if( segment_file_handle->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->version,
		 segment_file_handle->format_version );
		byte_stream_copy_to_uint32_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->flags,
		 segment_file_handle->flags );

		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->maximum_data_size,
		 segment_file_handle->maximum_data_size );
		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->grain_size,
		 segment_file_handle->grain_size );

		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->descriptor_offset,
		 segment_file_handle->descriptor_offset );
		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->descriptor_size,
		 segment_file_handle->descriptor_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->amount_of_grain_table_entries,
		 segment_file_handle->amount_of_grain_table_entries );

		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->secondary_grain_directory_offset,
		 segment_file_handle->secondary_grain_directory_offset );
		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->grain_directory_offset,
		 segment_file_handle->grain_directory_offset );

		byte_stream_copy_to_uint16_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->compression_method,
		 segment_file_handle->compression_method );
	}

#if defined( HAVE_VERBOSE_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: signature\t\t\t\t: %c%c%c%c\n",
		 function,
		 (char) file_header[ 0 ],
		 (char) file_header[ 1 ],
		 (char) file_header[ 2 ],
		 (char) file_header[ 3 ] );

		libnotify_printf(
		 "%s: format version\t\t\t\t: %" PRIu32 "\n",
		 function,
		 segment_file_handle->format_version );
		libnotify_printf(
		 "%s: flags\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 segment_file_handle->flags );

		libnotify_printf(
		 "%s: maximum data size\t\t\t: %" PRIu64 " sectors\n",
		 function,
		 segment_file_handle->maximum_data_size );
		libnotify_printf(
		 "%s: grain size\t\t\t\t: %" PRIu64 " sectors\n",
		 function,
		 segment_file_handle->grain_size );

		if( segment_file_handle->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
		{
			libnotify_printf(
			 "%s: descriptor offset\t\t\t: %" PRIu64 "\n",
			 function,
			 segment_file_handle->descriptor_offset );
			libnotify_printf(
			 "%s: descriptor size\t\t\t\t: %" PRIu64 " sectors\n",
			 function,
			 segment_file_handle->descriptor_size );
			libnotify_printf(
			 "%s: amount of grain table entries\t\t: %" PRIu32 "\n",
			 function,
			 segment_file_handle->amount_of_grain_table_entries );
			libnotify_printf(
			 "%s: secondary grain directory offset\t\t: %" PRIu64 "\n",
			 function,
			 segment_file_handle->secondary_grain_directory_offset );
		}
		libnotify_printf(
		 "%s: grain directory offset\t\t\t: %" PRIu64 "\n",
		 function,
		 segment_file_handle->grain_directory_offset );

		if( segment_file_handle->file_type == LIBVMDK_FILE_TYPE_COWD_SPARSE_DATA )
		{
			libnotify_printf(
			 "%s: padding:\n",
			 function );
			libnotify_print_data(
			 (uint8_t *) ( (vmdk_sparse_file_header_t *) file_header )->padding,
			 433 );
		}
		else if( segment_file_handle->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (vmdk_sparse_file_header_t *) file_header )->metadata_size,
			 value_64bit );
			libnotify_printf(
			 "%s: metadata size\t\t\t\t: %" PRIu64 " sectors\n",
			 function,
			 value_64bit );

			libnotify_printf(
			 "%s: single end of line character\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (vmdk_sparse_file_header_t *) file_header )->single_end_of_line_character );
			libnotify_printf(
			 "%s: non end of line character\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (vmdk_sparse_file_header_t *) file_header )->non_end_of_line_character );
			libnotify_printf(
			 "%s: first double end of line character\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (vmdk_sparse_file_header_t *) file_header )->first_double_end_of_line_character );
			libnotify_printf(
			 "%s: second double end of line character\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (vmdk_sparse_file_header_t *) file_header )->second_double_end_of_line_character );
			libnotify_printf(
			 "%s: compression method\t\t\t: %" PRIu16 "\n",
			 function,
			 segment_file_handle->compression_method );

			libnotify_printf(
			 "%s: padding:\n",
			 function );
			libnotify_print_data(
			 (uint8_t *) ( (vmdk_sparse_file_header_t *) file_header )->padding,
			 433 );
		}
	}
#endif

	if( segment_file_handle->grain_size == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported grain size value is 0.",
		 function );

		return( -1 );
	}
	if( segment_file_handle->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
	{
		if( segment_file_handle->grain_size <= 8 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported grain size value is less than or equal to 8.",
			 function );

			return( -1 );
		}
		if( ( segment_file_handle->grain_size % 2 ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported grain size value is not a power of 2.",
			 function );

			memory_free(
			 file_header );

			return( -1 );
		}
		if( segment_file_handle->amount_of_grain_table_entries == 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported amount of grain table entries value is 0.",
			 function );

			return( -1 );
		}
	}
	if( ( segment_file_handle->maximum_data_size % segment_file_handle->grain_size ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported maximum data size not a multide of the grain size.",
		 function );

		memory_free(
		 file_header );

		return( -1 );
	}
	if( segment_file_handle->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
	{
		if( ( (vmdk_sparse_file_header_t *) file_header )->single_end_of_line_character != (uint8_t) '\n' )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported single end of line character.",
			 function );

			memory_free(
			 file_header );

			return( -1 );
		}
		if( ( (vmdk_sparse_file_header_t *) file_header )->non_end_of_line_character != (uint8_t) ' ' )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported non end of line character.",
			 function );

			memory_free(
			 file_header );

			return( -1 );
		}
		if( ( (vmdk_sparse_file_header_t *) file_header )->first_double_end_of_line_character != (uint8_t) '\r' )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported first double end of line character.",
			 function );

			memory_free(
			 file_header );

			return( -1 );
		}
		if( ( (vmdk_sparse_file_header_t *) file_header )->second_double_end_of_line_character != (uint8_t) '\n' )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported second double end of line character.",
			 function );

			memory_free(
			 file_header );

			return( -1 );
		}
	}
	memory_free(
	 file_header );

	if( ( segment_file_handle->compression_method != LIBVMDK_COMPRESSION_METHOD_NONE )
	 && ( segment_file_handle->compression_method != LIBVMDK_COMPRESSION_METHOD_DEFLATE ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compression method: %" PRIu16 ".",
		 function,
		 segment_file_handle->compression_method );

		return( -1 );
	}
	/* Change all sector values to byte values
	 */
	segment_file_handle->maximum_data_size      *= LIBVMDK_SECTOR_SIZE;
	segment_file_handle->grain_size             *= LIBVMDK_SECTOR_SIZE;
	segment_file_handle->grain_directory_offset *= LIBVMDK_SECTOR_SIZE;

	if( segment_file_handle->file_type == LIBVMDK_FILE_TYPE_COWD_SPARSE_DATA )
	{
		segment_file_handle->amount_of_grain_table_entries = 4096;
	}
	else if( segment_file_handle->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
	{
		segment_file_handle->amount_of_grain_directory_entries = segment_file_handle->maximum_data_size
		                                                       / ( segment_file_handle->amount_of_grain_table_entries * segment_file_handle->grain_size );

		if( ( segment_file_handle->maximum_data_size % ( segment_file_handle->amount_of_grain_table_entries * segment_file_handle->grain_size ) ) != 0 )
		{
			segment_file_handle->amount_of_grain_directory_entries += 1;
		}
		segment_file_handle->descriptor_offset                *= LIBVMDK_SECTOR_SIZE;
		segment_file_handle->descriptor_size                  *= LIBVMDK_SECTOR_SIZE;
		segment_file_handle->secondary_grain_directory_offset *= LIBVMDK_SECTOR_SIZE;
	}
	return( (ssize_t) read_size );
}

