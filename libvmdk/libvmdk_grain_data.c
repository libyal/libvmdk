/*
 * Grain data functions
 *
 * Copyright (c) 2006-2013, Joachim Metz <joachim.metz@gmail.com>
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

#include "libvmdk_grain_data.h"
#include "libvmdk_libcerror.h"

/* Creates grain data
 * Make sure the value grain_data is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libvmdk_grain_data_initialize(
     libvmdk_grain_data_t **grain_data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_grain_data_initialize";

	if( grain_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain data.",
		 function );

		return( -1 );
	}
	if( *grain_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid grain data value already set.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	*grain_data = memory_allocate_structure(
	               libvmdk_grain_data_t );

	if( *grain_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create grain data.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *grain_data,
	     0,
	     sizeof( libvmdk_grain_data_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear grain data.",
		 function );

		goto on_error;
	}
	if( data_size > 0 )
	{
		( *grain_data )->data = (uint8_t *) memory_allocate(
		                                     sizeof( uint8_t ) * data_size );

		if( ( *grain_data )->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data.",
			 function );

			goto on_error;
		}
		( *grain_data )->data_size = data_size;
	}
	return( 1 );

on_error:
	if( *grain_data != NULL )
	{
		memory_free(
		 *grain_data );

		*grain_data = NULL;
	}
	return( -1 );
}

/* Frees grain data
 * Returns 1 if successful or -1 on error
 */
int libvmdk_grain_data_free(
     libvmdk_grain_data_t **grain_data,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_grain_data_free";

	if( grain_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain data.",
		 function );

		return( -1 );
	}
	if( *grain_data != NULL )
	{
		if( ( *grain_data )->data != NULL )
		{
			memory_free(
			 ( *grain_data )->data );
		}
		memory_free(
		 *grain_data );

		*grain_data = NULL;
	}
	return( 1 );
}

