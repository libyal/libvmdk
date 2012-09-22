/*
 * Handle functions
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
#include <memory.h>
#include <types.h>

#include "libvmdk_debug.h"
#include "libvmdk_definitions.h"
#include "libvmdk_descriptor_file.h"
#include "libvmdk_extent_descriptor.h"
#include "libvmdk_extent_file.h"
#include "libvmdk_handle.h"
#include "libvmdk_io_handle.h"
#include "libvmdk_libbfio.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libcnotify.h"
#include "libvmdk_libcstring.h"
#include "libvmdk_libmfdata.h"
#include "libvmdk_offset_table.h"

/* Initialize a handle
 * Make sure the value handle is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_initialize(
     libvmdk_handle_t **handle,
     libcerror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_initialize";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( *handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle value already set.",
		 function );

		return( -1 );
	}
	internal_handle = memory_allocate_structure(
	                   libvmdk_internal_handle_t );

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_handle,
	     0,
	     sizeof( libvmdk_internal_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear handle.",
		 function );

		memory_free(
		 internal_handle );

		return( -1 );
	}
	if( libvmdk_offset_table_initialize(
	     &( internal_handle->offset_table ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create offset table.",
		 function );

		goto on_error;
	}
	if( libvmdk_io_handle_initialize(
	     &( internal_handle->io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create io handle.",
		 function );

		goto on_error;
	}
	internal_handle->maximum_number_of_open_handles = LIBBFIO_POOL_UNLIMITED_NUMBER_OF_OPEN_HANDLES;

	*handle = (libvmdk_handle_t *) internal_handle;

	return( 1 );

on_error:
	if( internal_handle != NULL )
	{
		if( internal_handle->offset_table != NULL )
		{
			libvmdk_offset_table_free(
			 &( internal_handle->offset_table ),
			 NULL );
		}
		memory_free(
		 internal_handle );
	}
	return( -1 );
}

/* Frees an exisisting handle
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_free(
     libvmdk_handle_t **handle,
     libcerror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_free";
	int result                                 = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( *handle != NULL )
	{
		internal_handle = (libvmdk_internal_handle_t *) *handle;
		*handle         = NULL;

		if( internal_handle->io_handle != NULL )
		{
			if( libvmdk_io_handle_free(
			     &( internal_handle->io_handle ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free io handle.",
				 function );

				result = -1;
			}
		}
		if( internal_handle->file_io_pool_created_in_library != 0 )
		{
			if( internal_handle->file_io_pool != NULL )
			{
				if( libbfio_pool_free(
				     &( internal_handle->file_io_pool ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free file IO pool.",
					 function );

					result = -1;
				}
			}
		}
		if( libvmdk_offset_table_free(
		     &( internal_handle->offset_table ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free offset table.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_handle );
	}
	return( result );
}

/* Signals the libvmdk handle to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_signal_abort(
     libvmdk_handle_t *handle,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_handle_signal_abort";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	( (libvmdk_internal_handle_t *) handle )->abort = 1;

	return( 1 );
}

/* Opens a set of Mware Virtual Disk file(s)
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_open(
     libvmdk_handle_t *handle,
     char * const filenames[],
     int number_of_filenames,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle           = NULL;
	libbfio_pool_t *file_io_pool               = NULL;
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_open";
	size_t filename_length                     = 0;
	int file_io_pool_entry                     = 0;
	int filename_index                         = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libvmdk_internal_handle_t *) handle;

	if( filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid number of files zero or less.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBVMDK_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_initialize(
	     &file_io_pool,
	     0,
	     internal_handle->maximum_number_of_open_handles,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO pool.",
		 function );

		goto on_error;
	}
	if( ( access_flags & LIBVMDK_ACCESS_FLAG_READ ) != 0 )
	{
		for( filename_index = 0;
		     filename_index < number_of_filenames;
		     filename_index++ )
		{
			filename_length = libcstring_narrow_string_length(
					   filenames[ filename_index ] );

			/* Make sure there is more to the filename than the extension
			 */
			if( filename_length <= 4 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: filename: %s is too small.",
				 function,
				 filenames[ filename_index ] );

				goto on_error;
			}
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

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libbfio_handle_set_track_offsets_read(
			     file_io_handle,
			     1,
			     error ) != 1 )
			{
		                libcerror_error_set(
		                 error,
		                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		                 "%s: unable to set track offsets read in file IO handle.",
		                 function );

				goto on_error;
			}
