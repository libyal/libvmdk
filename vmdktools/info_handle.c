/*
 * Info handle
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
#include <byte_stream.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "byte_size_string.h"
#include "info_handle.h"
#include "vmdktools_libcerror.h"
#include "vmdktools_libcnotify.h"
#include "vmdktools_libvmdk.h"

#define INFO_HANDLE_NOTIFY_STREAM		stdout

/* Creates an info handle
 * Make sure the value info_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int info_handle_initialize(
     info_handle_t **info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_initialize";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle value already set.",
		 function );

		return( -1 );
	}
	*info_handle = memory_allocate_structure(
	                info_handle_t );

	if( *info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create info handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *info_handle,
	     0,
	     sizeof( info_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear info handle.",
		 function );

		goto on_error;
	}
	if( libvmdk_handle_initialize(
	     &( ( *info_handle )->input_handle ),
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
	( *info_handle )->notify_stream = INFO_HANDLE_NOTIFY_STREAM;

	return( 1 );

on_error:
	if( *info_handle != NULL )
	{
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( -1 );
}

/* Frees an info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_free(
     info_handle_t **info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_free";
	int result            = 1;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		if( ( *info_handle )->input_handle != NULL )
		{
			if( libvmdk_handle_free(
			     &( ( *info_handle )->input_handle ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free input handle.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( result );
}

/* Signals the info handle to abort
 * Returns 1 if successful or -1 on error
 */
