/*
 * Extent descriptor functions
 *
 * Copyright (c) 2009-2013 Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>

#include "libvmdk_definitions.h"
#include "libvmdk_extent_descriptor.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libcnotify.h"
#include "libvmdk_libcsplit.h"
#include "libvmdk_libfvalue.h"

/* Creates an extent descriptor
 * Make sure the value extent_descriptor is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_descriptor_initialize(
     libvmdk_extent_descriptor_t **extent_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_descriptor_initialize";

	if( extent_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent descriptor.",
		 function );

		return( -1 );
	}
	if( *extent_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extent descriptor value already set.",
		 function );

		return( -1 );
	}
	*extent_descriptor = memory_allocate_structure(
	                      libvmdk_extent_descriptor_t );

	if( *extent_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create extent descriptor.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *extent_descriptor,
	     0,
	     sizeof( libvmdk_extent_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear extent descriptor.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *extent_descriptor != NULL )
	{
		memory_free(
		 *extent_descriptor );

		*extent_descriptor = NULL;
	}
	return( -1 );
}

/* Frees an extent descriptor
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_descriptor_free(
     libvmdk_extent_descriptor_t **extent_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_descriptor_free";

	if( extent_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent descriptor.",
		 function );

		return( -1 );
	}
	if( *extent_descriptor != NULL )
	{
		if( ( *extent_descriptor )->filename != NULL )
		{
			memory_free(
			 ( *extent_descriptor )->filename );
		}
		memory_free(
		 *extent_descriptor );

		*extent_descriptor = NULL;
	}
	return( 1 );
}

/* Reads the descriptor header from an extent value string
 * Returns the number of bytes read if successful, or -1 on error
 */
int libvmdk_extent_descriptor_read(
     libvmdk_extent_descriptor_t *extent_descriptor,
     char *value_string,
     size_t value_string_size,
     libcerror_error_t **error )
{
	libcsplit_narrow_split_string_t *values = NULL;
	char *filename                          = NULL;
	char *value_string_segment              = NULL;
	static char *function                   = "libvmdk_extent_descriptor_read";
	size_t filename_length                  = 0;
	size_t value_string_index               = 0;
	size_t value_string_length              = 0;
	size_t value_string_segment_size        = 0;
	uint64_t value_64bit                    = 0;
	int number_of_values                    = 0;

	if( extent_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent descriptor.",
		 function );

		return( -1 );
	}
	if( extent_descriptor->filename != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extent descriptor - filename value already set.",
		 function );

		return( -1 );
	}
	if( value_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value string.",
		 function );

		return( -1 );
	}
	if( value_string_size < 10 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: value string is too small.",
		 function );

		return( -1 );
	}
	if( value_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid value string size value exceeds maximum.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: value string\t\t\t\t: %s\n",
		 function,
		 value_string );
	}
#endif
	/* Look for the start of the filename since this value
	 * can contain spaces, naive split cannot be used for it
	 */
	for( value_string_index = 0;
	     value_string_index < value_string_size;
	     value_string_index++ )
	{
		if( ( value_string[ value_string_index ] == '"' )
		 || ( value_string[ value_string_index ] == '\'' ) )
		{
			break;
		}
	}
	if( value_string_index == value_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value string missing filename value.",
		 function );

		goto on_error;
	}
	value_string_length = value_string_index - 1;

	value_string[ value_string_length ] = 0;

	value_string_index++;

	filename = &( value_string[ value_string_index ] );

	/* Look for the end of the filename
	 */
	for( filename_length = value_string_size - 1;
	     filename_length > value_string_index;
	     filename_length-- )
	{
		if( ( value_string[ filename_length ] == '"' )
		 || ( value_string[ filename_length ] == '\'' ) )
		{
			break;
		}
	}
	if( filename_length <= value_string_index )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value string missing filename value.",
		 function );

		goto on_error;
	}
	filename_length -= value_string_index;

	filename[ filename_length ] = 0;

	value_string_index += filename_length + 2;
	value_string_size  -= value_string_index;

	if( libcsplit_narrow_string_split(
	     value_string,
	     value_string_length + 1,
	     ' ',
	     &values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to get values from string.",
		 function );

		goto on_error;
	}
	if( libcsplit_narrow_split_string_get_number_of_segments(
	     values,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to retrieve number of values.",
		 function );

		goto on_error;
	}
	if( number_of_values != 3 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported number of extent values: %d.",
		 function,
		 number_of_values );

		goto on_error;
	}
	/* The extent value: 0 contains the access
	 */
	if( libcsplit_narrow_split_string_get_segment_by_index(
	     values,
	     0,
	     &value_string_segment,
	     &value_string_segment_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: 0.",
		 function );

		goto on_error;
	}
	if( value_string_segment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing value string segment: 0.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: access\t\t\t\t\t: %s\n",
		 function,
		 value_string_segment );
	}
