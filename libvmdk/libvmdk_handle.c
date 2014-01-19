/*
 * Handle functions
 *
 * Copyright (c) 2009-2014, Joachim Metz <joachim.metz@gmail.com>
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
#include "libvmdk_extent_table.h"
#include "libvmdk_handle.h"
#include "libvmdk_grain_data.h"
#include "libvmdk_grain_table.h"
#include "libvmdk_io_handle.h"
#include "libvmdk_libbfio.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libclocale.h"
#include "libvmdk_libcnotify.h"
#include "libvmdk_libcpath.h"
#include "libvmdk_libcstring.h"
#include "libvmdk_libfcache.h"
#include "libvmdk_libfdata.h"
#include "libvmdk_libuna.h"

/* Creates a handle
 * Make sure the value handle is referencing, is set to NULL
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
	if( libvmdk_io_handle_initialize(
	     &( internal_handle->io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( libvmdk_extent_table_initialize(
	     &( internal_handle->extent_table ),
	     internal_handle->io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create extent table.",
		 function );

		goto on_error;
	}
	internal_handle->maximum_number_of_open_handles = LIBBFIO_POOL_UNLIMITED_NUMBER_OF_OPEN_HANDLES;

	*handle = (libvmdk_handle_t *) internal_handle;

	return( 1 );

on_error:
	if( internal_handle != NULL )
	{
		if( internal_handle->io_handle != NULL )
		{
			libvmdk_io_handle_free(
			 &( internal_handle->io_handle ),
			 NULL );
		}
		memory_free(
		 internal_handle );
	}
	return( -1 );
}

/* Frees a handle
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

		if( internal_handle->extent_data_file_io_pool != NULL )
		{
			if( libvmdk_handle_close(
			     *handle,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close handle.",
				 function );

				result = -1;
			}
		}
		*handle = NULL;

		if( libvmdk_io_handle_free(
		     &( internal_handle->io_handle ),
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
		memory_free(
		 internal_handle );
	}
	return( result );
}

/* Signals the handle to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_signal_abort(
     libvmdk_handle_t *handle,
     libcerror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_signal_abort";

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
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	internal_handle->io_handle->abort = 1;

	return( 1 );
}

/* Opens a set of VMDK file(s)
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_open(
     libvmdk_handle_t *handle,
     const char *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle           = NULL;
	libvmdk_internal_handle_t *internal_handle = NULL;
	char *basename_end                         = NULL;
	static char *function                      = "libvmdk_handle_open";
	size_t basename_length                     = 0;
	size_t filename_length                     = 0;

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
	if( ( ( access_flags & LIBVMDK_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBVMDK_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
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
	filename_length = libcstring_narrow_string_length(
	                   filename );

	basename_end = libcstring_narrow_string_search_character_reverse(
	                filename,
	                (int) LIBCPATH_SEPARATOR,
	                filename_length + 1 );

	if( basename_end != NULL )
	{
		basename_length = (size_t) ( basename_end - filename ) + 1;
	}
	if( basename_length > 0 )
	{
		if( libvmdk_extent_table_set_basename(
		     internal_handle->extent_table,
		     filename,
		     basename_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set basename in extent table.",
			 function );

			goto on_error;
		}
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
	     filename,
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
	if( libvmdk_handle_open_file_io_handle(
	     handle,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle using a file IO handle.",
		 function );

		goto on_error;
	}
	if( libbfio_handle_close(
	     file_io_handle,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close file IO handle.",
		 function );

		goto on_error;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file IO handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a set of VMDK file(s)
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_open_wide(
     libvmdk_handle_t *handle,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle           = NULL;
	libvmdk_internal_handle_t *internal_handle = NULL;
	wchar_t *basename_end                      = NULL;
	static char *function                      = "libvmdk_handle_open_wide";
	size_t basename_length                     = 0;
	size_t filename_length                     = 0;

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
	if( ( ( access_flags & LIBVMDK_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBVMDK_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
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
	filename_length = libcstring_wide_string_length(
	                   filename );

/* TODO does this work for UTF-16 ? */
	basename_end = libcstring_wide_string_search_character_reverse(
	                filename,
	                (wint_t) LIBCPATH_SEPARATOR,
	                filename_length + 1 );

	if( basename_end != NULL )
	{
		basename_length = (size_t) ( basename_end - filename ) + 1;
	}
	if( basename_length > 0 )
	{
		if( libvmdk_extent_table_set_basename_wide(
		     internal_handle->extent_table,
		     filename,
		     basename_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set basename in extent table.",
			 function );

			goto on_error;
		}
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
	     filename,
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
	if( libvmdk_handle_open_file_io_handle(
	     handle,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle using a file IO handle.",
		 function );

		goto on_error;
	}
	if( libbfio_handle_close(
	     file_io_handle,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close file IO handle.",
		 function );

		goto on_error;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file IO handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#endif

/* Opens a set of VMDK file(s) using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_open_file_io_handle(
     libvmdk_handle_t *handle,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	libvmdk_extent_file_t *extent_file         = NULL;
	uint8_t *descriptor_data                   = NULL;
	static char *function                      = "libvmdk_handle_open_file_io_handle";
	int bfio_access_flags                      = 0;
	uint8_t file_type                          = 0;
	int file_io_handle_is_open                 = 0;
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
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
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
	if( ( access_flags & LIBVMDK_ACCESS_FLAG_READ ) != 0 )
	{
		bfio_access_flags = LIBBFIO_ACCESS_FLAG_READ;
	}
	file_io_handle_is_open = libbfio_handle_is_open(
	                          file_io_handle,
	                          error );

	if( file_io_handle_is_open == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		return( -1 );
	}
	else if( file_io_handle_is_open == 0 )
	{
		if( libbfio_handle_open(
		     file_io_handle,
		     bfio_access_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open file IO handle.",
			 function );

			return( -1 );
		}
	}
	result = libvmdk_handle_open_read_signature(
		  file_io_handle,
		  &file_type,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read signature.",
		 function );

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
			     file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read descriptor file.",
				 function );

				goto on_error;
			}
			break;

		case LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA:
			if( libvmdk_extent_file_initialize(
			     &extent_file,
			     internal_handle->io_handle,
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
			if( libvmdk_extent_file_read_file_header_file_io_handle(
			     extent_file,
			     file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read extent file header.",
				 function );

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
				if( libvmdk_extent_file_read_descriptor_data_file_io_handle(
				     extent_file,
				     file_io_handle,
				     descriptor_data,
				     (size_t) extent_file->descriptor_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read descriptor data.",
					 function );

					goto on_error;
				}
				if( libvmdk_descriptor_file_read_string(
				     internal_handle->descriptor_file,
				     (char *) descriptor_data,
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

		case LIBVMDK_FILE_TYPE_RAW_DATA:
		case LIBVMDK_FILE_TYPE_COWD_SPARSE_DATA:
		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported file type.",
			 function );

			goto on_error;
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

	internal_handle->access_flags = access_flags;

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

/* Opens the extent data files
 * If the extent data filenames were not set explicitly this function assumes the extent data files
 * are in the same location as the descriptor file
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_open_extent_data_files(
     libvmdk_handle_t *handle,
     libcerror_error_t **error )
{
	libcstring_system_character_t *extent_data_file_location  = NULL;
	libcstring_system_character_t *extent_data_filename_start = NULL;
	libvmdk_extent_descriptor_t *extent_descriptor            = NULL;
	libvmdk_internal_handle_t *internal_handle                = NULL;
	static char *function                                     = "libvmdk_handle_open_extent_data_files";
	size_t extent_data_file_location_size                     = 0;
	size_t extent_data_filename_size                          = 0;
	int extent_index                                          = 0;
	int number_of_extents                                     = 0;
	int result                                                = 0;

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
	if( internal_handle->extent_data_file_io_pool != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - extent data file IO pool already exists.",
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
	for( extent_index = 0;
	     extent_index < number_of_extents;
	     extent_index++ )
	{
		if( libvmdk_descriptor_file_get_extent_by_index(
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
		if( extent_descriptor->type != LIBVMDK_EXTENT_TYPE_ZERO )
		{
			if( ( extent_descriptor->filename == NULL )
			 || ( extent_descriptor->filename_size == 0 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid extent descriptor: %d - missing filename.",
				 function,
				 extent_index );

				goto on_error;
			}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			extent_data_filename_start = libcstring_wide_string_search_character_reverse(
			                              extent_descriptor->filename,
			                              (wint_t) LIBCPATH_SEPARATOR,
			                              extent_descriptor->filename_size );
#else
			extent_data_filename_start = libcstring_narrow_string_search_character_reverse(
			                              extent_descriptor->filename,
			                              (int) LIBCPATH_SEPARATOR,
			                              extent_descriptor->filename_size );
#endif
			if( extent_data_filename_start != NULL )
			{
				/* Ignore the path separator itself
				 */
				extent_data_filename_start++;

/* TODO does this work for UTF-16 ? */
				extent_data_filename_size = (size_t) ( extent_data_filename_start - extent_descriptor->filename );
			}
			else
			{
				extent_data_filename_start = extent_descriptor->filename;
				extent_data_filename_size  = extent_descriptor->filename_size;
			}
/* TODO refactor to a function in extent table */
			if( internal_handle->extent_table->basename != NULL )
			{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
				if( libcpath_path_join_wide(
				     &extent_data_file_location,
				     &extent_data_file_location_size,
				     internal_handle->extent_table->basename,
				     internal_handle->extent_table->basename_size - 1,
				     extent_data_filename_start,
				     extent_data_filename_size - 1,
				     error ) != 1 )
#else
				if( libcpath_path_join(
				     &extent_data_file_location,
				     &extent_data_file_location_size,
				     internal_handle->extent_table->basename,
				     internal_handle->extent_table->basename_size - 1,
				     extent_data_filename_start,
				     extent_data_filename_size - 1,
				     error ) != 1 )
#endif
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create extent data file location.",
					 function );

					goto on_error;
				}
			}
			else
			{
				extent_data_file_location      = extent_data_filename_start;
				extent_data_file_location_size = extent_data_filename_size;
			}
			/* Note that the open extent data file function will initialize extent_data_file_io_pool
			 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libvmdk_handle_open_extent_data_file_wide(
				  internal_handle,
				  extent_index,
				  extent_data_file_location,
				  error );
#else
			result = libvmdk_handle_open_extent_data_file(
				  internal_handle,
				  extent_index,
				  extent_data_file_location,
				  error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to open extent data file: %" PRIs_LIBCSTRING_SYSTEM ".",
				 function,
				 extent_data_file_location );

				goto on_error;
			}
			if( ( extent_data_file_location != NULL )
			 && ( extent_data_file_location != extent_data_filename_start ) )
			{
				memory_free(
				 extent_data_file_location );
			}
			extent_data_filename_start = NULL;
			extent_data_file_location  = NULL;
		}
	}
	if( libvmdk_handle_open_read_grain_table(
	     internal_handle,
	     internal_handle->extent_data_file_io_pool,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to read grain table.",
                 function );

                goto on_error;
	}
	return( 1 );

on_error:
	if( internal_handle->extent_data_file_io_pool != NULL )
	{
		libbfio_pool_close_all(
		 internal_handle->extent_data_file_io_pool,
		 NULL );
		libbfio_pool_free(
		 &( internal_handle->extent_data_file_io_pool ),
		 NULL );

		internal_handle->extent_data_file_io_pool_created_in_library = 0;
	}
	if( ( extent_data_file_location != NULL )
	 && ( extent_data_file_location != extent_data_filename_start ) )
	{
		memory_free(
		 extent_data_file_location );
	}
	return( -1 );
}

/* Opens the extent data files using a Basic File IO (bfio) pool
 * This function assumes the extent data files are in same order as defined by the descriptor file
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_open_extent_data_files_file_io_pool(
     libvmdk_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     libcerror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_open_extent_data_files_file_io_pool";

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
	if( internal_handle->extent_data_file_io_pool != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - extent data file IO pool already exists.",
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
	if( libvmdk_handle_open_read_grain_table(
	     internal_handle,
	     file_io_pool,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to read grain table.",
                 function );

                return( -1 );
	}
	internal_handle->extent_data_file_io_pool = file_io_pool;

	return( 1 );
}

/* Opens a specific extent data file
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_open_extent_data_file(
     libvmdk_internal_handle_t *internal_handle,
     int data_file_index,
     const char *filename,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libvmdk_handle_open_extent_data_file";

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
	     filename,
	     libcstring_narrow_string_length(
	      filename ) + 1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file IO handle.",
                 function );

                goto on_error;
	}
	if( libvmdk_handle_open_extent_data_file_io_handle(
	     internal_handle,
	     data_file_index,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open extent data file: %s.",
		 function,
		 filename );

		goto on_error;
	}
	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a specific extent data file
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_open_extent_data_file_wide(
     libvmdk_internal_handle_t *internal_handle,
     int data_file_index,
     const wchar_t *filename,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libvmdk_handle_open_extent_data_file_wide";

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
	     filename,
	     libcstring_wide_string_length(
	      filename ) + 1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file IO handle.",
                 function );

                goto on_error;
	}
	if( libvmdk_handle_open_extent_data_file_io_handle(
	     internal_handle,
	     data_file_index,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open extent data file: %ls.",
		 function,
		 filename );

		goto on_error;
	}
	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#endif

/* Opens an extent data file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_open_extent_data_file_io_handle(
     libvmdk_internal_handle_t *internal_handle,
     int extent_index,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_handle_open_extent_data_file_io_handle";
	int bfio_access_flags = 0;
	int number_of_extents = 0;

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
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( ( ( internal_handle->access_flags & LIBVMDK_ACCESS_FLAG_READ ) == 0 )
	 && ( ( internal_handle->access_flags & LIBVMDK_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( internal_handle->access_flags & LIBVMDK_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( internal_handle->extent_data_file_io_pool == NULL )
	{
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
		if( libbfio_pool_initialize(
		     &( internal_handle->extent_data_file_io_pool ),
		     number_of_extents,
		     internal_handle->maximum_number_of_open_handles,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create extent data file file IO pool.",
			 function );

			goto on_error;
		}
		internal_handle->extent_data_file_io_pool_created_in_library = 1;
	}
	if( ( internal_handle->access_flags & LIBVMDK_ACCESS_FLAG_READ ) != 0 )
	{
		bfio_access_flags = LIBBFIO_ACCESS_FLAG_READ;
	}
	if( libbfio_handle_open(
	     file_io_handle,
	     bfio_access_flags,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_IO,
                 LIBCERROR_IO_ERROR_OPEN_FAILED,
                 "%s: unable to open file IO handle.",
                 function );

		goto on_error;
	}
	/* This function currently does not allow the file_io_handle to be set more than once
	 */
	if( libbfio_pool_set_handle(
	     internal_handle->extent_data_file_io_pool,
	     extent_index,
	     file_io_handle,
	     bfio_access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set file IO handle: %d in pool.",
		 function,
		 extent_index );

		goto on_error;
	}
	return( 1 );

