/*
 * libvmdk Input/Output (IO) handle
 *
 * Copyright (c) 2009-2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include "libvmdk_debug.h"
#include "libvmdk_definitions.h"
#include "libvmdk_io_handle.h"
#include "libvmdk_libbfio.h"
#include "libvmdk_offset_table.h"
#include "libvmdk_string.h"

#include "cowd_sparse_file_header.h"
#include "vmdk_sparse_file_header.h"

const char cowd_sparse_file_signature[ 4 ] = "DWOC";
const char vmdk_sparse_file_signature[ 4 ] = "KDMV";

/* Initialize an io handle
 * Make sure the value io_handle is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libvmdk_io_handle_initialize(
     libvmdk_io_handle_t **io_handle,
     liberror_error_t **error )
{
	static char *function = "libvmdk_io_handle_initialize";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid io handle.",
		 function );

		return( -1 );
	}
	if( *io_handle == NULL )
	{
		*io_handle = (libvmdk_io_handle_t *) memory_allocate(
		                                      sizeof( libvmdk_io_handle_t ) );

		if( *io_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create io handle.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *io_handle,
		     0,
		     sizeof( libvmdk_io_handle_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear file.",
			 function );

			memory_free(
			 *io_handle );

			*io_handle = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees an exisisting io handle
 * Returns 1 if successful or -1 on error
 */
