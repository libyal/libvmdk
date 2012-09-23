/*
 * The segment table functions
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
#include "libmfdata_libbfio.h"
#include "libmfdata_libcdata.h"
#include "libmfdata_libcerror.h"
#include "libmfdata_libcnotify.h"
#include "libmfdata_segment.h"
#include "libmfdata_segment_table.h"
#include "libmfdata_types.h"
#include "libmfdata_unused.h"

/* Initializes the segment table
 *
 * If the flag LIBMFDATA_FLAG_IO_HANDLE_MANAGED is set the library
 * takes over management of the IO segment_table and the IO segment_table is freed when
 * no longer needed
 *
 * Returns 1 if successful or -1 on error
 */
int libmfdata_segment_table_initialize(
     libmfdata_segment_table_t **segment_table,
     intptr_t *io_handle,
     int (*free_io_handle)(
            intptr_t **io_handle,
            libcerror_error_t **error ),
     int (*clone_io_handle)(
            intptr_t **destination_io_handle,
            intptr_t *source_io_handle,
            libcerror_error_t **error ),
     int (*set_segment_name)(
            intptr_t *io_handle,
            libbfio_handle_t *file_io_handle,
            int segment_index,
            libcerror_error_t **error ),
     ssize_t (*read_segment_data)(
                intptr_t *io_handle,
                libbfio_handle_t *file_io_handle,
                int segment_index,
                uint8_t *segment_data,
                size_t segment_data_size,
                libcerror_error_t **error ),
     ssize_t (*write_segment_data)(
                intptr_t *io_handle,
                libbfio_handle_t *file_io_handle,
                int segment_index,
                const uint8_t *segment_data,
                size_t segment_data_size,
                libcerror_error_t **error ),
     off64_t (*seek_segment_offset)(
                intptr_t *io_handle,
                libbfio_handle_t *file_io_handle,
                int segment_index,
                off64_t segment_offset,
                int whence,
                libcerror_error_t **error ),
     uint8_t flags,
     libcerror_error_t **error )
{
	libmfdata_internal_segment_table_t *internal_segment_table = NULL;
	static char *function                                      = "libmfdata_segment_table_initialize";

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
	if( *segment_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid segment table value already set.",
		 function );

		return( -1 );
	}
	internal_segment_table = memory_allocate_structure(
	                          libmfdata_internal_segment_table_t );

	if( internal_segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create segment table.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_segment_table,
	     0,
	     sizeof( libmfdata_internal_segment_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear segment table.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( internal_segment_table->segments ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create segments array.",
		 function );

		goto on_error;
	}
	internal_segment_table->flags              |= flags;
	internal_segment_table->io_handle           = io_handle;
	internal_segment_table->free_io_handle      = free_io_handle;
	internal_segment_table->clone_io_handle     = clone_io_handle;
	internal_segment_table->set_segment_name    = set_segment_name;
	internal_segment_table->read_segment_data   = read_segment_data;
	internal_segment_table->write_segment_data  = write_segment_data;
	internal_segment_table->seek_segment_offset = seek_segment_offset;

	*segment_table = (libmfdata_segment_table_t *) internal_segment_table;

	return( 1 );

on_error:
	if( internal_segment_table != NULL )
	{
		memory_free(
		 internal_segment_table );
	}
	return( -1 );
}

/* Frees the segment table
 * Returns 1 if successful or -1 on error
 */