on_error:
	if( internal_handle->extent_data_file_io_pool != NULL )
	{
		libbfio_pool_close_all(
		 internal_handle->extent_data_file_io_pool,
		 NULL );
		libbfio_pool_free(
		 &( internal_handle->extent_data_file_io_pool ),
		 NULL );

		internal_handle->extent_data_file_io_pool_created_in_library = 0;
	}
	return( -1 );
}

/* Closes the VMDK file(s)
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

	if( internal_handle->extent_data_file_io_pool_created_in_library != 0 )
	{
		if( libbfio_pool_close_all(
		     internal_handle->extent_data_file_io_pool,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close all files.",
			 function );

			result = -1;
		}
		if( libbfio_pool_free(
		     &( internal_handle->extent_data_file_io_pool ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to extent data free file IO pool.",
			 function );

			result = -1;
		}
		internal_handle->extent_data_file_io_pool_created_in_library = 0;
	}
	internal_handle->extent_data_file_io_pool = NULL;

	if( libvmdk_io_handle_clear(
	     internal_handle->io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		result = -1;
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
	if( internal_handle->grain_table != NULL )
	{
		if( libvmdk_grain_table_free(
		     &( internal_handle->grain_table ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free grain table.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->grains_cache != NULL )
	{
		if( libfcache_cache_free(
		     &( internal_handle->grains_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free grains cache.",
			 function );

			result = -1;
		}
	}
	if( libvmdk_extent_table_clear(
	     internal_handle->extent_table,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to clear extent table.",
		 function );

		result = -1;
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
	libvmdk_extent_descriptor_t *extent_descriptor = NULL;
	libvmdk_extent_file_t *extent_file             = NULL;
	static char *function                          = "libvmdk_handle_open_read_grain_table";
	size64_t extent_file_size                      = 0;
	int extent_index                               = 0;
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
	if( internal_handle->grain_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - grain table already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->grains_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - grains cache already set.",
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
	if( libbfio_pool_get_number_of_handles(
	     file_io_pool,
	     &number_of_file_io_handles,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of file IO handles.",
		 function );

		return( -1 );
	}
	if( number_of_extents != number_of_file_io_handles )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid number of file IO handles.",
		 function );

		goto on_error;
	}
	if( libvmdk_extent_table_initialize_extents(
	     internal_handle->extent_table,
	     number_of_extents,
	     internal_handle->descriptor_file->disk_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize extent table extents.",
		 function );

		goto on_error;
	}
	if( libvmdk_grain_table_initialize(
	     &( internal_handle->grain_table ),
	     internal_handle->io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create grain table.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( internal_handle->grains_cache ),
	     LIBVMDK_MAXIMUM_CACHE_ENTRIES_GRAINS,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create grains cache.",
		 function );

		goto on_error;
	}
	for( extent_index = 0;
	     extent_index < number_of_extents;
	     extent_index++ )
	{
		if( libvmdk_descriptor_file_get_extent_by_index(
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
		if( libbfio_pool_get_size(
		     file_io_pool,
		     extent_index,
		     &extent_file_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size of file IO pool entry: %d.",
			 function,
			 extent_index );

			goto on_error;
		}
/* TODO check file size ? */
		if( libvmdk_extent_table_set_extent_by_extent_descriptor(
		     internal_handle->extent_table,
		     extent_descriptor,
		     extent_index,
		     extent_index,
		     extent_file_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set extent: %d in table.",
			 function,
			 extent_index );

			goto on_error;
		}
		if( ( extent_descriptor->type == LIBVMDK_EXTENT_TYPE_SPARSE )
		 || ( extent_descriptor->type == LIBVMDK_EXTENT_TYPE_VMFS_SPARSE ) )
		{
			if( libvmdk_extent_file_initialize(
			     &extent_file,
			     internal_handle->io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create extent file: %d.",
				 function,
				 extent_index );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "Reading extent file: %d file header:\n",
				 extent_index );
			}
