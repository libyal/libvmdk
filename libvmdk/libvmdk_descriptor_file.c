/*
 * Descriptor file functions
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
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

#if defined( HAVE_ERRNO_H ) || defined( WINAPI )
#include <errno.h>
#endif

#include <stdio.h>

#include "libvmdk_descriptor_file.h"

/* Creates the descriptor file
 * Returns 1 if successful or -1 on error
 */
int libvmdk_descriptor_file_initialize(
     libvmdk_descriptor_file_t **descriptor_file,
     liberror_error_t **error )
{
	static char *function = "libvmdk_descriptor_file_initialize";

	if( descriptor_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor file.",
		 function );

		return( -1 );
	}
	if( *descriptor_file == NULL )
	{
		*descriptor_file = (libvmdk_descriptor_file_t *) memory_allocate(
		                                                  sizeof( libvmdk_descriptor_file_t ) );

		if( *descriptor_file == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create descriptor file.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *descriptor_file,
		     0,
		     sizeof( libvmdk_descriptor_file_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear descriptor file.",
			 function );

			memory_free(
			 *descriptor_file );

			*descriptor_file = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees the descriptor file
 * Returns 1 if successful or -1 on error
 */
int libvmdk_descriptor_file_free(
     libvmdk_descriptor_file_t **descriptor_file,
     liberror_error_t **error )
{
	static char *function = "libvmdk_descriptor_file_free";
	int result            = 1;

	if( descriptor_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor file.",
		 function );

		return( -1 );
	}
	if( *descriptor_file != NULL )
	{
		if( ( *descriptor_file )->name != NULL )
		{
			memory_free(
			 ( *descriptor_file )->name );
		}
		memory_free(
		 *descriptor_file );

		*descriptor_file = NULL;
	}
	return( result );
}

/* Sets the filename
 * Returns 1 if successful or -1 on error
 */
int libvmdk_descriptor_file_set_name(
     libvmdk_descriptor_file_t *descriptor_file,
     const libcstring_system_character_t *name,
     size_t name_length,
     liberror_error_t **error )
{
	static char *function = "libvmdk_descriptor_file_set_name";

	if( descriptor_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor file.",
		 function );

		return( -1 );
	}
	if( name == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name.",
		 function );

		return( -1 );
	}
	if( name_length > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid name length value exceeds maximum.",
		 function );

		return( -1 );
	}
	descriptor_file->name = (libcstring_system_character_t *) memory_allocate(
	                                                           sizeof( libcstring_system_character_t ) * ( name_length + 1 ) );

	if( descriptor_file->name == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to allocate name.",
		 function );

		return( -1 );
	}
	if( libcstring_system_string_copy(
	     descriptor_file->name,
	     name,
	     name_length ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set name.",
		 function );

		return( -1 );
	}
	descriptor_file->name[ name_length ] = 0;

	descriptor_file->name_size = name_length + 1;

	return( 1 );
}

/* Opens a descriptor file
 * Returns 1 if successful or -1 on error
 */
int libvmdk_descriptor_file_open(
     libvmdk_descriptor_file_t *descriptor_file,
     const libcstring_system_character_t *mode,
     liberror_error_t **error )
{
	static char *function = "libvmdk_descriptor_file_open";

	if( descriptor_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor file.",
		 function );

		return( -1 );
	}
	if( descriptor_file->name == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid descriptor file - missing name.",
		 function );

		return( -1 );
	}
	if( descriptor_file->file_stream != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid descriptor file - file stream already set.",
		 function );

		return( -1 );
	}
	if( mode == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mode.",
		 function );

		return( -1 );
	}
#if defined( LIBSMRAW_HAVE_WIDE_SYSTEM_CHARACTER )
	descriptor_file->file_stream = file_stream_open_wide(
	                                descriptor_file->name,
	                                mode );
#else
	descriptor_file->file_stream = file_stream_open(
	                                descriptor_file->name,
	                                mode );
#endif

	if( descriptor_file->file_stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 descriptor_file->name );

		return( -1 );
	}
	return( 1 );
}

/* Closes the descriptor file
 * Returns the 0 if succesful or -1 on error
 */
int libvmdk_descriptor_file_close(
     libvmdk_descriptor_file_t *descriptor_file,
     liberror_error_t **error )
{
	static char *function = "libvmdk_descriptor_file_close";

	if( descriptor_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor file.",
		 function );

		return( -1 );
	}
	if( descriptor_file->name != NULL )
	{
		memory_free(
		 descriptor_file->name );

		descriptor_file->name = NULL;
	}
	if( descriptor_file->file_stream != NULL )
	{
		if( file_stream_close(
		     descriptor_file->file_stream ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file stream.",
			 function );

			return( -1 );
		}
		descriptor_file->file_stream = NULL;
	}
	return( 0 );
}

/* Reads the header from the descriptor file
 * Returns the 1 if succesful or -1 on error
 */
