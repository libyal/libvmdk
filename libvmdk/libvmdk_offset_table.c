/*
 * Offset table functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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
#include <endian.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>
#include <libnotify.h>

#include "libvmdk_definitions.h"
#include "libvmdk_grain_offset.h"
#include "libvmdk_libbfio.h"
#include "libvmdk_offset_table.h"

/* Initialize the offset table
 * Returns 1 if successful or -1 on error
 */
int libvmdk_offset_table_initialize(
     libvmdk_offset_table_t **offset_table,
     uint32_t amount_of_grain_offsets,
     liberror_error_t **error )
{
	static char *function    = "libvmdk_offset_table_initialize";
	size_t grain_offset_size = 0;

	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
		 function );

		return( -1 );
	}
	if( *offset_table == NULL )
	{
		grain_offset_size = sizeof( libvmdk_grain_offset_t ) * amount_of_grain_offsets;

		if( grain_offset_size > (size_t) SSIZE_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid grain offset size value exceeds maximum.",
			 function );

			return( -1 );
		}
		*offset_table = (libvmdk_offset_table_t *) memory_allocate(
		                                           sizeof( libvmdk_offset_table_t ) );

		if( *offset_table == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create offset table.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *offset_table,
		     0,
		     sizeof( libvmdk_offset_table_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear offset table.",
			 function );

			memory_free(
			 *offset_table );

			*offset_table = NULL;

			return( -1 );
		}
		if( amount_of_grain_offsets > 0 )
		{
			( *offset_table )->grain_offset = (libvmdk_grain_offset_t *) memory_allocate(
			                                                              grain_offset_size );

			if( ( *offset_table )->grain_offset == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create grain offsets.",
				 function );

				memory_free(
				 *offset_table );

				*offset_table = NULL;

				return( -1 );
			}
			if( memory_set(
			     ( *offset_table )->grain_offset,
			     0,
			     grain_offset_size ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear grain offsets.",
				 function );

				memory_free(
				 ( *offset_table )->grain_offset );
				memory_free(
				 *offset_table );

				*offset_table = NULL;

				return( -1 );
			}
		}
		( *offset_table )->amount_of_grain_offsets = amount_of_grain_offsets;
	}
	return( 1 );
}

/* Frees the offset table including elements
 * Returns 1 if successful or -1 on error
 */
int libvmdk_offset_table_free(
     libvmdk_offset_table_t **offset_table,
     liberror_error_t **error )
{
	static char *function = "libvmdk_offset_table_free";

	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
		 function );

		return( -1 );
	}
	if( *offset_table != NULL )
	{
		/* The segment file references are freed in the segment table
		 */
		if( ( *offset_table )->grain_offset != NULL )
		{
			memory_free(
			 ( *offset_table )->grain_offset );
		}
		memory_free(
		 *offset_table );

		*offset_table = NULL;
	}
	return( 1 );
}

/* Resizes the offset table
 * Returns 1 if successful or -1 on error
 */
int libvmdk_offset_table_resize(
     libvmdk_offset_table_t *offset_table,
     uint32_t amount_of_grain_offsets,
     liberror_error_t **error )
{
	void *reallocation       = NULL;
	static char *function    = "libvmdk_offset_table_resize";
	size_t grain_offset_size = 0;

	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset_table.",
		 function );

		return( -1 );
	}
	if( offset_table->amount_of_grain_offsets < amount_of_grain_offsets )
	{
		grain_offset_size = sizeof( libvmdk_grain_offset_t ) * amount_of_grain_offsets;

		if( grain_offset_size > (size_t) SSIZE_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid grain offset size value exceeds maximum.",
			 function );

			return( -1 );
		}
		reallocation = memory_reallocate(
				offset_table->grain_offset,
				grain_offset_size );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize grain offsets.",
			 function );

			return( -1 );
		}
		offset_table->grain_offset = (libvmdk_grain_offset_t *) reallocation;

		if( memory_set(
		     &( offset_table->grain_offset[ offset_table->amount_of_grain_offsets ] ),
		     0,
		     ( sizeof( libvmdk_grain_offset_t ) * ( amount_of_grain_offsets - offset_table->amount_of_grain_offsets ) ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear grain offsets.",
			 function );

			return( -1 );
		}
		offset_table->amount_of_grain_offsets = amount_of_grain_offsets;
	}
	return( 1 );
}