int libmfdata_segment_table_free(
     libmfdata_segment_table_t **segment_table,
     libcerror_error_t **error )
{
	libmfdata_internal_segment_table_t *internal_segment_table = NULL;
	static char *function                                      = "libmfdata_segment_table_free";
	int result                                                 = 1;

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
	if( *segment_table != NULL )
	{
		internal_segment_table = (libmfdata_internal_segment_table_t *) *segment_table;
		*segment_table         = NULL;

		if( libcdata_array_free(
		     &( internal_segment_table->segments ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libmfdata_segment_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free the segments array.",
			 function );

			result = -1;
		}
		if( ( internal_segment_table->flags & LIBMFDATA_FLAG_IO_HANDLE_MANAGED ) != 0 )
		{
			if( internal_segment_table->io_handle != NULL )
			{
				if( internal_segment_table->free_io_handle != NULL )
				{
					if( internal_segment_table->free_io_handle(
					     &( internal_segment_table->io_handle ),
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
						 "%s: unable to free IO handle.",
						 function );

						result = -1;
					}
				}
			}
		}
		memory_free(
		 internal_segment_table );
	}
	return( result );
}

/* Clones (duplicates) the segment table
 * Returns 1 if successful or -1 on error
 */
int libmfdata_segment_table_clone(
     libmfdata_segment_table_t **destination_segment_table,
     libmfdata_segment_table_t *source_segment_table,
     libcerror_error_t **error )
{
	libmfdata_internal_segment_table_t *internal_source_segment_table = NULL;
	intptr_t *destination_io_handle                                   = NULL;
	static char *function                                             = "libmfdata_segment_table_clone";

	if( destination_segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination segment table.",
		 function );

		return( -1 );
	}
	if( *destination_segment_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: destination segment table already set.",
		 function );

		return( -1 );
	}
	if( source_segment_table == NULL )
	{
		*destination_segment_table = NULL;

		return( 1 );
	}
	internal_source_segment_table = (libmfdata_internal_segment_table_t *) source_segment_table;

	if( internal_source_segment_table->io_handle != NULL )
	{
		if( internal_source_segment_table->free_io_handle == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid source segment table - missing free IO handle function.",
			 function );

			goto on_error;
		}
		if( internal_source_segment_table->clone_io_handle == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid source segment table - missing clone IO handle function.",
			 function );

			goto on_error;
		}
		if( internal_source_segment_table->clone_io_handle(
		     &destination_io_handle,
		     internal_source_segment_table->io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to clone IO handle.",
			 function );

			goto on_error;
		}
	}
	if( libmfdata_segment_table_initialize(
	     destination_segment_table,
	     destination_io_handle,
	     internal_source_segment_table->free_io_handle,
	     internal_source_segment_table->clone_io_handle,
	     internal_source_segment_table->set_segment_name,
	     internal_source_segment_table->read_segment_data,
	     internal_source_segment_table->write_segment_data,
	     internal_source_segment_table->seek_segment_offset,
	     LIBMFDATA_FLAG_IO_HANDLE_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination segment table.",
		 function );

		goto on_error;
	}
	if( *destination_segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing destination segment table.",
		 function );

		goto on_error;
	}
	destination_io_handle = NULL;

	if( libmfdata_segment_table_clone_segments(
	     *destination_segment_table,
	     source_segment_table,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy segments.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_segment_table != NULL )
	{
		libmfdata_segment_table_free(
		 destination_segment_table,
		 NULL );
	}
	if( destination_io_handle != NULL )
	{
		internal_source_segment_table->free_io_handle(
		 &destination_io_handle,
		 NULL );
	}
	return( -1 );
}

/* Clones (duplicates) the segments
 * Returns 1 if successful or -1 on error
 */
int libmfdata_segment_table_clone_segments(
     libmfdata_segment_table_t *destination_segment_table,
     libmfdata_segment_table_t *source_segment_table,
     libcerror_error_t **error )
{
	libmfdata_internal_segment_table_t *internal_destination_segment_table = NULL;
	libmfdata_internal_segment_table_t *internal_source_segment_table      = NULL;
	libmfdata_internal_segment_t *internal_destination_segment             = NULL;
	libmfdata_internal_segment_t *internal_source_segment                  = NULL;
	static char *function                                                  = "libmfdata_segment_table_clone_segments";
	int number_of_segments                                                 = 0;
	int segment_index                                                      = 0;

	if( destination_segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination segment table.",
		 function );

		return( -1 );
	}
	internal_destination_segment_table = (libmfdata_internal_segment_table_t *) destination_segment_table;

	if( source_segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source segment table.",
		 function );

		return( -1 );
	}
	internal_source_segment_table = (libmfdata_internal_segment_table_t *) source_segment_table;

	if( libcdata_array_get_number_of_entries(
	     internal_source_segment_table->segments,
	     &number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segments from source segments array.",
		 function );

		return( -1 );
	}
	if( libcdata_array_empty(
	     internal_destination_segment_table->segments,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libmfdata_segment_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty destination segments array.",
		 function );

		return( -1 );
	}
	if( libcdata_array_resize(
	     internal_destination_segment_table->segments,
	     number_of_segments,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libmfdata_segment_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize destination segments array.",
		 function );

		return( -1 );
	}
	for( segment_index = 0;
	     segment_index < number_of_segments;
	     segment_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_source_segment_table->segments,
		     segment_index,
		     (intptr_t **) &internal_source_segment,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment: %d from source segments array.",
			 function,
			 segment_index );

			return( -1 );
		}
		if( libmfdata_segment_initialize(
		     (libmfdata_segment_t **) &internal_destination_segment,
		     destination_segment_table,
		     segment_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination segment.",
			 function );

			return( -1 );
		}
		internal_destination_segment->file_io_pool_entry = internal_source_segment->file_io_pool_entry;
		internal_destination_segment->size               = internal_source_segment->size;
		internal_destination_segment->value_offset       = internal_source_segment->value_offset;

		if( libcdata_array_set_entry_by_index(
		     internal_destination_segment_table->segments,
		     segment_index,
		     (intptr_t *) internal_destination_segment,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set segment: %d in destination segments array.",
			 function,
			 segment_index );

			libmfdata_segment_free(
			 (libmfdata_segment_t **) &internal_destination_segment,
			 NULL );

			return( -1 );
		}
		internal_destination_segment = NULL;
	}
	internal_destination_segment_table->value_size = internal_source_segment_table->value_size;

	return( 1 );
}

/* Empties the segment table
 * Returns 1 if successful or -1 on error
 */
