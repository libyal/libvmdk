/*
 * Mount file system
 *
 * Copyright (C) 2009-2022, Joachim Metz <joachim.metz@gmail.com>
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

#if defined( HAVE_SYS_STAT_H )
#include <sys/stat.h>
#endif

#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "mount_file_system.h"
#include "vmdktools_libcdata.h"
#include "vmdktools_libcerror.h"
#include "vmdktools_libvmdk.h"

/* Creates a file system
 * Make sure the value file_system is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int mount_file_system_initialize(
     mount_file_system_t **file_system,
     libcerror_error_t **error )
{
#if defined( WINAPI )
	FILETIME filetime;
	SYSTEMTIME systemtime;

#elif defined( HAVE_CLOCK_GETTIME )
	struct timespec time_structure;
#endif

	static char *function = "mount_file_system_initialize";

#if defined( WINAPI )
	DWORD error_code      = 0;
#else
	int64_t timestamp     = 0;
#endif

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( *file_system != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system value already set.",
		 function );

		return( -1 );
	}
	*file_system = memory_allocate_structure(
	                mount_file_system_t );

	if( *file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file system.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *file_system,
	     0,
	     sizeof( mount_file_system_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file system.",
		 function );

		memory_free(
		 *file_system );

		*file_system = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *file_system )->handles_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize handles array.",
		 function );

		goto on_error;
	}
#if defined( WINAPI )
	if( memory_set(
	     &systemtime,
	     0,
	     sizeof( SYSTEMTIME ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear systemtime.",
		 function );

		goto on_error;
	}
	GetSystemTime(
	 &systemtime );

	if( SystemTimeToFileTime(
	     &systemtime,
	     &filetime ) == 0 )
	{
		error_code = GetLastError();

		libcerror_system_set_error(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 error_code,
		 "%s: unable to retrieve FILETIME of current time.",
		 function );

		goto on_error;
	}
	( *file_system )->mounted_timestamp = ( (uint64_t) filetime.dwHighDateTime << 32 ) | filetime.dwLowDateTime;

#elif defined( HAVE_CLOCK_GETTIME )
	if( clock_gettime(
	     CLOCK_REALTIME,
	     &time_structure ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current time structure.",
		 function );

		goto on_error;
	}
	timestamp = ( (int64_t) time_structure.tv_sec * 1000000000 ) + time_structure.tv_nsec;

	( *file_system )->mounted_timestamp = (uint64_t) timestamp;

#else
	timestamp = (int64_t) time( NULL );

	if( timestamp == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current time.",
		 function );

		goto on_error;
	}
	timestamp *= 1000000000;

	( *file_system )->mounted_timestamp = (uint64_t) timestamp;

#endif /* defined( HAVE_CLOCK_GETTIME ) */

	return( 1 );

on_error:
	if( *file_system != NULL )
	{
		memory_free(
		 *file_system );

		*file_system = NULL;
	}
	return( -1 );
}

/* Frees a file system
 * Returns 1 if successful or -1 on error
 */
int mount_file_system_free(
     mount_file_system_t **file_system,
     libcerror_error_t **error )
{
	static char *function = "mount_file_system_free";
	int result            = 1;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( *file_system != NULL )
	{
		if( ( *file_system )->path_prefix != NULL )
		{
			memory_free(
			 ( *file_system )->path_prefix );
		}
		if( libcdata_array_free(
		     &( ( *file_system )->handles_array ),
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free handles array.",
			 function );

			result = -1;
		}
		memory_free(
		 *file_system );

		*file_system = NULL;
	}
	return( result );
}

/* Signals the file system to abort
 * Returns 1 if successful or -1 on error
 */
int mount_file_system_signal_abort(
     mount_file_system_t *file_system,
     libcerror_error_t **error )
{
	libvmdk_handle_t *vmdk_handle = NULL;
	static char *function         = "mount_file_system_signal_abort";
	int handle_index              = 0;
	int number_of_handles         = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     file_system->handles_array,
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
		if( libcdata_array_get_entry_by_index(
		     file_system->handles_array,
		     handle_index,
		     (intptr_t **) &vmdk_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve handle: %d.",
			 function,
			 handle_index );

			return( -1 );
		}
		if( libvmdk_handle_signal_abort(
		     vmdk_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal handle: %d to abort.",
			 function,
			 handle_index );

			return( -1 );
		}
	}
	return( 1 );
}

/* Sets the path prefix
 * Returns 1 if successful or -1 on error
 */
int mount_file_system_set_path_prefix(
     mount_file_system_t *file_system,
     const system_character_t *path_prefix,
     size_t path_prefix_size,
     libcerror_error_t **error )
{
	static char *function = "mount_file_system_set_path_prefix";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( file_system->path_prefix != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system - path prefix value already set.",
		 function );

		return( -1 );
	}
	if( path_prefix == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path prefix.",
		 function );

		return( -1 );
	}
	if( path_prefix_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing path prefix.",
		 function );

		goto on_error;
	}
	if( path_prefix_size > (size_t) ( SSIZE_MAX / sizeof( system_character_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid path prefix size value exceeds maximum.",
		 function );

		goto on_error;
	}
	file_system->path_prefix = system_string_allocate(
	                            path_prefix_size );

	if( file_system->path_prefix == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create path prefix string.",
		 function );

		goto on_error;
	}
	if( system_string_copy(
	     file_system->path_prefix,
	     path_prefix,
	     path_prefix_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy path prefix.",
		 function );

		goto on_error;
	}
	file_system->path_prefix[ path_prefix_size - 1 ] = 0;

	file_system->path_prefix_size = path_prefix_size;

	return( 1 );