#endif
			if( libvmdk_extent_file_read_file_header(
			     extent_file,
			     file_io_pool,
			     extent_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read extent file: %d header.",
				 function,
				 extent_index );

				goto on_error;
			}
			if( ( extent_file->file_type != LIBVMDK_FILE_TYPE_COWD_SPARSE_DATA )
			 && ( extent_file->file_type != LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: extent file type not supported for extent type.",
				 function );

				goto on_error;
			}
			if( ( internal_handle->descriptor_file->disk_type != LIBVMDK_DISK_TYPE_STREAM_OPTIMIZED )
			 && ( extent_file->file_type == LIBVMDK_FILE_TYPE_VMDK_SPARSE_DATA )
			 && ( ( extent_file->flags & LIBVMDK_FLAG_HAS_GRAIN_COMPRESSION ) != 0 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: grain compression not supported for disk type.",
				 function );

				goto on_error;
			}
			if( extent_index == 0 )
			{
				internal_handle->io_handle->grain_size = extent_file->grain_size;
			}
			else if( extent_file->grain_size != internal_handle->io_handle->grain_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_INPUT,
				 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
				 "%s: extent file: %d grain size mismatch.",
				 function,
				 extent_index );

				goto on_error;
			}
			if( extent_file->is_dirty != 0 )
			{
				internal_handle->io_handle->is_dirty = 1;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "Reading extent file: %d grain directories:\n",
				 extent_index );
			}
