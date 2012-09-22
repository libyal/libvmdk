/*
 * The file list functions
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
#include "libmfdata_definitions.h"
#include "libmfdata_file.h"
#include "libmfdata_file_list.h"
#include "libmfdata_libbfio.h"
#include "libmfdata_libcerror.h"
#include "libmfdata_libcnotify.h"
#include "libmfdata_libmfcache.h"
#include "libmfdata_types.h"

#define libmfdata_file_list_calculate_cache_entry_index( data_file_entry, number_of_cache_entries ) \
	data_file_entry % number_of_cache_entries

/* Initializes the list
 *
 * If the flag LIBMFDATA_FLAG_IO_HANDLE_MANAGED is set the list
 * takes over management of the IO handle and the IO handle is freed when
 * no longer needed
 *
 * Returns 1 if successful or -1 on error
 */
int libmfdata_file_list_initialize(
     libmfdata_file_list_t **file_list,
     intptr_t *io_handle,
     int (*free_io_handle)(
            intptr_t **io_handle,
            libcerror_error_t **error ),
     int (*clone_io_handle)(
            intptr_t **destination_io_handle,
            intptr_t *source_io_handle,
            libcerror_error_t **error ),
     int (*read_file_data)(
            intptr_t *io_handle,
            libbfio_pool_t *file_io_pool,
            int file_io_pool_entry,
            libmfdata_file_t *file,
            libmfcache_cache_t *cache,
            uint8_t read_flags,
            libcerror_error_t **error ),
     uint8_t flags,
     libcerror_error_t **error )
{
	libmfdata_internal_file_list_t *internal_file_list = NULL;
	static char *function                              = "libmfdata_file_list_initialize";

	if( file_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file list.",
		 function );

		return( -1 );
	}
	if( *file_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file list value already set.",
		 function );

		return( -1 );
	}
	if( read_file_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid read file data function.",
		 function );

		return( -1 );
	}
	internal_file_list = memory_allocate_structure(
	                      libmfdata_internal_file_list_t );

	if( internal_file_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file list.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_file_list,
	     0,
	     sizeof( libmfdata_internal_file_list_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file list.",
		 function );

		goto on_error;
	}
	if( libmfdata_array_initialize(
	     &( internal_file_list->files ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create files array.",
		 function );

		goto on_error;
	}
	internal_file_list->flags          |= flags;
	internal_file_list->io_handle       = io_handle;
	internal_file_list->free_io_handle  = free_io_handle;
	internal_file_list->clone_io_handle = clone_io_handle;
	internal_file_list->read_file_data  = read_file_data;

	*file_list = (libmfdata_file_list_t *) internal_file_list;

	return( 1 );

on_error:
	if( internal_file_list != NULL )
	{
		memory_free(
		 internal_file_list );
	}
	return( -1 );
}

/* Frees the file list
 * Returns 1 if successful or -1 on error
 */