int libmfdata_segment_table_empty(
     libmfdata_segment_table_t *segment_table,
     libcerror_error_t **error )
{
	libmfdata_internal_segment_table_t *internal_segment_table = NULL;
	static char *function                                      = "libmfdata_segment_table_empty";

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
	internal_segment_table = (libmfdata_internal_segment_table_t *) segment_table;

	if( libcdata_array_empty(
	     internal_segment_table->segments,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libmfdata_segment_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty segments array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Resizes the segment table
 * Returns 1 if successful or -1 on error
 */
int libmfdata_segment_table_resize(
     libmfdata_segment_table_t *segment_table,
     int number_of_segments,
     libcerror_error_t **error )
{
	libmfdata_internal_segment_table_t *internal_segment_table = NULL;
	static char *function                                      = "libmfdata_segment_table_resize";

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
	internal_segment_table = (libmfdata_internal_segment_table_t *) segment_table;

	if( libcdata_array_resize(
	     internal_segment_table->segments,
	     number_of_segments,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libmfdata_segment_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize segments array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the value offset
 * Returns 1 if successful or -1 on error
 */
int libmfdata_segment_table_get_value_offset(
     libmfdata_segment_table_t *segment_table,
     off64_t *value_offset,
     libcerror_error_t **error )
{
	libmfdata_internal_segment_table_t *internal_segment_table = NULL;
	static char *function                                      = "libmfdata_segment_table_get_value_offset";

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
	internal_segment_table = (libmfdata_internal_segment_table_t *) segment_table;

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
	*value_offset = internal_segment_table->value_offset;

	return( 1 );
}

/* Retrieves the value size
 * Returns 1 if successful or -1 on error
 */
int libmfdata_segment_table_get_value_size(
     libmfdata_segment_table_t *segment_table,
     size64_t *value_size,
     libcerror_error_t **error )
{
	libmfdata_internal_segment_table_t *internal_segment_table = NULL;
	static char *function                                      = "libmfdata_segment_table_get_value_size";

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
	internal_segment_table = (libmfdata_internal_segment_table_t *) segment_table;

	if( value_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value size.",
		 function );

		return( -1 );
	}
	*value_size = internal_segment_table->value_size;

	return( 1 );
}

/* Retrieves the maximum segment size
 * Returns 1 if successful or -1 on error
 */
int libmfdata_segment_table_get_maximum_segment_size(
     libmfdata_segment_table_t *segment_table,
     size64_t *maximum_segment_size,
     libcerror_error_t **error )
{
	libmfdata_internal_segment_table_t *internal_segment_table = NULL;
	static char *function                                      = "libmfdata_segment_table_get_maximum_segment_size";

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
	internal_segment_table = (libmfdata_internal_segment_table_t *) segment_table;

	if( maximum_segment_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid maximum segment size.",
		 function );

		return( -1 );
	}
	*maximum_segment_size = internal_segment_table->maximum_segment_size;

	return( 1 );
}

/* Sets the maximum segment size
 * Returns 1 if successful or -1 on error
 */
int libmfdata_segment_table_set_maximum_segment_size(
     libmfdata_segment_table_t *segment_table,
     size64_t maximum_segment_size,
     libcerror_error_t **error )
{
	libmfdata_internal_segment_table_t *internal_segment_table = NULL;
	static char *function                                      = "libmfdata_segment_table_set_maximum_segment_size";

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
	internal_segment_table = (libmfdata_internal_segment_table_t *) segment_table;

	if( maximum_segment_size > (size64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid maximum segment size value exceeds maximum.",
		 function );

		return( -1 );
	}
	internal_segment_table->maximum_segment_size = maximum_segment_size;

	return( 1 );
}

/* Retrieves the number of segments
 * Returns 1 if successful or -1 on error
 */
int libmfdata_segment_table_get_number_of_segments(
     libmfdata_segment_table_t *segment_table,
     int *number_of_segments,
     libcerror_error_t **error )
{
	libmfdata_internal_segment_table_t *internal_segment_table = NULL;
	static char *function                                      = "libmfdata_segment_table_get_number_of_segments";

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
	internal_segment_table = (libmfdata_internal_segment_table_t *) segment_table;

	if( libcdata_array_get_number_of_entries(
	     internal_segment_table->segments,
	     number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segments from segments array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific segment
 * Returns 1 if successful or -1 on error
 */
int libmfdata_segment_table_get_segment_by_index(
     libmfdata_segment_table_t *segment_table,
     int segment_index,
     libmfdata_segment_t **segment,
     libcerror_error_t **error )
{
	libmfdata_internal_segment_table_t *internal_segment_table = NULL;
	static char *function                                      = "libmfdata_segment_table_get_segment_by_index";

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
	internal_segment_table = (libmfdata_internal_segment_table_t *) segment_table;

	if( libcdata_array_get_entry_by_index(
	     internal_segment_table->segments,
	     segment_index,
	     (intptr_t **) segment,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment: %d from segments array.",
		 function,
		 segment_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a segment for the specified value offset
 * Returns 1 if successful, 0 if no matching segment was found or -1 on error
 */
int libmfdata_segment_table_get_segment_at_value_offset(
     libmfdata_segment_table_t *segment_table,
     off64_t value_offset,
     int *segment_index,
     libmfdata_segment_t **segment,
     off64_t *segment_offset,
     libcerror_error_t **error )
{
	libmfdata_internal_segment_table_t *internal_segment_table = NULL;
	static char *function                                      = "libmfdata_segment_table_get_segment_index_at_value_offset";
	off64_t calculated_value_offset                            = 0;
	size64_t segment_size                                      = 0;
	int calculated_segment_index                               = 0;
	int file_io_pool_entry                                     = 0;
	int initial_segment_index                                  = 0;
	int number_of_segments                                     = 0;

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
	internal_segment_table = (libmfdata_internal_segment_table_t *) segment_table;

	if( internal_segment_table->value_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment table - value size value out of bounds.",
		 function );

		return( -1 );
	}
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
	if( segment_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment offset.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     internal_segment_table->segments,
	     &number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segments from segments array.",
		 function );

		return( -1 );
	}
	if( ( internal_segment_table->flags & LIBMFDATA_SEGMENT_TABLE_FLAG_CALCULATE_VALUE_OFFSETS ) != 0 )
	{
		for( calculated_segment_index = 0;
		     calculated_segment_index < number_of_segments;
		     calculated_segment_index++ )
		{
			if( libcdata_array_get_entry_by_index(
			     internal_segment_table->segments,
			     calculated_segment_index,
			     (intptr_t **) segment,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve segment: %d from segments array.",
				 function,
				 calculated_segment_index );

				return( -1 );
			}
			if( libmfdata_segment_get_range(
			     *segment,
			     &file_io_pool_entry,
			     &segment_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve segment range: %d.",
				 function,
				 calculated_segment_index );

				return( -1 );
			}
			if( libmfdata_segment_set_value_offset(
			     *segment,
			     calculated_value_offset,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set data offset of segment: %d.",
				 function,
				 calculated_segment_index );

				return( -1 );
			}
			if( value_offset < calculated_value_offset )
			{
				initial_segment_index = calculated_segment_index;
			}
			calculated_value_offset += (off64_t) segment_size;
		}
		internal_segment_table->flags &= ~( LIBMFDATA_SEGMENT_TABLE_FLAG_CALCULATE_VALUE_OFFSETS );
	}
	else
	{
		/* This assumes a fairly even distribution of the sizes of the segments
		 */
		initial_segment_index = (int) ( ( number_of_segments * value_offset ) / internal_segment_table->value_size );
	}
	/* Look for the corresponding segment upwards in the segment table
	 */
	for( calculated_segment_index = initial_segment_index;
	     calculated_segment_index < number_of_segments;
	     calculated_segment_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_segment_table->segments,
		     calculated_segment_index,
		     (intptr_t **) segment,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment: %d from segments array.",
			 function,
			 calculated_segment_index );

			return( -1 );
		}
		if( libmfdata_segment_get_range(
		     *segment,
		     &file_io_pool_entry,
		     &segment_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment range: %d.",
			 function,
			 calculated_segment_index );

			return( -1 );
		}
		if( libmfdata_segment_get_value_offset(
		     *segment,
		     &calculated_value_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to retrieve data offset of segment: %d.",
			 function,
			 calculated_segment_index );

			return( -1 );
		}
		/* Check if the data offset is in the segment
		 */
		if( ( value_offset >= calculated_value_offset )
		 && ( value_offset < ( calculated_value_offset + (off64_t) segment_size ) ) )
		{
			value_offset -= calculated_value_offset;

			break;
		}
		/* Check if the data offset is out of bounds
		 */
		if( value_offset < calculated_value_offset )
		{
			calculated_segment_index = number_of_segments;

			break;
		}
	}
	if( calculated_segment_index >= number_of_segments )
	{
		/* Look for the corresponding segment downwards in the segment table
		 */
		for( calculated_segment_index = initial_segment_index;
		     calculated_segment_index >= 0;
		     calculated_segment_index-- )
		{
			if( libcdata_array_get_entry_by_index(
			     internal_segment_table->segments,
			     calculated_segment_index,
			     (intptr_t **) segment,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve segment: %d from segments array.",
				 function,
				 calculated_segment_index );

				return( -1 );
			}
			if( libmfdata_segment_get_range(
			     *segment,
			     &file_io_pool_entry,
			     &segment_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve segment range: %d.",
				 function,
				 calculated_segment_index );

				return( -1 );
			}
			if( libmfdata_segment_get_value_offset(
			     *segment,
			     &calculated_value_offset,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to retrieve data offset of segment: %d.",
				 function,
				 calculated_segment_index );

				return( -1 );
			}
			/* Check if the data offset is in the segment
			 */
			if( ( value_offset >= calculated_value_offset )
			 && ( value_offset < ( calculated_value_offset + (off64_t) segment_size ) ) )
			{
				value_offset -= calculated_value_offset;

				break;
			}
			/* Check if the data offset is out of bounds
			 */
			if( value_offset > calculated_value_offset )
			{
				calculated_segment_index = -1;

				break;
			}
		}
	}
	if( calculated_segment_index < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment index value out of bounds.",
		 function );

		return( -1 );
	}
	if( value_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment offset value out of bounds.",
		 function );

		return( -1 );
	}
	*segment_index  = calculated_segment_index;
	*segment_offset = value_offset;

	return( 1 );
}

/* Sets the data offset and size of a specific segment
 * Returns 1 if successful or -1 on error
 */
int libmfdata_segment_table_set_segment_by_index(
     libmfdata_segment_table_t *segment_table,
     int segment_index,
     int file_io_pool_entry,
     size64_t segment_size,
     libcerror_error_t **error )
{
	libmfdata_internal_segment_table_t *internal_segment_table = NULL;
	libmfdata_segment_t *segment                               = NULL;
	static char *function                                      = "libmfdata_segment_table_set_segment_by_index";
	size64_t previous_segment_size                             = 0;
	int previous_file_io_pool_entry                            = 0;

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
	internal_segment_table = (libmfdata_internal_segment_table_t *) segment_table;

	if( libcdata_array_get_entry_by_index(
	     internal_segment_table->segments,
	     segment_index,
	     (intptr_t **) &segment,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment: %d from segments array.",
		 function,
		 segment_index );

		return( -1 );
	}
	if( segment == NULL )
	{
		if( libmfdata_segment_initialize(
		     &segment,
		     segment_table,
		     segment_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create segment.",
			 function );

			return( -1 );
		}
		if( libcdata_array_set_entry_by_index(
		     internal_segment_table->segments,
		     segment_index,
		     (intptr_t *) segment,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set segment: %d in segments array.",
			 function,
			 segment_index );

			libmfdata_segment_free(
			 &segment,
			 NULL );

			return( -1 );
		}
	}
	else
	{
		if( libmfdata_segment_get_range(
		     segment,
		     &previous_file_io_pool_entry,
		     &previous_segment_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment range: %d.",
			 function,
			 segment_index );

			return( -1 );
		}
		internal_segment_table->value_size -= previous_segment_size;
	}
	if( libmfdata_segment_set_range(
	     segment,
	     file_io_pool_entry,
	     segment_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment range: %d.",
		 function,
		 segment_index );

		return( -1 );
	}
	internal_segment_table->value_size += segment_size;
	internal_segment_table->flags      |= LIBMFDATA_SEGMENT_TABLE_FLAG_CALCULATE_VALUE_OFFSETS;

	return( 1 );
}

/* Appends a segment file IO pool entry and size
 * Returns 1 if successful or -1 on error
 */
int libmfdata_segment_table_append_segment(
     libmfdata_segment_table_t *segment_table,
     int *segment_index,
     int file_io_pool_entry,
     size64_t segment_size,
     libcerror_error_t **error )
{
	libmfdata_internal_segment_table_t *internal_segment_table = NULL;
	libmfdata_segment_t *segment                               = NULL;
	static char *function                                      = "libmfdata_segment_table_append_segment";

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
	internal_segment_table = (libmfdata_internal_segment_table_t *) segment_table;

	if( libmfdata_segment_initialize(
	     &segment,
	     segment_table,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create segment.",
		 function );

		return( -1 );
	}
	if( libmfdata_segment_set_range(
	     segment,
	     file_io_pool_entry,
	     segment_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment range.",
		 function );

		libmfdata_segment_free(
		 &segment,
		 NULL );

		return( -1 );
	}
	if( libmfdata_segment_set_value_offset(
	     segment,
	     internal_segment_table->value_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment data offset.",
		 function );

		libmfdata_segment_free(
		 &segment,
		 NULL );

		return( -1 );
	}
	if( libcdata_array_append_entry(
	     internal_segment_table->segments,
	     segment_index,
	     (intptr_t *) segment,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append segment to segments array.",
		 function );

		libmfdata_segment_free(
		 &segment,
		 NULL );

		return( -1 );
	}
	if( libmfdata_segment_set_segment_index(
	     segment,
	     *segment_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment index.",
		 function );

		return( -1 );
	}
	internal_segment_table->value_size += segment_size;

	return( 1 );
}

/* Reads segment data into a buffer
 * Returns the number of bytes read or -1 on error
 */
ssize_t libmfdata_segment_table_read_segment_data(
         intptr_t *io_handle LIBMFDATA_ATTRIBUTE_UNUSED,
         libbfio_handle_t *file_io_handle,
         int segment_index LIBMFDATA_ATTRIBUTE_UNUSED,
         uint8_t *segment_data,
         size_t segment_data_size,
         libcerror_error_t **error )
{
	static char *function = "libmfdata_segment_table_read_segment_data";
	ssize_t read_count    = 0;

	LIBMFDATA_UNREFERENCED_PARAMETER( io_handle )
	LIBMFDATA_UNREFERENCED_PARAMETER( segment_index )

	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              segment_data,
	              segment_data_size,
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read segment data.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Writes segment data from a buffer
 * Returns the number of bytes written or -1 on error
 */
ssize_t libmfdata_segment_table_write_segment_data(
         intptr_t *io_handle LIBMFDATA_ATTRIBUTE_UNUSED,
         libbfio_handle_t *file_io_handle,
         int segment_index LIBMFDATA_ATTRIBUTE_UNUSED,
         const uint8_t *segment_data,
         size_t segment_data_size,
         libcerror_error_t **error )
{
	static char *function = "libmfdata_segment_table_write_segment_data";
	ssize_t write_count    = 0;

	LIBMFDATA_UNREFERENCED_PARAMETER( io_handle )
	LIBMFDATA_UNREFERENCED_PARAMETER( segment_index )

	write_count = libbfio_handle_write_buffer(
	               file_io_handle,
	               segment_data,
	               segment_data_size,
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to write segment data.",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Seeks a certain segment offset
 * Returns the offset or -1 on error
 */
off64_t libmfdata_segment_table_seek_segment_offset(
         intptr_t *io_handle LIBMFDATA_ATTRIBUTE_UNUSED,
         libbfio_handle_t *file_io_handle,
         int segment_index LIBMFDATA_ATTRIBUTE_UNUSED,
         off64_t segment_offset,
         int whence,
         libcerror_error_t **error )
{
	static char *function = "libmfdata_segment_table_seek_segment_offset";

	LIBMFDATA_UNREFERENCED_PARAMETER( io_handle )
	LIBMFDATA_UNREFERENCED_PARAMETER( segment_index )

	segment_offset = libbfio_handle_seek_offset(
	                  file_io_handle,
	                  segment_offset,
	                  whence,
	                  error );

	if( segment_offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to seek segment offset.",
		 function );

		return( -1 );
	}
	return( segment_offset );
}

/* Reads a buffer
 * Returns the number of bytes read or -1 on error
 */
ssize_t libmfdata_segment_table_read_buffer(
         libmfdata_segment_table_t *segment_table,
         libbfio_pool_t *file_io_pool,
         uint8_t *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle                           = NULL;
	libmfdata_internal_segment_table_t *internal_segment_table = NULL;
	libmfdata_segment_t *segment                               = NULL;
	static char *function                                      = "libmfdata_segment_table_read_buffer";
	off64_t segment_offset                                     = 0;
	size64_t file_io_handle_size                               = 0;
	size64_t segment_size                                      = 0;
	size_t buffer_offset                                       = 0;
	size_t read_size                                           = 0;
	ssize_t read_count                                         = 0;
	int file_io_pool_entry                                     = 0;
	int number_of_segments                                     = 0;

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
	internal_segment_table = (libmfdata_internal_segment_table_t *) segment_table;

	if( internal_segment_table->value_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment table - value offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( internal_segment_table->read_segment_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment table - missing read segment data function.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( buffer_size == 0 )
	{
		return( 0 );
	}
	/* Do not read beyond the value size
	 */
	if( (size64_t) internal_segment_table->value_offset >= internal_segment_table->value_size )
	{
		return( 0 );
	}
	if( libcdata_array_get_number_of_entries(
	     internal_segment_table->segments,
	     &number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of entries from segments array.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_segment_table->segments,
	     internal_segment_table->segment_index,
	     (intptr_t **) &segment,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment: %d from segments array.",
		 function,
		 internal_segment_table->segment_index );

		return( -1 );
	}
	if( libmfdata_segment_get_range(
	     segment,
	     &file_io_pool_entry,
	     &segment_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment range: %d.",
		 function,
		 internal_segment_table->segment_index );

		return( -1 );
	}
	while( buffer_size > 0 )
	{
		if( libbfio_pool_get_handle(
		     file_io_pool,
		     file_io_pool_entry,
		     &file_io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file IO pool entry: %d from pool.",
			 function,
			 file_io_pool_entry );

			return( -1 );
		}
		if( libbfio_handle_get_offset(
		     file_io_handle,
		     &segment_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset from file IO pool entry: %d.",
			 function,
			 file_io_pool_entry );

			return( -1 );
		}
		if( libbfio_handle_get_size(
		     file_io_handle,
		     &file_io_handle_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size from file IO pool entry: %d.",
			 function,
			 file_io_pool_entry );

			return( -1 );
		}
		if( segment_offset > (off64_t) file_io_handle_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: file IO pool entry offset exceeds file IO pool entry size.",
			 function );

			return( -1 );
		}
		read_size = buffer_size;

		if( ( (size64_t) read_size + (size64_t) segment_offset ) > segment_size )
		{
			read_size = (size_t) ( segment_size - segment_offset );
		}
		if( read_size == 0 )
		{
			break;
		}
		read_count = internal_segment_table->read_segment_data(
		              internal_segment_table->io_handle,
		              file_io_handle,
		              internal_segment_table->segment_index,
		              &( buffer[ buffer_offset ] ),
		              read_size,
		              error );

		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data from segment: %d.",
			 function,
			 internal_segment_table->segment_index );

			return( -1 );
		}
		internal_segment_table->value_offset += (off64_t) read_count;

		if( ( (size64_t) segment_offset + (size64_t) read_count ) == segment_size )
		{
			internal_segment_table->segment_index += 1;

			if( internal_segment_table->segment_index < number_of_segments )
			{
				if( libcdata_array_get_entry_by_index(
				     internal_segment_table->segments,
				     internal_segment_table->segment_index,
				     (intptr_t **) &segment,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve segment: %d from segments array.",
					 function,
					 internal_segment_table->segment_index );

					return( -1 );
				}
				if( libmfdata_segment_get_range(
				     segment,
				     &file_io_pool_entry,
				     &segment_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve segment range: %d.",
					 function,
					 internal_segment_table->segment_index );

					return( -1 );
				}
				if( libbfio_pool_seek_offset(
				     file_io_pool,
				     file_io_pool_entry,
				     0,
				     SEEK_SET,
				     error ) != 0 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_SEEK_FAILED,
					 "%s: unable to seek offset: 0 in file IO pool entry: %d.",
					 function,
					 file_io_pool_entry );

					return( -1 );
				}
			}
		}
		buffer_size   -= (size_t) read_count;
		buffer_offset += (size_t) read_count;

		if( (size64_t) internal_segment_table->value_offset >= internal_segment_table->value_size )
		{
			break;
		}
	}
	return( (ssize_t) buffer_offset );
}

/* Writes a buffer
 * Returns the number of bytes written or -1 on error
 */
ssize_t libmfdata_segment_table_write_buffer(
         libmfdata_segment_table_t *segment_table,
         libbfio_pool_t *file_io_pool,
         const uint8_t *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle                           = NULL;
	libmfdata_internal_segment_table_t *internal_segment_table = NULL;
	libmfdata_segment_t *segment                               = NULL;
	static char *function                                      = "libmfdata_segment_table_write_buffer";
	off64_t segment_offset                                     = 0;
	size64_t file_io_handle_size                               = 0;
	size64_t segment_size                                      = 0;
	size64_t value_size                                        = 0;
	size_t buffer_offset                                       = 0;
	size_t write_size                                          = 0;
	ssize_t write_count                                        = 0;
	int file_io_pool_entry                                     = 0;
	int number_of_segments                                     = 0;
	int segment_index                                          = 0;

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
	internal_segment_table = (libmfdata_internal_segment_table_t *) segment_table;

	if( internal_segment_table->value_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment table - value offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( internal_segment_table->write_segment_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment table - missing write segment data function.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( buffer_size == 0 )
	{
		return( 0 );
	}
	if( libcdata_array_get_number_of_entries(
	     internal_segment_table->segments,
	     &number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segments from source segments array.",
		 function );

		return( -1 );
	}
	while( buffer_size > 0 )
	{
		if( internal_segment_table->segment_index >= number_of_segments )
		{
			file_io_handle = NULL;

			if( libbfio_file_initialize(
			     &file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create file IO handle.",
				 function );

				return( -1 );
			}
			if( internal_segment_table->set_segment_name != NULL )
			{
				if( internal_segment_table->set_segment_name(
				     internal_segment_table->io_handle,
				     file_io_handle,
				     internal_segment_table->segment_index,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set segment name.",
					 function );

					libbfio_handle_free(
					 &file_io_handle,
					 NULL );

					return( -1 );
				}
			}
			if( libbfio_pool_append_handle(
			     file_io_pool,
			     &file_io_pool_entry,
			     file_io_handle,
			     LIBBFIO_OPEN_WRITE_TRUNCATE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append file IO handle to pool.",
				 function );

				libbfio_handle_free(
				 &file_io_handle,
				 NULL );

				return( -1 );
			}
			if( libbfio_pool_open(
			     file_io_pool,
			     file_io_pool_entry,
			     LIBBFIO_OPEN_WRITE_TRUNCATE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to open file IO pool entry: %d.",
				 function,
				 file_io_pool_entry );

				return( -1 );
			}
			file_io_handle = NULL;

			if( libmfdata_segment_table_append_segment(
			     segment_table,
			     &segment_index,
			     file_io_pool_entry,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append segment: %d.",
				 function,
				 number_of_segments );

				return( -1 );
			}
			number_of_segments++;
		}
		if( libcdata_array_get_entry_by_index(
		     internal_segment_table->segments,
		     internal_segment_table->segment_index,
		     (intptr_t **) &segment,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment: %d from segments array.",
			 function,
			 internal_segment_table->segment_index );

			return( -1 );
		}
		if( libmfdata_segment_get_range(
		     segment,
		     &file_io_pool_entry,
		     &segment_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment range: %d.",
			 function,
			 internal_segment_table->segment_index );

			return( -1 );
		}
		if( libbfio_pool_get_handle(
		     file_io_pool,
		     file_io_pool_entry,
		     &file_io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file IO pool entry: %d from pool.",
			 function,
			 file_io_pool_entry );

			return( -1 );
		}
		if( libbfio_handle_get_offset(
		     file_io_handle,
		     &segment_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset from file IO pool entry: %d.",
			 function,
			 file_io_pool_entry );

			return( -1 );
		}
		if( libbfio_handle_get_size(
		     file_io_handle,
		     &file_io_handle_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size from file IO pool entry: %d.",
			 function,
			 file_io_pool_entry );

			return( -1 );
		}
		write_size = buffer_size;
		value_size = (size64_t) write_size + (size64_t) segment_offset;

		if( ( internal_segment_table->maximum_segment_size != 0 )
		 && ( value_size >= internal_segment_table->maximum_segment_size ) )
		{
			write_size = (size_t) ( internal_segment_table->maximum_segment_size - segment_offset );
		}
		if( write_size == 0 )
		{
			break;
		}
		write_count = internal_segment_table->write_segment_data(
		               internal_segment_table->io_handle,
		               file_io_handle,
		               internal_segment_table->segment_index,
		               &( buffer[ buffer_offset ] ),
		               write_size,
		               error );

		if( write_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write data to segment: %d.",
			 function,
			 internal_segment_table->segment_index );

			return( -1 );
		}
		internal_segment_table->value_offset += (off64_t) write_count;

		if( libmfdata_segment_set_range(
		     segment,
		     file_io_pool_entry,
		     segment_size + write_count,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set segment range: %d.",
			 function,
			 segment_index );

			return( -1 );
		}
		value_size = (size64_t) write_count + (size64_t) segment_offset;

		if( ( internal_segment_table->maximum_segment_size != 0 )
		 && ( value_size >= internal_segment_table->maximum_segment_size ) )
		{
			internal_segment_table->segment_index += 1;
		}
		if( value_size > internal_segment_table->value_size )
		{
			internal_segment_table->value_size = value_size;
		}
		buffer_size   -= write_count;
		buffer_offset += write_count;
	}
	return( (ssize_t) buffer_offset );
}

/* Seeks a certain offset
 * Returns the offset or -1 on error
 */
off64_t libmfdata_segment_table_seek_offset(
         libmfdata_segment_table_t *segment_table,
         libbfio_pool_t *file_io_pool,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle                           = NULL;
	libmfdata_internal_segment_table_t *internal_segment_table = NULL;
	libmfdata_segment_t *segment                               = NULL;
	static char *function                                      = "libmfdata_segment_table_seek_offset";
	off64_t segment_offset                                     = 0;
	size64_t segment_size                                      = 0;
	int file_io_pool_entry                                     = 0;
	int segment_index                                          = 0;

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
	internal_segment_table = (libmfdata_internal_segment_table_t *) segment_table;

	if( internal_segment_table->seek_segment_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment table - missing seek segment offset function.",
		 function );

		return( -1 );
	}
	if( ( whence != SEEK_CUR )
	 && ( whence != SEEK_END )
	 && ( whence != SEEK_SET ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported whence.",
		 function );

		return( -1 );
	}
	if( whence == SEEK_CUR )
	{
		offset += internal_segment_table->value_offset;
	}
	else if( whence == SEEK_END )
	{
		offset += (off64_t) internal_segment_table->value_size;
	}
	if( offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( offset < (off64_t) internal_segment_table->value_size )
	{
		if( libmfdata_segment_table_get_segment_at_value_offset(
		     segment_table,
		     offset,
		     &segment_index,
		     &segment,
		     &segment_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment for value offset: %" PRIi64 ".",
			 function,
			 offset );

			return( -1 );
		}
		if( libmfdata_segment_get_range(
		     segment,
		     &file_io_pool_entry,
		     &segment_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment range: %d.",
			 function,
			 internal_segment_table->segment_index );

			return( -1 );
		}
		if( libbfio_pool_get_handle(
		     file_io_pool,
		     file_io_pool_entry,
		     &file_io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file IO pool entry: %d from pool.",
			 function,
			 file_io_pool_entry );

			return( -1 );
		}
		if( internal_segment_table->seek_segment_offset(
		     internal_segment_table->io_handle,
		     file_io_handle,
		     segment_index,
		     segment_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek data offset: %" PRIi64 " in file IO pool entry: %d.",
			 function,
			 segment_offset,
			 file_io_pool_entry );

			return( -1 );
		}
	}
	else
	{
		if( libcdata_array_get_number_of_entries(
		     internal_segment_table->segments,
		     &segment_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the number of entries from segments array.",
			 function );

			return( -1 );
		}
	}
	internal_segment_table->segment_index = segment_index;
	internal_segment_table->value_offset  = offset;

	return( offset );
}

/* Retrieves the file IO handle of the segment file at the current offset
 * Returns 1 if successful or -1 on error
 */
int libmfdata_segment_table_get_file_io_handle(
     libmfdata_segment_table_t *segment_table,
     libbfio_pool_t *file_io_pool,
     libbfio_handle_t **file_io_handle,
     libcerror_error_t **error )
{
	libmfdata_internal_segment_table_t *internal_segment_table = NULL;
	static char *function                                      = "libmfdata_segment_table_get_file_io_handle";

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
	internal_segment_table = (libmfdata_internal_segment_table_t *) segment_table;

	if( libbfio_pool_get_handle(
	     file_io_pool,
	     internal_segment_table->segment_index,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file IO handle for pool entry: %d (offset: %" PRIi64 ").",
		 function,
		 internal_segment_table->segment_index,
		 internal_segment_table->value_offset );

		return( -1 );
	}
	return( 1 );
}

