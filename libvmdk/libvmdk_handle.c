/*
 * Handle functions
 *
 * Copyright (c) 2009-2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <libcstring.h>
#include <liberror.h>
#include <libnotify.h>

#include "libvmdk_debug.h"
#include "libvmdk_definitions.h"
#include "libvmdk_handle.h"
#include "libvmdk_io_handle.h"
#include "libvmdk_libbfio.h"
#include "libvmdk_offset_table.h"

/* Initialize a handle
 * Make sure the value handle is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_initialize(
     libvmdk_handle_t **handle,
     liberror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_initialize";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( *handle == NULL )
	{
		internal_handle = (libvmdk_internal_handle_t *) memory_allocate(
		                                                 sizeof( libvmdk_internal_handle_t ) );

		if( internal_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create handle.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     internal_handle,
		     0,
		     sizeof( libvmdk_internal_handle_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear handle.",
			 function );

			memory_free(
			 internal_handle );

			return( -1 );
		}
		if( libvmdk_offset_table_initialize(
		     &( internal_handle->offset_table ),
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
			 internal_handle );

			return( -1 );
		}
		if( libvmdk_io_handle_initialize(
		     &( internal_handle->io_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create io handle.",
			 function );

			libvmdk_offset_table_free(
			 &( internal_handle->offset_table ),
			 NULL );
			memory_free(
			 internal_handle );

			return( -1 );
		}
		*handle = (libvmdk_handle_t *) internal_handle;
	}
	return( 1 );
}

/* Frees an exisisting handle
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_free(
     libvmdk_handle_t **handle,
     liberror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_free";
	int result                                 = 1;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( *handle != NULL )
	{
		internal_handle = (libvmdk_internal_handle_t *) *handle;
		*handle         = NULL;

		if( libvmdk_offset_table_free(
		     &( internal_handle->offset_table ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free offset table.",
			 function );

			result = -1;
		}
		if( internal_handle->io_handle != NULL )
		{
			if( libvmdk_io_handle_free(
			     &( internal_handle->io_handle ),
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free io handle.",
				 function );

				result = -1;
			}
		}
		if( internal_handle->file_io_pool_created_in_library != 0 )
		{
			if( internal_handle->file_io_pool != NULL )
			{
				if( libbfio_pool_free(
				     &( internal_handle->file_io_pool ),
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free file io pool.",
					 function );

					result = -1;
				}
			}
		}
		memory_free(
		 internal_handle );
	}
	return( result );
}

/* Signals the libvmdk handle to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_signal_abort(
     libvmdk_handle_t *handle,
     liberror_error_t **error )
{
	static char *function = "libvmdk_handle_signal_abort";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	( (libvmdk_internal_handle_t *) handle )->abort = 1;

	return( 1 );
}

/* Opens a VMware Virtual Disk file(s)
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_open(
     libvmdk_handle_t *handle,
     char * const filenames[],
     int amount_of_filenames,
     int flags,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle           = NULL;
	libbfio_pool_t *file_io_pool               = NULL;
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_open";
	size_t filename_length                     = 0;
	int file_io_pool_entry                     = 0;
	int filename_iterator                      = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libvmdk_internal_handle_t *) handle;

	if( filenames == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( amount_of_filenames <= 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid amount of files zero or less.",
		 function );

		return( -1 );
	}
	if( ( flags & LIBVMDK_FLAG_WRITE ) == LIBVMDK_FLAG_WRITE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access to VMware Virtual Disk file(s) currently not supported.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_initialize(
	     &file_io_pool,
	     0,
	     internal_handle->maximum_amount_of_open_handles,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file io pool.",
		 function );

		return( -1 );
	}
	if( ( flags & LIBEWF_FLAG_READ ) == LIBEWF_FLAG_READ )
	{
		for( filename_iterator = 0;
		     filename_iterator < amount_of_filenames;
		     filename_iterator++ )
		{
			filename_length = libcstring_narrow_string_length(
					   filenames[ filename_iterator ] );

			/* Make sure there is more to the filename than the extension
			 */
			if( filename_length <= 4 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: filename: %s is too small.",
				 function,
				 filenames[ filename_iterator ] );

				libbfio_pool_free(
				 &file_io_pool,
				 NULL );

				return( -1 );
			}
			if( libbfio_file_initialize(
			     &file_io_handle,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create file io handle.",
				 function );

				libbfio_pool_free(
				 &file_io_pool,
				 NULL );

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

				libbfio_handle_free(
				 &file_io_handle,
				 NULL );
				libbfio_pool_free(
				 &file_io_pool,
				 NULL );

		                return( -1 );
			}
#endif
			if( libbfio_file_set_name(
			     file_io_handle,
			     filenames[ filename_iterator ],
			     filename_length + 1,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set name in file io handle.",
				 function );

				libbfio_handle_free(
				 &file_io_handle,
				 NULL );
				libbfio_pool_free(
				 &file_io_pool,
				 NULL );

				return( -1 );
			}
			if( libbfio_pool_add_handle(
			     file_io_pool,
			     &file_io_pool_entry,
			     file_io_handle,
			     LIBBFIO_OPEN_READ,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to add file io handle to pool.",
				 function );

				libbfio_handle_free(
				 &file_io_handle,
				 NULL );
				libbfio_pool_free(
				 &file_io_pool,
				 NULL );

				return( -1 );
			}
			file_io_handle = NULL;

