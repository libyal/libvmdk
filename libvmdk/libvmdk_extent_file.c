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

#include "libvmdk_debug.h"
#include "libvmdk_definitions.h"
#include "libvmdk_extent_file.h"
#include "libvmdk_libbfio.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libcnotify.h"
#include "libvmdk_libcstring.h"
#include "libvmdk_libmfcache.h"
#include "libvmdk_libmfdata.h"
#include "libvmdk_unused.h"

#include "cowd_sparse_file_header.h"
#include "vmdk_sparse_file_header.h"

const char cowd_sparse_file_signature[ 4 ] = "DWOC";
const char vmdk_sparse_file_signature[ 4 ] = "KDMV";

/* Initialize the extent file
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_file_initialize(
     libvmdk_extent_file_t **extent_file,
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

/* Reads the file header from the extent file using the file IO handle
 * Returns 1 if successful, or -1 on errror
 */
int libvmdk_extent_file_read_file_header_file_io_handle(
     libvmdk_extent_file_t *extent_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	uint8_t *file_header_data = NULL;
	static char *function     = "libvmdk_extent_file_read_file_header";
	size_t read_size          = 0;
	ssize_t read_count        = 0;

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
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     0,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek file header offset: 0.",
		 function );

		goto on_error;
	}
	file_header_data = (uint8_t *) memory_allocate(
	                                sizeof( uint8_t ) * 2048 );

	if( file_header_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file header data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              file_header_data,
	              4,
	              error );

	if( read_count != (ssize_t) 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header data.",
		 function );

		goto on_error;
	}
	if( memory_compare(
	     file_header_data,
	     cowd_sparse_file_signature,
	     4 ) == 0 )
	{
		read_size = sizeof( cowd_sparse_file_header_t );
	}
	else if( memory_compare(
	          file_header_data,
	          vmdk_sparse_file_signature,
	          4 ) == 0 )
	{
		read_size = sizeof( vmdk_sparse_file_header_t );
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
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              &( file_header_data[ 4 ] ),
	              read_size - 4,
	              error );

	if( read_count != (ssize_t) ( read_size - 4 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header data.",
		 function );

		goto on_error;
	}
	if( libvmdk_extent_file_read_file_header_data(
	     extent_file,
	     file_header_data,
	     read_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header data.",
		 function );

		goto on_error;
	}
	memory_free(
	 file_header_data );

	file_header_data = NULL;

	return( 1 );

on_error:
	if( file_header_data != NULL )
	{
		memory_free(
		 file_header_data );
	}
	return( -1 );
}

/* Reads the file header from the extent file using the file IO pool entry
 * Returns 1 if successful, or -1 on errror
 */
int libvmdk_extent_file_read_file_header(
     libvmdk_extent_file_t *extent_file,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     libcerror_error_t **error )
{
	uint8_t *file_header_data = NULL;
	static char *function     = "libvmdk_extent_file_read_file_header";
	size_t read_size          = 0;
	ssize_t read_count        = 0;

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
	     file_io_pool_entry,
	     0,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek file header offset: 0.",
		 function );

		goto on_error;
	}
	file_header_data = (uint8_t *) memory_allocate(
	                                sizeof( uint8_t ) * 2048 );

	if( file_header_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file header data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              file_header_data,
	              4,
	              error );

	if( read_count != (ssize_t) 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header data.",
		 function );

		goto on_error;
	}
	if( memory_compare(
	     file_header_data,
	     cowd_sparse_file_signature,
	     4 ) == 0 )
	{
		read_size = sizeof( cowd_sparse_file_header_t );
	}
	else if( memory_compare(
	          file_header_data,
	          vmdk_sparse_file_signature,
	          4 ) == 0 )
	{
		read_size = sizeof( vmdk_sparse_file_header_t );
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
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              &( file_header_data[ 4 ] ),
	              read_size - 4,
	              error );

	if( read_count != (ssize_t) ( read_size - 4 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header data.",
		 function );

		goto on_error;
	}
	if( libvmdk_extent_file_read_file_header_data(
	     extent_file,
	     file_header_data,
	     read_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header data.",
		 function );

		goto on_error;
	}
	memory_free(
	 file_header_data );

	file_header_data = NULL;

	return( 1 );