#endif
			if( libvmdk_extent_file_read_grain_directories(
			     extent_file,
			     file_io_pool,
			     extent_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read extent file: %d header.",
				 function,
				 extent_index );

				goto on_error;
			}
			if( libvmdk_extent_table_set_extent_storage_media_size_by_index(
			     internal_handle->extent_table,
			     extent_index,
			     extent_file->storage_media_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to set mapped range of element: %d in extent table.",
				 function,
				 extent_index );

				goto on_error;
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
		}
		else if( ( extent_descriptor->type != LIBVMDK_EXTENT_TYPE_FLAT )
		      && ( extent_descriptor->type != LIBVMDK_EXTENT_TYPE_VMFS_FLAT ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported extent type.",
			 function );

			goto on_error;
		}
		if( internal_handle->io_handle->abort == 1 )
		{
			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( extent_file != NULL )
	{
		libvmdk_extent_file_free(
		 &extent_file,
		 NULL );
	}
	if( internal_handle->grains_cache != NULL )
	{
		libfcache_cache_free(
		 &( internal_handle->grains_cache ),
		 NULL );
	}
	if( internal_handle->grain_table != NULL )
	{
		libvmdk_grain_table_free(
		 &( internal_handle->grain_table ),
		 NULL );
	}
	return( -1 );
}

