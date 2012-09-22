/*
 * Descriptor file functions
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
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#include "libvmdk_definitions.h"
#include "libvmdk_descriptor_file.h"
#include "libvmdk_extent_descriptor.h"
#include "libvmdk_libcdata.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libcsplit.h"
#include "libvmdk_libcstring.h"
#include "libvmdk_libfvalue.h"

const char vmdk_descriptor_file_signature[ 21 ]                       = "# Disk DescriptorFile";
const char vmdk_descriptor_file_extent_section_signature[ 20 ]        = "# Extent description";
const char vmdk_descriptor_file_disk_database_section_signature[ 20 ] = "# The Disk Data Base";

/* Creates the descriptor file
 * Returns 1 if successful or -1 on error
 */
int libvmdk_descriptor_file_initialize(
     libvmdk_descriptor_file_t **descriptor_file,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_descriptor_file_initialize";

	if( descriptor_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor file.",
		 function );

		return( -1 );
	}
	if( *descriptor_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid descriptor file value already set.",
		 function );

		return( -1 );
	}
	*descriptor_file = memory_allocate_structure(
	                    libvmdk_descriptor_file_t );

	if( *descriptor_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create descriptor file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *descriptor_file,
	     0,
	     sizeof( libvmdk_descriptor_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear descriptor file.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *descriptor_file )->extents_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create extents array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *descriptor_file != NULL )
	{
		memory_free(
		 *descriptor_file );

		*descriptor_file = NULL;
	}
	return( -1 );
}

/* Frees the descriptor file
 * Returns 1 if successful or -1 on error
 */
int libvmdk_descriptor_file_free(
     libvmdk_descriptor_file_t **descriptor_file,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_descriptor_file_free";
	int result            = 1;

	if( descriptor_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor file.",
		 function );

		return( -1 );
	}
	if( *descriptor_file != NULL )
	{
		if( libcdata_array_free(
		     &( ( *descriptor_file )->extents_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libvmdk_extent_descriptor_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free extents array.",
			 function );

			result = -1;
		}
		memory_free(
		 *descriptor_file );

		*descriptor_file = NULL;
	}
	return( result );
}

/* Reads the descriptor file
 * Returns the 1 if succesful or -1 on error
 */
int libvmdk_descriptor_file_read(
     libvmdk_descriptor_file_t *descriptor_file,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     libcerror_error_t **error )
{
	libcsplit_narrow_split_string_t *lines = NULL;
	uint8_t *file_data                     = NULL;
	static char *function                  = "libvmdk_descriptor_file_read";
	size64_t file_size                     = 0;
	ssize_t read_count                     = 0;
	int line_index                         = 0;
	int number_of_lines                    = 0;

	if( descriptor_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor file.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_get_size(
	     file_io_pool,
	     file_io_pool_entry,
	     &file_size,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine size of file IO pool entry: %d.",
		 function,
		 file_io_pool_entry );

		goto on_error;
	}
	if( file_size > (size64_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid file size value exceeds maximum.",
		 function );

		goto on_error;
	}
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
		 "%s: unable to seek offset: 0 in file IO pool entry: %d.",
		 function,
		 file_io_pool_entry );

		goto on_error;
	}
	file_data = (uint8_t *) memory_allocate(
	                         sizeof( uint8_t ) * file_size );

	if( file_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read(
	              file_io_pool,
	              file_io_pool_entry,
	              file_data,
	              (size_t) file_size,
	              error );

	if( read_count != (ssize_t) file_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read data of file IO pool entry: %d.",
		 function,
		 file_io_pool_entry );

		goto on_error;
	}
	if( libcsplit_narrow_string_split(
	     file_data,
	     (size_t) file_size,
	     '\n',
	     &lines,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split file data into lines.",
		 function );

		goto on_error;
	}
/* TODO pass mgt of file data onto lines ? */
	memory_free(
	 file_data );

	file_data = NULL;

	if( libcsplit_narrow_split_string_get_number_of_segments(
	     lines,
	     &number_of_lines,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to retrieve number of lines.",
		 function );

		goto on_error;
	}
	if( libvmdk_descriptor_file_read_header(
	     descriptor_file,
	     lines,
	     number_of_lines,
	     &line_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read descriptor file header.",
		 function );

		goto on_error;
	}