#endif
	if( ( value_string_segment_size == 3 )
	 && ( libcstring_narrow_string_compare(
	       value_string_segment,
	       "RW",
	       2 ) == 0 ) )
	{
		extent_descriptor->access = LIBVMDK_EXTENT_ACCESS_READ_WRITE;
	}
	else if( ( value_string_segment_size == 7 )
	      && ( libcstring_narrow_string_compare(
		    value_string_segment,
		    "RDONLY",
		    6 ) == 0 ) )
	{
		extent_descriptor->access = LIBVMDK_EXTENT_ACCESS_READ;
	}
	else if( ( value_string_segment_size == 9 )
	      && ( libcstring_narrow_string_compare(
		    value_string_segment,
		    "NOACCESS",
		    8 ) == 0 ) )
	{
		extent_descriptor->access = LIBVMDK_EXTENT_ACCESS_NONE;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported extent access.",
		 function );

		goto on_error;
	}
	/* The extent value: 1 contains the number of sectors
	 */
	if( libcsplit_narrow_split_string_get_segment_by_index(
	     values,
	     1,
	     &value_string_segment,
	     &value_string_segment_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: 1.",
		 function );

		goto on_error;
	}
	if( value_string_segment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing value string segment: 1.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of sectors\t\t\t: %s\n",
		 function,
		 value_string_segment );
	}
#endif
	if( libfvalue_utf8_string_copy_to_integer(
	     (uint8_t *) value_string_segment,
	     value_string_segment_size,
	     &value_64bit,
	     64,
	     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine number of sectors value from string.",
		 function );

		goto on_error;
	}
	if( value_64bit > (uint64_t) ( INT64_MAX / 512 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid extent number of sectors value exceeds maximum.",
		 function );

		goto on_error;
	}
	extent_descriptor->size = (size64_t) value_64bit * 512;

	/* The extent value: 2 contains the type
	 */
	if( libcsplit_narrow_split_string_get_segment_by_index(
	     values,
	     2,
	     &value_string_segment,
	     &value_string_segment_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: 2.",
		 function );

		goto on_error;
	}
	if( value_string_segment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing value string segment: 2.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: type\t\t\t\t\t: %s\n",
		 function,
		 value_string_segment );
	}
#endif
	if( ( value_string_segment_size == 5 )
	 && ( libcstring_narrow_string_compare(
	       value_string_segment,
	       "FLAT",
	       4 ) == 0 ) )
	{
		extent_descriptor->type = LIBVMDK_EXTENT_TYPE_FLAT;
	}
	else if( ( value_string_segment_size == 5 )
	      && ( libcstring_narrow_string_compare(
		    value_string_segment,
		    "VMFS",
		    4 ) == 0 ) )
	{
		extent_descriptor->type = LIBVMDK_EXTENT_TYPE_VMFS;
	}
	else if( ( value_string_segment_size == 5 )
	      && ( libcstring_narrow_string_compare(
		    value_string_segment,
		    "ZERO",
		    4 ) == 0 ) )
	{
		extent_descriptor->type = LIBVMDK_EXTENT_TYPE_ZERO;
	}
	else if( ( value_string_segment_size == 7 )
	      && ( libcstring_narrow_string_compare(
		    value_string_segment,
		    "SPARSE",
		    6 ) == 0 ) )
	{
		extent_descriptor->type = LIBVMDK_EXTENT_TYPE_SPARSE;
	}
	else if( ( value_string_segment_size == 8 )
	      && ( libcstring_narrow_string_compare(
		    value_string_segment,
		    "VMFSRAW",
		    7 ) == 0 ) )
	{
		extent_descriptor->type = LIBVMDK_EXTENT_TYPE_VMFS_RAW;
	}
	else if( ( value_string_segment_size == 8 )
	      && ( libcstring_narrow_string_compare(
		    value_string_segment,
		    "VMFSRDM",
		    7 ) == 0 ) )
	{
		extent_descriptor->type = LIBVMDK_EXTENT_TYPE_VMFS_RDM;
	}
	else if( ( value_string_segment_size == 11 )
	      && ( libcstring_narrow_string_compare(
		    value_string_segment,
		    "VMFSSPARSE",
		    10 ) == 0 ) )
	{
		extent_descriptor->type = LIBVMDK_EXTENT_TYPE_VMFS_SPARSE;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported extent type.",
		 function );

		goto on_error;
	}
	if( libcsplit_narrow_split_string_free(
	     &values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free values.",
		 function );

		goto on_error;
	}
	/* The extent value: 3 contains the filename
	 */
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: filename\t\t\t\t: %s\n",
		 function,
		 filename );
	}
#endif
	extent_descriptor->filename_size = filename_length + 1;

	extent_descriptor->filename = libcstring_narrow_string_allocate(
	                               extent_descriptor->filename_size );

	if( extent_descriptor->filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create filename.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     extent_descriptor->filename,
	     filename,
	     filename_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy filename.",
		 function );

		goto on_error;
	}
	extent_descriptor->filename[ filename_length ] = 0;

	if( value_string_size > 0 )
	{
		/* The extent value: 4 contains the offset
		 */
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: offset\t\t\t\t\t: %s\n",
			 function,
			 &( value_string[ value_string_index ] ) );
		}
#endif
		if( libfvalue_utf8_string_copy_to_integer(
		     (uint8_t *) &( value_string[ value_string_index ] ),
		     value_string_size,
		     &value_64bit,
		     64,
		     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine offset value from string.",
			 function );

			goto on_error;
		}
		if( value_64bit > (uint64_t) INT64_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid extent offset value exceeds maximum.",
			 function );

			goto on_error;
		}
		extent_descriptor->offset = (off64_t) value_64bit;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );

on_error:
	if( values != NULL )
	{
		libcsplit_narrow_split_string_free(
		 &values,
		 NULL );
	}
	if( extent_descriptor->filename != NULL )
	{
		memory_free(
		 extent_descriptor->filename );

		extent_descriptor->filename = NULL;
	}
	extent_descriptor->filename_size = 0;

	return( -1 );
}