int libmfdata_file_list_free(
     libmfdata_file_list_t **file_list,
     libcerror_error_t **error )
{
	libmfdata_internal_file_list_t *internal_file_list = NULL;
	static char *function                              = "libmfdata_file_list_free";
	int result                                         = 1;

	if( file_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file list.",
		 function );

		return( -1 );
	}
	if( *file_list != NULL )
	{
		internal_file_list = (libmfdata_internal_file_list_t *) *file_list;
		*file_list         = NULL;

		if( libmfdata_array_free(
		     &( internal_file_list->files ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libmfdata_file_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free the files array.",
			 function );

			result = -1;
		}
		if( ( internal_file_list->flags & LIBMFDATA_FLAG_IO_HANDLE_MANAGED ) != 0 )
		{
			if( internal_file_list->io_handle != NULL )
			{
				if( internal_file_list->free_io_handle != NULL )
				{
					if( internal_file_list->free_io_handle(
					     &( internal_file_list->io_handle ),
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
		 internal_file_list );
	}
	return( result );
}

/* Clones (duplicates) the file list
 * Returns 1 if successful or -1 on error
 */
int libmfdata_file_list_clone(
     libmfdata_file_list_t **destination_file_list,
     libmfdata_file_list_t *source_file_list,
     libcerror_error_t **error )
{
	libmfdata_internal_file_list_t *internal_source_file_list = NULL;
	intptr_t *destination_io_handle                           = NULL;
	static char *function                                     = "libmfdata_file_list_clone";

	if( destination_file_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination file list.",
		 function );

		return( -1 );
	}
	if( *destination_file_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: destination file list already set.",
		 function );

		return( -1 );
	}
	if( source_file_list == NULL )
	{
		*destination_file_list = NULL;

		return( 1 );
	}
	internal_source_file_list = (libmfdata_internal_file_list_t *) source_file_list;

	if( internal_source_file_list->io_handle != NULL )
	{
		if( internal_source_file_list->free_io_handle == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid source file list - missing free IO handle function.",
			 function );

			goto on_error;
		}
		if( internal_source_file_list->clone_io_handle == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid source file list - missing clone IO handle function.",
			 function );

			goto on_error;
		}
		if( internal_source_file_list->clone_io_handle(
		     &destination_io_handle,
		     internal_source_file_list->io_handle,
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
	if( libmfdata_file_list_initialize(
	     destination_file_list,
	     destination_io_handle,
	     internal_source_file_list->free_io_handle,
	     internal_source_file_list->clone_io_handle,
	     internal_source_file_list->read_file_data,
	     LIBMFDATA_FLAG_IO_HANDLE_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination file list.",
		 function );

		goto on_error;
	}
	if( *destination_file_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing destination file list.",
		 function );

		goto on_error;
	}
	destination_io_handle = NULL;

	if( libmfdata_file_list_clone_files(
	     *destination_file_list,
	     source_file_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy files.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_file_list != NULL )
	{
		libmfdata_file_list_free(
		 destination_file_list,
		 NULL );
	}
	if( destination_io_handle != NULL )
	{
		internal_source_file_list->free_io_handle(
		&destination_io_handle,
		 NULL );
	}
	return( -1 );
}

/* Clones (duplicates) the files of the file list
 * Returns 1 if successful or -1 on error
 */
int libmfdata_file_list_clone_files(
     libmfdata_file_list_t *destination_file_list,
     libmfdata_file_list_t *source_file_list,
     libcerror_error_t **error )
{
	libmfdata_internal_file_t *internal_destination_file           = NULL;
	libmfdata_internal_file_t *internal_source_file                = NULL;
	libmfdata_internal_file_list_t *internal_destination_file_list = NULL;
	libmfdata_internal_file_list_t *internal_source_file_list      = NULL;
	static char *function                                          = "libmfdata_file_list_clone_files";
	int file_index                                                 = 0;
	int number_of_files                                            = 0;

	if( destination_file_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination file list.",
		 function );

		return( -1 );
	}
	internal_destination_file_list = (libmfdata_internal_file_list_t *) destination_file_list;

	if( source_file_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source file list.",
		 function );

		return( -1 );
	}
	internal_source_file_list = (libmfdata_internal_file_list_t *) source_file_list;

	if( libmfdata_array_get_number_of_entries(
	     internal_source_file_list->files,
	     &number_of_files,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of files from source files array.",
		 function );

		goto on_error;
	}
	if( libmfdata_array_empty(
	     internal_destination_file_list->files,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libmfdata_file_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty destination files array.",
		 function );

		goto on_error;
	}
	if( libmfdata_array_resize(
	     internal_destination_file_list->files,
	     number_of_files,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libmfdata_file_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize destination files array.",
		 function );

		goto on_error;
	}
	for( file_index = 0;
	     file_index < number_of_files;
	     file_index++ )
	{
		if( libmfdata_array_get_entry_by_index(
		     internal_source_file_list->files,
		     file_index,
		     (intptr_t **) &internal_source_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file: %d from source files array.",
			 function,
			 file_index );

			goto on_error;
		}
		if( libmfdata_file_initialize(
		     (libmfdata_file_t **) &internal_destination_file,
		     destination_file_list,
		     file_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination file.",
			 function );

			goto on_error;
		}
		internal_destination_file->file_io_pool_entry  = internal_source_file->file_io_pool_entry;

		if( libmfdata_array_set_entry_by_index(
		     internal_destination_file_list->files,
		     file_index,
		     (intptr_t *) internal_destination_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set file: %d in destination files array.",
			 function,
			 file_index );

			goto on_error;
		}
		internal_destination_file = NULL;
	}
	return( 1 );

on_error:
	if( internal_destination_file != NULL )
	{
		libmfdata_file_free(
		 (libmfdata_file_t **) &internal_destination_file,
		 NULL );
	}
	return( -1 );
}

/* Resizes the files
 * Returns 1 if successful or -1 on error
 */
int libmfdata_file_list_resize(
     libmfdata_file_list_t *file_list,
     int number_of_files,
     libcerror_error_t **error )
{
	libmfdata_internal_file_list_t *internal_file_list = NULL;
	static char *function                              = "libmfdata_file_list_resize";

	if( file_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file list.",
		 function );

		return( -1 );
	}
	internal_file_list = (libmfdata_internal_file_list_t *) file_list;

	if( libmfdata_array_resize(
	     internal_file_list->files,
	     number_of_files,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libmfdata_file_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize files array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of files of the list
 * Returns 1 if successful or -1 on error
 */
int libmfdata_file_list_get_number_of_files(
     libmfdata_file_list_t *file_list,
     int *number_of_files,
     libcerror_error_t **error )
{
	libmfdata_internal_file_list_t *internal_file_list = NULL;
	static char *function                              = "libmfdata_file_list_get_number_of_files";

	if( file_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file list.",
		 function );

		return( -1 );
	}
	internal_file_list = (libmfdata_internal_file_list_t *) file_list;

	if( libmfdata_array_get_number_of_entries(
	     internal_file_list->files,
	     number_of_files,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from files array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific file
 * Returns 1 if successful or -1 on error
 */
int libmfdata_file_list_get_file_item_by_index(
     libmfdata_file_list_t *file_list,
     int file_index,
     libmfdata_file_t **file,
     libcerror_error_t **error )
{
	libmfdata_internal_file_list_t *internal_file_list = NULL;
	static char *function                              = "libmfdata_file_list_get_file_item_by_index";

	if( file_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file list.",
		 function );

		return( -1 );
	}
	internal_file_list = (libmfdata_internal_file_list_t *) file_list;

	if( libmfdata_array_get_entry_by_index(
	     internal_file_list->files,
	     file_index,
	     (intptr_t **) file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file: %d from files array.",
		 function,
		 file_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the file IO pool entry of a specific file
 * Returns 1 if successful or -1 on error
 */
int libmfdata_file_list_get_file_by_index(
     libmfdata_file_list_t *file_list,
     int file_index,
     int *file_io_pool_entry,
     libcerror_error_t **error )
{
	libmfdata_file_t *file                             = NULL;
	libmfdata_internal_file_list_t *internal_file_list = NULL;
	static char *function                              = "libmfdata_file_list_get_file_by_index";

	if( file_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file list.",
		 function );

		return( -1 );
	}
	internal_file_list = (libmfdata_internal_file_list_t *) file_list;

	if( libmfdata_array_get_entry_by_index(
	     internal_file_list->files,
	     file_index,
	     (intptr_t **) &file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file: %d from files array.",
		 function,
		 file_index );

		return( -1 );
	}
	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing file.",
		 function );

		return( -1 );
	}
	if( libmfdata_file_get_data_range(
	     file,
	     file_io_pool_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data range of file: %d.",
		 function,
		 file_index );

		return( -1 );
	}
	return( 1 );
}

/* Sets the file IO pool entry of a specific file
 * Returns 1 if successful or -1 on error
 */
int libmfdata_file_list_set_file_by_index(
     libmfdata_file_list_t *file_list,
     int file_index,
     int file_io_pool_entry,
     libcerror_error_t **error )
{
	libmfdata_internal_file_list_t *internal_file_list = NULL;
	libmfdata_file_t *file                             = NULL;
	static char *function                              = "libmfdata_file_list_set_file_by_index";
	int previous_file_io_pool_entry                    = 0;

	if( file_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file list.",
		 function );

		return( -1 );
	}
	internal_file_list = (libmfdata_internal_file_list_t *) file_list;

	if( libmfdata_array_get_entry_by_index(
	     internal_file_list->files,
	     file_index,
	     (intptr_t **) &file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file: %d from files array.",
		 function,
		 file_index );

		return( -1 );
	}
	if( file != NULL )
	{
		if( libmfdata_file_get_data_range(
		     file,
		     &previous_file_io_pool_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data range of file: %d.",
			 function,
			 file_index );

			return( -1 );
		}
	}
	if( file == NULL )
	{
		if( libmfdata_file_initialize(
		     &file,
		     file_list,
		     file_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file.",
			 function );

			return( -1 );
		}
		if( libmfdata_array_set_entry_by_index(
		     internal_file_list->files,
		     file_index,
		     (intptr_t *) file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set file: %d in files array.",
			 function,
			 file_index );

			libmfdata_file_free(
			 &file,
			 NULL );

			return( -1 );
		}
	}
	if( libmfdata_file_set_data_range(
	     file,
	     file_io_pool_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set data range of file: %d.",
		 function,
		 file_index );

		return( -1 );
	}
	return( 1 );
}

/* Appends a file IO pool entry
 * Returns 1 if successful or -1 on error
 */
int libmfdata_file_list_append_file(
     libmfdata_file_list_t *file_list,
     int *file_index,
     int file_io_pool_entry,
     libcerror_error_t **error )
{
	libmfdata_internal_file_list_t *internal_file_list = NULL;
	libmfdata_file_t *file                             = NULL;
	static char *function                              = "libmfdata_file_list_append_file";

	if( file_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file list.",
		 function );

		return( -1 );
	}
	internal_file_list = (libmfdata_internal_file_list_t *) file_list;

	if( libmfdata_file_initialize(
	     &file,
	     file_list,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file.",
		 function );

		goto on_error;
	}
	if( libmfdata_file_set_data_range(
	     file,
	     file_io_pool_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set data range of file.",
		 function );

		goto on_error;
	}
	if( libmfdata_array_append_entry(
	     internal_file_list->files,
	     file_index,
	     (intptr_t *) file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append file to files array.",
		 function );

		goto on_error;
	}
	if( libmfdata_file_set_element_index(
	     file,
	     *file_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set list element index.",
		 function );

		file = NULL;

		goto on_error;
	}
	return( 1 );

on_error:
	if( file != NULL )
	{
		libmfdata_file_free(
		 &file,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the value of the file
 * Returns 1 if successful or -1 on error
 */
int libmfdata_file_list_get_file_value(
     libmfdata_file_list_t *file_list,
     libbfio_pool_t *file_io_pool,
     libmfcache_cache_t *cache,
     libmfdata_file_t *file,
     intptr_t **file_value,
     uint8_t read_flags,
     libcerror_error_t **error )
{
	libmfcache_cache_value_t *cache_value              = NULL;
	libmfdata_internal_file_list_t *internal_file_list = NULL;
	static char *function                              = "libmfdata_file_list_get_file_value";
        off64_t cache_value_offset                         = (off64_t) -1;
	time_t cache_value_timestamp                       = 0;
	time_t file_timestamp                              = 0;
	int cache_entry_index                              = -1;
	int cache_value_file_io_pool_entry                 = -1;
	int element_index                                  = -1;
	int file_io_pool_entry                             = 0;
	int number_of_cache_entries                        = 0;
	int result                                         = 0;

	if( file_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file list.",
		 function );

		return( -1 );
	}
	internal_file_list = (libmfdata_internal_file_list_t *) file_list;

	if( libmfdata_file_get_data_range(
	     file,
	     &file_io_pool_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data range from file.",
		 function );

		return( -1 );
	}
	if( libmfcache_cache_get_number_of_entries(
	     cache,
	     &number_of_cache_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of cache entries.",
		 function );

		return( -1 );
	}
	if( libmfdata_file_get_element_index(
	     file,
	     &element_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element index from file.",
		 function );

		return( -1 );
	}
	if( number_of_cache_entries <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of cache entries value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( read_flags & LIBMFDATA_READ_FLAG_IGNORE_CACHE ) == 0 )
	{
		cache_entry_index = libmfdata_file_list_calculate_cache_entry_index(
		                     element_index,
		                     number_of_cache_entries );

		if( libmfcache_cache_get_value_by_index(
		     cache,
		     cache_entry_index,
		     &cache_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve cache entry: %d from cache.",
			 function,
			 cache_entry_index );

			return( -1 );
		}
		if( cache_value != NULL )
		{
			if( libmfdata_file_get_timestamp(
			     file,
			     &file_timestamp,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve time stamp from file.",
				 function );

				return( -1 );
			}
			if( libmfcache_cache_value_get_identifier(
			     cache_value,
			     &cache_value_file_io_pool_entry,
			     &cache_value_offset,
			     &cache_value_timestamp,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve cache value identifier.",
				 function );

				return( -1 );
			}
		}
		if( ( file_io_pool_entry == cache_value_file_io_pool_entry )
		 && ( file_timestamp == cache_value_timestamp ) )
		{
			result = 1;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( result == 0 )
			{
				libcnotify_printf(
				 "%s: cache: 0x%08" PRIjx " miss (%d out of %d)\n",
				 function,
				 (intptr_t) cache,
				 cache_entry_index,
				 number_of_cache_entries );
			}
			else
			{
				libcnotify_printf(
				 "%s: cache: 0x%08" PRIjx " hit (%d out of %d)\n",
				 function,
				 (intptr_t) cache,
				 cache_entry_index,
				 number_of_cache_entries );
			}
		}
#endif
	}
	if( result == 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: reading file data from pool entry: %d\n",
			 function,
			 file_io_pool_entry );
		}
#endif
		/* Read the file data from the file IO handle
		 */
		if( internal_file_list->read_file_data(
		     internal_file_list->io_handle,
		     file_io_pool,
		     file_io_pool_entry,
		     file,
		     cache,
		     read_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read file data from pool entry: %d.",
			 function,
			 file_io_pool_entry );

			return( -1 );
		}
		cache_entry_index = libmfdata_file_list_calculate_cache_entry_index(
		                     element_index,
		                     number_of_cache_entries );

		if( libmfcache_cache_get_value_by_index(
		     cache,
		     cache_entry_index,
		     &cache_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve cache entry: %d from cache.",
			 function,
			 cache_entry_index );

			return( -1 );
		}
		if( cache_value != NULL )
		{
			if( libmfdata_file_get_timestamp(
			     file,
			     &file_timestamp,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve time stamp from file.",
				 function );

				return( -1 );
			}
			if( libmfcache_cache_value_get_identifier(
			     cache_value,
			     &cache_value_file_io_pool_entry,
			     &cache_value_offset,
			     &cache_value_timestamp,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve cache value identifier.",
				 function );

				return( -1 );
			}
		}
		if( ( file_io_pool_entry != cache_value_file_io_pool_entry )
		 || ( file_timestamp != cache_value_timestamp ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing cache value.",
			 function );

			return( -1 );
		}
	}
	if( libmfcache_cache_value_get_value(
	     cache_value,
	     file_value,
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

/* Retrieves the value of a specific file
 * Returns 1 if successful or -1 on error
 */
int libmfdata_file_list_get_file_value_by_index(
     libmfdata_file_list_t *file_list,
     libbfio_pool_t *file_io_pool,
     libmfcache_cache_t *cache,
     int file_index,
     intptr_t **file_value,
     uint8_t read_flags,
     libcerror_error_t **error )
{
	libmfdata_internal_file_list_t *internal_file_list = NULL;
	libmfdata_file_t *file                             = NULL;
	static char *function                              = "libmfdata_file_list_get_file_value_by_index";

	if( file_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file list.",
		 function );

		return( -1 );
	}
	internal_file_list = (libmfdata_internal_file_list_t *) file_list;

	if( libmfdata_array_get_entry_by_index(
	     internal_file_list->files,
	     file_index,
	     (intptr_t **) &file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file: %d from files array.",
		 function,
		 file_index );

		return( -1 );
	}
	if( libmfdata_file_list_get_file_value(
	     file_list,
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

/* Sets the value of a specific file
 *
 * If the flag LIBMFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED is set the list
 * takes over management of the value and the value is freed when
 * no longer needed.
 *
 * Returns 1 if successful or -1 on error
 */
int libmfdata_file_list_set_file_value(
     libmfdata_file_list_t *file_list,
     libmfcache_cache_t *cache,
     libmfdata_file_t *file,
     intptr_t *file_value,
     int (*free_file_value)(
            intptr_t **file_value,
            libcerror_error_t **error ),
     uint8_t flags,
     libcerror_error_t **error )
{
	static char *function       = "libmfdata_file_list_set_file_value";
	time_t file_timestamp       = 0;
	int cache_entry_index       = -1;
	int element_index           = 0;
	int file_io_pool_entry      = 0;
	int number_of_cache_entries = 0;

	if( file_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file list.",
		 function );

		return( -1 );
	}
	if( libmfdata_file_get_data_range(
	     file,
	     &file_io_pool_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data range from file",
		 function );

		return( -1 );
	}
	if( libmfdata_file_get_timestamp(
	     file,
	     &file_timestamp,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve time stamp from file.",
		 function );

		return( -1 );
	}
	if( libmfcache_cache_get_number_of_entries(
	     cache,
	     &number_of_cache_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of cache entries.",
		 function );

		return( -1 );
	}
	if( libmfdata_file_get_element_index(
	     file,
	     &element_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element index from file.",
		 function );

		return( -1 );
	}
	if( number_of_cache_entries <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of cache entries value out of bounds.",
		 function );

		return( -1 );
	}
	cache_entry_index = libmfdata_file_list_calculate_cache_entry_index(
	                     element_index,
	                     number_of_cache_entries );

	if( libmfcache_cache_set_value_by_index(
	     cache,
	     cache_entry_index,
	     file_io_pool_entry,
	     0,
	     file_timestamp,
	     file_value,
	     free_file_value,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set value in cache entry: %d.",
		 function,
		 cache_entry_index );

		return( -1 );
	}
	return( 1 );
}

/* Sets the value of a specific file
 *
 * If the flag LIBMFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED is set the list
 * takes over management of the value and the value is freed when
 * no longer needed.
 *
 * Returns 1 if successful or -1 on error
 */
int libmfdata_file_list_set_file_value_by_index(
     libmfdata_file_list_t *file_list,
     libmfcache_cache_t *cache,
     int file_index,
     intptr_t *file_value,
     int (*free_file_value)(
            intptr_t **file_value,
            libcerror_error_t **error ),
     uint8_t flags,
     libcerror_error_t **error )
{
	libmfdata_internal_file_list_t *internal_file_list = NULL;
	libmfdata_file_t *file                             = NULL;
	static char *function                              = "libmfdata_file_list_set_file_value_by_index";

	if( file_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file list.",
		 function );

		return( -1 );
	}
	internal_file_list = (libmfdata_internal_file_list_t *) file_list;

	if( libmfdata_array_get_entry_by_index(
	     internal_file_list->files,
	     file_index,
	     (intptr_t **) &file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file: %d from files array.",
		 function,
		 file_index );

		return( -1 );
	}
	if( libmfdata_file_list_set_file_value(
	     file_list,
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
		 "%s: unable to set file value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