/* Reads the file signature and tries to determine the file type
 * Returns 1 if successful, 0 if no file type could be determined or -1 on error
 */
int libvmdk_handle_open_read_signature(
     libbfio_handle_t *file_io_handle,
     uint8_t *file_type,
     libcerror_error_t **error )
{
	libcsplit_narrow_split_string_t *lines = NULL;
	uint8_t *signature                     = NULL;
	static char *function                  = "libvmdk_handle_open_read_signature";
	ssize_t read_count                     = 0;
	int line_index                         = 0;
	int number_of_lines                    = 0;
	int result                             = 0;

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
	signature = (uint8_t *) memory_allocate(
	                         sizeof( uint8_t ) * 2048 );

	if( signature == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create signature.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     signature,
	     0,
	     sizeof( uint8_t ) * 2048 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear signature.",
		 function );

		goto on_error;
	}
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     0,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset: 0.",
		 function );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              signature,
	              32,
	              error );

	if( read_count != (ssize_t) 32 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read signature.",
		 function );

		goto on_error;
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
	else if( signature[ 0 ] == '#' )
	{
		read_count = libbfio_handle_read_buffer(
		              file_io_handle,
		              &( signature[ 32 ] ),
		              2048 - 32,
		              error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read signature.",
			 function );

			goto on_error;
		}
		if( libcsplit_narrow_string_split(
		     (char *) signature,
		     32 + read_count,
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
		result = libvmdk_descriptor_file_read_signature(
			  lines,
			  number_of_lines,
			  &line_index,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read descriptor file.",
			 function );

			goto on_error;
		}
		else if( result != 0 )
		{
			*file_type = LIBVMDK_FILE_TYPE_DESCRIPTOR_FILE;
		}
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
	}
	return( result );

