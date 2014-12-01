/*
 * Metadata functions
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
#include <types.h>

#include "libvmdk_descriptor_file.h"
#include "libvmdk_handle.h"
#include "libvmdk_io_handle.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libcthreads.h"
#include "libvmdk_types.h"

/* Retrieves the disk type
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_get_disk_type(
     libvmdk_handle_t *handle,
     int *disk_type,
     libcerror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_get_disk_type";

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
		 "%s: invalid handle - missing descriptor file.",
		 function );

		return( -1 );
	}
	if( disk_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid disk type.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*disk_type = internal_handle->descriptor_file->disk_type;

#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the media size
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_get_media_size(
     libvmdk_handle_t *handle,
     size64_t *media_size,
     libcerror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_get_media_size";

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
		 "%s: invalid handle - missing descriptor file.",
		 function );

		return( -1 );
	}
	if( media_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*media_size = internal_handle->descriptor_file->media_size;

#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the content identifier
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_get_content_identifier(
     libvmdk_handle_t *handle,
     uint32_t *content_identifier,
     libcerror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_get_content_identifier";

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
		 "%s: invalid handle - missing descriptor file.",
		 function );

		return( -1 );
	}
	if( content_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid content identifier.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*content_identifier = internal_handle->descriptor_file->content_identifier;

#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the parent content identifier
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libvmdk_handle_get_parent_content_identifier(
     libvmdk_handle_t *handle,
     uint32_t *parent_content_identifier,
     libcerror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_get_parent_content_identifier";
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

#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libvmdk_descriptor_file_get_parent_content_identifier(
	          internal_handle->descriptor_file,
	          parent_content_identifier,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve parent content identifier.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );

on_error:
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the size of the UTF-8 encoded parent filename
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libvmdk_handle_get_utf8_parent_filename_size(
     libvmdk_handle_t *handle,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_get_utf8_parent_filename_size";
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

#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libvmdk_descriptor_file_get_utf8_parent_filename_size(
	          internal_handle->descriptor_file,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 parent filename size.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );

on_error:
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the UTF-8 encoded parent filename
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libvmdk_handle_get_utf8_parent_filename(
     libvmdk_handle_t *handle,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_get_utf8_parent_filename";
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

#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libvmdk_descriptor_file_get_utf8_parent_filename(
	          internal_handle->descriptor_file,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 parent filename.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );

on_error:
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the size of the UTF-16 encoded parent filename
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libvmdk_handle_get_utf16_parent_filename_size(
     libvmdk_handle_t *handle,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_get_utf16_parent_filename_size";
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

#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libvmdk_descriptor_file_get_utf16_parent_filename_size(
	          internal_handle->descriptor_file,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 parent filename size.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );

on_error:
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the UTF-16 encoded parent filename
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libvmdk_handle_get_utf16_parent_filename(
     libvmdk_handle_t *handle,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_get_utf16_parent_filename";
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

#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libvmdk_descriptor_file_get_utf16_parent_filename(
	          internal_handle->descriptor_file,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 parent filename.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );

on_error:
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the number of extents
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_get_number_of_extents(
     libvmdk_handle_t *handle,
     int *number_of_extents,
     libcerror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_get_number_of_extents";

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
		 "%s: invalid handle - missing descriptor file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libvmdk_descriptor_file_get_number_of_extents(
	     internal_handle->descriptor_file,
	     number_of_extents,
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
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves a specific extent descriptor
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_get_extent_descriptor(
     libvmdk_handle_t *handle,
     int extent_index,
     libvmdk_extent_descriptor_t **extent_descriptor,
     libcerror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_get_extent_descriptor";

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
		 "%s: invalid handle - missing descriptor file.",
		 function );

		return( -1 );
	}
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
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libvmdk_descriptor_file_get_extent_by_index(
	     internal_handle->descriptor_file,
	     extent_index,
	     (libvmdk_internal_extent_descriptor_t **) extent_descriptor,
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
	if( *extent_descriptor == NULL )
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
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

