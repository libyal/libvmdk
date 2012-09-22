/*
 * The segment functions
 *
 * Copyright (c) 2010-2012, Joachim Metz <joachim.metz@gmail.com>
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

#include "libmfdata_definitions.h"
#include "libmfdata_libcerror.h"
#include "libmfdata_segment.h"
#include "libmfdata_segment_table.h"
#include "libmfdata_types.h"

/* Initializes the segment
 * Returns 1 if successful or -1 on error
 */
int libmfdata_segment_initialize(
     libmfdata_segment_t **segment,
     libmfdata_segment_table_t *segment_table,
     int segment_index,
     libcerror_error_t **error )
{
	libmfdata_internal_segment_t *internal_segment = NULL;
	static char *function                          = "libmfdata_segment_initialize";

	if( segment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment.",
		 function );

		return( -1 );
	}
	if( *segment != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid segment value already set.",
		 function );

		return( -1 );
	}
	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( segment_index < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid segment index value less than zero.",
		 function );

		return( -1 );
	}
	internal_segment = memory_allocate_structure(
	                    libmfdata_internal_segment_t );

	if( internal_segment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create segment.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_segment,
	     0,
	     sizeof( libmfdata_internal_segment_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear segment.",
		 function );

		goto on_error;
	}
	internal_segment->segment_table      = segment_table;
	internal_segment->segment_index      = segment_index;
	internal_segment->file_io_pool_entry = -1;

	*segment = (libmfdata_segment_t *) internal_segment;

	return( 1 );

on_error:
	if( internal_segment != NULL )
	{
		memory_free(
		 internal_segment );
	}
	return( -1 );
}

/* Frees the segment
 * Returns 1 if successful or -1 on error
 */
int libmfdata_segment_free(
     libmfdata_segment_t **segment,
     libcerror_error_t **error )
{
	static char *function = "libmfdata_segment_free";

	if( segment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment.",
		 function );

		return( -1 );
	}
	if( *segment != NULL )
	{
		memory_free(
		 *segment );

		*segment = NULL;
	}
	return( 1 );
}

/* Retrieves the file IO pool entry, offset and size of the segment
 * Returns 1 if successful or -1 on error
 */
int libmfdata_segment_get_range(
     libmfdata_segment_t *segment,
     int *file_io_pool_entry,
     size64_t *segment_size,
     libcerror_error_t **error )
{
	libmfdata_internal_segment_t *internal_segment = NULL;
	static char *function                          = "libmfdata_segment_get_range";

	if( segment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment.",
		 function );

		return( -1 );
	}
	internal_segment = (libmfdata_internal_segment_t *) segment;

	if( file_io_pool_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO pool entry.",
		 function );

		return( -1 );
	}
	if( segment_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment size.",
		 function );

		return( -1 );
	}
	*file_io_pool_entry = internal_segment->file_io_pool_entry;
	*segment_size       = internal_segment->size;

	return( 1 );
}

/* Sets the file IO pool entry, offset and size of the segment
 * Returns 1 if successful or -1 on error
 */
int libmfdata_segment_set_range(
     libmfdata_segment_t *segment,
     int file_io_pool_entry,
     size64_t segment_size,
     libcerror_error_t **error )
{
	libmfdata_internal_segment_t *internal_segment = NULL;
	static char *function                          = "libmfdata_segment_set_offset";

	if( segment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment.",
		 function );

		return( -1 );
	}
	internal_segment = (libmfdata_internal_segment_t *) segment;

	if( file_io_pool_entry < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid file IO pool entry value less than zero.",
		 function );

		return( -1 );
	}
	if( segment_size > (size64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid segment size value exceeds maximum.",
		 function );

		return( -1 );
	}
	internal_segment->file_io_pool_entry = file_io_pool_entry;
	internal_segment->size               = segment_size;

	return( 1 );
}

/* Retrieves the segment index of the segment
 * Returns 1 if successful or -1 on error
 */
int libmfdata_segment_get_segment_index(
     libmfdata_segment_t *segment,
     int *segment_index,
     libcerror_error_t **error )
{
	libmfdata_internal_segment_t *internal_segment = NULL;
	static char *function                          = "libmfdata_segment_get_segment_index";

	if( segment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment.",
		 function );

		return( -1 );
	}
	internal_segment = (libmfdata_internal_segment_t *) segment;

	if( segment_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment index.",
		 function );

		return( -1 );
	}
	*segment_index = internal_segment->segment_index;

	return( 1 );
}

/* Sets the segment index of the segment
 * Returns 1 if successful or -1 on error
 */
int libmfdata_segment_set_segment_index(
     libmfdata_segment_t *segment,
     int segment_index,
     libcerror_error_t **error )
{
	libmfdata_internal_segment_t *internal_segment = NULL;
	static char *function                          = "libmfdata_segment_set_segment_index";

	if( segment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment.",
		 function );

		return( -1 );
	}
	internal_segment = (libmfdata_internal_segment_t *) segment;

	if( segment_index < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid segment index value less than zero.",
		 function );

		return( -1 );
	}
	internal_segment->segment_index = segment_index;

	return( 1 );
}

/* Retrieves the value offset of the segment
 * Returns 1 if successful or -1 on error
 */
int libmfdata_segment_get_value_offset(
     libmfdata_segment_t *segment,
     off64_t *value_offset,
     libcerror_error_t **error )
{
	libmfdata_internal_segment_t *internal_segment = NULL;
	static char *function                          = "libmfdata_segment_get_value_offset";

	if( segment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment.",
		 function );

		return( -1 );
	}
	internal_segment = (libmfdata_internal_segment_t *) segment;

	if( value_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value offset.",
		 function );

		return( -1 );
	}
	*value_offset = internal_segment->value_offset;

	return( 1 );
}

/* Sets the value offset of the segment
 * Returns 1 if successful or -1 on error
 */
int libmfdata_segment_set_value_offset(
     libmfdata_segment_t *segment,
     off64_t value_offset,
     libcerror_error_t **error )
{
	libmfdata_internal_segment_t *internal_segment = NULL;
	static char *function                          = "libmfdata_segment_set_value_offset";

	if( segment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment.",
		 function );

		return( -1 );
	}
	internal_segment = (libmfdata_internal_segment_t *) segment;

	if( value_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid value offset value less than zero.",
		 function );

		return( -1 );
	}
	internal_segment->value_offset = value_offset;

	return( 1 );
}