#endif
			if( libbfio_file_set_name(
			     file_io_handle,
			     filenames[ filename_index ],
			     filename_length + 1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set name in file IO handle.",
				 function );

				goto on_error;
			}
			if( libbfio_pool_append_handle(
			     file_io_pool,
			     &file_io_pool_entry,
			     file_io_handle,
			     LIBBFIO_OPEN_READ,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to add file IO handle to pool.",
				 function );

				goto on_error;
			}
			file_io_handle = NULL;

#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: added pool entry: %d with filename: %s.\n",
				 function,
				 file_io_pool_entry,
				 filenames[ filename_index ] );
			}
#endif
		}
	}
	if( libvmdk_handle_open_file_io_pool(
	     handle,
	     file_io_pool,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle using a file IO pool.",
		 function );

		goto on_error;
	}
	internal_handle->file_io_pool_created_in_library = 1;

	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &file_io_pool,
		 NULL );
	}
	return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a set of VMware Virtual Disk file(s)
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_open_wide(
     libvmdk_handle_t *file,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle           = NULL;
	libbfio_pool_t *file_io_pool               = NULL;
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_open_wide";
	size_t filename_length                     = 0;
	int file_io_pool_entry                     = 0;
	int filename_index                         = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libvmdk_internal_handle_t *) handle;

	if( filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid number of files zero or less.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBVMDK_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_initialize(
	     &file_io_pool,
	     0,
	     internal_handle->maximum_number_of_open_handles,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO pool.",
		 function );

		goto on_error;
	}
	if( ( access_flags & LIBVMDK_ACCESS_FLAG_READ ) != 0 )
	{
		for( filename_index = 0;
		     filename_index < number_of_filenames;
		     filename_index++ )
		{
			filename_length = libcstring_wide_string_length(
					   filenames[ filename_index ] );

			/* Make sure there is more to the filename than the extension
			 */
			if( filename_length <= 4 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: filename: %ls is too small.",
				 function,
				 filenames[ filename_index ] );

				goto on_error;
			}
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

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libbfio_handle_set_track_offsets_read(
			     file_io_handle,
			     1,
			     error ) != 1 )
			{
		                libcerror_error_set(
		                 error,
		                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		                 "%s: unable to set track offsets read in file IO handle.",
		                 function );

				goto on_error;
			}
#endif
			if( libbfio_file_set_name_wide(
			     file_io_handle,
			     filenames[ filename_index ],
			     filename_length + 1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set name in file IO handle.",
				 function );

				goto on_error;
			}
			if( libbfio_pool_append_handle(
			     file_io_pool,
			     &file_io_pool_entry,
			     file_io_handle,
			     LIBBFIO_OPEN_READ,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to add file IO handle to pool.",
				 function );

				goto on_error;
			}
			file_io_handle = NULL;

#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: added pool entry: %d with filename: %ls.\n",
				 function,
				 file_io_pool_entry,
				 filenames[ filename_index ] );
			}
#endif
		}
	}
	if( libvmdk_handle_open_file_io_pool(
	     handle,
	     file_io_pool,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle using a file IO pool.",
		 function );

		goto on_error;
	}
	internal_handle->file_io_pool_created_in_library = 1;

	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &file_io_pool,
		 NULL );
	}
	return( -1 );
}

#endif

