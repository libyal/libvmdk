/* 
 * Mount handle
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

#include "mount_handle.h"
#include "vmdktools_libcdata.h"
#include "vmdktools_libcerror.h"
#include "vmdktools_libcnotify.h"
#include "vmdktools_libvmdk.h"

/* Initializes the mount handle
 * Returns 1 if successful or -1 on error
 */
int mount_handle_initialize(
     mount_handle_t **mount_handle,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_initialize";

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( *mount_handle == NULL )
	{
		*mount_handle = memory_allocate_structure(
		                 mount_handle_t );

		if( *mount_handle == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create mount handle.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     *mount_handle,
		     0,
		     sizeof( mount_handle_t ) ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear mount handle.",
			 function );

			memory_free(
			 *mount_handle );

			*mount_handle = NULL;

			return( -1 );
		}
		if( libcdata_array_initialize(
		     &( ( *mount_handle )->input_handles_array ),
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize input handles array.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *mount_handle != NULL )
	{
		memory_free(
		 *mount_handle );

		*mount_handle = NULL;
	}
	return( -1 );
}

/* Frees the mount handle and its elements
 * Returns 1 if successful or -1 on error
 */
int mount_handle_free(
     mount_handle_t **mount_handle,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_free";
	int result            = 1;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( *mount_handle != NULL )
	{
		if( libcdata_array_free(
		     &( ( *mount_handle )->input_handles_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libvmdk_handle_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free input handles array.",
			 function );

			result = -1;
		}
		memory_free(
		 *mount_handle );

		*mount_handle = NULL;
	}
	return( result );
}

/* Signals the mount handle to abort
 * Returns 1 if successful or -1 on error
 */
int mount_handle_signal_abort(
     mount_handle_t *mount_handle,
     libcerror_error_t **error )
{
	libvmdk_handle_t *input_handle = NULL;
	static char *function          = "mount_handle_signal_abort";	
	int input_handle_index         = 0;
	int number_ofinput__handles    = 0;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     mount_handle->input_handles_array,
	     &number_of_input_handles,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of input handles.",
		 function );

		return( -1 );
	}
	for( input_handle_index = number_of_input_handles - 1;
	     input_handle_index > 0;
	     input_handle_index-- )
	{
		if( libcdata_array_get_entry_by_index(
		     mount_handle->input_handles_array,
		     input_handle_index,
		     (intptr_t **) &input_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve input handle: %d.",
			 function,
			 input_handle_index );

			return( -1 );
		}
		if( libvmdk_handle_signal_abort(
		     input_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal input handle: %d to abort.",
			 function,
			 input_handle_index );

			return( -1 );
		}
	}
	return( 1 );
}

/* Opens the input of the mount handle
 * Returns 1 if successful or -1 on error
 */
int mount_handle_open_input(
     mount_handle_t *mount_handle,
     libcstring_system_character_t * const * filenames,
     int number_of_filenames,
     libcerror_error_t **error )
{
	libvmdk_handle_t *input_handle = NULL;
	static char *function          = "mount_handle_open_input";
	int result                     = 0;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
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
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: number of filenames value out of bounds.",
		 function );

		return( -1 );
	}
