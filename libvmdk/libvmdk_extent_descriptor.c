/*
 * Extent descriptor functions
 *
 * Copyright (C) 2009-2023, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#include "libvmdk_extent_descriptor.h"
#include "libvmdk_extent_values.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libcthreads.h"
#include "libvmdk_types.h"

/* Creates an extent descriptor
 * Make sure the value extent_descriptor is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_descriptor_initialize(
     libvmdk_extent_descriptor_t **extent_descriptor,
     libvmdk_extent_values_t *extent_values,
     libcerror_error_t **error )
{
	libvmdk_internal_extent_descriptor_t *internal_extent_descriptor = NULL;
	static char *function                                            = "libvmdk_extent_descriptor_initialize";

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
	internal_extent_descriptor = memory_allocate_structure(
	                              libvmdk_internal_extent_descriptor_t );

	if( internal_extent_descriptor == NULL )
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
	     internal_extent_descriptor,
	     0,
	     sizeof( libvmdk_internal_extent_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear extent descriptor.",
		 function );

		memory_free(
		 internal_extent_descriptor );

		return( -1 );
	}
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_extent_descriptor->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	internal_extent_descriptor->extent_values = extent_values;

	*extent_descriptor = (libvmdk_extent_descriptor_t *) internal_extent_descriptor;

	return( 1 );

on_error:
	if( internal_extent_descriptor != NULL )
	{
		memory_free(
		 internal_extent_descriptor );
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
	libvmdk_internal_extent_descriptor_t *internal_extent_descriptor = NULL;
	static char *function                                            = "libvmdk_extent_descriptor_free";
	int result                                                       = 1;

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
		internal_extent_descriptor = (libvmdk_internal_extent_descriptor_t *) *extent_descriptor;
		*extent_descriptor         = NULL;

#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_extent_descriptor->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		/* The extent values reference is freed elsewhere
		 */
		memory_free(
		 internal_extent_descriptor );
	}
	return( result );
}

/* Retrieves the extent type
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_descriptor_get_type(
     libvmdk_extent_descriptor_t *extent_descriptor,
     int *type,
     libcerror_error_t **error )
{
	libvmdk_internal_extent_descriptor_t *internal_extent_descriptor = NULL;
	static char *function                                            = "libvmdk_extent_descriptor_get_type";
	int result                                                       = 1;

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
	internal_extent_descriptor = (libvmdk_internal_extent_descriptor_t *) extent_descriptor;

#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_extent_descriptor->read_write_lock,
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
	if( libvmdk_extent_values_get_type(
	     internal_extent_descriptor->extent_values,
	     type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve type.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_extent_descriptor->read_write_lock,
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
}

/* Retrieves the extent range (offset and size)
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_descriptor_get_range(
     libvmdk_extent_descriptor_t *extent_descriptor,
     off64_t *offset,
     size64_t *size,
     libcerror_error_t **error )
{
	libvmdk_internal_extent_descriptor_t *internal_extent_descriptor = NULL;
	static char *function                                            = "libvmdk_extent_descriptor_get_range";
	int result                                                       = 1;

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
	internal_extent_descriptor = (libvmdk_internal_extent_descriptor_t *) extent_descriptor;

#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_extent_descriptor->read_write_lock,
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
	if( libvmdk_extent_values_get_range(
	     internal_extent_descriptor->extent_values,
	     offset,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve range.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_extent_descriptor->read_write_lock,
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
}

/* Retrieves the size of the UTF-8 encoded filename
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libvmdk_extent_descriptor_get_utf8_filename_size(
     libvmdk_extent_descriptor_t *extent_descriptor,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libvmdk_internal_extent_descriptor_t *internal_extent_descriptor = NULL;
	static char *function                                            = "libvmdk_extent_descriptor_get_utf8_filename_size";
	int result                                                       = 1;

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
	internal_extent_descriptor = (libvmdk_internal_extent_descriptor_t *) extent_descriptor;

#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_extent_descriptor->read_write_lock,
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
	if( libvmdk_extent_values_get_utf8_filename_size(
	     internal_extent_descriptor->extent_values,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_extent_descriptor->read_write_lock,
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
}

/* Retrieves the UTF-8 encoded filename
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libvmdk_extent_descriptor_get_utf8_filename(
     libvmdk_extent_descriptor_t *extent_descriptor,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libvmdk_internal_extent_descriptor_t *internal_extent_descriptor = NULL;
	static char *function                                            = "libvmdk_extent_descriptor_get_utf8_filename";
	int result                                                       = 1;

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
	internal_extent_descriptor = (libvmdk_internal_extent_descriptor_t *) extent_descriptor;

#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_extent_descriptor->read_write_lock,
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
	if( libvmdk_extent_values_get_utf8_filename(
	     internal_extent_descriptor->extent_values,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_extent_descriptor->read_write_lock,
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
}

/* Retrieves the size of the UTF-16 encoded filename
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libvmdk_extent_descriptor_get_utf16_filename_size(
     libvmdk_extent_descriptor_t *extent_descriptor,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libvmdk_internal_extent_descriptor_t *internal_extent_descriptor = NULL;
	static char *function                                            = "libvmdk_extent_descriptor_get_utf16_filename_size";
	int result                                                       = 1;

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
	internal_extent_descriptor = (libvmdk_internal_extent_descriptor_t *) extent_descriptor;

#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_extent_descriptor->read_write_lock,
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
	if( libvmdk_extent_values_get_utf16_filename_size(
	     internal_extent_descriptor->extent_values,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_extent_descriptor->read_write_lock,
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
}

/* Retrieves the UTF-16 encoded filename
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libvmdk_extent_descriptor_get_utf16_filename(
     libvmdk_extent_descriptor_t *extent_descriptor,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libvmdk_internal_extent_descriptor_t *internal_extent_descriptor = NULL;
	static char *function                                            = "libvmdk_extent_descriptor_get_utf16_filename";
	int result                                                       = 1;

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
	internal_extent_descriptor = (libvmdk_internal_extent_descriptor_t *) extent_descriptor;

#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_extent_descriptor->read_write_lock,
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
	if( libvmdk_extent_values_get_utf16_filename(
	     internal_extent_descriptor->extent_values,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBVMDK_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_extent_descriptor->read_write_lock,
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
}

