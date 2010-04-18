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

/* Creates the information file
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
		 "%s: invalid information file.",
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
			 "%s: unable to create information file.",
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
			 "%s: unable to clear information file.",
			 function );

			memory_free(
			 *descriptor_file );

			*descriptor_file = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees the information file
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
		 "%s: invalid information file.",
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
		 "%s: invalid information file.",
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

/* Opens a information file
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
		 "%s: invalid information file.",
		 function );

		return( -1 );
	}
	if( descriptor_file->name == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid information file - missing name.",
		 function );

		return( -1 );
	}
	if( descriptor_file->file_stream != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid information file - file stream already set.",
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

/* Closes the information file
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
		 "%s: invalid information file.",
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

/* Reas a section with its values from the information file
 * Returns the 1 if succesful, 0 if no such section or -1 on error
 */
int libvmdk_descriptor_file_read_section(
     libvmdk_descriptor_file_t *descriptor_file,
     const uint8_t *section_identifier,
     size_t section_identifier_length,
     libvmdk_values_table_t *values_table,
     liberror_error_t **error )
{
	uint8_t input_string[ 128 ];

	uint8_t *value_identifier      = NULL;
	uint8_t *value                 = NULL;
	char *result_string            = NULL;
	static char *function          = "libvmdk_descriptor_file_read_section";
	size_t input_string_index      = 0;
	size_t value_identifier_length = 0;
	size_t value_length            = 0;
	uint8_t in_section             = 0;
	int result                     = 0;

	if( descriptor_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid information file.",
		 function );

		return( -1 );
	}
	if( descriptor_file->file_stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid information file - missing file stream.",
		 function );

		return( -1 );
	}
	if( section_identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section identifier.",
		 function );

		return( -1 );
	}
	if( section_identifier_length > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section identifier length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values table.",
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
		if( in_section != 0 )
		{
			/* Check for the end of the section
			 */
			if( ( ( input_string_index + section_identifier_length + 2 ) < 128 )
			 && ( input_string[ input_string_index ] == (uint8_t) '<' )
			 && ( input_string[ input_string_index + 1 ] == (uint8_t) '/' )
			 && ( libcstring_narrow_string_compare(
			       (char *) &( input_string[ input_string_index + 2 ] ),
			       (char *) section_identifier,
			       section_identifier_length ) == 0 )
			 && ( input_string[ input_string_index + section_identifier_length + 2 ] == (uint8_t) '>' ) )
			{
				in_section = 0;
				result     = 1;

				break;
			}
			if( input_string[ input_string_index ] == (uint8_t) '<' )
			{
				/* Determine the value identifier
				 */
				input_string_index++;

				value_identifier        = &( input_string[ input_string_index ] );
				value_identifier_length = 0;

				while( input_string_index < 128 )
				{
					if( ( ( input_string[ input_string_index ] >= (uint8_t) 'A' )
					  &&  ( input_string[ input_string_index ] <= (uint8_t) 'I' ) )
					 || ( ( input_string[ input_string_index ] >= (uint8_t) 'J' )
					  &&  ( input_string[ input_string_index ] <= (uint8_t) 'R' ) )
					 || ( ( input_string[ input_string_index ] >= (uint8_t) 'S' )
					  &&  ( input_string[ input_string_index ] <= (uint8_t) 'Z' ) )
					 || ( ( input_string[ input_string_index ] >= (uint8_t) 'a' )
					  &&  ( input_string[ input_string_index ] <= (uint8_t) 'i' ) )
					 || ( ( input_string[ input_string_index ] >= (uint8_t) 'j' )
					  &&  ( input_string[ input_string_index ] <= (uint8_t) 'r' ) )
					 || ( ( input_string[ input_string_index ] >= (uint8_t) 's' )
					  &&  ( input_string[ input_string_index ] <= (uint8_t) 'z' ) )
					 || ( ( input_string[ input_string_index ] >= (uint8_t) '0' )
					  &&  ( input_string[ input_string_index ] <= (uint8_t) '9' ) )
					 ||   ( input_string[ input_string_index ] == (uint8_t) '_' ) )
					{
						value_identifier_length++;
					}
					else
					{
						break;
					}
					input_string_index++;
				}
				/* Check if there is a supported value identifier
				 */
				if( input_string[ input_string_index ] != (uint8_t) '>' )
				{
					continue;
				}
				/* Make sure the value identifier is terminated by an end of string
				 */
				input_string[ input_string_index ] = 0;

				/* Determine the value
				 */
				input_string_index++;

				value        = &( input_string[ input_string_index ] );
				value_length = 0;

				while( input_string_index < 128 )
				{
					if( ( input_string[ input_string_index ] == 0 )
					 || ( input_string[ input_string_index ] == (uint8_t) '<' ) )
					{
						break;
					}
					value_length++;

					input_string_index++;
				}
				/* Check if there is a supported value
				 */
				if( input_string[ input_string_index ] != (uint8_t) '<' )
				{
					continue;
				}
				/* Make sure the value is terminated by an end of string
				 */
				input_string[ input_string_index ] = 0;

				/* Check the value identifier
				 */
				input_string_index++;

				if( ( ( input_string_index + value_identifier_length + 2 ) >= 128 )
				 || ( input_string[ input_string_index ] != (uint8_t) '/' )
				 || ( libcstring_narrow_string_compare(
				       (char *) &( input_string[ input_string_index + 1 ] ),
				       (char *) value_identifier,
				       value_identifier_length ) != 0 )
				 || ( input_string[ input_string_index + value_identifier_length + 1 ] != (uint8_t) '>' ) )
				{
					continue;
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
		}
		else
		{
			/* Check for the start of the section
			 */
			if( ( ( input_string_index + section_identifier_length + 1 ) < 128 )
			 && ( input_string[ input_string_index ] == (uint8_t) '<' )
			 && ( libcstring_narrow_string_compare(
			       (char *) &( input_string[ input_string_index + 1 ] ),
			       (char *) section_identifier,
			       section_identifier_length ) == 0 )
			 && ( input_string[ input_string_index + section_identifier_length + 1 ] == (uint8_t) '>' ) )
			{
				in_section = 1;
			}
		}
	}
	return( result );
}

/* Write a section with its values to the information file
 * Returns the 1 if succesful or -1 on error
 */
int libvmdk_descriptor_file_write_section(
     libvmdk_descriptor_file_t *descriptor_file,
     const uint8_t *section_identifier,
     size_t section_identifier_length,
     libvmdk_values_table_t *values_table,
     liberror_error_t **error )
{
	static char *function = "libvmdk_descriptor_file_write_section";
	int amount_of_values  = 0;
	int print_count       = 0;
	int result            = 1;
	int value_iterator    = 0;

	if( descriptor_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid information file.",
		 function );

		return( -1 );
	}
	if( descriptor_file->file_stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid information file - missing file stream.",
		 function );

		return( -1 );
	}
	if( section_identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section identifier.",
		 function );

		return( -1 );
	}
	if( section_identifier_length > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section identifier length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( values_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values table.",
		 function );

		return( -1 );
	}
	/* Write section start
	 */
	print_count = fprintf(
	               descriptor_file->file_stream,
	               "<%s>\n",
	               section_identifier );

	if( ( print_count < 0 )
	 || ( (size_t) print_count > ( section_identifier_length + 3 ) ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section start to file stream.",
		 function );

		return( -1 );
	}
	/* Write section values
	 */
	if( libvmdk_values_table_get_amount_of_values(
	     values_table,
	     &amount_of_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of values.",
		 function );

		return( -1 );
	}
	if( amount_of_values > 0 )
	{
		if( values_table->identifier == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid values table - missing identifiers.",
			 function );

			return( -1 );
		}
		if( values_table->identifier_length == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid values table - missing identifier lengths.",
			 function );

			return( -1 );
		}
		if( values_table->value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid values table - missing values.",
			 function );

			return( -1 );
		}
		if( values_table->value_length == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid values table - missing value lengths.",
			 function );

			return( -1 );
		}
		for( value_iterator = 0;
		     value_iterator < amount_of_values;
		     value_iterator++ )
		{
			if( values_table->identifier[ value_iterator ] == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing identifier for value: %d.",
				 function,
				 value_iterator );

				result = -1;

				continue;
			}
			/* Write the section value start
			 */
			print_count = fprintf(
				       descriptor_file->file_stream,
				       "\t<%s>",
				       values_table->identifier[ value_iterator ] );

			if( ( print_count < 0 )
			 || ( (size_t) print_count > ( values_table->identifier_length[ value_iterator ] + 3 ) ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write section value start to file stream for value: %d.",
				 function,
				 value_iterator );

				result = -1;

				continue;
			}
			/* Write the section value data
			 */
			if( values_table->value[ value_iterator ] != NULL )
			{
				print_count = fprintf(
					       descriptor_file->file_stream,
					       "%s",
					       values_table->value[ value_iterator ] );

				if( ( print_count < 0 )
				 || ( (size_t) print_count > ( values_table->value_length[ value_iterator ] + 2 ) ) )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write section value data to file stream for value: %d.",
					 function,
					 value_iterator );

					result = -1;
				}
			}
			/* Write the section value end
			 */
			print_count = fprintf(
				       descriptor_file->file_stream,
				       "</%s>\n",
				       values_table->identifier[ value_iterator ] );

			if( ( print_count < 0 )
			 || ( (size_t) print_count > ( values_table->identifier_length[ value_iterator ] + 4 ) ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write section value end to file stream for value: %d.",
				 function,
				 value_iterator );

				result = -1;
			}
		}
	}
	/* Write section end
	 */
	print_count = fprintf(
	               descriptor_file->file_stream,
	               "</%s>\n\n",
	               section_identifier );

	if( ( print_count < 0 )
	 || ( (size_t) print_count > ( section_identifier_length + 5 ) ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section end to file stream.",
		 function );

		return( -1 );
	}
	return( result );
}