#if defined( HAVE_VERBOSE_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: added pool entry: %d with filename: %s.\n",
				 function,
				 file_io_pool_entry,
				 filenames[ filename_iterator ] );
			}
#endif
	}
	if( libvmdk_handle_open_file_io_pool(
	     handle,
	     file_io_pool,
	     flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle using a file io pool.",
		 function );

		libbfio_pool_free(
		 &file_io_pool,
		 NULL );

		return( -1 );
	}
	internal_handle->file_io_pool_created_in_library = 1;

	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a VMware Virtual Disk file(s)
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_open_wide(
     libvmdk_handle_t *file,
     const wchar_t *filename,
     int flags,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle           = NULL;
	libbfio_pool_t *file_io_pool               = NULL;
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_open_wide";
	size_t filename_length                     = 0;
	int file_io_pool_entry                     = 0;
	int filename_iterator                      = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libvmdk_internal_handle_t *) handle;

	if( filenames == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( amount_of_filenames <= 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid amount of files zero or less.",
		 function );

		return( -1 );
	}
	if( ( flags & LIBVMDK_FLAG_WRITE ) == LIBVMDK_FLAG_WRITE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access to VMware Virtual Disk file(s) currently not supported.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_initialize(
	     &file_io_pool,
	     0,
	     internal_handle->maximum_amount_of_open_handles,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file io pool.",
		 function );

		return( -1 );
	}
	if( ( flags & LIBEWF_FLAG_READ ) == LIBEWF_FLAG_READ )
	{
		for( filename_iterator = 0;
		     filename_iterator < amount_of_filenames;
		     filename_iterator++ )
		{
			filename_length = libcstring_wide_string_length(
					   filenames[ filename_iterator ] );

			/* Make sure there is more to the filename than the extension
			 */
			if( filename_length <= 4 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: filename: %ls is too small.",
				 function,
				 filenames[ filename_iterator ] );

				libbfio_pool_free(
				 &file_io_pool,
				 NULL );

				return( -1 );
			}
			if( libbfio_file_initialize(
			     &file_io_handle,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create file io handle.",
				 function );

				libbfio_pool_free(
				 &file_io_pool,
				 NULL );

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

				libbfio_handle_free(
				 &file_io_handle,
				 NULL );
				libbfio_pool_free(
				 &file_io_pool,
				 NULL );

		                return( -1 );
			}
#endif
			if( libbfio_file_set_name_wide(
			     file_io_handle,
			     filenames[ filename_iterator ],
			     filename_length + 1,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set name in file io handle.",
				 function );

				libbfio_handle_free(
				 &file_io_handle,
				 NULL );
				libbfio_pool_free(
				 &file_io_pool,
				 NULL );

				return( -1 );
			}
			if( libbfio_pool_add_handle(
			     file_io_pool,
			     &file_io_pool_entry,
			     file_io_handle,
			     LIBBFIO_OPEN_READ,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to add file io handle to pool.",
				 function );

				libbfio_handle_free(
				 &file_io_handle,
				 NULL );
				libbfio_pool_free(
				 &file_io_pool,
				 NULL );

				return( -1 );
			}
			file_io_handle = NULL;

#if defined( HAVE_VERBOSE_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: added pool entry: %d with filename: %s.\n",
				 function,
				 file_io_pool_entry,
				 filenames[ filename_iterator ] );
			}
#endif
	}
	if( libvmdk_handle_open_file_io_pool(
	     handle,
	     file_io_pool,
	     flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle using a file io pool.",
		 function );

		libbfio_pool_free(
		 &file_io_pool,
		 NULL );

		return( -1 );
	}
	internal_handle->file_io_pool_created_in_library = 1;

	return( 1 );
}

#endif

