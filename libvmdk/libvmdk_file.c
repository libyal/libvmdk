/*
 * libvmdk file
 *
 * Copyright (c) 2008-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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
#include <narrow_string.h>
#include <memory.h>
#include <types.h>
#include <wide_string.h>

#include <liberror.h>
#include <libnotify.h>

#include "libvmdk_definitions.h"
#include "libvmdk_io_handle.h"
#include "libvmdk_file.h"
#include "libvmdk_libbfio.h"
#include "libvmdk_offset_table.h"

/* Initialize a file
 * Make sure the value file is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libvmdk_file_initialize(
     libvmdk_file_t **file,
     liberror_error_t **error )
{
	libvmdk_internal_file_t *internal_file = NULL;
	static char *function                  = "libvmdk_file_initialize";

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( *file == NULL )
	{
		internal_file = (libvmdk_internal_file_t *) memory_allocate(
		                                             sizeof( libvmdk_internal_file_t ) );

		if( internal_file == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create file.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     internal_file,
		     0,
		     sizeof( libvmdk_internal_file_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear file.",
			 function );

			memory_free(
			 internal_file );

			return( -1 );
		}
		if( libvmdk_offset_table_initialize(
		     &( internal_file->offset_table ),
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create offset table.",
			 function );

			memory_free(
			 internal_file );

			return( -1 );
		}
		if( libvmdk_io_handle_initialize(
		     &( internal_file->io_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize io handle.",
			 function );

			libvmdk_offset_table_free(
			 &( internal_file->offset_table ),
			 NULL );
			memory_free(
			 internal_file );

			return( -1 );
		}
		*file = (libvmdk_file_t *) internal_file;
	}
	return( 1 );
}

/* Frees an exisisting file
 * Returns 1 if successful or -1 on error
 */
int libvmdk_file_free(
     libvmdk_file_t **file,
     liberror_error_t **error )
{
	libvmdk_internal_file_t *internal_file = NULL;
	static char *function                  = "libvmdk_file_free";
	int result                             = 1;

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( *file != NULL )
	{
		internal_file = (libvmdk_internal_file_t *) *file;

		if( libvmdk_offset_table_free(
		     &( internal_file->offset_table ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free offset table.",
			 function );
		}
		if( ( internal_file->io_handle != NULL )
		 && ( libvmdk_io_handle_free(
		       &( internal_file->io_handle ),
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free io handle.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_file );

		*file = NULL;
	}
	return( result );
}

/* Signals the libvmdk file to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libvmdk_file_signal_abort(
     libvmdk_file_t *file,
     liberror_error_t **error )
{
	static char *function = "libvmdk_file_signal_abort";

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	( (libvmdk_internal_file_t *) file )->abort = 1;

	return( 1 );
}

/* Opens a VMware Virtual Disk file
 * Returns 1 if successful or -1 on error
 */
int libvmdk_file_open(
     libvmdk_file_t *file,
     const char *filename,
     int flags,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle       = NULL;
	libvmdk_internal_file_t *internal_file = NULL;
	static char *function                  = "libvmdk_file_open";

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( flags & LIBVMDK_FLAG_READ ) != LIBVMDK_FLAG_READ )
	 && ( ( flags & LIBVMDK_FLAG_WRITE ) != LIBVMDK_FLAG_WRITE ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.",
		 function );

		return( -1 );
	}
	if( ( flags & LIBVMDK_FLAG_WRITE ) == LIBVMDK_FLAG_WRITE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access to VMware Virtual Disk files currently not supported.",
		 function );

		return( -1 );
	}
	internal_file = (libvmdk_internal_file_t *) file;

	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize file io handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                liberror_error_set(
                 error,
                 LIBERROR_ERROR_DOMAIN_RUNTIME,
                 LIBERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file io handle.",
                 function );

                return( -1 );
	}
#endif
	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     narrow_string_length(
	      filename ) + 1,
	     error ) != 1 )
	{
                liberror_error_set(
                 error,
                 LIBERROR_ERROR_DOMAIN_RUNTIME,
                 LIBERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file io handle.",
                 function );

                return( -1 );
	}
	if( libvmdk_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file: %s.",
		 function,
		 filename );

		return( -1 );
	}
	internal_file->io_handle->handle_created_in_library = 1;

	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a VMware Virtual Disk file
 * Returns 1 if successful or -1 on error
 */
int libvmdk_file_open_wide(
     libvmdk_file_t *file,
     const wchar_t *filename,
     int flags,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle       = NULL;
	libvmdk_internal_file_t *internal_file = NULL;
	static char *function                  = "libvmdk_file_open_wide";

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( flags & LIBVMDK_FLAG_READ ) != LIBVMDK_FLAG_READ )
	 && ( ( flags & LIBVMDK_FLAG_WRITE ) != LIBVMDK_FLAG_WRITE ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.",
		 function );

		return( -1 );
	}
	if( ( flags & LIBVMDK_FLAG_WRITE ) == LIBVMDK_FLAG_WRITE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access to VMware Virtual Disk files currently not supported.",
		 function );

		return( -1 );
	}
	internal_file = (libvmdk_internal_file_t *) file;

	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize file io handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                liberror_error_set(
                 error,
                 LIBERROR_ERROR_DOMAIN_RUNTIME,
                 LIBERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file io handle.",
                 function );

                return( -1 );
	}