on_error:
	if( lines != NULL )
	{
		libcsplit_narrow_split_string_free(
		 &lines,
		 NULL );
	}
	if( signature != NULL )
	{
		memory_free(
		 signature );
	}
	return( -1 );
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
	libvmdk_grain_data_t *grain_data           = NULL;
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_read_buffer";
	size_t buffer_offset                       = 0;
	size_t read_size                           = 0;
	ssize_t read_count                         = 0;
	uint64_t grain_index                       = 0;
	off64_t grain_data_offset                  = 0;
	int grain_is_sparse                        = 0;

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
	if( internal_handle->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid internal handle - invalid IO handle - current offset value out of bounds.",
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
	if( ( internal_handle->descriptor_file->parent_content_identifier_set != 0 )
	 && ( internal_handle->descriptor_file->parent_content_identifier != 0xffffffffUL ) )
	{
		if( internal_handle->parent_handle == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid internal handle - missing parent handle.",
			 function );

			return( -1 );
		}
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
	if( buffer_size == 0 )
	{
		return( 0 );
	}
	if( (size64_t) internal_handle->current_offset >= internal_handle->io_handle->media_size )
	{
		return( 0 );
	}
	if( internal_handle->extent_table->extent_files_stream != NULL )
	{
		if( libfdata_stream_seek_offset(
		     internal_handle->extent_table->extent_files_stream,
		     internal_handle->current_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to seek offset in extent files stream.",
			 function );

			return( -1 );
		}
		read_size = buffer_size;

		if( (size64_t) ( internal_handle->current_offset + read_size ) > internal_handle->io_handle->media_size )
		{
			read_size = (size_t) ( internal_handle->io_handle->media_size - internal_handle->current_offset );
		}
		read_count = libfdata_stream_read_buffer(
		              internal_handle->extent_table->extent_files_stream,
			      (intptr_t *) internal_handle->extent_data_file_io_pool,
			      (uint8_t *) buffer,
			      read_size,
			      0,
			      error );

		if( read_count != (ssize_t) read_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read buffer from extent files stream.",
			 function );

			return( -1 );
		}
		buffer_offset = read_size;

		internal_handle->current_offset += (off64_t) read_size;
	}
	else
	{
/* TODO refactor to separate function */
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: requested offset\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 internal_handle->current_offset );
		}