on_error:
	if( file_header_data != NULL )
	{
		memory_free(
		 file_header_data );
	}
	return( -1 );
}

/* Reads the file header from the extent file
 * Returns 1 if successful, or -1 on errror
 */
int libvmdk_extent_file_read_file_header_data(
     libvmdk_extent_file_t *extent_file,
     const uint8_t *file_header_data,
     size_t file_header_data_size,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_file_read_file_header_data";

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
	if( file_header_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file header data.",
		 function );

		return( -1 );
	}
	if( file_header_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid file header data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( memory_compare(
	     file_header_data,
	     cowd_sparse_file_signature,
	     4 ) == 0 )
	{
		if( file_header_data_size < sizeof( cowd_sparse_file_header_t ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: invalid file header data value too small.",
			 function );

			return( -1 );
		}
		extent_file->file_type = LIBVMDK_FILE_TYPE_COWD_SPARSE_DATA;
	}
	else if( memory_compare(
	          file_header_data,
	          vmdk_sparse_file_signature,
	          4 ) == 0 )
	{
		if( file_header_data_size < sizeof( vmdk_sparse_file_header_t ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: invalid file header data value too small.",
			 function );

			return( -1 );
		}
		extent_file->file_type = LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file signature.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: file header:\n",
		 function );
		libcnotify_print_data(
		 file_header_data,
		 file_header_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( extent_file->file_type == LIBVMDK_FILE_TYPE_COWD_SPARSE_DATA )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header_data )->version,
		 extent_file->format_version );

		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header_data )->flags,
		 extent_file->flags );

		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header_data )->maximum_data_number_of_sectors,
		 extent_file->maximum_data_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header_data )->grain_number_of_sectors,
		 extent_file->grain_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header_data )->primary_grain_directory_sector_number,
		 extent_file->primary_grain_directory_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (cowd_sparse_file_header_t *) file_header_data )->number_of_grain_directory_entries,
		 extent_file->number_of_grain_directory_entries );
	}
	else if( extent_file->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header_data )->version,
		 extent_file->format_version );

		byte_stream_copy_to_uint32_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header_data )->flags,
		 extent_file->flags );

		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header_data )->maximum_data_number_of_sectors,
		 extent_file->maximum_data_size );

		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header_data )->grain_number_of_sectors,
		 extent_file->grain_size );

		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header_data )->descriptor_sector_number,
		 extent_file->descriptor_offset );

		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header_data )->descriptor_number_of_sectors,
		 extent_file->descriptor_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header_data )->number_of_grain_table_entries,
		 extent_file->number_of_grain_table_entries );

		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header_data )->secondary_grain_directory_sector_number,
		 extent_file->secondary_grain_directory_offset );

		byte_stream_copy_to_uint64_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header_data )->primary_grain_directory_sector_number,
		 extent_file->primary_grain_directory_offset );

		extent_file->is_dirty = ( (vmdk_sparse_file_header_t *) file_header_data )->is_dirty;

		byte_stream_copy_to_uint16_little_endian(
		 ( (vmdk_sparse_file_header_t *) file_header_data )->compression_method,
		 extent_file->compression_method );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t\t: %c%c%c%c\n",
		 function,
		 (char) file_header_data[ 0 ],
		 (char) file_header_data[ 1 ],
		 (char) file_header_data[ 2 ],
		 (char) file_header_data[ 3 ] );

		libcnotify_printf(
		 "%s: format version\t\t\t\t: %" PRIu32 "\n",
		 function,
		 extent_file->format_version );

		libcnotify_printf(
		 "%s: flags\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 extent_file->flags );

		if( extent_file->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
		{
			libvmdk_debug_print_vmdk_flags(
			 extent_file->flags );
		}
		libcnotify_printf(
		 "%s: maximum data number of sectors\t\t: %" PRIu64 "\n",
		 function,
		 extent_file->maximum_data_size );

		libcnotify_printf(
		 "%s: grain number of sectors\t\t\t: %" PRIu64 "\n",
		 function,
		 extent_file->grain_size );

		if( extent_file->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
		{
			libcnotify_printf(
			 "%s: descriptor sector number\t\t\t: %" PRIu64 "\n",
			 function,
			 extent_file->descriptor_offset );

			libcnotify_printf(
			 "%s: descriptor number of sectors\t\t\t: %" PRIu64 "\n",
			 function,
			 extent_file->descriptor_size );

			libcnotify_printf(
			 "%s: number of grain table entries\t\t: %" PRIu32 "\n",
			 function,
			 extent_file->number_of_grain_table_entries );

			libcnotify_printf(
			 "%s: secondary grain directory sector number\t: %" PRIu64 "\n",
			 function,
			 extent_file->secondary_grain_directory_offset );
		}
		libcnotify_printf(
		 "%s: primary grain directory sector number\t: %" PRIu64 "\n",
		 function,
		 extent_file->primary_grain_directory_offset );

		if( extent_file->file_type == LIBVMDK_FILE_TYPE_COWD_SPARSE_DATA )
		{
			libcnotify_printf(
			 "%s: padding:\n",
			 function );
			libcnotify_print_data(
			 (uint8_t *) ( (vmdk_sparse_file_header_t *) file_header_data )->padding,
			 433,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
		else if( extent_file->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (vmdk_sparse_file_header_t *) file_header_data )->metadata_number_of_sectors,
			 value_64bit );
			libcnotify_printf(
			 "%s: metadata number of sectors\t\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			libcnotify_printf(
			 "%s: is dirty\t\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 extent_file->is_dirty );

			libcnotify_printf(
			 "%s: single end of line character\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (vmdk_sparse_file_header_t *) file_header_data )->single_end_of_line_character );

			libcnotify_printf(
			 "%s: non end of line character\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (vmdk_sparse_file_header_t *) file_header_data )->non_end_of_line_character );

			libcnotify_printf(
			 "%s: first double end of line character\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (vmdk_sparse_file_header_t *) file_header_data )->first_double_end_of_line_character );

			libcnotify_printf(
			 "%s: second double end of line character\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (vmdk_sparse_file_header_t *) file_header_data )->second_double_end_of_line_character );

			libcnotify_printf(
			 "%s: compression method\t\t\t\t: %" PRIu16 "\n",
			 function,
			 extent_file->compression_method );

			libcnotify_printf(
			 "%s: padding:\n",
			 function );
			libcnotify_print_data(
			 ( (vmdk_sparse_file_header_t *) file_header_data )->padding,
			 433,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
	}
#endif
	if( extent_file->grain_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported grain number of sectors value is 0.",
		 function );

		return( -1 );
	}
	if( extent_file->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
	{
		if( extent_file->grain_size <= 8 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported grain number of sectors value is less than or equal to 8.",
			 function );

			return( -1 );
		}
		if( ( extent_file->grain_size % 2 ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported grain number of sectors value is not a power of 2.",
			 function );

			return( -1 );
		}
		if( extent_file->number_of_grain_table_entries == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported number of grain table entries value is 0.",
			 function );

			return( -1 );
		}
		if( (size_t) extent_file->number_of_grain_table_entries > (size_t) INT_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid number of grain table entries value exceeds maximum.",
			 function );

			return( -1 );
		}
	}
	if( ( extent_file->maximum_data_size % extent_file->grain_size ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported maximum data number of sectors not a multide of the grain number of sectors.",
		 function );

		return( -1 );
	}
	if( extent_file->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
	{
		if( ( (vmdk_sparse_file_header_t *) file_header_data )->single_end_of_line_character != (uint8_t) '\n' )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported single end of line character.",
			 function );

			return( -1 );
		}
		if( ( (vmdk_sparse_file_header_t *) file_header_data )->non_end_of_line_character != (uint8_t) ' ' )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported non end of line character.",
			 function );

			return( -1 );
		}
		if( ( (vmdk_sparse_file_header_t *) file_header_data )->first_double_end_of_line_character != (uint8_t) '\r' )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported first double end of line character.",
			 function );

			return( -1 );
		}
		if( ( (vmdk_sparse_file_header_t *) file_header_data )->second_double_end_of_line_character != (uint8_t) '\n' )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported second double end of line character.",
			 function );

			return( -1 );
		}
	}
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

		return( -1 );
	}
	/* Change all sector values to byte values
	 */
	extent_file->maximum_data_size              *= 512;
	extent_file->grain_size                     *= 512;
	extent_file->primary_grain_directory_offset *= 512;

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
		extent_file->descriptor_offset                *= 512;
		extent_file->descriptor_size                  *= 512;
		extent_file->secondary_grain_directory_offset *= 512;
	}
	if( extent_file->descriptor_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid descriptor size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( (size_t) extent_file->number_of_grain_directory_entries > (size_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of grain directory entries value exceeds maximum.",
		 function );

		return( -1 );
	}
#if SIZEOF_SIZE_T < 8
	if( (size_t) extent_file->number_of_grain_table_entries > (size_t) ( SSIZE_MAX / 4 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid grain table size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( (size_t) extent_file->number_of_grain_directory_entries > (size_t) ( SSIZE_MAX / 4 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid grain directory size value exceeds maximum.",
		 function );

		return( -1 );
	}
#endif
	extent_file->grain_table_size = (size_t) extent_file->number_of_grain_table_entries * 4;

	/* The grain table data is sector aligned
	 */
	if( ( extent_file->grain_table_size % 512 ) != 0 )
	{
		extent_file->grain_table_size /= 512;
		extent_file->grain_table_size += 1;
		extent_file->grain_table_size *= 512;
	}
	extent_file->grain_directory_size = (size_t) extent_file->number_of_grain_directory_entries * 4;

	/* The grain directory data is sector aligned
	 */
	if( ( extent_file->grain_directory_size % 512 ) != 0 )
	{
		extent_file->grain_directory_size /= 512;
		extent_file->grain_directory_size += 1;
		extent_file->grain_directory_size *= 512;
	}
	return( 1 );
}

/* Reads the descriptor data from the extent file
 * Returns 1 if successful, 0 if no descriptor data, or -1 on errror
 */
int libvmdk_extent_file_read_descriptor_data_file_io_handle(
     libvmdk_extent_file_t *extent_file,
     libbfio_handle_t *file_io_handle,
     uint8_t *descriptor_data,
     size_t descriptor_data_size,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_file_read_descriptor_data_file_io_handle";
	ssize_t read_count    = 0;

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
	if( descriptor_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor data.",
		 function );

		return( -1 );
	}
	if( descriptor_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid descriptor data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( descriptor_data_size < extent_file->descriptor_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid descriptor data value too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	 "%s: reading descriptor at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 extent_file->descriptor_offset,
		 extent_file->descriptor_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     extent_file->descriptor_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek descriptor offset: %" PRIi64 ".",
		 function,
		 extent_file->descriptor_offset );

		return( -1 );
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              descriptor_data,
	              (size_t) extent_file->descriptor_size,
	              error );

	if( read_count != (ssize_t) extent_file->descriptor_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read descriptor data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the grain directory
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_file_read_grain_directory(
     libvmdk_extent_file_t *extent_file,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     off64_t file_offset,
     libvmdk_grain_table_t *grain_table,
     libmfdata_list_t *grain_table_list,
     libcerror_error_t **error )
{
	uint8_t *grain_directory_data        = NULL;
	uint8_t *grain_directory_entry       = NULL;
	static char *function                = "libvmdk_extent_file_read_grain_directory";
	off64_t grain_table_offset           = 0;
	size64_t grain_directory_size        = 0;
	ssize_t read_count                   = 0;
	uint32_t grain_directory_entry_index = 0;
	uint32_t range_flags                 = 0;

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
		 "%s: reading grain directory at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	if( libbfio_pool_seek_offset(
	     file_io_pool,
	     file_io_pool_entry,
	     file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek grain directory offset: %" PRIi64 ".",
		 function,
		 file_offset );

		goto on_error;
	}
	grain_directory_data = (uint8_t *) memory_allocate(
	                                    sizeof( uint8_t ) * extent_file->grain_directory_size );

	if( grain_directory_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create grain directory data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              grain_directory_data,
	              extent_file->grain_directory_size,
	              error );

	if( read_count != (ssize_t) extent_file->grain_directory_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read grain directory data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: grain directory data:\n",
		 function );
		libcnotify_print_data(
		 grain_directory_data,
		 extent_file->grain_directory_size,
		 0 );
	}
#endif
	grain_directory_entry = grain_directory_data;

	for( grain_directory_entry_index = 0;
	     grain_directory_entry_index < extent_file->number_of_grain_directory_entries;
	     grain_directory_entry_index++ )
	{
		byte_stream_copy_to_uint32_little_endian(
		 grain_directory_entry,
		 grain_table_offset );
		 
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: grain directory entry: %" PRIu32 " sector number: %" PRIi64 " (offset: 0x%08" PRIx64 ") file IO pool entry: %d\n",
			 function,
			 grain_directory_entry_index,
			 grain_table_offset,
			 grain_table_offset * 512,
			 file_io_pool_entry );
		}