/* Opens a set of VMware Virtual Disk file(s) using a Basic File IO (bfio) pool
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_open_file_io_pool(
     libvmdk_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle           = NULL;
	libvmdk_internal_handle_t *internal_handle = NULL;
	libvmdk_extent_file_t *extent_file         = NULL;
	uint8_t *descriptor_data                   = NULL;
	static char *function                      = "libvmdk_handle_open_file_io_pool";
	ssize_t read_count                         = 0;
	uint8_t file_type                          = 0;
	int file_io_pool_entry                     = 0;
	int number_of_file_io_handles              = 0;
	int result                                 = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libvmdk_internal_handle_t *) handle;

	if( internal_handle->descriptor_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - descriptor file already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing io handle.",
		 function );

		return( -1 );
	}
	if( file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO pool.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_get_number_of_handles(
	     file_io_pool,
	     &number_of_file_io_handles,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of handles in the file IO pool.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBVMDK_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBVMDK_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		goto on_error;
	}
	if( ( access_flags & LIBVMDK_ACCESS_FLAG_READ ) != 0 )
	{
		for( file_io_pool_entry = 0;
		     file_io_pool_entry < number_of_file_io_handles;
		     file_io_pool_entry++ )
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
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to retrieve file IO handle from pool entry: %d.",
				 function,
				 file_io_pool_entry );

				goto on_error;
			}
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: processing pool entry: %d.\n",
				 function,
				 file_io_pool_entry );
			}
#endif
			result = libvmdk_handle_open_read_signature(
			          file_io_pool,
			          file_io_pool_entry,
			          &file_type,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read signature of pool entry: %d.",
				 function,
				 file_io_pool_entry );

				goto on_error;
			}
			else if( result == 0 )
			{
				file_type = LIBVMDK_FILE_TYPE_RAW_DATA;
			}
			switch( file_type )
			{
				case LIBVMDK_FILE_TYPE_DESCRIPTOR_FILE:
					if( internal_handle->descriptor_file != NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
						 "%s: invalid handle - descriptor file already set.",
						 function );

						goto on_error;
					}
					if( libvmdk_descriptor_file_initialize(
					     &( internal_handle->descriptor_file ),
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
						 "%s: unable to create descriptor file.",
						 function );

						goto on_error;
					}
					if( libvmdk_descriptor_file_read(
					     internal_handle->descriptor_file,
					     file_io_pool,
					     file_io_pool_entry,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read descriptor file from file IO pool entry: %d.",
						 function,
						 file_io_pool_entry );

						goto on_error;
					}
					break;

				case LIBVMDK_FILE_TYPE_RAW_DATA:
					break;

				case LIBVMDK_FILE_TYPE_COWD_SPARSE_DATA:
					break;

				case LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA:
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
					if( libvmdk_extent_file_read_file_header(
					     extent_file,
					     file_io_pool,
					     file_io_pool_entry,
					     error ) == -1 )
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
					if( extent_file->descriptor_size > 0 )
					{
						if( internal_handle->descriptor_file != NULL )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
							 "%s: invalid handle - descriptor file already set.",
							 function );

							goto on_error;
						}
						if( libvmdk_descriptor_file_initialize(
						     &( internal_handle->descriptor_file ),
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
							 "%s: unable to create descriptor file.",
							 function );

							goto on_error;
						}
						if( extent_file->descriptor_size > (size64_t) SSIZE_MAX )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
							 "%s: invalid extent file - descriptor size value exceeds maximum.",
							 function );

							goto on_error;
						}
						descriptor_data = (uint8_t *) memory_allocate(
						                               sizeof( uint8_t ) * (size_t) extent_file->descriptor_size );

						if( descriptor_data == NULL )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_MEMORY,
							 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
							 "%s: unable to create descriptor data.",
							 function );

							goto on_error;
						}
						read_count = libvmdk_extent_file_read_descriptor_data(
							      extent_file,
							      file_io_pool,
							      file_io_pool_entry,
							      descriptor_data,
							      (size_t) extent_file->descriptor_size,
							      error );

						if( read_count != (ssize_t) extent_file->descriptor_size )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_IO,
							 LIBCERROR_IO_ERROR_READ_FAILED,
							 "%s: unable to read descriptor data from file IO pool entry: %d.",
							 function,
							 file_io_pool_entry );

							goto on_error;
						}
						if( libvmdk_descriptor_file_read_string(
						     internal_handle->descriptor_file,
						     descriptor_data,
						     (size_t) extent_file->descriptor_size,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_IO,
							 LIBCERROR_IO_ERROR_READ_FAILED,
							 "%s: unable to read descriptor from string.",
							 function );

							goto on_error;
						}
						memory_free(
						 descriptor_data );

						descriptor_data = NULL;
					}
					if( libvmdk_extent_file_free(
					     &extent_file,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
						 "%s: unable to free extent file.",
						 function );

						goto on_error;
					}
					break;

				default:
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported file type.",
					 function );

					goto on_error;
			}
		}
		result = libvmdk_handle_open_read_grain_table(
		          internal_handle,
		          file_io_pool,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read files.",
			 function );

			goto on_error;
		}
	}
	if( internal_handle->descriptor_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing descriptor file.",
		 function );

		goto on_error;
	}
	internal_handle->io_handle->media_size   = internal_handle->descriptor_file->media_size;
	internal_handle->io_handle->access_flags = access_flags;
	internal_handle->file_io_pool            = file_io_pool;

	return( 1 );

on_error:
	if( descriptor_data != NULL )
	{
		memory_free(
		 descriptor_data );
	}
	if( extent_file != NULL )
	{
		libvmdk_extent_file_free(
		 &extent_file,
		 NULL );
	}
	if( internal_handle->descriptor_file != NULL )
	{
		libvmdk_descriptor_file_free(
		 &( internal_handle->descriptor_file ),
		 NULL );
	}
	return( -1 );
}

/* Closes a VMware Virtual Disk file(s)
 * Returns 0 if successful or -1 on error
 */