int libvmdk_io_handle_free(
     libvmdk_io_handle_t **io_handle,
     liberror_error_t **error )
{
	static char *function = "libvmdk_io_handle_free";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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

/* Reads the file header
 * Returns 1 if successful or -1 on error
 */
int libvmdk_io_handle_read_file_header(
     libvmdk_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t *descriptor_offset,
     size64_t *descriptor_size,
     off64_t *grain_directory_offset,
     off64_t *secondary_grain_directory_offset,
     uint32_t *amount_of_grain_directory_entries,
     uint32_t *amount_of_grain_table_entries,
     size64_t *grain_size,
     liberror_error_t **error )
{
	uint8_t *file_header  = NULL;
	void *reallocation    = NULL;
	static char *function = "libvmdk_io_handle_read_file_header";
	size_t read_size      = 0;
	ssize_t read_count    = 0;
	uint32_t flags        = 0;

#if defined( HAVE_VERBOSE_OUTPUT )
	uint64_t value_64bit  = 0;
#endif

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid io handle.",
		 function );

		return( -1 );
	}
	if( descriptor_offset == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor offset.",
		 function );

		return( -1 );
	}
	if( descriptor_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor size.",
		 function );

		return( -1 );
	}
	if( grain_directory_offset == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain direcotry offset.",
		 function );

		return( -1 );
	}
	if( secondary_grain_directory_offset == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid secondary grain direcotry offset.",
		 function );

		return( -1 );
	}
	if( amount_of_grain_directory_entries == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid amount of grain directory entries.",
		 function );

		return( -1 );
	}
	if( amount_of_grain_table_entries == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid amount of grain table entries.",
		 function );

		return( -1 );
	}
	if( grain_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain size.",
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

	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     0,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek file header offset: 0.",
		 function );

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
	read_count = libbfio_handle_read(
	              file_io_handle,
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
		io_handle->file_type = LIBVMDK_FILE_TYPE_COWD_SPARSE_DATA;
		read_size            = sizeof( cowd_sparse_file_header_t );
	}
	else if( memory_compare(
	          file_header,
	          vmdk_sparse_file_signature,
	          4 ) == 0 )
	{
		io_handle->file_type = LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA;
		read_size            = sizeof( vmdk_sparse_file_header_t );
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

	read_count = libbfio_handle_read(
	              file_io_handle,
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

	if( io_handle->file_type == LIBVMDK_FILE_TYPE_COWD_SPARSE_DATA )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header )->version,
		 io_handle->format_version );
		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header )->flags,
		 flags );

		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header )->maximum_data_size,
		 io_handle->maximum_data_size );
		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header )->grain_size,
		 *grain_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header )->grain_directory_offset,
		 *grain_directory_offset );
		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header )->amount_of_grain_directory_entries,
		 *amount_of_grain_directory_entries );

		/* TODO */
	}
	else if( io_handle->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->version,
		 io_handle->format_version );
		byte_stream_copy_to_uint32_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->flags,
		 flags );

		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->maximum_data_size,
		 io_handle->maximum_data_size );
		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->grain_size,
		 *grain_size );

		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->descriptor_offset,
		 *descriptor_offset );
		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->descriptor_size,
		 *descriptor_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->amount_of_grain_table_entries,
		 *amount_of_grain_table_entries );

		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->secondary_grain_directory_offset,
		 *secondary_grain_directory_offset );
		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->grain_directory_offset,
		 *grain_directory_offset );

		byte_stream_copy_to_uint16_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header )->compression_method,
		 io_handle->compression_method );
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
		 "%s: version\t\t\t\t: %" PRIu32 "\n",
		 function,
		 io_handle->format_version );
		libnotify_printf(
		 "%s: flags\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 flags );

		libnotify_printf(
		 "%s: maximum data size\t\t\t: %" PRIu64 " sectors\n",
		 function,
		 io_handle->maximum_data_size );
		libnotify_printf(
		 "%s: grain size\t\t\t\t: %" PRIu64 " sectors\n",
		 function,
		 *grain_size );

		if( io_handle->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
		{
			libnotify_printf(
			 "%s: descriptor offset\t\t\t: %" PRIu64 "\n",
			 function,
			 *descriptor_offset );
			libnotify_printf(
			 "%s: descriptor size\t\t\t: %" PRIu64 " sectors\n",
			 function,
			 *descriptor_size );
			libnotify_printf(
			 "%s: amount of grain table entries\t: %" PRIu32 "\n",
			 function,
			 *amount_of_grain_table_entries );
			libnotify_printf(
			 "%s: secondary grain directory offset\t: %" PRIu64 "\n",
			 function,
			 *secondary_grain_directory_offset );
		}
		libnotify_printf(
		 "%s: grain directory offset\t\t: %" PRIu64 "\n",
		 function,
		 *grain_directory_offset );

		if( io_handle->file_type == LIBVMDK_FILE_TYPE_COWD_SPARSE_DATA )
		{
			libnotify_printf(
			 "%s: padding:\n",
			 function );
			libnotify_print_data(
			 (uint8_t *) ( (vmdk_sparse_file_header_t *) file_header )->padding,
			 433 );
		}
		else if( io_handle->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (vmdk_sparse_file_header_t *) file_header )->metadata_size,
			 value_64bit );
			libnotify_printf(
			 "%s: metadata size\t\t\t: %" PRIu64 " sectors\n",
			 function,
			 value_64bit );

			libnotify_printf(
			 "%s: single end of line character\t: 0x%02" PRIx8 "\n",
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
			 io_handle->compression_method );

			libnotify_printf(
			 "%s: padding:\n",
			 function );
			libnotify_print_data(
			 (uint8_t *) ( (vmdk_sparse_file_header_t *) file_header )->padding,
			 433 );
		}
	}