/* TODO */

	if( libcsplit_narrow_split_string_free(
	     &lines,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free lines.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( lines != NULL )
	{
		libcsplit_narrow_split_string_free(
		 &lines,
		 NULL );
	}
	if( file_data != NULL )
	{
		memory_free(
		 file_data );
	}
	return( -1 );
}

/* Reads the header from the descriptor file
 * Returns the 1 if succesful or -1 on error
 */
int libvmdk_descriptor_file_read_header(
     libvmdk_descriptor_file_t *descriptor_file,
     libcsplit_narrow_split_string_t *lines,
     int number_of_lines,
     int *line_index,
     libcerror_error_t **error )
{
	char *line_string_segment        = NULL;
	char *value_identifier           = NULL;
	static char *function            = "libvmdk_descriptor_file_read_header";
	size_t line_string_segment_index = 0;
	size_t line_string_segment_size  = 0;
	size_t value_identifier_length   = 0;
	size_t value_index               = 0;
	size_t value_length              = 0;
	uint64_t value_64bit             = 0;

	if( descriptor_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor file.",
		 function );

		return( -1 );
	}
	if( line_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line index.",
		 function );

		return( -1 );
	}
	*line_index = 0;

	if( libcsplit_narrow_split_string_get_segment_by_index(
	     lines,
	     *line_index,
	     &line_string_segment,
	     &line_string_segment_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line: %d.",
		 function,
		 *line_index );

		return( -1 );
	}
	if( line_string_segment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing line string segment: %d.",
		 function,
		 *line_index );

		return( -1 );
	}
	if( ( line_string_segment_size != 22 )
	 || ( libcstring_narrow_string_compare(
	       line_string_segment,
	       vmdk_descriptor_file_signature,
	       21 ) != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported descriptor file signature.",
		 function );

		return( -1 );
	}
	for( *line_index = 1;
	     *line_index < number_of_lines;
	     *line_index += 1 )
	{
		if( libcsplit_narrow_split_string_get_segment_by_index(
		     lines,
		     *line_index,
		     &line_string_segment,
		     &line_string_segment_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve line: %d.",
			 function,
			 *line_index );

			return( -1 );
		}
		if( line_string_segment == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing line string segment: %d.",
			 function,
			 *line_index );

			return( -1 );
		}
		if( line_string_segment_size == 21 )
		{
			/* Check for the end of the header
			 */
			if( libcstring_narrow_string_compare(
			     line_string_segment,
			     vmdk_descriptor_file_extent_section_signature,
			     20 ) == 0 )
			{
				break;
			}
		}
		/* Skip leading white space
		 */
		for( line_string_segment_index = 0;
		     line_string_segment_index < line_string_segment_size;
		     line_string_segment_index++ )
		{
			if( ( line_string_segment[ line_string_segment_index ] != '\t' )
			 && ( line_string_segment[ line_string_segment_index ] != '\n' )
			 && ( line_string_segment[ line_string_segment_index ] != '\f' )
			 && ( line_string_segment[ line_string_segment_index ] != '\v' )
			 && ( line_string_segment[ line_string_segment_index ] != '\r' )
			 && ( line_string_segment[ line_string_segment_index ] != ' ' ) )
			{
				break;
			}
		}
		/* Skip an empty line
		 */
		if( line_string_segment[ line_string_segment_index ] == 0 )
		{
			continue;
		}
		/* Determine the value identifier
		 */
		value_identifier        = &( line_string_segment[ line_string_segment_index ] );
		value_identifier_length = 0;

		while( line_string_segment_index < line_string_segment_size )
		{
			if( line_string_segment[ line_string_segment_index ] == '=' )
			{
				break;
			}
			value_identifier_length++;

			line_string_segment_index++;
		}
		/* Make sure the value identifier is terminated by an end of string
		 */
		line_string_segment[ line_string_segment_index ] = 0;

		/* Determine the value
		 */
		line_string_segment_index++;

		value_index  = line_string_segment_index;
		value_length = 0;

		/* Ingore quotes at the beginning of the value data
		 */
		if( ( line_string_segment[ line_string_segment_index ] == '"' )
		 || ( line_string_segment[ line_string_segment_index ] == '\'' ) )
		{
			value_index++;
		}
		while( line_string_segment_index < line_string_segment_size )
		{
			if( ( line_string_segment[ line_string_segment_index ] == 0 )
			 || ( line_string_segment[ line_string_segment_index ] == '\t' )
			 || ( line_string_segment[ line_string_segment_index ] == '\n' )
			 || ( line_string_segment[ line_string_segment_index ] == '\f' )
			 || ( line_string_segment[ line_string_segment_index ] == '\v' )
			 || ( line_string_segment[ line_string_segment_index ] == '\r' ) )
			{
				break;
			}
			value_length++;

			line_string_segment_index++;
		}
		/* Ingore quotes at the end of the value data
		 */
		if( ( line_string_segment[ line_string_segment_index ] == '"' )
		 || ( line_string_segment[ line_string_segment_index ] == '\'' ) )
		{
			value_length--;

			line_string_segment_index--;
		}
		/* Make sure the value is terminated by an end of string
		 */
		line_string_segment[ line_string_segment_index ] = 0;

		if( value_identifier_length == 3 )
		{
			if( libcstring_narrow_string_compare(
			     value_identifier,
			     "CID",
			     3 ) == 0 )
			{
				if( libfvalue_utf8_string_with_index_copy_to_integer(
				     line_string_segment,
				     value_index,
				     value_index + value_length,
				     &value_64bit,
				     64,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL | LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to determine content identifier value from string.",
					 function );

					return( -1 );
				}
				if( value_64bit > (uint64_t) INT32_MAX )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
					 "%s: invalid content identifier value exceeds maximum.",
					 function );

					return( -1 );
				}
				descriptor_file->content_identifier = (uint32_t) value_64bit;
			}
		}
		else if( value_identifier_length == 7 )
		{
			if( libcstring_narrow_string_compare(
			     value_identifier,
			     "version",
			     7 ) == 0 )
			{
				if( libfvalue_utf8_string_with_index_copy_to_integer(
				     line_string_segment,
				     value_index,
				     value_index + value_length,
				     &value_64bit,
				     64,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to determine version value from string.",
					 function );

					return( -1 );
				}
				if( value_64bit > (uint64_t) INT_MAX )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
					 "%s: invalid version value exceeds maximum.",
					 function );

					return( -1 );
				}
				descriptor_file->version = (int) value_64bit;
			}
		}
		else if( value_identifier_length == 9 )
		{
			if( libcstring_narrow_string_compare(
			     value_identifier,
			     "parentCID",
			     9 ) == 0 )
			{
				if( libfvalue_utf8_string_with_index_copy_to_integer(
				     line_string_segment,
				     value_index,
				     value_index + value_length,
				     &value_64bit,
				     64,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL | LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to determine parent content identifier value from string.",
					 function );

					return( -1 );
				}
				if( value_64bit > (uint64_t) INT32_MAX )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
					 "%s: invalid content parent identifier value exceeds maximum.",
					 function );

					return( -1 );
				}
				descriptor_file->parent_content_identifier = (uint32_t) value_64bit;
			}
		}
		else if( value_identifier_length == 10 )
		{
			if( libcstring_narrow_string_compare(
			     value_identifier,
			     "createType",
			     10 ) == 0 )
			{
				if( value_length == 6 )
				{
					if( libcstring_narrow_string_compare(
					     &( line_string_segment[ value_index ] ),
					     "custom",
					     6 ) == 0 )
					{
						descriptor_file->disk_type = LIBVMDK_DISK_TYPE_CUSTOM;
					}
				}
				else if( value_length == 7 )
				{
					if( libcstring_narrow_string_compare(
					     &( line_string_segment[ value_index ] ),
					     "vmfsRaw",
					     7 ) == 0 )
					{
						descriptor_file->disk_type = LIBVMDK_DISK_TYPE_VMFS_RAW;
					}
					else if( libcstring_narrow_string_compare(
					          &( line_string_segment[ value_index ] ),
					          "vmfsRDM",
					          7 ) == 0 )
					{
						descriptor_file->disk_type = LIBVMDK_DISK_TYPE_VMFS_RDM;
					}
				}
				else if( value_length == 8 )
				{
					if( libcstring_narrow_string_compare(
					     &( line_string_segment[ value_index ] ),
					     "vmfsRDMP",
					     8 ) == 0 )
					{
						descriptor_file->disk_type = LIBVMDK_DISK_TYPE_VMFS_RDMP;
					}
					else if( libcstring_narrow_string_compare(
					          &( line_string_segment[ value_index ] ),
					          "vmfsThin",
					          8 ) == 0 )
					{
						descriptor_file->disk_type = LIBVMDK_DISK_TYPE_VMFS_THIN;
					}
				}
				else if( value_length == 10 )
				{
					if( libcstring_narrow_string_compare(
					     &( line_string_segment[ value_index ] ),
					     "fullDevice",
					     10 ) == 0 )
					{
						descriptor_file->disk_type = LIBVMDK_DISK_TYPE_DEVICE;
					}
					else if( libcstring_narrow_string_compare(
					          &( line_string_segment[ value_index ] ),
					          "vmfsSparse",
					          10 ) == 0 )
					{
						descriptor_file->disk_type = LIBVMDK_DISK_TYPE_VMFS_SPARSE;
					}
				}
				else if( value_length == 14 )
				{
					if( libcstring_narrow_string_compare(
					     &( line_string_segment[ value_index ] ),
					     "monolithicFlat",
					     14 ) == 0 )
					{
						descriptor_file->disk_type = LIBVMDK_DISK_TYPE_MONOLITHIC_FLAT;
					}
				}
				else if( value_length == 15 )
				{
					if( libcstring_narrow_string_compare(
					     &( line_string_segment[ value_index ] ),
					     "streamOptimized",
					     15 ) == 0 )
					{
						descriptor_file->disk_type = LIBVMDK_DISK_TYPE_STREAM_OPTIMIZED;
					}
				}
				else if( value_length == 16 )
				{
					if( libcstring_narrow_string_compare(
					     &( line_string_segment[ value_index ] ),
					     "2GbMaxExtentFlat",
					     16 ) == 0 )
					{
						descriptor_file->disk_type = LIBVMDK_DISK_TYPE_2GB_EXTENT_FLAT;
					}
					else if( libcstring_narrow_string_compare(
					          &( line_string_segment[ value_index ] ),
					          "monolithicSparse",
					          16 ) == 0 )
					{
						descriptor_file->disk_type = LIBVMDK_DISK_TYPE_MONOLITHIC_SPARSE;
					}
					else if( libcstring_narrow_string_compare(
					          &( line_string_segment[ value_index ] ),
					          "partitionedDevice",
					          16 ) == 0 )
					{
						descriptor_file->disk_type = LIBVMDK_DISK_TYPE_DEVICE_PARITIONED;
					}
					else if( libcstring_narrow_string_compare(
					          &( line_string_segment[ value_index ] ),
					          "vmfsPreallocated",
					          16 ) == 0 )
					{
						descriptor_file->disk_type = LIBVMDK_DISK_TYPE_VMFS_FLAT;
					}
				}
				else if( value_length == 18 )
				{
					if( libcstring_narrow_string_compare(
					     &( line_string_segment[ value_index ] ),
					     "2GbMaxExtentSparse",
					     18 ) == 0 )
					{
						descriptor_file->disk_type = LIBVMDK_DISK_TYPE_2GB_EXTENT_SPARSE;
					}
				}
				else if( value_length == 20 )
				{
					if( libcstring_narrow_string_compare(
					     &( line_string_segment[ value_index ] ),
					     "vmfsEagerZeroedThick",
					     20 ) == 0 )
					{
						descriptor_file->disk_type = LIBVMDK_DISK_TYPE_VMFS_FLAT_ZEROED;
					}
				}
			}
		}
		else if( value_identifier_length == 18 )
		{
			if( libcstring_narrow_string_compare(
			     value_identifier,
			     "parentFileNameHint",
			     18 ) == 0 )
			{
/* TODO */
			}
		}
	}
	return( 1 );
}

