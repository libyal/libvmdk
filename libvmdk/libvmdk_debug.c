/*
 * Debug functions
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
#include <types.h>

#include "libvmdk_debug.h"
#include "libvmdk_definitions.h"
#include "libvmdk_libbfio.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libcnotify.h"

#if defined( HAVE_DEBUG_OUTPUT )

/* Prints the flags
 * Returns 1 if successful or -1 on error
 */
void libvmdk_debug_print_vmdk_flags(
      uint32_t flags )
{
	if( ( flags & LIBVMDK_FLAG_NEW_LINE_DETECION_VALID ) != 0 )
	{
		libcnotify_printf(
		 "\tNew line detection valid\n" );
	}
	if( ( flags & LIBVMDK_FLAG_USE_SECONDARY_GRAIN_DIRECTORY ) != 0 )
	{
		libcnotify_printf(
		 "\tUse secondary grain directory\n" );
	}

	if( ( flags & LIBVMDK_FLAG_HAS_GRAIN_COMPRESSION ) != 0 )
	{
		libcnotify_printf(
		 "\tHas grain compression\n" );
	}
	if( ( flags & LIBVMDK_FLAG_HAS_DATA_MARKERS ) != 0 )
	{
		libcnotify_printf(
		 "\tHas data markers\n" );
	}
	libcnotify_printf(
	 "\n" );
}

/* Returns a string with the compression method description
 */
const char *libvmdk_debug_get_compression_method_description(
             uint32_t compression_method )
{
	switch( compression_method )
	{
		case LIBVMDK_COMPRESSION_METHOD_NONE:
			return( "None" );

		case LIBVMDK_COMPRESSION_METHOD_DEFLATE:
			return( "Deflate" );

		default:
			break;
	}
	return( "UNKNOWN" );
}

/* Returns a string with the maker description
 */
const char *libvmdk_debug_get_marker_description(
             uint32_t marker )
{
	switch( marker )
	{
		case LIBVMDK_MARKER_END_OF_STREAM:
			return( "End of stream" );

		case LIBVMDK_MARKER_GRAIN_TABLE:
			return( "Grain table" );

		case LIBVMDK_MARKER_GRAIN_DIRECTORY:
			return( "Grain directory" );

		case LIBVMDK_MARKER_FOOTER:
			return( "Footer" );

		default:
			break;
	}
	return( "UNKNOWN" );
}

/* Prints the read offsets
 * Returns 1 if successful or -1 on error
 */
int libvmdk_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_debug_print_read_offsets";
	off64_t offset        = 0;
	size64_t size         = 0;
	int number_of_offsets = 0;
	int offset_iterator   = 0;

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libbfio_handle_get_number_of_offsets_read(
	     file_io_handle,
	     &number_of_offsets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of offsets read.",
		 function );

		return( -1 );
	}
	libcnotify_printf(
	 "Offsets read:\n" );

	for( offset_iterator = 0;
	     offset_iterator < number_of_offsets;
	     offset_iterator++ )
	{
		if( libbfio_handle_get_offset_read(
		     file_io_handle,
		     offset_iterator,
		     &offset,
		     &size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset: %d.",
			 function,
			 offset_iterator );

			return( -1 );
		}
		libcnotify_printf(
		 "%08" PRIu64 " ( 0x%08" PRIx64 " ) - %08" PRIu64 " ( 0x%08" PRIx64 " ) size: %" PRIu64 "\n",
		 offset,
		 offset,
		 offset + size,
		 offset + size,
		 size );
	}
	libcnotify_printf(
	 "\n" );

	return( 1 );
}

#endif