#endif
		if( grain_table_offset == 0 )
		{
			range_flags = LIBVMDK_RANGE_FLAG_IS_SPARSE;
		}
		else
		{
			grain_table_offset *= 512;
			range_flags         = 0;
		}
		grain_table->previous_last_grain_offset_filled = grain_table->last_grain_offset_filled;

		if( libmfdata_list_append_group(
		     grain_table_list,
		     &( grain_table->last_grain_offset_filled ),
		     (int) extent_file->number_of_grain_table_entries,
		     file_io_pool_entry,
		     grain_table_offset,
		     (size64_t) extent_file->grain_table_size,
		     range_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append grain offset group.",
			 function );

			goto on_error;
		}
		grain_table->last_grain_offset_filled += (int) extent_file->number_of_grain_directory_entries;

		grain_directory_entry += sizeof( uint32_t );
	}
/* TODO check if remainder of sector block is emtpy */

	memory_free(
	 grain_directory_data );

	grain_directory_data = NULL;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );

on_error:
	if( grain_directory_data != NULL )
	{
		memory_free(
		 grain_directory_data );
	}
	return( -1 );
}

/* Reads the backup grain directory
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_file_read_backup_grain_directory(
     libvmdk_extent_file_t *extent_file,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     off64_t file_offset,
     libvmdk_grain_table_t *grain_table,
     libmfdata_list_t *grain_table_list,
     libcerror_error_t **error )
{
	uint8_t *grain_directory_data        = NULL;
	uint8_t *grain_directory_entry       = NULL;
	static char *function                = "libvmdk_extent_file_read_backup_grain_directory";
	off64_t grain_table_offset           = 0;
	off64_t group_offset                 = 0;
	size64_t grain_directory_size        = 0;
	size64_t group_size                  = 0;
	ssize_t read_count                   = 0;
	uint32_t grain_directory_entry_index = 0;
	uint32_t group_flags                 = 0;
	uint32_t range_flags                 = 0;
	int group_number_of_entries          = 0;
	int group_file_io_pool_entry         = 0;

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
		 "%s: reading backup grain directory at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	if( libbfio_pool_seek_offset(
	     file_io_pool,
	     file_io_pool_entry,
	     file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek backup grain directory offset: %" PRIi64 ".",
		 function,
		 file_offset );

		goto on_error;
	}
	grain_directory_data = (uint8_t *) memory_allocate(
	                                    sizeof( uint8_t ) * extent_file->grain_directory_size );

	if( grain_directory_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create grain directory data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              grain_directory_data,
	              extent_file->grain_directory_size,
	              error );

	if( read_count != (ssize_t) extent_file->grain_directory_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read grain directory data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: grain directory data:\n",
		 function );
		libcnotify_print_data(
		 grain_directory_data,
		 extent_file->grain_directory_size,
		 0 );
	}
#endif
	grain_directory_entry = grain_directory_data;

	for( grain_directory_entry_index = 0;
	     grain_directory_entry_index < extent_file->number_of_grain_directory_entries;
	     grain_directory_entry_index++ )
	{
		byte_stream_copy_to_uint32_little_endian(
		 grain_directory_entry,
		 grain_table_offset );
		 
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: grain directory entry: %" PRIu32 " sector number: %" PRIi64 " (offset: 0x%08" PRIx64 ") file IO pool entry: %d\n",
			 function,
			 grain_directory_entry_index,
			 grain_table_offset,
			 grain_table_offset * 512,
			 file_io_pool_entry );
		}
#endif
		if( grain_table_offset == 0 )
		{
			range_flags = LIBVMDK_RANGE_FLAG_IS_SPARSE;
		}
		else
		{
			grain_table_offset *= 512;
			range_flags         = 0;
		}
		if( libmfdata_list_get_group_by_index(
		     grain_table_list,
		     grain_table->last_grain_offset_compared,
		     &group_number_of_entries,
		     &group_file_io_pool_entry,
		     &group_offset,
		     &group_size,
		     &group_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve grain offset group: %d - %d.",
			 function,
			 grain_table->last_grain_offset_compared,
			 grain_table->last_grain_offset_compared + (int) extent_file->number_of_grain_table_entries );

			goto on_error;
		}
		if( group_number_of_entries != (int) extent_file->number_of_grain_table_entries )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
			 "%s: mismatch between number of grain table entries in grain directory and backup.",
			 function );

			goto on_error;
		}
		if( libmfdata_list_set_backup_data_range_by_index(
		     grain_table_list,
		     grain_table->last_grain_offset_compared,
		     file_io_pool_entry,
		     grain_table_offset,
		     (size64_t) extent_file->grain_directory_size,
		     range_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set backup data range of grain offset group: %d - %d.",
			 function,
			 grain_table->last_grain_offset_compared,
			 grain_table->last_grain_offset_compared + (int) extent_file->number_of_grain_table_entries );

			goto on_error;
		}
		grain_table->last_grain_offset_compared += (int) extent_file->number_of_grain_table_entries;

		grain_directory_entry += sizeof( uint32_t );
	}
/* TODO check if remainder of sector block is emtpy */

	memory_free(
	 grain_directory_data );

	grain_directory_data = NULL;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );

