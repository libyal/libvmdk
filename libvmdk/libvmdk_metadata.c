/*
 * Metadata functions
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
#include <types.h>

#include "libvmdk_descriptor_file.h"
#include "libvmdk_handle.h"
#include "libvmdk_libcerror.h"
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
		 "%s: invalid internal handle - missing descriptor file.",
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
	*disk_type = internal_handle->descriptor_file->disk_type;

	return( 1 );
}

/* Retrieves the number of media size
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
		 "%s: invalid internal handle - missing descriptor file.",
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
	*media_size = internal_handle->descriptor_file->media_size;

	return( 1 );
}