int libvmdk_descriptor_file_read_header(
     libvmdk_descriptor_file_t *descriptor_file,
     liberror_error_t **error )
{
	uint8_t input_string[ 128 ];

	uint8_t *value_identifier      = NULL;
	uint8_t *value                 = NULL;
	char *result_string            = NULL;
	static char *function          = "libvmdk_descriptor_file_read_header";
	size_t input_string_index      = 0;
	size_t value_identifier_length = 0;
	size_t value_length            = 0;
	uint8_t in_header              = 0;

	if( descriptor_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor file.",
		 function );

		return( -1 );
	}
	if( descriptor_file->file_stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid descriptor file - missing file stream.",
		 function );

		return( -1 );
	}
	/* Reset the offset to start of the file stream
	 */
	if( file_stream_seek_offset(
	     descriptor_file->file_stream,
	     0,
	     SEEK_SET ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek start of stream.",
		 function );

		return( -1 );
	}
	result_string = file_stream_get_string(
			 descriptor_file->file_stream,
			 (char *) input_string,
			 128 );

	if( result_string == NULL )
	{
		if( file_stream_at_end(
		     descriptor_file->file_stream ) != 0 )
		{
			break;
		}
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: error reading string from file stream.",
		 function );

		return( -1 );
	}
	/* Check for the start of the header
	 */
	if( libcstring_narrow_string_compare(
	     (char *) input_string,
	     "# Disk DescriptorFile",
	     21 ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file signature.",
		 function );

		return( -1 );
	}
	while( file_stream_at_end(
	        descriptor_file->file_stream ) == 0 )
	{
		result_string = file_stream_get_string(
		                 descriptor_file->file_stream,
		                 (char *) input_string,
		                 128 );

		if( result_string == NULL )
		{
			if( file_stream_at_end(
			     descriptor_file->file_stream ) != 0 )
			{
				break;
			}
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: error reading string from file stream.",
			 function );

			return( -1 );
		}
		/* Check for the end of the header
		 */
		if( input_string[ 0 ] == (uint8_t) '#' )
		{
			if( libcstring_narrow_string_compare(
			     (char *) input_string,
			     "# Extent description",
			     20 ) == 0 )
			{
				break;
			}
		}
		/* Skip leading white space
		 */
		for( input_string_index = 0;
		     input_string_index < 128;
		     input_string_index++ )
		{
			if( ( input_string[ input_string_index ] != (uint8_t) '\t' )
			 && ( input_string[ input_string_index ] != (uint8_t) '\n' )
			 && ( input_string[ input_string_index ] != (uint8_t) '\f' )
			 && ( input_string[ input_string_index ] != (uint8_t) '\v' )
			 && ( input_string[ input_string_index ] != (uint8_t) '\r' )
			 && ( input_string[ input_string_index ] != (uint8_t) ' ' ) )
			{
				break;
			}
		}
		/* Skip an empty line
		 */
		if( input_string[ input_string_index ] == 0 )
		{
			continue;
		}
		/* Determine the value identifier
		 */
		value_identifier        = &( input_string[ input_string_index ] );
		value_identifier_length = 0;

		while( input_string_index < 128 )
		{
			if( input_string[ input_string_index ] == (uint8_t) '=' )
			{
				break;
			}
			value_identifier_length++;

			input_string_index++;
		}
		/* Make sure the value identifier is terminated by an end of string
		 */
		input_string[ input_string_index ] = 0;

		/* Determine the value
		 */
		input_string_index++;

		value        = &( input_string[ input_string_index ] );
		value_length = 0;

		/* TODO handle quotes */
		while( input_string_index < 128 )
		{
			if( ( input_string[ input_string_index ] == 0 )
			 || ( input_string[ input_string_index ] == (uint8_t) '\t' )
			 || ( input_string[ input_string_index ] == (uint8_t) '\n' )
			 || ( input_string[ input_string_index ] == (uint8_t) '\f' )
			 || ( input_string[ input_string_index ] == (uint8_t) '\v' )
			 || ( input_string[ input_string_index ] == (uint8_t) '\r' ) )
			{
				break;
			}
			value_length++;

			input_string_index++;
		}
		/* Make sure the value is terminated by an end of string
		 */
		input_string[ input_string_index ] = 0;

		if( value_identifier_length == 3 )
		{
			if( libcstring_narrow_string_compare(
			     (char *) value_identifier,
			     "CID",
			     3 ) == 0 )
			{
			}
		}
		else if( value_identifier_length == 7 )
		{
			if( libcstring_narrow_string_compare(
			     (char *) value_identifier,
			     "version",
			     7 ) == 0 )
			{
			}
		}
		else if( value_identifier_length == 9 )
		{
			if( libcstring_narrow_string_compare(
			     (char *) value_identifier,
			     "parentCID",
			     9 ) == 0 )
			{
			}
		}
		else if( value_identifier_length == 10 )
		{
			if( libcstring_narrow_string_compare(
			     (char *) value_identifier,
			     "createType",
			     10 ) == 0 )
			{
			}
		}
		else if( value_identifier_length == 18 )
		{
			if( libcstring_narrow_string_compare(
			     (char *) value_identifier,
			     "parentFileNameHint",
			     18 ) == 0 )
			{
			}
		}

		if( libvmdk_values_table_set_value(
		     values_table,
		     value_identifier,
		     value_identifier_length,
		     value,
		     value_length,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value with identifier: %s.",
			 function,
			 value_identifier );

			return( -1 );
		}
	}
	return( 1 );
}

/* Reads the extents from the descriptor file
 * Returns the 1 if succesful or -1 on error
 */
int libvmdk_descriptor_file_read_extents(
     libvmdk_descriptor_file_t *descriptor_file,
     liberror_error_t **error )
{
	return( 1 );
}

/* Reads the disk database from the descriptor file
 * Returns the 1 if succesful or -1 on error
 */
int libvmdk_descriptor_file_read_disk_database(
     libvmdk_descriptor_file_t *descriptor_file,
     liberror_error_t **error )
{
	return( 1 );
}