/* TODO basename support */
	if( libvmdk_handle_initialize(
	     &input_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize input handle.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libvmdk_handle_open_wide(
	     input_handle,
	     filenames[ 0 ],
	     LIBVMDK_OPEN_READ,
	     error ) != 1 )
#else
	if( libvmdk_handle_open(
	     input_handle,
	     filenames[ 0 ],
	     LIBVMDK_OPEN_READ,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open input handle.",
		 function );

		goto on_error;
	}
	if( mount_handle_open_input_parent_handle(
	     mount_handle,
	     handle,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open parent input handle.",
		 function );

		goto on_error;
	}
	if( number_of_filenames == 1 )
	{
		result = libvmdk_handle_open_extent_data_files(
		          input_handle,
		          error );
	}
	else
	{
/* TODO */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open extent data files.",
		 function );

		goto on_error;
	}
	if( libcdata_array_append_value(
	     mount_handle->input_handles_array,
	     (intptr_t *) input_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_ARRAY_FAILED,
		 "%s: unable to append input handle to array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
/* TODO flush array */
	if( handle != NULL )
	{
		libvmdk_handle_free(
		 &input_handle,
		 NULL );
	}
	return( -1 );
}

/* Opens the parent input handle
 * Returns 1 if successful, 0 if no parent or -1 on error
 */
int mount_handle_open_input_parent_handle(
     mount_handle_t *mount_handle,
     libvmdk_handle_t *input_handle,
     libcerror_error_t **error )
{
	libcstring_system_character_t *parent_filename = NULL;
	libvmdk_handle_t *parent_input_handle          = NULL;
	static char *function                          = "mount_handle_open_input_parent_handle";
	size_t parent_filename_size                    = 0;
	uint32_t parent_content_identifier             = 0;
	int result                                     = 0;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( libvmdk_handle_get_parent_content_identifier(
	     input_handle,
	     &parent_content_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve parent content identifier.",
		 function );

		goto on_error;
	}
	if( parent_content_identifier == 0xffffffffUL )
	{
		return( 0 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libvmdk_handle_get_utf16_parent_filename_size(
		  input_handle,
		  &parent_filename_size,
		  error );
#else
	result = libvmdk_handle_get_utf8_parent_filename_size(
		  input_handle,
		  &parent_filename_size,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve parent filename size.",
		 function );

		goto on_error;
	}
	if( parent_filename_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing parent filename.",
		 function );

		goto on_error;
	}
	if( ( parent_filename_size > (size_t) SSIZE_MAX )
	 || ( ( sizeof( libcstring_system_character_t ) * parent_filename_size ) > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid parent filename size value exceeds maximum.",
		 function );

		goto on_error;
	}
	parent_filename = libcstring_system_string_allocate(
			   parent_filename_size );

	if( parent_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create parent filename string.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libvmdk_handle_get_utf16_parent_filename(
		  input_handle,
		  (uint16_t *) parent_filename,
		  parent_filename_size,
		  error );
#else
	result = libvmdk_handle_get_utf8_parent_filename(
		  input_handle,
		  (uint8_t *) parent_filename,
		  parent_filename_size,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve parent filename.",
		 function );

		goto on_error;
	}
/* TODO basename support */
	if( libvmdk_handle_initialize(
	     &parent_input_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize parent input handle.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libvmdk_handle_open_wide(
	     parent_input_handle,
	     parent_filename,
	     LIBVMDK_OPEN_READ,
	     error ) != 1 )
#else
	if( libvmdk_handle_open(
	     parent_input_handle,
	     parent_filename,
	     LIBVMDK_OPEN_READ,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open parent input handle: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 parent_filename );

		goto on_error;
	}
	memory_free(
	 parent_filename );

	parent_filename = NULL;

	if( mount_handle_open_input_parent_handle(
	     mount_handle,
	     parent_handle,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open parent input handle.",
		 function );

		return( -1 );
	}
	if( libvmdk_handle_open_extent_data_files(
	     parent_input_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open parent extent data files.",
		 function );

		goto on_error;
	}
	if( libvmdk_handle_set_parent_handle(
	     input_handle,
	     parent_input_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set parent input handle.",
		 function );

		goto on_error;
	}
	if( libcdata_array_append_value(
	     mount_handle->input_handles_array,
	     (intptr_t *) parent_input_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_ARRAY_FAILED,
		 "%s: unable to append parent input handle to array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( parent_handle != NULL )
	{
		libvmdk_handle_free(
		 &parent_input_handle,
		 NULL );
	}
	if( parent_filename != NULL )
	{
		memory_free(
		 parent_filename );
	}
	return( -1 );
}

/* Closes the mount handle
 * Returns the 0 if succesful or -1 on error
 */
int mount_handle_close(
     mount_handle_t *mount_handle,
     libcerror_error_t **error )
{
	libvmdk_handle_t *input_handle = NULL;
	static char *function          = "mount_handle_close";
	int input_handle_index         = 0;
	int number_ofinput__handles    = 0;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     mount_handle->input_handles_array,
	     &number_of_input_handles,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of input handles.",
		 function );

		return( -1 );
	}
	for( input_handle_index = number_of_input_handles - 1;
	     input_handle_index > 0;
	     input_handle_index-- )
	{
		if( libcdata_array_get_entry_by_index(
		     mount_handle->input_handles_array,
		     input_handle_index,
		     (intptr_t **) &input_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve input handle: %d.",
			 function,
			 input_handle_index );

			return( -1 );
		}
		if( libvmdk_handle_close(
		     input_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close input handle: %d.",
			 function,
			 input_handle_index );

			return( -1 );
		}
	}
	return( 0 );
}

/* Read a buffer from the input handle
 * Returns the number of bytes read if successful or -1 on error
 */
ssize_t mount_handle_read_buffer(
         mount_handle_t *mount_handle,
         int input_handle_index,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error )
{
	libvmdk_handle_t *input_handle = NULL;
	static char *function          = "mount_handle_read_buffer";
	ssize_t read_count             = 0;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     mount_handle->input_handles_array,
	     input_handle_index,
	     (intptr_t **) &input_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve input handle: %d.",
		 function,
		 input_handle_index );

		return( -1 );
	}
	read_count = libvmdk_handle_read_buffer(
	              input_handle,
	              buffer,
	              size,
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer from input handle: %d.",
		 function,
		 input_handle_index );

		return( -1 );
	}
	return( read_count );
}

/* Seeks a specific offset from the input handle
 * Returns the offset if successful or -1 on error
 */
off64_t mount_handle_seek_offset(
         mount_handle_t *mount_handle,
         int input_handle_index,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libvmdk_handle_t *input_handle = NULL;
	static char *function          = "mount_handle_seek_offset";

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     mount_handle->input_handles_array,
	     input_handle_index,
	     (intptr_t **) &input_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve input handle: %d.",
		 function,
		 input_handle_index );

		return( -1 );
	}
	offset = libvmdk_handle_seek_offset(
	          mount_handle->input_handle,
	          offset,
	          whence,
	          error );

	if( offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset in input handle: %d.",
		 function,
		 input_handle_index );

		return( -1 );
	}
	return( offset );
}

/* Retrieves the media size of the input handle
 * Returns 1 if successful or -1 on error
 */
int mount_handle_get_media_size(
     mount_handle_t *mount_handle,
     int input_handle_index,
     size64_t *size,
     libcerror_error_t **error )
{
	libvmdk_handle_t *input_handle = NULL;
	static char *function          = "mount_handle_get_media_size";

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     mount_handle->input_handles_array,
	     input_handle_index,
	     (intptr_t **) &input_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve input handle: %d.",
		 function,
		 input_handle_index );

		return( -1 );
	}
	if( libvmdk_handle_get_media_size(
	     mount_handle->input_handle,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media size from input handle: %d.",
		 function,
		 input_handle_index );

		return( -1 );
	}
	return( 1 );
}

