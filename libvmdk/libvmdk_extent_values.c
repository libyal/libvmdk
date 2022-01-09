/*
 * Extent values functions
 *
 * Copyright (C) 2009-2022, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "libvmdk_debug.h"
#include "libvmdk_definitions.h"
#include "libvmdk_extent_values.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libcnotify.h"
#include "libvmdk_libcsplit.h"
#include "libvmdk_libfvalue.h"
#include "libvmdk_libuna.h"
#include "libvmdk_system_string.h"

/* Creates extent values
 * Make sure the value extent_values is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_values_initialize(
     libvmdk_extent_values_t **extent_values,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_values_initialize";

	if( extent_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent values.",
		 function );

		return( -1 );
	}
	if( *extent_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extent values value already set.",
		 function );

		return( -1 );
	}
	*extent_values = memory_allocate_structure(
	                  libvmdk_extent_values_t );

	if( *extent_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create extent values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *extent_values,
	     0,
	     sizeof( libvmdk_extent_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear extent values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *extent_values != NULL )
	{
		memory_free(
		 *extent_values );

		*extent_values = NULL;
	}
	return( -1 );
}

/* Frees extent values
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_values_free(
     libvmdk_extent_values_t **extent_values,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_values_free";

	if( extent_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent values.",
		 function );

		return( -1 );
	}
	if( *extent_values != NULL )
	{
		if( ( *extent_values )->filename != NULL )
		{
			memory_free(
			 ( *extent_values )->filename );
		}
		if( ( *extent_values )->alternate_filename != NULL )
		{
			memory_free(
			 ( *extent_values )->alternate_filename );
		}
		memory_free(
		 *extent_values );

		*extent_values = NULL;
	}
	return( 1 );
}

/* Reads the values header from an extent description string
 * Returns the number of bytes read if successful, or -1 on error
 */
