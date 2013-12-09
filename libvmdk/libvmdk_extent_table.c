/*
 * Extent table functions
 *
 * Copyright (c) 2009-2013, Joachim Metz <joachim.metz@gmail.com>
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

#include "libvmdk_definitions.h"
#include "libvmdk_extent_descriptor.h"
#include "libvmdk_extent_file.h"
#include "libvmdk_extent_table.h"
#include "libvmdk_libbfio.h"
#include "libvmdk_libcdata.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libclocale.h"
#include "libvmdk_libcstring.h"
#include "libvmdk_libfdata.h"
#include "libvmdk_libuna.h"

/* Creates a extent table
 * Make sure the value extent_table is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_table_initialize(
     libvmdk_extent_table_t **extent_table,
     libvmdk_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_table_initialize";

	if( extent_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent table.",
		 function );

		return( -1 );
	}
	if( *extent_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extent table value already set.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	*extent_table = memory_allocate_structure(
	                 libvmdk_extent_table_t );

	if( *extent_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create extent table.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *extent_table,
	     0,
	     sizeof( libvmdk_extent_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear extent table.",
		 function );

		goto on_error;
	}
	( *extent_table )->io_handle = io_handle;

	return( 1 );

on_error:
	if( *extent_table != NULL )
	{
		memory_free(
		 *extent_table );

		*extent_table = NULL;
	}
	return( -1 );
}

/* Frees a extent table
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_table_free(
     libvmdk_extent_table_t **extent_table,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_table_free";
	int result            = 1;

	if( extent_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent table.",
		 function );

		return( -1 );
	}
	if( *extent_table != NULL )
	{
		if( libvmdk_extent_table_clear(
		     *extent_table,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to clear extent table.",
			 function );

			result = -1;
		}
		memory_free(
		 *extent_table );

		*extent_table = NULL;
	}
	return( result );
}

/* Clears an extent table
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_table_clear(
     libvmdk_extent_table_t *extent_table,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_table_clear";
	int result            = 1;

	if( extent_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent table.",
		 function );

		return( -1 );
	}
	if( extent_table->basename != NULL )
	{
		memory_free(
		 extent_table->basename );

		extent_table->basename      = NULL;
		extent_table->basename_size = 0;
	}
	if( extent_table->extent_files_list != NULL )
	{
		if( libfdata_list_free(
		     &( extent_table->extent_files_list ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free extent files list.",
			 function );

			result = -1;
		}
	}
	if( extent_table->extent_files_cache != NULL )
	{
		if( libfcache_cache_free(
		     &( extent_table->extent_files_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free extent files cache.",
			 function );

			result = -1;
		}
	}
	if( extent_table->extent_files_stream != NULL )
	{
		if( libfdata_stream_free(
		     &( extent_table->extent_files_stream ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free extent files stream.",
			 function );

			result = -1;
		}
	}
	if( memory_set(
	     extent_table,
	     0,
	     sizeof( libvmdk_extent_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to extent table.",
		 function );

		result = -1;
	}
	return( result );
}

/* Clones the extent table
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_table_clone(
     libvmdk_extent_table_t **destination_extent_table,
     libvmdk_extent_table_t *source_extent_table,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_table_clone";

	if( destination_extent_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination extent table.",
		 function );

		return( -1 );
	}
	if( *destination_extent_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination extent table value already set.",
		 function );

		return( -1 );
	}
	if( source_extent_table == NULL )
	{
		*destination_extent_table = NULL;

		return( 1 );
	}
	*destination_extent_table = memory_allocate_structure(
	                             libvmdk_extent_table_t );

	if( *destination_extent_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination extent table.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *destination_extent_table,
	     0,
	     sizeof( libvmdk_extent_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear source to destination extent table.",
		 function );

		memory_free(
		 *destination_extent_table );

		*destination_extent_table = NULL;

		return( -1 );
	}
	if( source_extent_table->basename != NULL )
	{
		( *destination_extent_table )->basename = libcstring_system_string_allocate(
					                    source_extent_table->basename_size );

		if( *destination_extent_table == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination extent table.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_extent_table )->basename,
		     source_extent_table->basename,
		     sizeof( libcstring_system_character_t ) * source_extent_table->basename_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination basename.",
			 function );

			goto on_error;
		}
		( *destination_extent_table )->basename_size = source_extent_table->basename_size;
	}
	if( libfdata_list_clone(
	     &( ( *destination_extent_table )->extent_files_list ),
	     source_extent_table->extent_files_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination extent files list.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_clone(
	     &( ( *destination_extent_table )->extent_files_cache ),
	     source_extent_table->extent_files_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination extent files cache.",
		 function );

		goto on_error;
	}
	if( libfdata_stream_clone(
	     &( ( *destination_extent_table )->extent_files_stream ),
	     source_extent_table->extent_files_stream,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination extent files stream.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_extent_table != NULL )
	{
		if( ( *destination_extent_table )->extent_files_cache != NULL )
		{
			libfcache_cache_free(
			 &( ( *destination_extent_table )->extent_files_cache ),
			 NULL );
		}
		if( ( *destination_extent_table )->extent_files_list != NULL )
		{
			libfdata_list_free(
			 &( ( *destination_extent_table )->extent_files_list ),
			 NULL );
		}
		if( ( *destination_extent_table )->basename != NULL )
		{
			memory_free(
			 ( *destination_extent_table )->basename );
		}
		memory_free(
		 *destination_extent_table );

		*destination_extent_table = NULL;
	}
	return( -1 );
}

/* Retrieves the size of the basename
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libvmdk_extent_table_get_basename_size(
     libvmdk_extent_table_t *extent_table,
     size_t *basename_size,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_table_get_basename_size";

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result            = 0;
#endif

	if( extent_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent table.",
		 function );

		return( -1 );
	}
	if( basename_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename size.",
		 function );

		return( -1 );
	}
	if( extent_table->basename == NULL )
	{
		return( 0 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_size_from_utf32(
		          (libuna_utf32_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_size_from_utf16(
		          (libuna_utf16_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_size_from_utf32(
		          (libuna_utf32_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          libclocale_codepage,
		          basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          libclocale_codepage,
		          basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine basename size.",
		 function );

		return( -1 );
	}
#else
	*basename_size = extent_table->basename_size;
#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

/* Retrieves the basename
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libvmdk_extent_table_get_basename(
     libvmdk_extent_table_t *extent_table,
     char *basename,
     size_t basename_size,
     libcerror_error_t **error )
{
	static char *function       = "libvmdk_extent_table_get_basename";
	size_t narrow_basename_size = 0;

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result                  = 0;
#endif

	if( extent_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent table.",
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
	if( extent_table->basename == NULL )
	{
		return( 0 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_size_from_utf32(
		          (libuna_utf32_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          &narrow_basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_size_from_utf16(
		          (libuna_utf16_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          &narrow_basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_size_from_utf32(
		          (libuna_utf32_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          libclocale_codepage,
		          &narrow_basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          libclocale_codepage,
		          &narrow_basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine narrow basename size.",
		 function );

		return( -1 );
	}
#else
	narrow_basename_size = extent_table->basename_size;
#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */

	if( basename_size < narrow_basename_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: basename too small.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_copy_from_utf32(
		          (libuna_utf8_character_t *) basename,
		          basename_size,
		          (libuna_utf32_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_copy_from_utf16(
		          (libuna_utf8_character_t *) basename,
		          basename_size,
		          (libuna_utf16_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_copy_from_utf32(
		          (uint8_t *) basename,
		          basename_size,
		          libclocale_codepage,
		          (libuna_utf32_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_copy_from_utf16(
		          (uint8_t *) basename,
		          basename_size,
		          libclocale_codepage,
		          (libuna_utf16_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set basename.",
		 function );

		return( -1 );
	}
#else
	if( libcstring_system_string_copy(
	     basename,
	     extent_table->basename,
	     extent_table->basename_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set basename.",
		 function );

		return( -1 );
	}
	basename[ extent_table->basename_size - 1 ] = 0;
#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

/* Sets the basename
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_table_set_basename(
     libvmdk_extent_table_t *extent_table,
     const char *basename,
     size_t basename_length,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_table_set_basename";

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result            = 0;
#endif

	if( extent_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent table.",
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
	if( extent_table->basename != NULL )
	{
		memory_free(
		 extent_table->basename );

		extent_table->basename      = NULL;
		extent_table->basename_size = 0;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_utf8(
		          (libuna_utf8_character_t *) basename,
		          basename_length + 1,
		          &( extent_table->basename_size ),
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_utf8(
		          (libuna_utf8_character_t *) basename,
		          basename_length + 1,
		          &( extent_table->basename_size ),
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_byte_stream(
		          (uint8_t *) basename,
		          basename_length + 1,
		          libclocale_codepage,
		          &( extent_table->basename_size ),
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_byte_stream(
		          (uint8_t *) basename,
		          basename_length + 1,
		          libclocale_codepage,
		          &( extent_table->basename_size ),
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine basename size.",
		 function );

		return( -1 );
	}
#else
	extent_table->basename_size = basename_length + 1;
#endif
	extent_table->basename = libcstring_system_string_allocate(
	                           extent_table->basename_size );

	if( extent_table->basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create basename.",
		 function );

		extent_table->basename_size = 0;

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_utf8(
		          (libuna_utf32_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          (libuna_utf8_character_t *) basename,
		          basename_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_utf8(
		          (libuna_utf16_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          (libuna_utf8_character_t *) basename,
		          basename_length + 1,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_byte_stream(
		          (libuna_utf32_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          (uint8_t *) basename,
		          basename_length + 1,
		          libclocale_codepage,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_byte_stream(
		          (libuna_utf16_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          (uint8_t *) basename,
		          basename_length + 1,
		          libclocale_codepage,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set basename.",
		 function );

		memory_free(
		 extent_table->basename );

		extent_table->basename      = NULL;
		extent_table->basename_size = 0;

		return( -1 );
	}
#else
	if( libcstring_system_string_copy(
	     extent_table->basename,
	     basename,
	     basename_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set basename.",
		 function );

		memory_free(
		 extent_table->basename );

		extent_table->basename      = NULL;
		extent_table->basename_size = 0;

		return( -1 );
	}
	extent_table->basename[ basename_length ] = 0;
#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )
/* Retrieves the size of the basename
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libvmdk_extent_table_get_basename_size_wide(
     libvmdk_extent_table_t *extent_table,
     size_t *basename_size,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_table_get_basename_size_wide";

#if !defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result            = 0;
#endif

	if( extent_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent table.",
		 function );

		return( -1 );
	}
	if( basename_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename size.",
		 function );

		return( -1 );
	}
	if( extent_table->basename == NULL )
	{
		return( 0 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	*basename_size = extent_table->basename_size;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_utf8(
		          (libuna_utf8_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_utf8(
		          (libuna_utf8_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_byte_stream(
		          (uint8_t *) extent_table->basename,
		          extent_table->basename_size,
		          libclocale_codepage,
		          basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_byte_stream(
		          (uint8_t *) extent_table->basename,
		          extent_table->basename_size,
		          libclocale_codepage,
		          basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine basename size.",
		 function );

		return( -1 );
	}
#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */
	return( 1 );
}

/* Retrieves the basename
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libvmdk_extent_table_get_basename_wide(
     libvmdk_extent_table_t *extent_table,
     wchar_t *basename,
     size_t basename_size,
     libcerror_error_t **error )
{
	static char *function     = "libvmdk_extent_table_get_basename_wide";
	size_t wide_basename_size = 0;

#if !defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result                = 0;
#endif

	if( extent_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent table.",
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
	if( extent_table->basename == NULL )
	{
		return( 0 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	wide_basename_size = extent_table->basename_size;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_utf8(
		          (libuna_utf8_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          &wide_basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_utf8(
		          (libuna_utf8_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          &wide_basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_byte_stream(
		          (uint8_t *) extent_table->basename,
		          extent_table->basename_size,
		          libclocale_codepage,
		          &wide_basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_byte_stream(
		          (uint8_t *) extent_table->basename,
		          extent_table->basename_size,
		          libclocale_codepage,
		          &wide_basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine basename size.",
		 function );

		return( -1 );
	}
#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */
	if( basename_size < wide_basename_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: basename too small.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcstring_system_string_copy(
	     basename,
	     extent_table->basename,
	     extent_table->basename_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set basename.",
		 function );

		return( -1 );
	}
	basename[ extent_table->basename_size - 1 ] = 0;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_utf8(
		          (libuna_utf32_character_t *) basename,
		          basename_size,
		          (libuna_utf8_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_utf8(
		          (libuna_utf16_character_t *) basename,
		          basename_size,
		          (libuna_utf8_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_byte_stream(
		          (libuna_utf32_character_t *) basename,
		          basename_size,
		          (uint8_t *) extent_table->basename,
		          extent_table->basename_size,
		          libclocale_codepage,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_byte_stream(
		          (libuna_utf16_character_t *) basename,
		          basename_size,
		          (uint8_t *) extent_table->basename,
		          extent_table->basename_size,
		          libclocale_codepage,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set basename.",
		 function );

		return( -1 );
	}
#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */
	return( 1 );
}

/* Sets the basename
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_table_set_basename_wide(
     libvmdk_extent_table_t *extent_table,
     const wchar_t *basename,
     size_t basename_length,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_table_set_basename_wide";

#if !defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result            = 0;
#endif

	if( extent_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent table.",
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
	if( extent_table->basename != NULL )
	{
		memory_free(
		 extent_table->basename );

		extent_table->basename      = NULL;
		extent_table->basename_size = 0;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	extent_table->basename_size = basename_length + 1;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_size_from_utf32(
		          (libuna_utf32_character_t *) basename,
		          basename_length + 1,
		          &( extent_table->basename_size ),
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_size_from_utf16(
		          (libuna_utf16_character_t *) basename,
		          basename_length + 1,
		          &( extent_table->basename_size ),
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_size_from_utf32(
		          (libuna_utf32_character_t *) basename,
		          basename_length + 1,
		          libclocale_codepage,
		          &( extent_table->basename_size ),
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) basename,
		          basename_length + 1,
		          libclocale_codepage,
		          &( extent_table->basename_size ),
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine basename size.",
		 function );

		return( -1 );
	}
#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */
	extent_table->basename = libcstring_system_string_allocate(
	                           extent_table->basename_size );

	if( extent_table->basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create basename.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcstring_system_string_copy(
	     extent_table->basename,
	     basename,
	     basename_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set basename.",
		 function );

		memory_free(
		 extent_table->basename );

		extent_table->basename      = NULL;
		extent_table->basename_size = 0;

		return( -1 );
	}
	extent_table->basename[ basename_length ] = 0;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_copy_from_utf32(
		          (libuna_utf8_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          (libuna_utf32_character_t *) basename,
		          basename_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_copy_from_utf16(
		          (libuna_utf8_character_t *) extent_table->basename,
		          extent_table->basename_size,
		          (libuna_utf16_character_t *) basename,
		          basename_length + 1,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_copy_from_utf32(
		          (uint8_t *) extent_table->basename,
		          extent_table->basename_size,
		          libclocale_codepage,
		          (libuna_utf32_character_t *) basename,
		          basename_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_copy_from_utf16(
		          (uint8_t *) extent_table->basename,
		          extent_table->basename_size,
		          libclocale_codepage,
		          (libuna_utf16_character_t *) basename,
		          basename_length + 1,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set basename.",
		 function );

		memory_free(
		 extent_table->basename );

		extent_table->basename      = NULL;
		extent_table->basename_size = 0;

		return( -1 );
	}
#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */
	return( 1 );
}
#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Initializes the extents
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_table_initialize_extents(
     libvmdk_extent_table_t *extent_table,
     int number_of_extents,
     int disk_type,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_table_initialize_extents";
	int result            = 0;

	if( extent_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent table.",
		 function );

		return( -1 );
	}
	if( ( disk_type != LIBVMDK_DISK_TYPE_2GB_EXTENT_FLAT )
	 && ( disk_type != LIBVMDK_DISK_TYPE_MONOLITHIC_FLAT )
	 && ( disk_type != LIBVMDK_DISK_TYPE_2GB_EXTENT_SPARSE )
	 && ( disk_type != LIBVMDK_DISK_TYPE_MONOLITHIC_SPARSE )
	 && ( disk_type != LIBVMDK_DISK_TYPE_STREAM_OPTIMIZED )
	 && ( disk_type != LIBVMDK_DISK_TYPE_VMFS_FLAT )
	 && ( disk_type != LIBVMDK_DISK_TYPE_VMFS_FLAT_PRE_ALLOCATED )
	 && ( disk_type != LIBVMDK_DISK_TYPE_VMFS_FLAT_ZEROED )
	 && ( disk_type != LIBVMDK_DISK_TYPE_VMFS_SPARSE )
	 && ( disk_type != LIBVMDK_DISK_TYPE_VMFS_SPARSE_THIN ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported disk type.",
		 function );

		return( -1 );
	}
	if( ( disk_type == LIBVMDK_DISK_TYPE_2GB_EXTENT_FLAT )
	 || ( disk_type == LIBVMDK_DISK_TYPE_MONOLITHIC_FLAT )
	 || ( disk_type == LIBVMDK_DISK_TYPE_VMFS_FLAT )
	 || ( disk_type == LIBVMDK_DISK_TYPE_VMFS_FLAT_PRE_ALLOCATED )
	 || ( disk_type == LIBVMDK_DISK_TYPE_VMFS_FLAT_ZEROED ) )
	{
/* TODO add write support ? */
		result = libfdata_stream_initialize(
		          &( extent_table->extent_files_stream ),
		          (intptr_t *) extent_table->io_handle,
		          NULL,
		          NULL,
		          NULL,
		          (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libvmdk_extent_file_read_segment_data,
		          NULL,
		          (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libvmdk_extent_file_seek_segment_offset,
		          LIBFDATA_FLAG_DATA_HANDLE_NON_MANAGED,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create extent files stream.",
			 function );

			goto on_error;
		}
		if( libfdata_stream_resize(
		     extent_table->extent_files_stream,
		     number_of_extents,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize extents file stream.",
			 function );

			goto on_error;
		}
	}
	else if( ( disk_type == LIBVMDK_DISK_TYPE_2GB_EXTENT_SPARSE )
	      || ( disk_type == LIBVMDK_DISK_TYPE_MONOLITHIC_SPARSE )
	      || ( disk_type == LIBVMDK_DISK_TYPE_STREAM_OPTIMIZED )
	      || ( disk_type == LIBVMDK_DISK_TYPE_VMFS_SPARSE )
	      || ( disk_type == LIBVMDK_DISK_TYPE_VMFS_SPARSE_THIN ) )
	{
/* TODO add write support ? */
		result = libfdata_list_initialize(
		          &( extent_table->extent_files_list ),
		          (intptr_t *) extent_table->io_handle,
		          NULL,
		          NULL,
		          (int (*)(intptr_t *, intptr_t *, libfdata_list_element_t *, libfcache_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libvmdk_extent_file_read_element_data,
		          NULL,
		          LIBFDATA_FLAG_DATA_HANDLE_NON_MANAGED,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create extent files list.",
			 function );

			goto on_error;
		}
		if( libfdata_list_resize(
		     extent_table->extent_files_list,
		     number_of_extents,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize extent files list.",
			 function );

			goto on_error;
		}
		result = libfcache_cache_initialize(
			  &( extent_table->extent_files_cache ),
			  LIBVMDK_MAXIMUM_CACHE_ENTRIES_EXTENT_FILES,
			  error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create extent files cache.",
			 function );

			goto on_error;
		}
	}
	extent_table->disk_type         = disk_type;
	extent_table->number_of_extents = number_of_extents;

	return( 1 );

on_error:
	if( extent_table->extent_files_list != NULL )
	{
		libfdata_list_free(
		 &( extent_table->extent_files_list ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of extents
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_table_get_number_of_extents(
     libvmdk_extent_table_t *extent_table,
     int *number_of_extents,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_table_get_number_of_extents";

	if( extent_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent table.",
		 function );

		return( -1 );
	}
	if( number_of_extents == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of extents.",
		 function );

		return( -1 );
	}
	*number_of_extents = extent_table->number_of_extents;

	return( 1 );
}

/* Retrieves a specific extent from the extent table
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_table_get_extent_by_index(
     libvmdk_extent_table_t *extent_table,
     int extent_index,
     int *file_io_pool_entry,
     size64_t *extent_file_size,
     libcerror_error_t **error )
{
	static char *function  = "libvmdk_extent_table_get_extent_by_index";
	off64_t element_offset = 0;
	uint32_t element_flags = 0;

	if( extent_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent table.",
		 function );

		return( -1 );
	}
	if( libfdata_list_get_element_by_index(
	     extent_table->extent_files_list,
	     extent_index,
	     file_io_pool_entry,
	     &element_offset,
	     extent_file_size,
	     &element_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element: %d from extent files list.",
		 function,
		 extent_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a extent at a specific offset from the extent table
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libvmdk_extent_table_get_extent_at_offset(
     libvmdk_extent_table_t *extent_table,
     off64_t offset,
     int *file_io_pool_entry,
     size64_t *extent_file_size,
     libcerror_error_t **error )
{
	static char *function           = "libvmdk_extent_table_get_extent_at_offset";
	off64_t element_offset          = 0;
	off64_t extent_file_data_offset = 0;
	uint32_t element_flags          = 0;
	int extent_index                = 0;
	int result                      = 0;

	if( extent_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent table.",
		 function );

		return( -1 );
	}
	result = libfdata_list_get_element_at_offset(
	          extent_table->extent_files_list,
	          offset,
	          &extent_index,
	          &extent_file_data_offset,
	          file_io_pool_entry,
	          &element_offset,
	          extent_file_size,
	          &element_flags,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element at offset: %" PRIi64 " from extent files list.",
		 function,
		 offset );

		return( -1 );
	}
	return( result );
}

/* Sets the extent storage media size for a specific extent in the extent table
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_table_set_extent_storage_media_size_by_index(
     libvmdk_extent_table_t *extent_table,
     int extent_index,
     size64_t storage_media_size,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_table_set_extent_storage_media_size_by_index";

	if( extent_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent table.",
		 function );

		return( -1 );
	}
	if( libfdata_list_set_mapped_size_by_index(
	     extent_table->extent_files_list,
	     extent_index,
	     storage_media_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to set mapped size of element: %d in extent files list.",
		 function,
		 extent_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific extent file from the extent table
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_table_get_extent_file_by_index(
     libvmdk_extent_table_t *extent_table,
     int extent_index,
     libbfio_pool_t *file_io_pool,
     libvmdk_extent_file_t **extent_file,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_table_get_extent_file_by_index";

	if( extent_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent table.",
		 function );

		return( -1 );
	}
	if( libfdata_list_get_element_value_by_index(
	     extent_table->extent_files_list,
	     (intptr_t *) file_io_pool,
	     extent_table->extent_files_cache,
	     extent_index,
	     (intptr_t **) extent_file,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element value: %d from extent files list.",
		 function,
		 extent_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a extent file at a specific offset from the extent table
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libvmdk_extent_table_get_extent_file_at_offset(
     libvmdk_extent_table_t *extent_table,
     off64_t offset,
     libbfio_pool_t *file_io_pool,
     int *extent_index,
     off64_t *extent_file_data_offset,
     libvmdk_extent_file_t **extent_file,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_table_get_extent_file_at_offset";
	int result            = 0;

	if( extent_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent table.",
		 function );

		return( -1 );
	}
	result = libfdata_list_get_element_value_at_offset(
	          extent_table->extent_files_list,
	          (intptr_t *) file_io_pool,
	          extent_table->extent_files_cache,
	          offset,
	          extent_index,
	          extent_file_data_offset,
	          (intptr_t **) extent_file,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element at offset: %" PRIi64 " from extent files list.",
		 function,
		 offset );

		return( -1 );
	}
	return( result );
}

/* Sets a specific extent file in the extent table
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_table_set_extent_file_by_index(
     libvmdk_extent_table_t *extent_table,
     libbfio_pool_t *file_io_pool,
     int extent_index,
     libvmdk_extent_file_t *extent_file,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_table_set_extent_file_by_index";

	if( extent_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent table.",
		 function );

		return( -1 );
	}
	if( libfdata_list_set_element_value_by_index(
	     extent_table->extent_files_list,
	     (intptr_t *) file_io_pool,
	     extent_table->extent_files_cache,
	     extent_index,
	     (intptr_t *) extent_file,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libvmdk_extent_file_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set element value: %d extent files list.",
		 function,
		 extent_index );

		return( -1 );
	}
	return( 1 );
}

/* Sets an extent in the extent table based on the extent descriptor
 * Returns 1 if successful or -1 on error
 */
int libvmdk_extent_table_set_extent_by_extent_descriptor(
     libvmdk_extent_table_t *extent_table,
     libvmdk_extent_descriptor_t *extent_descriptor,
     int extent_index,
     int file_io_pool_entry,
     size64_t extent_file_size,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_extent_table_set_extent_by_extent_descriptor";

	if( extent_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent table.",
		 function );

		return( -1 );
	}
	if( extent_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent descriptor.",
		 function );

		return( -1 );
	}
	if( ( extent_index < 0 )
	 || ( extent_index > extent_table->number_of_extents ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid extent index value out of bounds.",
		 function );

		return( -1 );
	}
	if( extent_index == 0 )
	{
		if( extent_descriptor->type == LIBVMDK_EXTENT_TYPE_FLAT )
		{
			if( ( extent_table->disk_type != LIBVMDK_DISK_TYPE_2GB_EXTENT_FLAT )
			 && ( extent_table->disk_type != LIBVMDK_DISK_TYPE_MONOLITHIC_FLAT ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: extent type not supported for disk type.",
				 function );

				return( -1 );
			}
		}
		else if( extent_descriptor->type == LIBVMDK_EXTENT_TYPE_SPARSE )
		{
			if( ( extent_table->disk_type != LIBVMDK_DISK_TYPE_2GB_EXTENT_SPARSE )
			 && ( extent_table->disk_type != LIBVMDK_DISK_TYPE_MONOLITHIC_SPARSE )
			 && ( extent_table->disk_type != LIBVMDK_DISK_TYPE_STREAM_OPTIMIZED ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: extent type not supported for disk type.",
				 function );

				return( -1 );
			}
		}
		else if( extent_descriptor->type == LIBVMDK_EXTENT_TYPE_VMFS_FLAT )
		{
			if( ( extent_table->disk_type != LIBVMDK_DISK_TYPE_VMFS_FLAT )
			 && ( extent_table->disk_type != LIBVMDK_DISK_TYPE_VMFS_FLAT_PRE_ALLOCATED )
			 && ( extent_table->disk_type != LIBVMDK_DISK_TYPE_VMFS_FLAT_ZEROED ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: extent type not supported for disk type.",
				 function );

				return( -1 );
			}
		}
		else if( extent_descriptor->type == LIBVMDK_EXTENT_TYPE_VMFS_SPARSE )
		{
			if( ( extent_table->disk_type != LIBVMDK_DISK_TYPE_VMFS_SPARSE )
			 && ( extent_table->disk_type != LIBVMDK_DISK_TYPE_VMFS_SPARSE_THIN ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: extent type not supported for disk type.",
				 function );

				return( -1 );
			}
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported extent type.",
			 function );

			return( -1 );
		}
		extent_table->extent_type = extent_descriptor->type;
	}
	else if( extent_table->extent_type != extent_descriptor->type )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: mixed extent types not supported.",
		 function );

		return( -1 );
	}
	if( ( extent_descriptor->type == LIBVMDK_EXTENT_TYPE_FLAT )
	 || ( extent_descriptor->type == LIBVMDK_EXTENT_TYPE_VMFS_FLAT ) )
	{
		if( libfdata_stream_set_segment_by_index(
		     extent_table->extent_files_stream,
		     extent_index,
		     file_io_pool_entry,
		     0,
		     extent_file_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set segment: %d in extent files stream.",
			 function,
			 extent_index );

			return( -1 );
		}
	}
	else if( ( extent_descriptor->type == LIBVMDK_EXTENT_TYPE_SPARSE )
	      || ( extent_descriptor->type == LIBVMDK_EXTENT_TYPE_VMFS_SPARSE ) )
	{
		if( libfdata_list_set_element_by_index(
		     extent_table->extent_files_list,
		     extent_index,
		     file_io_pool_entry,
		     0,
		     extent_file_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set element: %d in extent files list.",
			 function,
			 extent_index );

			return( -1 );
		}
	}
	return( 1 );
}