#endif
	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     wide_string_length(
	      filename ) + 1,
	     error ) != 1 )
	{
                liberror_error_set(
                 error,
                 LIBERROR_ERROR_DOMAIN_RUNTIME,
                 LIBERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file io handle.",
                 function );

                return( -1 );
	}
	if( libvmdk_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file: %ls.",
		 function,
		 filename );

		return( -1 );
	}
	internal_file->io_handle->handle_created_in_library = 1;

	return( 1 );
}

#endif

/* Opens a VMware Virtual Disk file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libvmdk_file_open_file_io_handle(
     libvmdk_file_t *file,
     libbfio_handle_t *file_io_handle,
     int flags,
     liberror_error_t **error )
{
	libvmdk_internal_file_t *internal_file = NULL;
	static char *function                  = "libvmdk_file_open_file_io_handle";
	int file_io_flags                      = 0;

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io handle.",
		 function );

		return( -1 );
	}
	if( ( ( flags & LIBVMDK_FLAG_READ ) != LIBVMDK_FLAG_READ )
	 && ( ( flags & LIBVMDK_FLAG_WRITE ) != LIBVMDK_FLAG_WRITE ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.",
		 function );

		return( -1 );
	}
	if( ( flags & LIBVMDK_FLAG_WRITE ) == LIBVMDK_FLAG_WRITE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access to personal folder files currently not supported.",
		 function );

		return( -1 );
	}
	internal_file = (libvmdk_internal_file_t *) file;

	if( ( flags & LIBVMDK_FLAG_READ ) == LIBVMDK_FLAG_READ )
	{
		file_io_flags = LIBBFIO_FLAG_READ;
	}
	if( libvmdk_io_handle_open(
	     internal_file->io_handle,
	     file_io_handle,
	     file_io_flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file handle.",
		 function );

		return( -1 );
	}
	if( libvmdk_file_open_read(
	     internal_file,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from file handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Closes a VMware Virtual Disk file
 * Returns 0 if successful or -1 on error
 */
int libvmdk_file_close(
     libvmdk_file_t *file,
     liberror_error_t **error )
{
	libvmdk_internal_file_t *internal_file = NULL;
	static char *function                  = "libvmdk_file_close";
	int result                             = 0;

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libvmdk_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing io handle.",
		 function );

		return( -1 );
	}
	result = libvmdk_io_handle_close(
	          internal_file->io_handle,
	          error );

	if( result != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close io handle.",
		 function );
	}
	return( result );
}

/* Opens a VMware Virtual Disk file for reading
 * Returns 1 if successful or -1 on error
 */
int libvmdk_file_open_read(
     libvmdk_internal_file_t *internal_file,
     liberror_error_t **error )
{
	static char *function                      = "libvmdk_file_open_read";
	off64_t descriptor_offset                  = 0;
	off64_t grain_directory_offset             = 0;
	off64_t secondary_grain_directory_offset   = 0;
	size64_t descriptor_size                   = 0;
	size64_t grain_size                        = 0;
	uint32_t amount_of_grain_directory_entries = 0;
	uint32_t amount_of_grain_table_entries     = 0;
	uint32_t version                           = 0;

	if( internal_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal file.",
		 function );

		return( -1 );
	}
	if( internal_file->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal file - missing io handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	libnotify_verbose_printf(
	 "Reading file header:\n" );
#endif

	if( libvmdk_io_handle_read_file_header(
	     internal_file->io_handle,
	     &version,
	     &descriptor_offset,
	     &descriptor_size,
	     &grain_directory_offset,
	     &secondary_grain_directory_offset,
	     &amount_of_grain_directory_entries,
	     &amount_of_grain_table_entries,
	     &grain_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header.",
		 function );

		return( -1 );
	}
	if( descriptor_offset > 0 )
	{
		if( descriptor_size == 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: invalid descriptor size value is out of range.",
			 function );

			return( -1 );
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		libnotify_verbose_printf(
		 "Reading descriptor:\n" );
#endif

		/* TODO read descriptor */
	}
	if( ( grain_directory_offset == 0 )
	 && ( secondary_grain_directory_offset == 0 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid primary and secondary grain directory offset value out of range.",
		 function );

		return( -1 );
	}
	if( grain_directory_offset > 0 )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		libnotify_verbose_printf(
		 "Reading (primary) grain directory:\n" );
#endif

		if( libvmdk_io_handle_read_grain_directory(
		     internal_file->io_handle,
		     internal_file->offset_table,
		     grain_directory_offset,
		     amount_of_grain_directory_entries,
		     amount_of_grain_table_entries,
		     grain_size,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read (primary) grain directory.",
			 function );

			return( -1 );
		}
	}
	if( secondary_grain_directory_offset > 0 )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		libnotify_verbose_printf(
		 "Reading secondary grain directory:\n" );
#endif

		if( libvmdk_io_handle_read_grain_directory(
		     internal_file->io_handle,
		     internal_file->offset_table,
		     secondary_grain_directory_offset,
		     amount_of_grain_directory_entries,
		     amount_of_grain_table_entries,
		     grain_size,
		     1,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read secondary grain directory.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

