/*
 * Mount handle
 *
 * Copyright (C) 2009-2021, Joachim Metz <joachim.metz@gmail.com>
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "mount_file_entry.h"
#include "mount_file_system.h"
#include "mount_handle.h"
#include "vmdktools_libcerror.h"
#include "vmdktools_libcpath.h"
#include "vmdktools_libvmdk.h"

/* Creates a mount handle
 * Make sure the value mount_handle is referencing, is set to NULL
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
	if( mount_file_system_initialize(
	     &( ( *mount_handle )->file_system ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize file system.",
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
		if( mount_file_system_free(
		     &( ( *mount_handle )->file_system ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file system.",
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
	static char *function = "mount_handle_signal_abort";

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
	if( mount_file_system_signal_abort(
	     mount_handle->file_system,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to signal file system to abort.",
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
	if( mount_handle->basename != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid mount handle - basename value already set.",
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
	if( basename_size > (size_t) ( MEMORY_MAXIMUM_ALLOCATION_SIZE / sizeof( system_character_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid basename size value exceeds maximum.",
		 function );

		goto on_error;
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

/* Sets the path prefix
 * Returns 1 if successful or -1 on error
 */
int mount_handle_set_path_prefix(
     mount_handle_t *mount_handle,
     const system_character_t *path_prefix,
     size_t path_prefix_size,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_set_path_prefix";

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
	if( mount_file_system_set_path_prefix(
	     mount_handle->file_system,
	     path_prefix,
	     path_prefix_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set path prefix.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Opens the mount handle
 * Returns 1 if successful, 0 if not or -1 on error
 */
int mount_handle_open(
     mount_handle_t *mount_handle,
     system_character_t * const * filenames,
     int number_of_filenames,
     libcerror_error_t **error )
{
	libvmdk_handle_t *vmdk_handle      = NULL;
	system_character_t *basename_end   = NULL;
	static char *function              = "mount_handle_open";
	size_t basename_length             = 0;
	size_t filename_length             = 0;
	uint32_t parent_content_identifier = 0;
	int disk_type                      = 0;
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
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
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
	     &vmdk_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libvmdk_handle_open_wide(
	          vmdk_handle,
	          filenames[ 0 ],
	          LIBVMDK_OPEN_READ,
	          error );
#else
	result = libvmdk_handle_open(
	          vmdk_handle,
	          filenames[ 0 ],
	          LIBVMDK_OPEN_READ,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle.",
		 function );

		goto on_error;
	}
	if( libvmdk_handle_get_disk_type(
	     vmdk_handle,
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
	if( ( disk_type != LIBVMDK_DISK_TYPE_FLAT_2GB_EXTENT )
	 && ( disk_type != LIBVMDK_DISK_TYPE_SPARSE_2GB_EXTENT )
	 && ( disk_type != LIBVMDK_DISK_TYPE_MONOLITHIC_SPARSE )
	 && ( disk_type != LIBVMDK_DISK_TYPE_STREAM_OPTIMIZED )
	 && ( disk_type != LIBVMDK_DISK_TYPE_VMFS_FLAT )
	 && ( disk_type != LIBVMDK_DISK_TYPE_VMFS_FLAT_PRE_ALLOCATED )
	 && ( disk_type != LIBVMDK_DISK_TYPE_VMFS_FLAT_ZEROED )
	 && ( disk_type != LIBVMDK_DISK_TYPE_VMFS_SPARSE )
	 && ( disk_type != LIBVMDK_DISK_TYPE_VMFS_SPARSE_THIN ) )
	{
		/* Unsupported disk type
		 */
		result = 0;
	}
	else
	{
		result = libvmdk_handle_get_parent_content_identifier(
			  vmdk_handle,
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
			result = mount_handle_open_parent(
			          mount_handle,
			          vmdk_handle,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to open parent handle.",
				 function );

				goto on_error;
			}
		}
		else
		{
			result = 1;
		}
	}
	if( result == 0 )
	{
		if( libvmdk_handle_close(
		     vmdk_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close handle.",
			 function );

			goto on_error;
		}
		if( libvmdk_handle_free(
		     &vmdk_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free handle.",
			 function );

			goto on_error;
		}
		return( 0 );
	}
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
	if( libvmdk_handle_open_extent_data_files(
	     vmdk_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open extent data files.",
		 function );

		goto on_error;
	}
	if( mount_file_system_append_handle(
	     mount_handle->file_system,
	     vmdk_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append handle to file system.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( vmdk_handle != NULL )
	{
		libvmdk_handle_free(
		 &vmdk_handle,
		 NULL );
	}
	return( -1 );
}

/* Opens the parent handle
 * Returns 1 if successful, 0 if no parent or -1 on error
 */
int mount_handle_open_parent(
     mount_handle_t *mount_handle,
     libvmdk_handle_t *vmdk_handle,
     libcerror_error_t **error )
{
	libvmdk_handle_t *parent_vmdk_handle = NULL;
	system_character_t *parent_filename  = NULL;
	system_character_t *parent_path      = NULL;
	static char *function                = "mount_handle_open_parent";
	size_t parent_filename_size          = 0;
	size_t parent_path_size              = 0;
	uint32_t parent_content_identifier   = 0;
	int parent_disk_type                 = 0;
	int result                           = 0;

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
	          vmdk_handle,
	          &parent_filename_size,
	          error );
#else
	result = libvmdk_handle_get_utf8_parent_filename_size(
	          vmdk_handle,
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
	if( parent_filename_size > (size_t) ( MEMORY_MAXIMUM_ALLOCATION_SIZE / sizeof( system_character_t ) ) )
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
	          vmdk_handle,
	          (uint16_t *) parent_filename,
	          parent_filename_size,
	          error );
#else
	result = libvmdk_handle_get_utf8_parent_filename(
	          vmdk_handle,
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
	     &parent_vmdk_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize parent handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libvmdk_handle_open_wide(
	     parent_vmdk_handle,
	     parent_path,
	     LIBVMDK_OPEN_READ,
	     error ) != 1 )
#else
	if( libvmdk_handle_open(
	     parent_vmdk_handle,
	     parent_path,
	     LIBVMDK_OPEN_READ,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open parent handle: %" PRIs_SYSTEM ".",
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
	     parent_vmdk_handle,
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
	if( ( parent_disk_type != LIBVMDK_DISK_TYPE_FLAT_2GB_EXTENT )
	 && ( parent_disk_type != LIBVMDK_DISK_TYPE_SPARSE_2GB_EXTENT )
	 && ( parent_disk_type != LIBVMDK_DISK_TYPE_MONOLITHIC_SPARSE )
	 && ( parent_disk_type != LIBVMDK_DISK_TYPE_STREAM_OPTIMIZED )
	 && ( parent_disk_type != LIBVMDK_DISK_TYPE_VMFS_FLAT )
	 && ( parent_disk_type != LIBVMDK_DISK_TYPE_VMFS_FLAT_PRE_ALLOCATED )
	 && ( parent_disk_type != LIBVMDK_DISK_TYPE_VMFS_FLAT_ZEROED )
	 && ( parent_disk_type != LIBVMDK_DISK_TYPE_VMFS_SPARSE )
	 && ( parent_disk_type != LIBVMDK_DISK_TYPE_VMFS_SPARSE_THIN ) )
	{
		/* Unsupported disk type
		 */
		result = 0;
	}
	else
	{
		result = libvmdk_handle_get_parent_content_identifier(
			  parent_vmdk_handle,
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
			result = mount_handle_open_parent(
				  mount_handle,
				  parent_vmdk_handle,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to open parent handle.",
				 function );

				goto on_error;
			}
		}
		else
		{
			result = 1;
		}
	}
	if( result == 0 )
	{
		if( libvmdk_handle_close(
		     parent_vmdk_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close parent handle.",
			 function );

			goto on_error;
		}
		if( libvmdk_handle_free(
		     &parent_vmdk_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free parent handle.",
			 function );

			goto on_error;
		}
		return( 0 );
	}
	if( libvmdk_handle_open_extent_data_files(
	     parent_vmdk_handle,
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
	     vmdk_handle,
	     parent_vmdk_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set parent handle.",
		 function );

		goto on_error;
	}
	if( mount_file_system_append_handle(
	     mount_handle->file_system,
	     parent_vmdk_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append parent handle to file system.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( parent_vmdk_handle != NULL )
	{
		libvmdk_handle_free(
		 &parent_vmdk_handle,
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
	libvmdk_handle_t *vmdk_handle = NULL;
	static char *function         = "mount_handle_close";
	int handle_index              = 0;
	int number_of_handles         = 0;
	int result                    = 0;

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
	if( mount_file_system_get_number_of_handles(
	     mount_handle->file_system,
	     &number_of_handles,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of handles.",
		 function );

		return( -1 );
	}
	for( handle_index = number_of_handles - 1;
	     handle_index > 0;
	     handle_index-- )
	{
		if( mount_file_system_get_handle_by_index(
		     mount_handle->file_system,
		     handle_index,
		     &vmdk_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve handle: %d.",
			 function,
			 handle_index );

			result = -1;
		}
		if( libvmdk_handle_close(
		     vmdk_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close handle: %d.",
			 function,
			 handle_index );

			result = -1;
		}
		if( libvmdk_handle_free(
		     &vmdk_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free handle: %d.",
			 function,
			 handle_index );

			result = -1;
		}
	}
	return( result );
}

/* Retrieves a file entry for a specific path
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int mount_handle_get_file_entry_by_path(
     mount_handle_t *mount_handle,
     const system_character_t *path,
     mount_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libvmdk_handle_t *vmdk_handle      = NULL;
	const system_character_t *filename = NULL;
	static char *function              = "mount_handle_get_file_entry_by_path";
	size_t filename_length             = 0;
	size_t path_index                  = 0;
	size_t path_length                 = 0;
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
	if( path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		return( -1 );
	}
	path_length = system_string_length(
	               path );

	if( path_length == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid path length value out of bounds.",
		 function );

		goto on_error;
	}
	if( ( path_length >= 2 )
	 && ( path[ path_length - 1 ] == LIBCPATH_SEPARATOR ) )
	{
		path_length--;
	}
	path_index = path_length;

	while( path_index > 0 )
	{
		if( path[ path_index ] == LIBCPATH_SEPARATOR )
		{
			break;
		}
		path_index--;
	}
	/* Ignore the name of the root item
	 */
	if( path_length == 0 )
	{
		filename        = _SYSTEM_STRING( "" );
		filename_length = 0;
	}
	else
	{
		filename        = &( path[ path_index + 1 ] );
		filename_length = path_length - ( path_index + 1 );
	}
	result = mount_file_system_get_handle_by_path(
	          mount_handle->file_system,
	          path,
	          path_length,
	          &vmdk_handle,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve handle.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( mount_file_entry_initialize(
		     file_entry,
		     mount_handle->file_system,
		     filename,
		     filename_length,
		     vmdk_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize file entry.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	return( -1 );
}