on_error:
	if( grain_directory_data != NULL )
	{
		memory_free(
		 grain_directory_data );
	}
	return( -1 );
}

/* Reads the extent file
 * Callback function for the extent files list
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_file_read(
     intptr_t *io_handle LIBVMDK_ATTRIBUTE_UNUSED,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     libmfdata_file_t *file,
     libmfcache_cache_t *cache,
     uint8_t read_flags LIBVMDK_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libvmdk_extent_file_t *extent_file = NULL;
	static char *function              = "libvmdk_extent_file_read";
	size64_t extent_file_size          = 0;

	LIBVMDK_UNREFERENCED_PARAMETER( io_handle )
	LIBVMDK_UNREFERENCED_PARAMETER( read_flags )

	if( libvmdk_extent_file_initialize(
	     &extent_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create extent file.",
		 function );

		goto on_error;
	}
	if( libbfio_pool_get_size(
	     file_io_pool,
	     file_io_pool_entry,
	     &extent_file_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of file IO pool entry: %d.",
		 function,
		 file_io_pool_entry );

		goto on_error;
	}
	if( libvmdk_extent_file_read_file_header(
	     extent_file,
	     file_io_pool,
	     file_io_pool_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read extent file header from file IO pool entry: %d.",
		 function,
		 file_io_pool_entry );

		goto on_error;
	}
/* TODO */
	if( libmfdata_file_set_file_value(
	     file,
	     cache,
	     (intptr_t *) extent_file,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libvmdk_extent_file_free,
	     LIBMFDATA_FILE_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set extent file as file value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( extent_file != NULL )
	{
		libvmdk_extent_file_free(
		 &extent_file,
		 NULL );
	}
	return( -1 );
}