on_error:
	if( file_system->path_prefix != NULL )
	{
		memory_free(
		 file_system->path_prefix );

		file_system->path_prefix = NULL;
	}
	file_system->path_prefix_size = 0;

	return( -1 );
}

/* Retrieves the mounted timestamp
 * On Windows the timestamp is an unsigned 64-bit FILETIME timestamp
 * otherwise the timestamp is a signed 64-bit POSIX date and time value in number of nanoseconds
 * Returns 1 if successful or -1 on error
 */
int mount_file_system_get_mounted_timestamp(
     mount_file_system_t *file_system,
     uint64_t *mounted_timestamp,
     libcerror_error_t **error )
{
	static char *function = "mount_file_system_get_mounted_timestamp";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( mounted_timestamp == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mounted timestamp.",
		 function );

		return( -1 );
	}
	*mounted_timestamp = file_system->mounted_timestamp;

	return( 1 );
}

/* Retrieves the number of handles
 * Returns 1 if successful or -1 on error
 */
int mount_file_system_get_number_of_handles(
     mount_file_system_t *file_system,
     int *number_of_handles,
     libcerror_error_t **error )
{
	static char *function = "mount_file_system_get_number_of_handles";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     file_system->handles_array,
	     number_of_handles,
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
	return( 1 );
}

/* Retrieves a specific handle
 * Returns 1 if successful or -1 on error
 */
int mount_file_system_get_handle_by_index(
     mount_file_system_t *file_system,
     int handle_index,
     libvmdk_handle_t **vmdk_handle,
     libcerror_error_t **error )
{
	static char *function = "mount_file_system_get_handle_by_index";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     file_system->handles_array,
	     handle_index,
	     (intptr_t **) vmdk_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve handle: %d.",
		 function,
		 handle_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the handle for a specific path
 * Returns 1 if successful, 0 if no such handle or -1 on error
 */
int mount_file_system_get_handle_by_path(
     mount_file_system_t *file_system,
     const system_character_t *path,
     size_t path_length,
     libvmdk_handle_t **vmdk_handle,
     libcerror_error_t **error )
{
	static char *function        = "mount_file_system_get_handle_by_path";
	system_character_t character = 0;
	size_t path_index            = 0;
	int handle_index             = 0;
	int result                   = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( file_system->path_prefix == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file system - missing path prefix.",
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
	if( path_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid path length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( vmdk_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	path_length = system_string_length(
	               path );

	if( ( path_length == 1 )
	 && ( path[ 0 ] == file_system->path_prefix[ 0 ] ) )
	{
		*vmdk_handle = NULL;

		return( 1 );
	}
	if( ( path_length < file_system->path_prefix_size )
	 || ( path_length > ( file_system->path_prefix_size + 3 ) ) )
	{
		return( 0 );
	}
#if defined( WINAPI )
	result = system_string_compare_no_case(
	          path,
	          file_system->path_prefix,
	          file_system->path_prefix_size - 1 );
#else
	result = system_string_compare(
	          path,
	          file_system->path_prefix,
	          file_system->path_prefix_size - 1 );
#endif
	if( result != 0 )
	{
		return( 0 );
	}
	handle_index = 0;

	path_index = file_system->path_prefix_size - 1;

	while( path_index < path_length )
	{
		character = path[ path_index++ ];

		if( ( character < (system_character_t) '0' )
		 || ( character > (system_character_t) '9' ) )
		{
			return( 0 );
		}
		handle_index *= 10;
		handle_index += character - (system_character_t) '0';
	}
	handle_index -= 1;

	if( libcdata_array_get_entry_by_index(
	     file_system->handles_array,
	     handle_index,
	     (intptr_t **) vmdk_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve handle: %d.",
		 function,
		 handle_index );

		return( -1 );
	}
	return( 1 );
}

/* Appends a handle to the file system
 * Returns 1 if successful or -1 on error
 */
int mount_file_system_append_handle(
     mount_file_system_t *file_system,
     libvmdk_handle_t *vmdk_handle,
     libcerror_error_t **error )
{
	static char *function = "mount_file_system_append_handle";
	int entry_index       = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( libcdata_array_append_entry(
	     file_system->handles_array,
	     &entry_index,
	     (intptr_t *) vmdk_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append handle to array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the path from a handle index.
 * Returns 1 if successful or -1 on error
 */
int mount_file_system_get_path_from_handle_index(
     mount_file_system_t *file_system,
     int handle_index,
     system_character_t *path,
     size_t path_size,
     libcerror_error_t **error )
{
	static char *function     = "mount_file_system_get_path_from_handle_index";
	size_t path_index         = 0;
	size_t required_path_size = 0;
	int handle_number         = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( file_system->path_prefix == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file system - missing path prefix.",
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
	if( path_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid path length value exceeds maximum.",
		 function );

		return( -1 );
	}
        required_path_size = file_system->path_prefix_size;

	handle_number = handle_index + 1;

	while( handle_number > 0 )
	{
		required_path_size++;

		handle_number /= 10;
	}
	if( path_size <= required_path_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid path size value too small.",
		 function );

		return( -1 );
	}
	if( system_string_copy(
	     path,
	     file_system->path_prefix,
	     file_system->path_prefix_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy path prefix.",
		 function );

		return( -1 );
	}
	path_index = required_path_size - 1;

	handle_number = handle_index + 1;

	path[ path_index-- ] = 0;

	while( handle_number > 0 )
	{
		path[ path_index-- ] = (system_character_t) '0' + ( handle_number % 10 );

		handle_number /= 10;
	}
	return( 1 );
}