#endif
		grain_index       = internal_handle->current_offset / internal_handle->io_handle->grain_size;
		grain_data_offset = (off64_t) ( internal_handle->current_offset % internal_handle->io_handle->grain_size );

		while( buffer_size > 0 )
		{
			grain_is_sparse = libvmdk_grain_table_grain_is_sparse_at_offset(
					   internal_handle->grain_table,
					   grain_index,
					   internal_handle->extent_data_file_io_pool,
					   internal_handle->extent_table,
					   internal_handle->current_offset,
					   error );

			if( grain_is_sparse == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine if the grain: %" PRIu64 " is sparse.",
				 function,
				 grain_index );

				return( -1 );
			}
			read_size = (size_t) ( internal_handle->io_handle->grain_size - grain_data_offset );

			if( read_size > buffer_size )
			{
				read_size = buffer_size;
			}
			if( (size64_t) ( internal_handle->current_offset + read_size ) > internal_handle->io_handle->media_size )
			{
				read_size = (size_t) ( internal_handle->io_handle->media_size - internal_handle->current_offset );
			}
			if( grain_is_sparse != 0 )
			{
				if( internal_handle->parent_handle == NULL )
				{
					if( memory_set(
					     &( ( (uint8_t *) buffer )[ buffer_offset ] ),
					     0,
					     read_size ) == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_MEMORY,
						 LIBCERROR_MEMORY_ERROR_SET_FAILED,
						 "%s: unable to fill buffer with sparse grain.",
						 function );

						return( -1 );
					}
				}
				else
				{
/* TODO do we need grain offset or current offset ? */
					if( libvmdk_handle_seek_offset(
					     internal_handle->parent_handle,
					     internal_handle->current_offset,
					     SEEK_SET,
					     error ) == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_SEEK_FAILED,
						 "%s: unable to seek grain offset: %" PRIi64 " in parent.",
						 function,
						 internal_handle->current_offset );

						return( -1 );
					}
					read_count = libvmdk_handle_read_buffer(
						      internal_handle->parent_handle,
						      &( ( (uint8_t *) buffer )[ buffer_offset ] ),
						      read_size,
						      error );

					if( read_count != (ssize_t) read_size )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read grain data from parent.",
						 function );

						return( -1 );
					}
				}
			}
			else
			{
				if( libvmdk_grain_table_get_grain_data_at_offset(
				     internal_handle->grain_table,
				     grain_index,
				     internal_handle->extent_data_file_io_pool,
				     internal_handle->extent_table,
				     internal_handle->grains_cache,
				     internal_handle->current_offset,
				     &grain_data,
				     &grain_data_offset,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve grain: %d data.",
					 function,
					 grain_index );

					return( -1 );
				}
				if( grain_data == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing grain: %d data.",
					 function,
					 grain_index );

					return( -1 );
				}
				if( (size64_t) grain_data_offset > grain_data->data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: grain data offset value out of bounds.",
					 function );

					return( -1 );
				}
				if( read_size > (size_t) ( grain_data->data_size - grain_data_offset ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: read size value out of bounds.",
					 function );

					return( -1 );
				}
				if( memory_copy(
				     &( ( (uint8_t *) buffer )[ buffer_offset ] ),
				     &( ( grain_data->data )[ grain_data_offset ] ),
				     read_size ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
					 "%s: unable to copy grain data to buffer.",
					 function );

					return( -1 );
				}
			}
			buffer_offset    += read_size;
			buffer_size      -= read_size;
			grain_index      += 1;
			grain_data_offset = 0;

			internal_handle->current_offset += (off64_t) read_size;

			if( (size64_t) internal_handle->current_offset >= internal_handle->io_handle->media_size )
			{
				break;
			}
			if( internal_handle->io_handle->abort != 0 )
			{
				break;
			}
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

/* Writes (media) data in VMDK format at a specific offset,
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
		offset += internal_handle->current_offset;
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
	internal_handle->current_offset = offset;

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
	*offset = internal_handle->current_offset;

	return( 1 );
}

/* Sets the parent handle
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_set_parent_handle(
     libvmdk_handle_t *handle,
     libvmdk_handle_t *parent_handle,
     libcerror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_set_parent_handle";
	uint32_t content_identifier                = 0;

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
	if( libvmdk_handle_get_content_identifier(
	     parent_handle,
	     &content_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve content identifier from parent handle.",
		 function );

		return( -1 );
	}
	if( content_identifier != internal_handle->descriptor_file->parent_content_identifier )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: mismatch in content identifier.",
		 function );

		return( -1 );
	}
	internal_handle->parent_handle = parent_handle;

	return( 1 );
}