int libvmdk_extent_values_read(
     libvmdk_extent_values_t *extent_values,
     const char *value_string,
     size_t value_string_size,
     int encoding,
     libcerror_error_t **error )
{
	libcsplit_narrow_split_string_t *values = NULL;
	const char *filename                    = NULL;
	static char *function                   = "libvmdk_extent_values_read";
	char *value_string_segment              = NULL;
	size_t filename_length                  = 0;
	size_t value_string_index               = 0;
	size_t value_string_length              = 0;
	size_t value_string_segment_size        = 0;
	uint64_t value_64bit                    = 0;
	int number_of_values                    = 0;

	if( extent_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent values.",
		 function );

		return( -1 );
	}
	if( extent_values->filename != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extent values - filename value already set.",
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
	if( value_string_index >= value_string_size )
	{
		/* No filename was found in the extent values
		 */
		value_string_length = value_string_index - 1;
		value_string_size   = 0;
	}
	else
	{
		value_string_length = value_string_index - 1;

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

		value_string_index += filename_length + 2;
		value_string_size  -= value_string_index;
	}
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
	 && ( narrow_string_compare(
	       value_string_segment,
	       "RW",
	       2 ) == 0 ) )
	{
		extent_values->access = LIBVMDK_EXTENT_ACCESS_READ_WRITE;
	}
	else if( ( value_string_segment_size == 7 )
	      && ( narrow_string_compare(
		    value_string_segment,
		    "RDONLY",
		    6 ) == 0 ) )
	{
		extent_values->access = LIBVMDK_EXTENT_ACCESS_READ;
	}
	else if( ( value_string_segment_size == 9 )
	      && ( narrow_string_compare(
		    value_string_segment,
		    "NOACCESS",
		    8 ) == 0 ) )
	{
		extent_values->access = LIBVMDK_EXTENT_ACCESS_NONE;
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
		 "%s: number of sectors\t\t\t\t: %s\n",
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
	extent_values->size = (size64_t) value_64bit * 512;

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
	 && ( narrow_string_compare(
	       value_string_segment,
	       "FLAT",
	       4 ) == 0 ) )
	{
		extent_values->type = LIBVMDK_EXTENT_TYPE_FLAT;
	}
	else if( ( value_string_segment_size == 5 )
	      && ( narrow_string_compare(
		    value_string_segment,
		    "VMFS",
		    4 ) == 0 ) )
	{
		extent_values->type = LIBVMDK_EXTENT_TYPE_VMFS_FLAT;
	}
	else if( ( value_string_segment_size == 5 )
	      && ( narrow_string_compare(
		    value_string_segment,
		    "ZERO",
		    4 ) == 0 ) )
	{
		extent_values->type = LIBVMDK_EXTENT_TYPE_ZERO;
	}
	else if( ( value_string_segment_size == 7 )
	      && ( narrow_string_compare(
		    value_string_segment,
		    "SPARSE",
		    6 ) == 0 ) )
	{
		extent_values->type = LIBVMDK_EXTENT_TYPE_SPARSE;
	}
	else if( ( value_string_segment_size == 8 )
	      && ( narrow_string_compare(
		    value_string_segment,
		    "VMFSRAW",
		    7 ) == 0 ) )
	{
		extent_values->type = LIBVMDK_EXTENT_TYPE_VMFS_RAW;
	}
	else if( ( value_string_segment_size == 8 )
	      && ( narrow_string_compare(
		    value_string_segment,
		    "VMFSRDM",
		    7 ) == 0 ) )
	{
		extent_values->type = LIBVMDK_EXTENT_TYPE_VMFS_RDM;
	}
	else if( ( value_string_segment_size == 11 )
	      && ( narrow_string_compare(
		    value_string_segment,
		    "VMFSSPARSE",
		    10 ) == 0 ) )
	{
		extent_values->type = LIBVMDK_EXTENT_TYPE_VMFS_SPARSE;
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
	if( ( filename_length == 0 )
	 && ( extent_values->type != LIBVMDK_EXTENT_TYPE_ZERO ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing value string segment: 3 filename.",
		 function );

		goto on_error;
	}
	if( filename_length > 0 )
	{
		/* The extent value: 3 contains the filename
		 */
		if( libvmdk_extent_values_set_filename(
		     extent_values,
		     filename,
		     filename_length,
		     encoding,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set filename.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( encoding != 0 )
			{
				if( libvmdk_debug_print_string_value(
				     function,
				     "filename\t\t\t\t\t",
				     extent_values->filename,
				     extent_values->filename_size,
				     encoding,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
					 "%s: unable to print string value.",
					 function );

					goto on_error;
				}
			}
			else
			{
				if( libvmdk_debug_print_utf8_string_value(
				     function,
				     "filename\t\t\t\t\t",
				     extent_values->filename,
				     extent_values->filename_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
					 "%s: unable to print UTF-8 string value.",
					 function );

					goto on_error;
				}
			}
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
	if( value_string_size > 0 )
	{
		if( libcsplit_narrow_string_split(
		     &( value_string[ value_string_index ] ),
		     value_string_size,
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
			 "%s: missing value string segment: 4.",
			 function );

			goto on_error;
		}
		/* The extent value: 4 contains the offset
		 */
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: offset\t\t\t\t\t: %s\n",
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
		extent_values->offset = (off64_t) value_64bit;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( number_of_values > 1 )
			{
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
					 "%s: missing value string segment: 5.",
					 function );

					goto on_error;
				}
				libcnotify_printf(
				 "%s: string segment 5\t\t\t: %s\n",
				 function,
				 value_string_segment );
			}
			if( number_of_values > 2 )
			{
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
					 "%s: missing value string segment: 6.",
					 function );

					goto on_error;
				}
				libcnotify_printf(
				 "%s: string segment 6\t\t\t: %s\n",
				 function,
				 value_string_segment );
			}
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

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
	if( extent_values->filename != NULL )
	{
		memory_free(
		 extent_values->filename );

		extent_values->filename = NULL;
	}
	extent_values->filename_size = 0;

	return( -1 );
}