#endif

	if( *grain_size == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported grain size value is 0.",
		 function );

		return( -1 );
	}
	if( io_handle->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
	{
		if( *grain_size <= 8 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported grain size value is less than or equal to 8.",
			 function );

			return( -1 );
		}
		if( ( *grain_size % 2 ) != 0 )
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
		if( *amount_of_grain_table_entries == 0 )
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
	if( ( io_handle->maximum_data_size % *grain_size ) != 0 )
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
	if( io_handle->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
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

	if( ( io_handle->compression_method != LIBVMDK_COMPRESSION_METHOD_NONE )
	 && ( io_handle->compression_method != LIBVMDK_COMPRESSION_METHOD_DEFLATE ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compression method: %" PRIu16 ".",
		 function,
		 io_handle->compression_method );

		return( -1 );
	}
	/* Change all sector values to byte values
	 */
	io_handle->maximum_data_size *= LIBVMDK_SECTOR_SIZE;
	*grain_size                  *= LIBVMDK_SECTOR_SIZE;
	*grain_directory_offset      *= LIBVMDK_SECTOR_SIZE;

	if( io_handle->file_type == LIBVMDK_FILE_TYPE_COWD_SPARSE_DATA )
	{
		*amount_of_grain_table_entries = 4096;
	}
	else if( io_handle->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
	{
		*amount_of_grain_directory_entries = io_handle->maximum_data_size / ( *amount_of_grain_table_entries * *grain_size );

		if( ( io_handle->maximum_data_size % ( *amount_of_grain_table_entries * *grain_size ) ) != 0 )
		{
			*amount_of_grain_directory_entries += 1;
		}
		*descriptor_offset                *= LIBVMDK_SECTOR_SIZE;
		*descriptor_size                  *= LIBVMDK_SECTOR_SIZE;
		*secondary_grain_directory_offset *= LIBVMDK_SECTOR_SIZE;
	}
	return( 1 );
}

/* Reads a grain directory
 * Returns 1 if successful or -1 on error
 */
int libvmdk_io_handle_read_grain_directory(
     libvmdk_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libvmdk_offset_table_t *offset_table,
     off64_t grain_directory_offset,
     uint32_t amount_of_grain_directory_entries,
     uint32_t amount_of_grain_table_entries,
     size64_t grain_size,
     uint8_t is_secondary_grain_directory,
     liberror_error_t **error )
{
	uint8_t *grain_directory_data           = NULL;
	uint8_t *sector_blocks_data             = NULL;
	static char *function                   = "libvmdk_io_handle_read_grain_directory";
	size_t grain_directory_data_size        = 0;
	size_t sector_blocks_data_size          = 0;
	ssize_t read_count                      = 0;
	uint32_t grain_directory_entry_iterator = 0;
	uint32_t grain_table_offset             = 0;

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid io handle.",
		 function );

		return( -1 );
	}
	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
		 function );

		return( -1 );
	}
	grain_directory_data_size = amount_of_grain_directory_entries * sizeof( uint32_t );

	if( grain_directory_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid grain directory size value exceeds maximum.",
		 function );

		return( -1 );
	}
	sector_blocks_data_size = grain_directory_data_size / 512;

	if( ( grain_directory_data_size % 512 ) != 0 )
	{
		sector_blocks_data_size += 1;
	}
	sector_blocks_data_size *= 512;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: reading grain directory at offset: %" PRIu64 " (0x%08" PRIx64 ")\n",
		 function,
		 grain_directory_offset,
		 grain_directory_offset );
	}
#endif

	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     grain_directory_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek grain directory offset: %" PRIu64 ".",
		 function,
		 grain_directory_offset );

		return( -1 );
	}
	sector_blocks_data = (uint8_t *) memory_allocate(
	                                  sector_blocks_data_size );

	if( sector_blocks_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create sector blocks data.",
		 function );

		return( -1 );
	}
	read_count = libbfio_handle_read(
	              file_io_handle,
	              sector_blocks_data,
	              sector_blocks_data_size,
	              error );

	if( read_count != (ssize_t) sector_blocks_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read grain directory data.",
		 function );

		memory_free(
		 sector_blocks_data );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: grain directory data:\n",
		 function );
		libnotify_print_data(
		 sector_blocks_data,
		 sector_blocks_data_size );
	}