/* Fills the offset table from the table offsets
 * Returns 1 if successful or -1 on error
 */
int libvmdk_offset_table_fill(
     libvmdk_offset_table_t *offset_table,
     uint8_t *grain_table,
     uint32_t amount_of_grain_table_entries,
     uint8_t tainted,
     liberror_error_t **error )
{
#if defined( HAVE_VERBOSE_OUTPUT )
	char *remarks                        = NULL;
#endif

	libvmdk_grain_offset_t *grain_offset = NULL;
	static char *function                = "libvmdk_offset_table_fill";
	uint32_t grain_size                  = 0;
	uint32_t current_offset              = 0;
	uint32_t iterator                    = 0;
	uint8_t corrupted                    = 0;
	uint8_t overflow                     = 0;

	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
		 function );

		return( -1 );
	}
	if( grain_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain table.",
		 function );

		return( -1 );
	}
	/* TODO check size of grain table */

	/* Allocate additional entries in the offset table if needed
	 * - a single reallocation saves processing time
	 */
	if( offset_table->amount_of_grain_offsets < ( offset_table->last_grain_offset_filled + amount_of_grain_table_entries ) )
	{
		if( libvmdk_offset_table_resize(
		     offset_table,
		     offset_table->last_grain_offset_filled + amount_of_grain_table_entries,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize offset table.",
			 function );

			return( -1 );
		}
	}
	for( iterator = 0;
	     iterator < amount_of_grain_table_entries;
	     iterator++ )
	{
		endian_little_convert_32bit(
		 current_offset,
		 grain_table );

		if( grain_size == 0 )
		{
			libnotify_verbose_printf(
			 "%s: invalid grain size value is zero.\n",
			 function );

			corrupted = 1;
		}
		if( grain_size > (uint32_t) INT32_MAX )
		{
			libnotify_verbose_printf(
			 "%s: invalid grain size value exceeds maximum.\n",
			 function );

			corrupted = 1;
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		if( corrupted != 0 )
		{
			remarks = " corrupted";
		}
		else if( tainted != 0 )
		{
			remarks = " tainted";
		}
		else
		{
			remarks = "";
		}
		libnotify_verbose_printf(
		 "%s: grain %" PRIu32 " read with: base %" PRIu64 ", offset %" PRIu32 " and size %" PRIu32 "%s.\n",
		 function,
		 offset_table->last_grain_offset_filled + 1,
		 current_offset,
		 grain_size,
		 remarks );
#endif

		grain_offset = &( offset_table->grain_offset[ offset_table->last_grain_offset_filled ] );

		grain_offset->file_offset = (off64_t) current_offset;
		grain_offset->size        = (size_t) grain_size;

		if( corrupted != 0 )
		{
			grain_offset->flags |= LIBVMDK_GRAIN_OFFSET_FLAGS_CORRUPTED;
		}
		else if( tainted != 0 )
		{
			grain_offset->flags |= LIBVMDK_GRAIN_OFFSET_FLAGS_TAINTED;
		}
		offset_table->last_grain_offset_filled++;

		/* This is to compensate for the crappy >2Gb segment file
		 * solution in EnCase 6.7
		 */
		if( ( overflow == 0 )
		 && ( ( current_offset + grain_size ) > (uint32_t) INT32_MAX ) )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libnotify_verbose_printf(
			 "%s: grain offset overflow at: %" PRIu32 ".\n",
			 function,
			 current_offset );
#endif

			overflow   = 1;
		}
		iterator++;
	}
	return( 1 );
}

/* Compares the offsets with the ones in the offset table and makes corrections if necessary
 * Returns 1 if successful or -1 on error
 */