/* Reads the extents from the descriptor file
 * Returns the 1 if succesful or -1 on error
 */
int libvmdk_descriptor_file_read_extents(
     libvmdk_descriptor_file_t *descriptor_file,
     libcsplit_narrow_split_string_t *lines,
     int number_of_lines,
     int *line_index,
     libcerror_error_t **error )
{
	libvmdk_extent_descriptor_t *extent_descriptor = NULL;
	char *line_string_segment                      = NULL;
	static char *function                          = "libvmdk_descriptor_file_read_extents";
	size_t line_string_segment_index               = 0;
	size_t line_string_segment_length              = 0;
	size_t line_string_segment_size                = 0;
	int entry_index                                = 0;

	if( descriptor_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor file.",
		 function );

		return( -1 );
	}
	if( line_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line index.",
		 function );

		return( -1 );
	}
	if( number_of_lines <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of lines value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( *line_index != 0 )
	 || ( *line_index >= number_of_lines ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid line index value out of bounds.",
		 function );

		return( -1 );
	}
	if( libcsplit_narrow_split_string_get_segment_by_index(
	     lines,
	     *line_index,
	     &line_string_segment,
	     &line_string_segment_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line: %d.",
		 function,
		 *line_index );

		goto on_error;
	}
	if( line_string_segment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing line string segment: %d.",
		 function,
		 *line_index );

		goto on_error;
	}
	if( ( line_string_segment_size != 21 )
	 || ( libcstring_narrow_string_compare(
	       line_string_segment,
	       vmdk_descriptor_file_extent_section_signature,
	       21 ) != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported extent section signature.",
		 function );

		goto on_error;
	}
	if( libcdata_array_emtpy(
	     descriptor_file->extents_array,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libvmdk_extent_descriptor_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty extents array.",
		 function );

		goto on_error;
	}
	for( *line_index = 1;
	     *line_index < number_of_lines;
	     *line_index += 1 )
	{
		if( libcsplit_narrow_split_string_get_segment_by_index(
		     lines,
		     *line_index,
		     &line_string_segment,
		     &line_string_segment_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve line: %d.",
			 function,
			 *line_index );

			goto on_error;
		}
		if( line_string_segment == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing line string segment: %d.",
			 function,
			 *line_index );

			goto on_error;
		}
		if( line_string_segment_size == 21 )
		{
			/* Check for the end of the section
			 */
			if( libcstring_narrow_string_compare(
			     line_string_segment,
			     vmdk_descriptor_file_disk_database_section_signature,
			     20 ) == 0 )
			{
				break;
			}
		}
		/* Skip leading white space
		 */
		for( line_string_segment_index = 0;
		     line_string_segment_index < line_string_segment_size;
		     line_string_segment_index++ )
		{
			if( ( line_string_segment[ line_string_segment_index ] != '\t' )
			 && ( line_string_segment[ line_string_segment_index ] != '\n' )
			 && ( line_string_segment[ line_string_segment_index ] != '\f' )
			 && ( line_string_segment[ line_string_segment_index ] != '\v' )
			 && ( line_string_segment[ line_string_segment_index ] != '\r' )
			 && ( line_string_segment[ line_string_segment_index ] != ' ' ) )
			{
				break;
			}
		}
		/* Skip an empty line
		 */
		if( line_string_segment[ line_string_segment_index ] == 0 )
		{
			continue;
		}
		/* Ingore white space at the end of the string
		 */
		while( line_string_segment_index < line_string_segment_size )
		{
			if( ( line_string_segment[ line_string_segment_size - 1 ] == 0 )
			 || ( line_string_segment[ line_string_segment_size - 1 ] == '\t' )
			 || ( line_string_segment[ line_string_segment_size - 1 ] == '\n' )
			 || ( line_string_segment[ line_string_segment_size - 1 ] == '\f' )
			 || ( line_string_segment[ line_string_segment_size - 1 ] == '\v' )
			 || ( line_string_segment[ line_string_segment_size - 1 ] == '\r' )
			 || ( line_string_segment[ line_string_segment_size - 1 ] == ' ' ) )
			{
				break;
			}
			line_string_segment_size--;
		}
		/* Make sure the string is terminated by an end of string
		 */
		line_string_segment[ line_string_segment_size - 1 ] = 0;

		if( libvmdk_extent_descriptor_initialize(
		     &extent_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create extent descriptor.",
			 function );

			goto on_error;
		}
		if( libvmdk_extent_descriptor_read(
		     extent_descriptor,
		     &( line_string_segment[ line_string_segment_index ] ),
		     line_string_segment_size - line_string_segment_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read extent descriptor from line: %d.",
			 function,
			 *line_index );

			goto on_error;
		}
		if( libcdata_array_append_entry(
		     descriptor_file->extents_array,
		     &entry_index,
		     (intptr_t *) extent_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append extent descriptor to extents array.",
			 function );

			goto on_error;
		}
		extent_descriptor = NULL;
	}
	return( 1 );