/* Sets the filename
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_values_set_filename(
     libvmdk_extent_values_t *extent_values,
     const char *filename,
     size_t filename_length,
     int encoding,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_values_set_filename";

	if( extent_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent values.",
		 function );

		return( -1 );
	}
	if( extent_values->filename != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extent values - filename value already set.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( filename_length == 0 )
	 || ( filename_length > ( (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE - 1 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid filename length value out of bounds.",
		 function );

		return( -1 );
	}
	extent_values->filename_size = filename_length + 1;

	extent_values->filename = (uint8_t *) memory_allocate(
	                                       sizeof( uint8_t ) * extent_values->filename_size );

	if( extent_values->filename == NULL )
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
	     extent_values->filename,
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
	extent_values->filename[ filename_length ] = 0;

	extent_values->filename_encoding = encoding;

	return( 1 );

on_error:
	if( extent_values->filename != NULL )
	{
		memory_free(
		 extent_values->filename );

		extent_values->filename = NULL;
	}
	extent_values->filename_size = 0;

	return( -1 );
}

/* Sets the alternate filename
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_values_set_alternate_filename(
     libvmdk_extent_values_t *extent_values,
     const char *filename,
     size_t filename_length,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_values_set_alternate_filename";

	if( extent_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent values.",
		 function );

		return( -1 );
	}
	if( extent_values->alternate_filename != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extent values - alternate filename value already set.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( filename_length == 0 )
	 || ( filename_length > ( (size_t) SSIZE_MAX - 1 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid filename length value out of bounds.",
		 function );

		return( -1 );
	}
	if( libvmdk_system_string_size_from_narrow_string(
	     filename,
	     filename_length + 1,
	     &( extent_values->alternate_filename_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine alternate filename size.",
		 function );

		goto on_error;
	}
	if( ( extent_values->alternate_filename_size == 0 )
	 || ( extent_values->alternate_filename_size > ( (size_t) SSIZE_MAX / sizeof( system_character_t ) ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid file IO handle - alternate filename size value out of bounds.",
		 function );

		goto on_error;
	}
	extent_values->alternate_filename = system_string_allocate(
	                                     extent_values->alternate_filename_size );

	if( extent_values->alternate_filename == NULL )
	{
		libcerror_error_set(
		error,
		LIBCERROR_ERROR_DOMAIN_MEMORY,
		LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		"%s: unable to create alternate filename.",
		function );

		goto on_error;
	}
	if( libvmdk_system_string_copy_from_narrow_string(
	     extent_values->alternate_filename,
	     extent_values->alternate_filename_size,
	     filename,
	     filename_length + 1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy alternate filename.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( extent_values->alternate_filename != NULL )
	{
		memory_free(
		 extent_values->alternate_filename );

		extent_values->alternate_filename = NULL;
	}
	extent_values->alternate_filename_size = 0;

	return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Sets the alternate filename
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_values_set_alternate_filename_wide(
     libvmdk_extent_values_t *extent_values,
     const wchar_t *filename,
     size_t filename_length,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_values_set_alternate_filename_wide";

	if( extent_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent values.",
		 function );

		return( -1 );
	}
	if( extent_values->alternate_filename != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extent values - alternate filename value already set.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( filename_length == 0 )
	 || ( filename_length > ( (size_t) SSIZE_MAX - 1 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid filename length value out of bounds.",
		 function );

		return( -1 );
	}
	if( libvmdk_system_string_size_from_wide_string(
	     filename,
	     filename_length + 1,
	     &( extent_values->alternate_filename_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine alternate filename size.",
		 function );

		goto on_error;
	}
	if( ( extent_values->alternate_filename_size == 0 )
	 || ( extent_values->alternate_filename_size > ( (size_t) SSIZE_MAX / sizeof( system_character_t ) ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid file IO handle - alternate filename size value out of bounds.",
		 function );

		goto on_error;
	}
	extent_values->alternate_filename = system_string_allocate(
	                                     extent_values->alternate_filename_size );

	if( extent_values->alternate_filename == NULL )
	{
		libcerror_error_set(
		error,
		LIBCERROR_ERROR_DOMAIN_MEMORY,
		LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		"%s: unable to create alternate filename.",
		function );

		goto on_error;
	}
	if( libvmdk_system_string_copy_from_wide_string(
	     extent_values->alternate_filename,
	     extent_values->alternate_filename_size,
	     filename,
	     filename_length + 1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy alternate filename.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( extent_values->alternate_filename != NULL )
	{
		memory_free(
		 extent_values->alternate_filename );

		extent_values->alternate_filename = NULL;
	}
	extent_values->alternate_filename_size = 0;

	return( -1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Retrieves the extent type
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_values_get_type(
     libvmdk_extent_values_t *extent_values,
     int *type,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_values_get_type";

	if( extent_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent values.",
		 function );

		return( -1 );
	}
	if( type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid type.",
		 function );

		return( -1 );
	}
	*type = extent_values->type;

	return( 1 );
}

/* Retrieves the extent range (offset and size)
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_values_get_range(
     libvmdk_extent_values_t *extent_values,
     off64_t *offset,
     size64_t *size,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_values_get_range";

	if( extent_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent values.",
		 function );

		return( -1 );
	}
	if( offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset.",
		 function );

		return( -1 );
	}
	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
	*offset = extent_values->offset;
	*size   = extent_values->size;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded filename
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libvmdk_extent_values_get_utf8_filename_size(
     libvmdk_extent_values_t *extent_values,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_values_get_utf8_filename_size";
	int result            = 0;

	if( extent_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent values.",
		 function );

		return( -1 );
	}
	if( ( extent_values->filename == NULL )
	 || ( extent_values->filename_size == 0 ) )
	{
		return( 0 );
	}
	if( extent_values->filename_encoding != 0 )
	{
		result = libuna_utf8_string_size_from_byte_stream(
		          extent_values->filename,
		          extent_values->filename_size,
		          extent_values->filename_encoding,
		          utf8_string_size,
		          error );
	}
	else
	{
		result = libuna_utf8_string_size_from_utf8_stream(
		          extent_values->filename,
		          extent_values->filename_size,
		          utf8_string_size,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded filename
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libvmdk_extent_values_get_utf8_filename(
     libvmdk_extent_values_t *extent_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_values_get_utf8_filename";
	int result            = 0;

	if( extent_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent values.",
		 function );

		return( -1 );
	}
	if( ( extent_values->filename == NULL )
	 || ( extent_values->filename_size == 0 ) )
	{
		return( 0 );
	}
	if( extent_values->filename_encoding != 0 )
	{
		result = libuna_utf8_string_copy_from_byte_stream(
		          utf8_string,
		          utf8_string_size,
		          extent_values->filename,
		          extent_values->filename_size,
		          extent_values->filename_encoding,
		          error );
	}
	else
	{
		result = libuna_utf8_string_copy_from_utf8_stream(
		          utf8_string,
		          utf8_string_size,
		          extent_values->filename,
		          extent_values->filename_size,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded filename
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libvmdk_extent_values_get_utf16_filename_size(
     libvmdk_extent_values_t *extent_values,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_values_get_utf16_filename_size";
	int result            = 0;

	if( extent_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent values.",
		 function );

		return( -1 );
	}
	if( ( extent_values->filename == NULL )
	 || ( extent_values->filename_size == 0 ) )
	{
		return( 0 );
	}
	if( extent_values->filename_encoding != 0 )
	{
		result = libuna_utf16_string_size_from_byte_stream(
		          extent_values->filename,
		          extent_values->filename_size,
		          extent_values->filename_encoding,
		          utf16_string_size,
		          error );
	}
	else
	{
		result = libuna_utf16_string_size_from_utf8_stream(
		          extent_values->filename,
		          extent_values->filename_size,
		          utf16_string_size,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded filename
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libvmdk_extent_values_get_utf16_filename(
     libvmdk_extent_values_t *extent_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_values_get_utf16_filename";
	int result            = 0;

	if( extent_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent values.",
		 function );

		return( -1 );
	}
	if( ( extent_values->filename == NULL )
	 || ( extent_values->filename_size == 0 ) )
	{
		return( 0 );
	}
	if( extent_values->filename_encoding != 0 )
	{
		result = libuna_utf16_string_copy_from_byte_stream(
		          utf16_string,
		          utf16_string_size,
		          extent_values->filename,
		          extent_values->filename_size,
		          extent_values->filename_encoding,
		          error );
	}
	else
	{
		result = libuna_utf16_string_copy_from_utf8_stream(
		          utf16_string,
		          utf16_string_size,
		          extent_values->filename,
		          extent_values->filename_size,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