int libvmdk_offset_table_compare(
     libvmdk_offset_table_t *offset_table,
     uint8_t *grain_table,
     uint32_t amount_of_grain_table_entries,
     uint8_t tainted,
     liberror_error_t **error )
{
#if defined( HAVE_VERBOSE_OUTPUT )
	char *remarks                        = NULL;
#endif

	libvmdk_grain_offset_t *grain_offset = NULL;
	static char *function                = "libvmdk_offset_table_compare";
	uint32_t grain_size                  = 0;
	uint32_t current_offset              = 0;
	uint32_t iterator                    = 0;
	uint8_t corrupted                    = 0;
	uint8_t mismatch                     = 0;
	uint8_t overflow                     = 0;

	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
		 function );

		return( -1 );
	}
	if( grain_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid grain table.",
		 function );

		return( -1 );
	}
	/* Allocate additional entries in the offset table if needed
	 * - a single reallocation saves processing time
	 */
	if( ( offset_table->amount_of_grain_offsets < ( offset_table->last_grain_offset_compared + amount_of_grain_table_entries ) )
	 && ( libvmdk_offset_table_resize(
	       offset_table,
	       offset_table->last_grain_offset_compared + amount_of_grain_table_entries,
	       error ) != 1 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize offset table.",
		 function );

		return( -1 );
	}
	for( iterator = 0;
	     iterator < amount_of_grain_table_entries;
	     iterator++ )
	{
		endian_little_convert_32bit(
		 current_offset,
		 grain_table );

		if( grain_size == 0 )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libnotify_verbose_printf(
			 "%s: invalid grain size - size is zero.\n",
			 function );
#endif

			corrupted = 1;
		}
		if( grain_size > (uint32_t) INT32_MAX )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libnotify_verbose_printf(
			 "%s: invalid grain size value exceeds maximum.\n",
			 function );
#endif

			corrupted = 1;
		}
		grain_offset = &( offset_table->grain_offset[ offset_table->last_grain_offset_compared ] );

		if( grain_offset->file_offset != (off64_t) current_offset )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libnotify_verbose_printf(
			 "%s: file offset mismatch for grain offset: %" PRIu32 ".\n",
			 function,
			 offset_table->last_grain_offset_compared );
#endif

			mismatch = 1;
		}
		else if( grain_offset->size != (size_t) grain_size )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libnotify_verbose_printf(
			 "%s: grain size mismatch for grain offset: %" PRIu32 ".\n",
			 function,
			 offset_table->last_grain_offset_compared );
#endif

			mismatch = 1;
		}
		else
		{
			mismatch = 0;
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		if( corrupted != 0 )
		{
			remarks = " corrupted";
		}
		else if( tainted != 0 )
		{
			remarks = " tainted";
		}
		else if( mismatch == 1 )
		{
			remarks = " corrected";
		}
		else
		{
			remarks = "";
		}
		libnotify_verbose_printf(
		 "%s: grain %" PRIu32 " read with offset %" PRIu32 " and size %" PRIu32 "%s.\n",
		 function,
		 offset_table->last_grain_offset_compared + 1,
		 current_offset,
		 grain_size,
		 remarks );
#endif

		if( ( corrupted == 0 )
		 && ( tainted == 0 )
		 && ( mismatch == 1 ) )
		{
			grain_offset->file_offset = (off64_t) current_offset;
			grain_offset->size        = (size_t) grain_size;
			grain_offset->flags       &= ~ ( LIBVMDK_GRAIN_OFFSET_FLAGS_TAINTED | LIBVMDK_GRAIN_OFFSET_FLAGS_CORRUPTED ) ;
		}
		offset_table->last_grain_offset_compared++;

		/* This is to compensate for the crappy >2Gb segment file
		 * solution in EnCase 6.7
		 */
		if( ( overflow == 0 )
		 && ( ( current_offset + grain_size ) > (uint32_t) INT32_MAX ) )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libnotify_verbose_printf(
			 "%s: grain offset overflow at: %" PRIu32 ".\n",
			 function,
			 current_offset );
#endif

			overflow   = 1;
		}
		iterator++;
	}
	return( 1 );
}

/* Seeks a certain grain offset within the offset table
 * Returns the grain segment file offset if the seek is successful or -1 on error
 */
off64_t libvmdk_offset_table_seek_grain_offset(
         libvmdk_offset_table_t *offset_table,
         uint32_t grain,
         liberror_error_t **error )
{
	static char *function = "libvmdk_segment_table_seek_grain_offset";

	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
		 function );

		return( -1 );
	}
	if( offset_table->grain_offset == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid offset table - missing grain offsets.",
		 function );

		return( -1 );
	}
	if( grain >= offset_table->amount_of_grain_offsets )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: grain: %" PRIu32 " out of range [0,%" PRIu32 "].",
		 function,
		 grain,
		 offset_table->amount_of_grain_offsets - 1 );

		return( -1 );
	}
	/* TODO */

	return( offset_table->grain_offset[ grain ].file_offset );
}