on_error:
	if( extent_descriptor != NULL )
	{
		libvmdk_extent_descriptor_free(
		 &extent_descriptor,
		 NULL );
	}
	libcdata_array_empty(
	 descriptor_file->extents_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libvmdk_extent_descriptor_free,
	 NULL );

	return( -1 );
}

/* Reads the disk database from the descriptor file
 * Returns the 1 if succesful or -1 on error
 */
int libvmdk_descriptor_file_read_disk_database(
     libvmdk_descriptor_file_t *descriptor_file,
     libcsplit_narrow_split_string_t *lines,
     int number_of_lines,
     int *line_index,
     libcerror_error_t **error )
{
	char *line_string_segment        = NULL;
	char *value_identifier           = NULL;
	static char *function            = "libvmdk_descriptor_file_read_disk_database";
	size_t line_string_segment_index = 0;
	size_t line_string_segment_size  = 0;
	size_t value_identifier_length   = 0;
	size_t value_index               = 0;
	size_t value_length              = 0;
	uint64_t value_64bit             = 0;

	if( descriptor_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor file.",
		 function );

		return( -1 );
	}
	if( line_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line index.",
		 function );

		return( -1 );
	}
	if( number_of_lines <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of lines value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( *line_index != 0 )
	 || ( *line_index >= number_of_lines ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid line index value out of bounds.",
		 function );

		return( -1 );
	}
	if( libcsplit_narrow_split_string_get_segment_by_index(
	     lines,
	     *line_index,
	     &line_string_segment,
	     &line_string_segment_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve line: %d.",
		 function,
		 *line_index );

		return( -1 );
	}
	if( line_string_segment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing line string segment: %d.",
		 function,
		 *line_index );

		return( -1 );
	}
	if( ( line_string_segment_size != 21 )
	 || ( libcstring_narrow_string_compare(
	       line_string_segment,
	       vmdk_descriptor_file_disk_database_section_signature,
	       21 ) != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported disk database section signature.",
		 function );

		return( -1 );
	}
	for( *line_index = 1;
	     *line_index < number_of_lines;
	     *line_index += 1 )
	{
		if( libcsplit_narrow_split_string_get_segment_by_index(
		     lines,
		     *line_index,
		     &line_string_segment,
		     &line_string_segment_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve line: %d.",
			 function,
			 *line_index );

			return( -1 );
		}
		if( line_string_segment == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing line string segment: %d.",
			 function,
			 *line_index );

			return( -1 );
		}
		/* Skip leading white space
		 */
		for( line_string_segment_index = 0;
		     line_string_segment_index < line_string_segment_size;
		     line_string_segment_index++ )
		{
			if( ( line_string_segment[ line_string_segment_index ] != '\t' )
			 && ( line_string_segment[ line_string_segment_index ] != '\n' )
			 && ( line_string_segment[ line_string_segment_index ] != '\f' )
			 && ( line_string_segment[ line_string_segment_index ] != '\v' )
			 && ( line_string_segment[ line_string_segment_index ] != '\r' )
			 && ( line_string_segment[ line_string_segment_index ] != ' ' ) )
			{
				break;
			}
		}
		/* Skip an empty line
		 */
		if( line_string_segment[ line_string_segment_index ] == 0 )
		{
			continue;
		}
		/* Determine the value identifier
		 */
		value_identifier        = &( line_string_segment[ line_string_segment_index ] );
		value_identifier_length = 0;

		while( line_string_segment_index < line_string_segment_size )
		{
			if( line_string_segment[ line_string_segment_index ] == '=' )
			{
				break;
			}
			value_identifier_length++;

			line_string_segment_index++;
		}
		/* Make sure the value identifier is terminated by an end of string
		 */
		line_string_segment[ line_string_segment_index ] = 0;

		/* Determine the value
		 */
		line_string_segment_index++;

		value_index  = line_string_segment_index;
		value_length = 0;

		/* Ingore quotes at the beginning of the value data
		 */
		if( ( line_string_segment[ line_string_segment_index ] == '"' )
		 || ( line_string_segment[ line_string_segment_index ] == '\'' ) )
		{
			value_index++;
		}
		while( line_string_segment_index < line_string_segment_size )
		{
			if( ( line_string_segment[ line_string_segment_index ] == 0 )
			 || ( line_string_segment[ line_string_segment_index ] == '\t' )
			 || ( line_string_segment[ line_string_segment_index ] == '\n' )
			 || ( line_string_segment[ line_string_segment_index ] == '\f' )
			 || ( line_string_segment[ line_string_segment_index ] == '\v' )
			 || ( line_string_segment[ line_string_segment_index ] == '\r' ) )
			{
				break;
			}
			value_length++;

			line_string_segment_index++;
		}
		/* Ingore quotes at the end of the value data
		 */
		if( ( line_string_segment[ line_string_segment_index ] == '"' )
		 || ( line_string_segment[ line_string_segment_index ] == '\'' ) )
		{
			value_length--;

			line_string_segment_index--;
		}
		/* Make sure the value is terminated by an end of string
		 */
		line_string_segment[ line_string_segment_index ] = 0;
/* TODO */
	}
	return( 1 );
}