int libvmdk_handle_close(
     libvmdk_handle_t *handle,
     libcerror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_close";
	int result                                 = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libvmdk_internal_handle_t *) handle;

	if( internal_handle->file_io_pool_created_in_library != 0 )
	{
		if( libbfio_pool_close_all(
		     internal_handle->file_io_pool,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close all files.",
			 function );

			return( -1 );
		}
	}
	if( internal_handle->descriptor_file != NULL )
	{
		if( libvmdk_descriptor_file_free(
		     &( internal_handle->descriptor_file ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free descriptor file.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->extent_files_list != NULL )
	{
		if( libmfdata_file_list_free(
		     &( internal_handle->extent_files_list ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free extent files list.",
			 function );

			result = -1;
		}
	}
	return( result );
}

/* Reads the grain table
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_open_read_grain_table(
     libvmdk_internal_handle_t *internal_handle,
     libbfio_pool_t *file_io_pool,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle               = NULL;
	libvmdk_extent_descriptor_t *extent_descriptor = NULL;
	libvmdk_extent_file_t *extent_file             = NULL;
	static char *function                          = "libvmdk_handle_open_read_grain_table";
	int extent_index                               = 0;
	int file_io_pool_entry                         = 0;
	int number_of_extents                          = 0;
	int number_of_file_io_handles                  = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->descriptor_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal handle - missing descriptor file.",
		 function );

		return( -1 );
	}
	if( internal_handle->extent_files_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - extent file list value already set.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_get_number_of_handles(
	     file_io_pool,
	     &number_of_file_io_handles,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of handles in the file IO pool.",
		 function );

		return( -1 );
	}
	if( libvmdk_descriptor_file_get_number_of_extents(
	     internal_handle->descriptor_file,
	     &number_of_extents,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of extents.",
		 function );

		goto on_error;
	}
	if( number_of_extents == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing extents.",
		 function );

		goto on_error;
	}
	if( ( number_of_extents + 1 ) != number_of_file_io_handles )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid number of file IO handles.",
		 function );

		goto on_error;
	}
	if( libmfdata_file_list_initialize(
	     &( internal_handle->extent_files_list ),
	     NULL,
	     NULL,
	     NULL,
	     &libvmdk_extent_file_read,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create extent files list.",
		 function );

		goto on_error;
	}
	if( libmfdata_file_list_resize(
	     internal_handle->extent_files_list,
	     number_of_extents,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize extent files list.",
		 function );

		goto on_error;
	}
	for( extent_index = 0;
	     extent_index < number_of_extents;
	     extent_index++ )
	{
		if( libvmdk_descriptor_file_get_extent(
		     internal_handle->descriptor_file,
		     extent_index,
		     &extent_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve extent: %d from descriptor file.",
			 function,
			 extent_index );

			goto on_error;
		}
		if( extent_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing extent descriptor: %d.",
			 function,
			 extent_index );

			goto on_error;
		}
		for( file_io_pool_entry = 0;
		     file_io_pool_entry < number_of_file_io_handles;
		     file_io_pool_entry++ )
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
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to retrieve file IO handle from pool entry: %d.",
				 function,
				 file_io_pool_entry );

				goto on_error;
			}
/* TODO find the correct extent file */
		}
		if( internal_handle->abort == 1 )
		{
			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( internal_handle->extent_files_list != NULL )
	{
		libmfdata_file_list_free(
		 &( internal_handle->extent_files_list ),
		 NULL );
	}
	return( -1 );
}

#ifdef TODO

/* Opens a VMware Virtual Disk file(s) for reading
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_open_read(
     libvmdk_internal_handle_t *internal_handle,
     libcerror_error_t **error )
{
	static char *function                      = "libvmdk_handle_open_read";
	off64_t descriptor_offset                  = 0;
	off64_t grain_directory_offset             = 0;
	off64_t secondary_grain_directory_offset   = 0;
	size64_t descriptor_size                   = 0;
	size64_t grain_size                        = 0;
	uint32_t number_of_grain_directory_entries = 0;
	uint32_t number_of_grain_table_entries     = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal file.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal file - missing io handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading file header:\n" );
	}
#endif
	if( libvmdk_io_handle_read_file_header(
	     internal_handle->io_handle,
	     internal_handle->file_io_handle,
	     &descriptor_offset,
	     &descriptor_size,
	     &grain_directory_offset,
	     &secondary_grain_directory_offset,
	     &number_of_grain_directory_entries,
	     &number_of_grain_table_entries,
	     &grain_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header.",
		 function );

		return( -1 );
	}
	if( descriptor_offset > 0 )
	{
		if( descriptor_size == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: invalid descriptor size value is out of range.",
			 function );

			return( -1 );
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "Reading descriptor:\n" );
		}
#endif

		/* TODO read descriptor */
	}
	if( ( grain_directory_offset == 0 )
	 && ( secondary_grain_directory_offset == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid primary and secondary grain directory offset value out of range.",
		 function );

		return( -1 );
	}
	if( grain_directory_offset > 0 )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "Reading (primary) grain directory:\n" );
		}