int info_handle_signal_abort(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_signal_abort";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle != NULL )
	{
		if( libvmdk_handle_signal_abort(
		     info_handle->input_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal input handle to abort.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Opens the info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_open_input(
     info_handle_t *info_handle,
     system_character_t * const * filenames,
     int number_of_filenames,
     libcerror_error_t **error )
{
	static char *function = "info_handle_open_input";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libvmdk_handle_open_wide(
	     info_handle->input_handle,
	     filenames[ 0 ],
	     LIBVMDK_OPEN_READ,
	     error ) != 1 )
#else
	if( libvmdk_handle_open(
	     info_handle->input_handle,
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

		return( -1 );
	}
	return( 1 );
}

/* Closes the info handle
 * Returns the 0 if succesful or -1 on error
 */
int info_handle_close(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_close";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( libvmdk_handle_close(
	     info_handle->input_handle,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close input handle.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Prints the file information to a stream
 * Returns 1 if successful or -1 on error
 */
int info_handle_file_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	system_character_t byte_size_string[ 16 ];

	libvmdk_extent_descriptor_t *extent_descriptor = NULL;
	system_character_t *filename                   = NULL;
	static char *function                          = "info_handle_file_fprint";
	size64_t extent_size                           = 0;
	size64_t media_size                            = 0;
	size_t filename_size                           = 0;
	off64_t extent_offset                          = 0;
	uint32_t content_identifier                    = 0;
	int disk_type                                  = 0;
	int extent_index                               = 0;
	int extent_type                                = 0;
	int number_of_extents                          = 0;
	int result                                     = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "VMware Virtual Disk (VMDK) information:\n" );

	if( libvmdk_handle_get_disk_type(
	     info_handle->input_handle,
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
	fprintf(
	 info_handle->notify_stream,
	 "\tDisk type:\t\t\t" );

	switch( disk_type )
	{
		case LIBVMDK_DISK_TYPE_FLAT_2GB_EXTENT:
			fprintf(
			 info_handle->notify_stream,
			 "2GB extent flat" );
			break;

		case LIBVMDK_DISK_TYPE_SPARSE_2GB_EXTENT:
			fprintf(
			 info_handle->notify_stream,
			 "2GB extent sparse" );
			break;

		case LIBVMDK_DISK_TYPE_CUSTOM:
			fprintf(
			 info_handle->notify_stream,
			 "Custom" );
			break;

		case LIBVMDK_DISK_TYPE_DEVICE:
			fprintf(
			 info_handle->notify_stream,
			 "Device" );
			break;

		case LIBVMDK_DISK_TYPE_DEVICE_PARITIONED:
			fprintf(
			 info_handle->notify_stream,
			 "Device paritioned" );
			break;

		case LIBVMDK_DISK_TYPE_MONOLITHIC_FLAT:
			fprintf(
			 info_handle->notify_stream,
			 "Monolithic flat" );
			break;

		case LIBVMDK_DISK_TYPE_MONOLITHIC_SPARSE:
			fprintf(
			 info_handle->notify_stream,
			 "Monolithic sparse" );
			break;

		case LIBVMDK_DISK_TYPE_STREAM_OPTIMIZED:
			fprintf(
			 info_handle->notify_stream,
			 "Stream optimized" );
			break;

		case LIBVMDK_DISK_TYPE_VMFS_FLAT:
			fprintf(
			 info_handle->notify_stream,
			 "VMFS flat" );
			break;

		case LIBVMDK_DISK_TYPE_VMFS_FLAT_PRE_ALLOCATED:
			fprintf(
			 info_handle->notify_stream,
			 "VMFS flat (pre-allocated)" );
			break;

		case LIBVMDK_DISK_TYPE_VMFS_FLAT_ZEROED:
			fprintf(
			 info_handle->notify_stream,
			 "VMFS flat (zeroed)" );
			break;

		case LIBVMDK_DISK_TYPE_VMFS_RAW:
			fprintf(
			 info_handle->notify_stream,
			 "VMFS RAW" );
			break;

		case LIBVMDK_DISK_TYPE_VMFS_RDM:
			fprintf(
			 info_handle->notify_stream,
			 "VMFS RDM" );
			break;

		case LIBVMDK_DISK_TYPE_VMFS_RDMP:
			fprintf(
			 info_handle->notify_stream,
			 "VMFS RDMP" );
			break;

		case LIBVMDK_DISK_TYPE_VMFS_SPARSE:
			fprintf(
			 info_handle->notify_stream,
			 "VMFS sparse" );
			break;

		case LIBVMDK_DISK_TYPE_VMFS_SPARSE_THIN:
			fprintf(
			 info_handle->notify_stream,
			 "VMFS sparse (thin)" );
			break;

		default:
			fprintf(
			 info_handle->notify_stream,
			 "Unknown" );
			break;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	if( libvmdk_handle_get_media_size(
	     info_handle->input_handle,
	     &media_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media size.",
		 function );

		goto on_error;
	}
	result = byte_size_string_create(
	          byte_size_string,
	          16,
	          media_size,
	          BYTE_SIZE_STRING_UNIT_MEBIBYTE,
	          NULL );

	if( result == 1 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tMedia size:\t\t\t%" PRIs_SYSTEM " (%" PRIu64 " bytes)\n",
		 byte_size_string,
		 media_size );
	}
	else
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tMedia size:\t\t\t%" PRIu64 " bytes\n",
		 media_size );
	}
	if( libvmdk_handle_get_content_identifier(
	     info_handle->input_handle,
	     &content_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve content identifier.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tContent identifier:\t\t0x%08" PRIx32 "\n",
	 content_identifier );

	result = libvmdk_handle_get_parent_content_identifier(
	          info_handle->input_handle,
	          &content_identifier,
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
	else if( result != 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tParent content identifier:\t0x%08" PRIx32 "\n",
		 content_identifier );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libvmdk_handle_get_utf16_parent_filename_size(
	          info_handle->input_handle,
	          &filename_size,
	          error );
#else
	result = libvmdk_handle_get_utf8_parent_filename_size(
	          info_handle->input_handle,
	          &filename_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve parent filename size.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( filename_size == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing parent filename.",
			 function );

			goto on_error;
		}
		if( ( filename_size > (size_t) SSIZE_MAX )
		 || ( ( sizeof( system_character_t ) * filename_size ) > (size_t) SSIZE_MAX ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid parent filename size value exceeds maximum.",
			 function );

			goto on_error;
		}
		filename = system_string_allocate(
		            filename_size );

		if( filename == NULL )
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
		          info_handle->input_handle,
		          (uint16_t *) filename,
		          filename_size,
		          error );
#else
		result = libvmdk_handle_get_utf8_parent_filename(
		          info_handle->input_handle,
		          (uint8_t *) filename,
		          filename_size,
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
		fprintf(
		 info_handle->notify_stream,
		 "\tParent filename:\t\t%" PRIs_SYSTEM "\n",
		 filename );

		memory_free(
		 filename );

		filename = NULL;
	}
	if( libvmdk_handle_get_number_of_extents(
	     info_handle->input_handle,
	     &number_of_extents,
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
	fprintf(
	 info_handle->notify_stream,
	 "\tNumber of extents:\t\t%d\n",
	 number_of_extents );

	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	for( extent_index = 0;
	     extent_index < number_of_extents;
	     extent_index++ )
	{
		fprintf(
		 info_handle->notify_stream,
		 "Extent: %d\n",
		 extent_index + 1 );

		if( libvmdk_handle_get_extent_descriptor(
		     info_handle->input_handle,
		     extent_index,
		     &extent_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve extent: %d descriptor.",
			 function,
			 extent_index );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libvmdk_extent_descriptor_get_utf16_filename_size(
		          extent_descriptor,
		          &filename_size,
		          error );
#else
		result = libvmdk_extent_descriptor_get_utf8_filename_size(
		          extent_descriptor,
		          &filename_size,
		          error );
#endif
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve extent: %d descriptor filename size.",
			 function,
			 extent_index );

			goto on_error;
		}
		else if( result != 0 )
		{
			if( filename_size == 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing filename.",
				 function );

				goto on_error;
			}
			if( ( filename_size > (size_t) SSIZE_MAX )
			 || ( ( sizeof( system_character_t ) * filename_size ) > (size_t) SSIZE_MAX ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
				 "%s: invalid filename size value exceeds maximum.",
				 function );

				goto on_error;
			}
			filename = system_string_allocate(
			            filename_size );

			if( filename == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create filename string.",
				 function );

				goto on_error;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libvmdk_extent_descriptor_get_utf16_filename(
			          extent_descriptor,
				  (uint16_t *) filename,
				  filename_size,
				  error );
#else
			result = libvmdk_extent_descriptor_get_utf8_filename(
			          extent_descriptor,
				  (uint8_t *) filename,
				  filename_size,
				  error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve extent: %d descriptor filename.",
				 function,
				 extent_index );

				goto on_error;
			}
			fprintf(
			 info_handle->notify_stream,
			 "\tFilename:\t\t\t%" PRIs_SYSTEM "\n",
			 filename );

			memory_free(
			 filename );

			filename = NULL;
		}
		if( libvmdk_extent_descriptor_get_type(
		     extent_descriptor,
		     &extent_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve extent: %d descriptor type.",
			 function,
			 extent_index );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tType:\t\t\t\t" );

		switch( extent_type )
		{
			case LIBVMDK_EXTENT_TYPE_FLAT:
				fprintf(
				 info_handle->notify_stream,
				 "Flat" );
				break;

			case LIBVMDK_EXTENT_TYPE_SPARSE:
				fprintf(
				 info_handle->notify_stream,
				 "Sparse" );
				break;

			case LIBVMDK_EXTENT_TYPE_VMFS_FLAT:
				fprintf(
				 info_handle->notify_stream,
				 "VMFS flat" );
				break;

        		case LIBVMDK_EXTENT_TYPE_VMFS_SPARSE:
				fprintf(
				 info_handle->notify_stream,
				 "VMFS sparse" );
				break;

        		case LIBVMDK_EXTENT_TYPE_VMFS_RAW:
				fprintf(
				 info_handle->notify_stream,
				 "VMFS RAW" );
				break;

        		case LIBVMDK_EXTENT_TYPE_VMFS_RDM:
				fprintf(
				 info_handle->notify_stream,
				 "VMFS RDM" );
				break;

        		case LIBVMDK_EXTENT_TYPE_ZERO:
				fprintf(
				 info_handle->notify_stream,
				 "Zero" );
				break;

			default:
				fprintf(
				 info_handle->notify_stream,
				 "Unknown" );
				break;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\n" );

		if( libvmdk_extent_descriptor_get_range(
		     extent_descriptor,
		     &extent_offset,
		     &extent_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve extent: %d descriptor range.",
			 function,
			 extent_index );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tStart offset:\t\t\t%" PRIi64 "\n",
		 extent_offset );

		result = byte_size_string_create(
		          byte_size_string,
		          16,
		          extent_size,
		          BYTE_SIZE_STRING_UNIT_MEBIBYTE,
		          NULL );

		if( result == 1 )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\tSize:\t\t\t\t%" PRIs_SYSTEM " (%" PRIu64 " bytes)\n",
			 byte_size_string,
			 extent_size );
		}
		else
		{
			fprintf(
			 info_handle->notify_stream,
			 "\tSize:\t\t\t\t%" PRIu64 " bytes\n",
			 extent_size );
		}
		if( libvmdk_extent_descriptor_free(
		     &extent_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free extent: %d descriptor.",
			 function,
			 extent_index );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\n" );
	}
	return( 1 );

on_error:
	if( extent_descriptor != NULL )
	{
		libvmdk_extent_descriptor_free(
		 &extent_descriptor,
		 NULL );
	}
	if( filename != NULL )
	{
		memory_free(
		 filename );
	}
	return( -1 );
}

