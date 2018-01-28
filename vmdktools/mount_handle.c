/*
 * Mount handle
 *
 * Copyright (C) 2009-2018, Joachim Metz <joachim.metz@gmail.com>
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "mount_handle.h"
#include "vmdktools_libcdata.h"
#include "vmdktools_libcerror.h"
#include "vmdktools_libcnotify.h"
#include "vmdktools_libcpath.h"
#include "vmdktools_libvmdk.h"

/* Creates a mount handle
 * Make sure the value moutn_handle is referencing, is set to NULL
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
	if( *mount_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid mount handle value already set.",
		 function );

		return( -1 );
	}
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

		goto on_error;
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

/* Frees a mount handle
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
		if( ( *mount_handle )->basename != NULL )
		{
			memory_free(
			 ( *mount_handle )->basename );
		}
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
	int number_of_input_handles    = 0;

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
 * Returns 1 if successful, 0 if disk type is not supported or -1 on error
 */
int mount_handle_open_input(
     mount_handle_t *mount_handle,
     system_character_t * const * filenames,
     int number_of_filenames,
     libcerror_error_t **error )
{
	libvmdk_handle_t *input_handle     = NULL;
	system_character_t *basename_end   = NULL;
	static char *function              = "mount_handle_open_input";
	size_t basename_length             = 0;
	size_t filename_length             = 0;
	uint32_t parent_content_identifier = 0;
	int disk_type                      = 0;
	int entry_index                    = 0;
	int result                         = 0;

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
	filename_length = system_string_length(
	                   filenames[ 0 ] );

	basename_end = system_string_search_character_reverse(
	                filenames[ 0 ],
	                (system_character_t) LIBCPATH_SEPARATOR,
	                filename_length + 1 );

	if( basename_end != NULL )
	{
		basename_length = (size_t) ( basename_end - filenames[ 0 ] ) + 1;
	}
	if( basename_length > 0 )
	{
		if( mount_handle_set_basename(
		     mount_handle,
		     filenames[ 0 ],
		     basename_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set basename.",
			 function );

			goto on_error;
		}
	}
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
	if( libvmdk_handle_get_disk_type(
	     input_handle,
	     &disk_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve disk type.",
		 function );

		goto on_error;
	}
	if( ( disk_type == LIBVMDK_DISK_TYPE_2GB_EXTENT_FLAT )
	 || ( disk_type == LIBVMDK_DISK_TYPE_2GB_EXTENT_SPARSE )
	 || ( disk_type == LIBVMDK_DISK_TYPE_MONOLITHIC_SPARSE )
	 || ( disk_type == LIBVMDK_DISK_TYPE_STREAM_OPTIMIZED )
	 || ( disk_type == LIBVMDK_DISK_TYPE_VMFS_FLAT )
	 || ( disk_type == LIBVMDK_DISK_TYPE_VMFS_FLAT_PRE_ALLOCATED )
	 || ( disk_type == LIBVMDK_DISK_TYPE_VMFS_FLAT_ZEROED )
	 || ( disk_type == LIBVMDK_DISK_TYPE_VMFS_SPARSE )
	 || ( disk_type == LIBVMDK_DISK_TYPE_VMFS_SPARSE_THIN ) )
	{
		result = libvmdk_handle_get_parent_content_identifier(
			  input_handle,
			  &parent_content_identifier,
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
		else if( ( result != 0 )
		      && ( parent_content_identifier != 0xffffffffUL ) )
		{
			result = mount_handle_open_input_parent_handle(
			          mount_handle,
			          input_handle,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to open parent input handle.",
				 function );

				goto on_error;
			}
		}
		else
		{
			result = 1;
		}
	}
	else
	{
		/* Unsupported disk type
		 */
		result = 0;
	}
	if( result != 0 )
	{
/* TODO add support for passing extent data files ? */
		if( number_of_filenames != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported number of filenames.",
			 function );

			goto on_error;
		}
		result = libvmdk_handle_open_extent_data_files(
			  input_handle,
			  error );

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
		if( libcdata_array_append_entry(
		     mount_handle->input_handles_array,
		     &entry_index,
		     (intptr_t *) input_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append input handle to array.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libvmdk_handle_close(
		     input_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close input handle.",
			 function );

			goto on_error;
		}
		if( libvmdk_handle_free(
		     &input_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free input handle.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( input_handle != NULL )
	{
		libvmdk_handle_free(
		 &input_handle,
		 NULL );
	}
	libcdata_array_empty(
	 mount_handle->input_handles_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libvmdk_handle_free,
	 NULL );

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
	libvmdk_handle_t *parent_input_handle = NULL;
	system_character_t *parent_filename   = NULL;
	system_character_t *parent_path       = NULL;
	static char *function                 = "mount_handle_open_input_parent_handle";
	size_t parent_filename_size           = 0;
	size_t parent_path_size               = 0;
	uint32_t parent_content_identifier    = 0;
	int entry_index                       = 0;
	int parent_disk_type                  = 0;
	int result                            = 0;

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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
	 || ( ( sizeof( system_character_t ) * parent_filename_size ) > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid parent filename size value exceeds maximum.",
		 function );

		goto on_error;
	}
	parent_filename = system_string_allocate(
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
	if( mount_handle->basename == NULL )
	{
		parent_path      = parent_filename;
		parent_path_size = parent_filename_size;
	}
	else
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libcpath_path_join_wide(
		     &parent_path,
		     &parent_path_size,
		     mount_handle->basename,
		     mount_handle->basename_size - 1,
		     parent_filename,
		     parent_filename_size - 1,
		     error ) != 1 )
#else
		if( libcpath_path_join(
		     &parent_path,
		     &parent_path_size,
		     mount_handle->basename,
		     mount_handle->basename_size - 1,
		     parent_filename,
		     parent_filename_size - 1,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create parent path.",
			 function );

			goto on_error;
		}
	}
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libvmdk_handle_open_wide(
	     parent_input_handle,
	     parent_path,
	     LIBVMDK_OPEN_READ,
	     error ) != 1 )
#else
	if( libvmdk_handle_open(
	     parent_input_handle,
	     parent_path,
	     LIBVMDK_OPEN_READ,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open parent input handle: %" PRIs_SYSTEM ".",
		 function,
		 parent_path );

		goto on_error;
	}
	if( parent_path != NULL )
	{
		if( mount_handle->basename != NULL )
		{
			memory_free(
			 parent_path );
		}
		parent_path = NULL;
	}
	if( parent_filename != NULL )
	{
		memory_free(
		 parent_filename );

		parent_filename = NULL;
	}
	if( libvmdk_handle_get_disk_type(
	     parent_input_handle,
	     &parent_disk_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve disk type.",
		 function );

		goto on_error;
	}
	if( ( parent_disk_type == LIBVMDK_DISK_TYPE_2GB_EXTENT_FLAT )
	 || ( parent_disk_type == LIBVMDK_DISK_TYPE_2GB_EXTENT_SPARSE )
	 || ( parent_disk_type == LIBVMDK_DISK_TYPE_MONOLITHIC_SPARSE )
	 || ( parent_disk_type == LIBVMDK_DISK_TYPE_STREAM_OPTIMIZED )
	 || ( parent_disk_type == LIBVMDK_DISK_TYPE_VMFS_FLAT )
	 || ( parent_disk_type == LIBVMDK_DISK_TYPE_VMFS_FLAT_PRE_ALLOCATED )
	 || ( parent_disk_type == LIBVMDK_DISK_TYPE_VMFS_FLAT_ZEROED )
	 || ( parent_disk_type == LIBVMDK_DISK_TYPE_VMFS_SPARSE )
	 || ( parent_disk_type == LIBVMDK_DISK_TYPE_VMFS_SPARSE_THIN ) )
	{
		result = libvmdk_handle_get_parent_content_identifier(
			  parent_input_handle,
			  &parent_content_identifier,
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
		else if( ( result != 0 )
		      && ( parent_content_identifier != 0xffffffffUL ) )
		{
			result = mount_handle_open_input_parent_handle(
				  mount_handle,
				  parent_input_handle,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to open parent input handle.",
				 function );

				goto on_error;
			}
		}
		else
		{
			result = 1;
		}
	}
	else
	{
		/* Unsupported disk type
		 */
		result = 0;
	}
	if( result != 0 )
	{
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
		if( libcdata_array_append_entry(
		     mount_handle->input_handles_array,
		     &entry_index,
		     (intptr_t *) parent_input_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append parent input handle to array.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libvmdk_handle_close(
		     parent_input_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close parent input handle.",
			 function );

			goto on_error;
		}
		if( libvmdk_handle_free(
		     &parent_input_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free parent input handle.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( parent_input_handle != NULL )
	{
		libvmdk_handle_free(
		 &parent_input_handle,
		 NULL );
	}
	if( ( parent_path != NULL )
	 && ( mount_handle->basename != NULL ) )
	{
		memory_free(
		 parent_path );
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
	int number_of_input_handles    = 0;

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

/* Read a buffer from a specific input handle
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

/* Seeks a specific offset in a specific input handle
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
	          input_handle,
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

/* Retrieves the media size of a specific input handle
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
	     input_handle,
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

/* Retrieves the number of input handles
 * Returns 1 if successful or -1 on error
 */
int mount_handle_get_number_of_input_handles(
     mount_handle_t *mount_handle,
     int *number_of_input_handles,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_get_number_of_input_handles";

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
	     number_of_input_handles,
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
	return( 1 );
}

/* Sets the basename
 * Returns 1 if successful or -1 on error
 */
int mount_handle_set_basename(
     mount_handle_t *mount_handle,
     const system_character_t *basename,
     size_t basename_size,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_set_basename";

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
	if( basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename.",
		 function );

		return( -1 );
	}
	if( basename_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing basename.",
		 function );

		goto on_error;
	}
	if( ( basename_size > (size_t) SSIZE_MAX )
	 || ( ( sizeof( system_character_t ) * basename_size ) > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid basename size value exceeds maximum.",
		 function );

		goto on_error;
	}
	if( mount_handle->basename != NULL )
	{
		memory_free(
		 mount_handle->basename );

		mount_handle->basename      = NULL;
		mount_handle->basename_size = 0;
	}
	mount_handle->basename = system_string_allocate(
	                          basename_size );

	if( mount_handle->basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create basename string.",
		 function );

		goto on_error;
	}
	if( system_string_copy(
	     mount_handle->basename,
	     basename,
	     basename_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy basename.",
		 function );

		goto on_error;
	}
	mount_handle->basename[ basename_size - 1 ] = 0;

	mount_handle->basename_size = basename_size;

	return( 1 );

on_error:
	if( mount_handle->basename != NULL )
	{
		memory_free(
		 mount_handle->basename );

		mount_handle->basename = NULL;
	}
	mount_handle->basename_size = 0;

	return( -1 );
}