#endif

		if( libvmdk_io_handle_read_grain_directory(
		     internal_handle->io_handle,
		     internal_handle->file_io_handle,
		     internal_handle->offset_table,
		     grain_directory_offset,
		     number_of_grain_directory_entries,
		     number_of_grain_table_entries,
		     grain_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read (primary) grain directory.",
			 function );

			return( -1 );
		}
	}
	if( secondary_grain_directory_offset > 0 )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "Reading secondary grain directory:\n" );
		}
#endif

		if( libvmdk_io_handle_read_grain_directory(
		     internal_handle->io_handle,
		     internal_handle->file_io_handle,
		     internal_handle->offset_table,
		     secondary_grain_directory_offset,
		     number_of_grain_directory_entries,
		     number_of_grain_table_entries,
		     grain_size,
		     1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read secondary grain directory.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

#endif

/* Reads the file signature and tries to determine the file type
 * Returns 1 if successful, 0 if no file type could be determined or -1 on error
 */
int libvmdk_handle_open_read_signature(
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     uint8_t *file_type,
     libcerror_error_t **error )
{
	uint8_t signature[ 32 ];

	static char *function = "libvmdk_handle_open_read_signature";
	ssize_t read_count    = 0;
	int result            = 0;

	if( file_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file type.",
		 function );

		return( -1 );
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

		return( -1 );
	}
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              signature,
	              32,
	              error );

	if( read_count != (ssize_t) 32 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read signature from file IO pool entry: %d.",
		 function,
		 file_io_pool_entry );

		return( -1 );
	}
	if( memory_compare(
	     signature,
	     cowd_sparse_file_signature,
	     4 ) == 0 )
	{
		*file_type = LIBVMDK_FILE_TYPE_COWD_SPARSE_DATA;
		result     = 1;
	}
	else if( memory_compare(
	          signature,
	          vmdk_sparse_file_signature,
	          4 ) == 0 )
	{
		*file_type = LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA;
		result     = 1;
	}
	else if( memory_compare(
	          signature,
	          vmdk_descriptor_file_signature,
	          21 ) == 0 )
	{
		if( signature[ 21 ] == (uint8_t) '\n' )
		{
			*file_type = LIBVMDK_FILE_TYPE_DESCRIPTOR_FILE;
			result     = 1;
		}
	}
	return( result );
}