/* Opens a VMware Virtual Disk file(s) using a Basic File IO (bfio) pool
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_open_file_io_pool(
     libvmdk_handle_t *file,
     libbfio_handle_t *file_io_pool,
     int flags,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle           = NULL;
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libvmdk_handle_open_file_io_pool";
	uint16_t segment_number                    = 0;
	int amount_of_handles                      = 0;
	int file_io_handle_iterator                = 0;
	int result                                 = 0;
	uint8_t segment_file_type                  = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libvmdk_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing io handle.",
		 function );

		return( -1 );
	}
	if( file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io pool.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_get_amount_of_handles(
	     file_io_pool,
	     &amount_of_handles,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the amount of handles in the file io pool.",
		 function );

		return( -1 );
	}
	if( ( ( flags & LIBEWF_FLAG_READ ) != LIBEWF_FLAG_READ )
	 && ( ( flags & LIBEWF_FLAG_WRITE ) != LIBEWF_FLAG_WRITE ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.",
		 function );

		return( -1 );
	}
#ifdef TODO
	if( ( flags & LIBEWF_FLAG_READ ) == LIBEWF_FLAG_READ )
	{
		/* TODO */
		if( libvmdk_read_io_handle_initialize(
		     &( internal_handle->read_io_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create read io handle.",
			 function );

			return( -1 );
		}
	}
	if( ( flags & LIBEWF_FLAG_WRITE ) == LIBEWF_FLAG_WRITE )
	{
		/* TODO */
		if( libvmdk_write_io_handle_initialize(
		     &( internal_handle->write_io_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create write io handle.",
			 function );

			return( -1 );
		}
	}
#endif
	internal_handle->io_handle->flags = flags;
	internal_handle->file_io_pool     = file_io_pool;

	if( ( flags & LIBEWF_FLAG_READ ) == LIBEWF_FLAG_READ )
	{
		for( file_io_handle_iterator = 0;
		     file_io_handle_iterator < amount_of_handles;
		     file_io_handle_iterator++ )
		{
			file_io_handle = NULL;

			if( libbfio_pool_get_handle(
			     file_io_pool,
			     file_io_handle_iterator,
			     &file_io_handle,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to retrieve file io handle from pool entry: %d.",
				 function,
				 file_io_handle_iterator );

				internal_handle->file_io_pool = NULL;

				return( -1 );
			}
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: processing pool entry: %d.\n",
				 function,
				 file_io_handle_iterator );
			}
#endif

			result = libvmdk_internal_handle_add_segment_file(
			          internal_handle,
			          file_io_handle_iterator,
			          flags,
			          &segment_number,
			          &segment_file_type,
			          error );

			if( result == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to add segment file.",
				 function );

				internal_handle->file_io_pool = NULL;

				return( -1 );
			}
			else if( result == 0 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: segment file: %" PRIu16 " already exists.",
				 function,
				 segment_number );

				return( -1 );
			}
			if( (int) segment_number > amount_of_handles )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_INPUT,
				 LIBERROR_INPUT_ERROR_INVALID_DATA,
				 "%s: invalid segment number: %" PRIu16 " - value out of range or missing segment files.",
				 function,
				 segment_number );

				internal_handle->file_io_pool = NULL;

				return( -1 );
			}
		}
		result = libvmdk_segment_table_build(
		          internal_handle->segment_table,
		          internal_handle->io_handle,
		          internal_handle->file_io_pool,
		          internal_handle->header_sections,
		          internal_handle->hash_sections,
		          internal_handle->media_values,
		          internal_handle->offset_table,
		          internal_handle->sessions,
		          internal_handle->acquiry_errors,
		          internal_handle->single_files,
		          &( internal_handle->abort ),
		          error );

		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to build segment table.",
			 function );

			internal_handle->file_io_pool = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Closes a VMware Virtual Disk file(s)
 * Returns 0 if successful or -1 on error
 */
int libvmdk_handle_close(
     libvmdk_handle_t *file,
     liberror_error_t **error )
{
	libvmdk_internal_handle_t *internal_handle = NULL;
	static char *function                    = "libvmdk_handle_close";
	int result                               = 0;

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
	internal_handle = (libvmdk_internal_handle_t *) file;

	if( internal_handle->file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->file_io_handle_created_in_library != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			if( libvmdk_debug_print_read_offsets(
			     internal_handle->file_io_handle,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print the read offsets.",
				 function );

				result = -1;
			}
		}
#endif
		if( libbfio_handle_close(
		     internal_handle->file_io_handle,
		     error ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file io handle.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Opens a VMware Virtual Disk file(s) for reading
 * Returns 1 if successful or -1 on error
 */
int libvmdk_handle_open_read(
     libvmdk_internal_handle_t *internal_handle,
     liberror_error_t **error )
{
	static char *function                      = "libvmdk_handle_open_read";
	off64_t descriptor_offset                  = 0;
	off64_t grain_directory_offset             = 0;
	off64_t secondary_grain_directory_offset   = 0;
	size64_t descriptor_size                   = 0;
	size64_t grain_size                        = 0;
	uint32_t amount_of_grain_directory_entries = 0;
	uint32_t amount_of_grain_table_entries     = 0;

	if( internal_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal file.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle == NULL )
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
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "Reading file header:\n" );
	}
#endif

	if( libvmdk_io_handle_read_file_header(
	     internal_handle->io_handle,
	     internal_handle->file_io_handle,
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
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "Reading descriptor:\n" );
		}
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
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "Reading (primary) grain directory:\n" );
		}
#endif

		if( libvmdk_io_handle_read_grain_directory(
		     internal_handle->io_handle,
		     internal_handle->file_io_handle,
		     internal_handle->offset_table,
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
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "Reading secondary grain directory:\n" );
		}
#endif

		if( libvmdk_io_handle_read_grain_directory(
		     internal_handle->io_handle,
		     internal_handle->file_io_handle,
		     internal_handle->offset_table,
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

