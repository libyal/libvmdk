/*
 * The file functions
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

#include "libmfdata_date_time.h"
#include "libmfdata_file.h"
#include "libmfdata_file_list.h"
#include "libmfdata_libbfio.h"
#include "libmfdata_libcerror.h"
#include "libmfdata_libmfcache.h"
#include "libmfdata_types.h"

/* Initializes the file
 * Returns 1 if successful or -1 on error
 */
int libmfdata_file_initialize(
     libmfdata_file_t **file,
     libmfdata_file_list_t *list,
     int element_index,
     libcerror_error_t **error )
{
	libmfdata_internal_file_t *internal_file = NULL;
	static char *function                    = "libmfdata_file_initialize";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( *file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file value already set.",
		 function );

		return( -1 );
	}
	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	if( element_index < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid element index value less than zero.",
		 function );

		return( -1 );
	}
	internal_file = memory_allocate_structure(
	                 libmfdata_internal_file_t );

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_file,
	     0,
	     sizeof( libmfdata_internal_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file.",
		 function );

		goto on_error;
	}
	internal_file->list               = list;
	internal_file->element_index      = element_index;
	internal_file->file_io_pool_entry = -1;
	internal_file->timestamp          = libmfdata_date_time_get_timestamp();

	*file = (libmfdata_file_t *) internal_file;

	return( 1 );

on_error:
	if( internal_file != NULL )
	{
		memory_free(
		 internal_file );
	}
	return( -1 );
}

/* Frees the file
 * Returns 1 if successful or -1 on error
 */
int libmfdata_file_free(
     libmfdata_file_t **file,
     libcerror_error_t **error )
{
	static char *function = "libmfdata_file_free";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( *file != NULL )
	{
		memory_free(
		 *file );

		*file = NULL;
	}
	return( 1 );
}

/* Retrieves the element index
 * Returns 1 if successful or -1 on error
 */
int libmfdata_file_get_element_index(
     libmfdata_file_t *file,
     int *element_index,
     libcerror_error_t **error )
{
	libmfdata_internal_file_t *internal_file = NULL;
	static char *function                    = "libmfdata_file_get_element_index";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libmfdata_internal_file_t *) file;

	if( element_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid element index.",
		 function );

		return( -1 );
	}
	*element_index = internal_file->element_index;

	return( 1 );
}

/* Sets the element index
 * Returns 1 if successful or -1 on error
 */
int libmfdata_file_set_element_index(
     libmfdata_file_t *file,
     int element_index,
     libcerror_error_t **error )
{
	libmfdata_internal_file_t *internal_file = NULL;
	static char *function                    = "libmfdata_file_set_element_index";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libmfdata_internal_file_t *) file;

	if( element_index < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid element index value less than zero.",
		 function );

		return( -1 );
	}
	internal_file->element_index = element_index;

	return( 1 );
}

/* Retrieves the time stamp
 * Returns 1 if successful or -1 on error
 */
int libmfdata_file_get_timestamp(
     libmfdata_file_t *file,
     time_t *timestamp,
     libcerror_error_t **error )
{
	libmfdata_internal_file_t *internal_file = NULL;
	static char *function                    = "libmfdata_file_get_timestamp";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libmfdata_internal_file_t *) file;

	if( timestamp == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid time stamp.",
		 function );

		return( -1 );
	}
	*timestamp = internal_file->timestamp;

	return( 1 );
}

/* Data range functions
 */

/* Retrieves the file IO pool entry
 * Returns 1 if successful or -1 on error
 */
int libmfdata_file_get_data_range(
     libmfdata_file_t *file,
     int *file_io_pool_entry,
     libcerror_error_t **error )
{
	libmfdata_internal_file_t *internal_file = NULL;
	static char *function                    = "libmfdata_file_get_data_range";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libmfdata_internal_file_t *) file;

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
	*file_io_pool_entry = internal_file->file_io_pool_entry;

	return( 1 );
}

/* Sets the file IO pool entry
 * Returns 1 if successful or -1 on error
 */
int libmfdata_file_set_data_range(
     libmfdata_file_t *file,
     int file_io_pool_entry,
     libcerror_error_t **error )
{
	libmfdata_internal_file_t *internal_file = NULL;
	static char *function                    = "libmfdata_file_set_data_range";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libmfdata_internal_file_t *) file;

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
	internal_file->file_io_pool_entry = file_io_pool_entry;
	internal_file->timestamp          = libmfdata_date_time_get_timestamp();

	return( 1 );
}

/* File value functions
 */

/* Retrieves the file value
 * Returns 1 if successful or -1 on error
 */
int libmfdata_file_get_file_value(
     libmfdata_file_t *file,
     libbfio_pool_t *file_io_pool,
     libmfcache_cache_t *cache,
     intptr_t **file_value,
     uint8_t read_flags,
     libcerror_error_t **error )
{
	libmfdata_internal_file_t *internal_file = NULL;
	static char *function                    = "libmfdata_file_get_file_value";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libmfdata_internal_file_t *) file;

	if( libmfdata_file_list_get_file_value(
	     internal_file->list,
	     file_io_pool,
	     cache,
	     file,
	     file_value,
	     read_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the file value
 *
 * If the flag LIBMFDATA_FILE_VALUE_FLAG_MANAGED is set the file
 * takes over management of the value and the value is freed when
 * no longer needed.
 *
 * Returns 1 if successful or -1 on error
 */
int libmfdata_file_set_file_value(
     libmfdata_file_t *file,
     libmfcache_cache_t *cache,
     intptr_t *file_value,
     int (*free_file_value)(
            intptr_t **file_value,
            libcerror_error_t **error ),
     uint8_t flags,
     libcerror_error_t **error )
{
	libmfdata_internal_file_t *internal_file = NULL;
	static char *function                    = "libmfdata_file_set_file_value";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libmfdata_internal_file_t *) file;

	if( libmfdata_file_list_set_file_value(
	     internal_file->list,
	     cache,
	     file,
	     file_value,
	     free_file_value,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set file file value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