#endif

	grain_directory_data = sector_blocks_data;

	for( grain_directory_entry_iterator = 0;
	     grain_directory_entry_iterator < amount_of_grain_directory_entries;
	     grain_directory_entry_iterator++ )
	{
		byte_stream_copy_to_uint32_little_endian(
		 grain_directory_data,
		 grain_table_offset );
		 
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: grain directory entry: %03" PRIu32 " offset: 0x%08" PRIx32 " (%" PRIu32 ")\n",
			 function,
			 grain_directory_entry_iterator,
			 grain_table_offset,
			 grain_table_offset );
		}
#endif

		grain_table_offset *= LIBVMDK_SECTOR_SIZE;

		if( libvmdk_io_handle_read_grain_table(
		     io_handle,
		     file_io_handle,
		     offset_table,
		     grain_table_offset,
		     amount_of_grain_table_entries,
		     grain_size,
		     is_secondary_grain_directory,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read grain table.",
			 function );

			return( -1 );
		}
		grain_directory_data += sizeof( uint32_t );
	}
	/* TODO check if remainder of sector block is emtpy */

	memory_free(
	 sector_blocks_data );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "\n" );
	}
#endif

	return( 1 );
}

/* Reads a grain table
 * Returns 1 if successful or -1 on error
 */
int libvmdk_io_handle_read_grain_table(
     libvmdk_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libvmdk_offset_table_t *offset_table,
     off64_t grain_table_offset,
     uint32_t amount_of_grain_table_entries,
     size64_t grain_size,
     uint8_t is_secondary_grain_directory,
     liberror_error_t **error )
{
	uint8_t *sector_blocks_data    = NULL;
	static char *function          = "libvmdk_io_handle_read_grain_table";
	size_t grain_table_data_size   = 0;
	size_t sector_blocks_data_size = 0;
	ssize_t read_count             = 0;

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid io handle.",
		 function );

		return( -1 );
	}
	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
		 function );

		return( -1 );
	}
	grain_table_data_size = amount_of_grain_table_entries * sizeof( uint32_t );

	if( grain_table_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid grain table size value exceeds maximum.",
		 function );

		return( -1 );
	}
	sector_blocks_data_size = grain_table_data_size / 512;

	if( ( grain_table_data_size % 512 ) != 0 )
	{
		sector_blocks_data_size += 1;
	}
	sector_blocks_data_size *= 512;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: reading grain table at offset: %" PRIu64 " (0x%08" PRIx64 ")\n",
		 function,
		 grain_table_offset,
		 grain_table_offset );
	}
#endif

	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     grain_table_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek grain table offset: %" PRIu64 ".",
		 function,
		 grain_table_offset );

		return( -1 );
	}
	sector_blocks_data = (uint8_t *) memory_allocate(
	                                  sector_blocks_data_size );

	if( sector_blocks_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create sector blocks data.",
		 function );

		return( -1 );
	}
	read_count = libbfio_handle_read(
	              file_io_handle,
	              sector_blocks_data,
	              sector_blocks_data_size,
	              error );

	if( read_count != (ssize_t) sector_blocks_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read grain table data.",
		 function );

		memory_free(
		 sector_blocks_data );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: grain table data:\n",
		 function );
		libnotify_print_data(
		 sector_blocks_data,
		 sector_blocks_data_size );
	}
#endif

	if( is_secondary_grain_directory ==  0 )
	{
		if( libvmdk_offset_table_fill(
		     offset_table,
		     sector_blocks_data,
		     grain_table_data_size,
		     amount_of_grain_table_entries,
		     grain_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to fill offset table.",
			 function );

			memory_free(
			 sector_blocks_data );

			return( -1 );
		}
	}
	else
	{
		if( libvmdk_offset_table_compare(
		     offset_table,
		     sector_blocks_data,
		     grain_table_data_size,
		     amount_of_grain_table_entries,
		     grain_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to compare offset table.",
			 function );

			memory_free(
			 sector_blocks_data );

			return( -1 );
		}
	}
	/* TODO check if remainder of sector block is emtpy */

	memory_free(
	 sector_blocks_data );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "\n" );
	}
#endif

	return( 1 );
}