/* Reads (media) data from the last current into a buffer
 * Returns the number of bytes read or -1 on error
 */
ssize_t libvmdk_handle_read_buffer(
         libvmdk_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_read_buffer";
	size_t buffer_offset                       = 0;
	size_t read_size                           = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libvmdk_internal_handle_t *) handle;

	if( internal_handle->descriptor_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal handle - missing descriptor file.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid internal handle - invalid IO handle - current offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( internal_handle->descriptor_file->disk_type != LIBVMDK_DISK_TYPE_2GB_EXTENT_FLAT )
	 && ( internal_handle->descriptor_file->disk_type != LIBVMDK_DISK_TYPE_2GB_EXTENT_SPARSE )
	 && ( internal_handle->descriptor_file->disk_type != LIBVMDK_DISK_TYPE_MONOLITHIC_SPARSE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported disk type.",
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
		 "%s: invalid element data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( (size64_t) internal_handle->io_handle->current_offset >= internal_handle->io_handle->media_size )
	{
		return( 0 );
	}
	while( buffer_offset < buffer_size )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: requested offset\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 internal_handle->io_handle->current_offset );
		}
#endif
/* TODO */
		internal_handle->io_handle->current_offset += read_size;

		buffer_offset += read_size;

		if( (size64_t) internal_handle->io_handle->current_offset >= internal_handle->io_handle->media_size )
		{
			break;
		}
	}
	return( (ssize_t) buffer_offset );
}

/* Reads (media) data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libvmdk_handle_read_random(
         libvmdk_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	static char *function = "libvmdk_handle_read_random";
	ssize_t read_count    = 0;

	if( libvmdk_handle_seek_offset(
	     handle,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	read_count = libvmdk_handle_read_buffer(
	              handle,
	              buffer,
	              buffer_size,
	              error );

	if( read_count <= -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		return( -1 );
	}
	return( read_count );
}

#ifdef TODO
/* Writes (media) data at the current offset
 * the necessary settings of the write values must have been made
 * Will initialize write if necessary
 * Returns the number of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libvmdk_handle_write_buffer(
         libvmdk_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_write_buffer";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libvmdk_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal handle - missing IO handle.",
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
		 "%s: invalid element data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* TODO */

	return( -1 );
}

/* Writes (media) data in EWF format at a specific offset,
 * the necessary settings of the write values must have been made
 * Will initialize write if necessary
 * Returns the number of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libvmdk_handle_write_random(
         libvmdk_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	static char *function = "libvmdk_handle_write_random";
	ssize_t write_count   = 0;

	if( libvmdk_handle_seek_offset(
	     handle,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	write_count = libvmdk_handle_write_buffer(
	               handle,
	               buffer,
	               buffer_size,
	               error );

	if( write_count <= -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write buffer.",
		 function );

		return( -1 );
	}
	return( write_count );
}

#endif

/* Seeks a certain offset of the (media) data
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libvmdk_handle_seek_offset(
         libvmdk_handle_t *handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_seek_offset";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libvmdk_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal handle - missing IO handle.",
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
		offset += internal_handle->io_handle->current_offset;
	}
	else if( whence == SEEK_END )
	{
		offset += (off64_t) internal_handle->io_handle->media_size;
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
	internal_handle->io_handle->current_offset = offset;

	return( offset );
}

/* Retrieves the current offset of the (media) data
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_get_offset(
     libvmdk_handle_t *handle,
     off64_t *offset,
     libcerror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_get_offset";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libvmdk_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal handle - missing IO handle.",
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
	*offset = internal_handle->io_handle->current_offset;

	return( 1 );
}

