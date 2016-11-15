/*
 * Library handle type testing program
 *
 * Copyright (C) 2009-2016, Joachim Metz <joachim.metz@gmail.com>
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
#include <file_stream.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "vmdk_test_libcerror.h"
#include "vmdk_test_libclocale.h"
#include "vmdk_test_libcsystem.h"
#include "vmdk_test_libuna.h"
#include "vmdk_test_libvmdk.h"
#include "vmdk_test_macros.h"
#include "vmdk_test_memory.h"

#if defined( HAVE_WIDE_SYSTEM_CHARACTER ) && SIZEOF_WCHAR_T != 2 && SIZEOF_WCHAR_T != 4
#error Unsupported size of wchar_t
#endif

/* Define to make vmdk_test_handle generate verbose output
#define VMDK_TEST_HANDLE_VERBOSE
 */

/* Retrieves source as a narrow string
 * Returns 1 if successful or -1 on error
 */
int vmdk_test_handle_get_narrow_source(
     const system_character_t *source,
     char *narrow_string,
     size_t narrow_string_size,
     libcerror_error_t **error )
{
	static char *function     = "vmdk_test_handle_get_narrow_source";
	size_t narrow_source_size = 0;
	size_t source_length      = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	int result                = 0;
#endif

	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( narrow_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid narrow string.",
		 function );

		return( -1 );
	}
	if( narrow_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid narrow string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	source_length = system_string_length(
	                 source );

	if( source_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid source length value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_size_from_utf32(
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          &narrow_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_size_from_utf16(
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          &narrow_source_size,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_size_from_utf32(
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &narrow_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &narrow_source_size,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine narrow string size.",
		 function );

		return( -1 );
	}
#else
	narrow_source_size = source_length + 1;

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	if( narrow_string_size < narrow_source_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: narrow string too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_copy_from_utf32(
		          (libuna_utf8_character_t *) narrow_string,
		          narrow_string_size,
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_copy_from_utf16(
		          (libuna_utf8_character_t *) narrow_string,
		          narrow_string_size,
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_copy_from_utf32(
		          (uint8_t *) narrow_string,
		          narrow_string_size,
		          libclocale_codepage,
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_copy_from_utf16(
		          (uint8_t *) narrow_string,
		          narrow_string_size,
		          libclocale_codepage,
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set narrow string.",
		 function );

		return( -1 );
	}
#else
	if( system_string_copy(
	     narrow_string,
	     source,
	     source_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set narrow string.",
		 function );

		return( -1 );
	}
	narrow_string[ source_length ] = 0;

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves source as a wide string
 * Returns 1 if successful or -1 on error
 */
int vmdk_test_handle_get_wide_source(
     const system_character_t *source,
     wchar_t *wide_string,
     size_t wide_string_size,
     libcerror_error_t **error )
{
	static char *function   = "vmdk_test_handle_get_wide_source";
	size_t source_length    = 0;
	size_t wide_source_size = 0;

#if !defined( HAVE_WIDE_SYSTEM_CHARACTER )
	int result              = 0;
#endif

	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( wide_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid wide string.",
		 function );

		return( -1 );
	}
	if( wide_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid wide string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	source_length = system_string_length(
	                 source );

	if( source_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid source length value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	wide_source_size = source_length + 1;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_utf8(
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          &wide_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_utf8(
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          &wide_source_size,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_byte_stream(
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &wide_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_byte_stream(
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &wide_source_size,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine wide string size.",
		 function );

		return( -1 );
	}

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	if( wide_string_size < wide_source_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: wide string too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( system_string_copy(
	     wide_string,
	     source,
	     source_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set wide string.",
		 function );

		return( -1 );
	}
	wide_string[ source_length ] = 0;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_utf8(
		          (libuna_utf32_character_t *) wide_string,
		          wide_string_size,
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_utf8(
		          (libuna_utf16_character_t *) wide_string,
		          wide_string_size,
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_byte_stream(
		          (libuna_utf32_character_t *) wide_string,
		          wide_string_size,
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_byte_stream(
		          (libuna_utf16_character_t *) wide_string,
		          wide_string_size,
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set wide string.",
		 function );

		return( -1 );
	}

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Creates and opens a source handle
 * Returns 1 if successful or -1 on error
 */
int vmdk_test_handle_open_source(
     libvmdk_handle_t **handle,
     const system_character_t *source,
     libcerror_error_t **error )
{
	static char *function = "vmdk_test_handle_open_source";
	int result            = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( libvmdk_handle_initialize(
	     handle,
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
	          *handle,
	          source,
	          LIBVMDK_OPEN_READ,
	          error );
#else
	result = libvmdk_handle_open(
	          *handle,
	          source,
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
	if( libvmdk_handle_open_extent_data_files(
	     *handle,
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
	return( 1 );

on_error:
	if( *handle != NULL )
	{
		libvmdk_handle_free(
		 handle,
		 NULL );
	}
	return( -1 );
}

/* Closes and frees a source handle
 * Returns 1 if successful or -1 on error
 */
int vmdk_test_handle_close_source(
     libvmdk_handle_t **handle,
     libcerror_error_t **error )
{
	static char *function = "vmdk_test_handle_close_source";
	int result            = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( libvmdk_handle_close(
	     *handle,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close handle.",
		 function );

		result = -1;
	}
	if( libvmdk_handle_free(
	     handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free handle.",
		 function );

		result = -1;
	}
	return( result );
}

/* Tests the libvmdk_handle_initialize function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_handle_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libvmdk_handle_t *handle        = NULL;
	int result                      = 0;

#if defined( HAVE_VMDK_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libvmdk_handle_initialize(
	          &handle,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        VMDK_TEST_ASSERT_IS_NOT_NULL(
         "handle",
         handle );

        VMDK_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libvmdk_handle_free(
	          &handle,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        VMDK_TEST_ASSERT_IS_NULL(
         "handle",
         handle );

        VMDK_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libvmdk_handle_initialize(
	          NULL,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        VMDK_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	handle = (libvmdk_handle_t *) 0x12345678UL;

	result = libvmdk_handle_initialize(
	          &handle,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        VMDK_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	handle = NULL;

#if defined( HAVE_VMDK_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libvmdk_handle_initialize with malloc failing
		 */
		vmdk_test_malloc_attempts_before_fail = test_number;

		result = libvmdk_handle_initialize(
		          &handle,
		          &error );

		if( vmdk_test_malloc_attempts_before_fail != -1 )
		{
			vmdk_test_malloc_attempts_before_fail = -1;

			if( handle != NULL )
			{
				libvmdk_handle_free(
				 &handle,
				 NULL );
			}
		}
		else
		{
			VMDK_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			VMDK_TEST_ASSERT_IS_NULL(
			 "handle",
			 handle );

			VMDK_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libvmdk_handle_initialize with memset failing
		 */
		vmdk_test_memset_attempts_before_fail = test_number;

		result = libvmdk_handle_initialize(
		          &handle,
		          &error );

		if( vmdk_test_memset_attempts_before_fail != -1 )
		{
			vmdk_test_memset_attempts_before_fail = -1;

			if( handle != NULL )
			{
				libvmdk_handle_free(
				 &handle,
				 NULL );
			}
		}
		else
		{
			VMDK_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			VMDK_TEST_ASSERT_IS_NULL(
			 "handle",
			 handle );

			VMDK_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_VMDK_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		libvmdk_handle_free(
		 &handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libvmdk_handle_free function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_handle_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libvmdk_handle_free(
	          NULL,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        VMDK_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libvmdk_handle_open function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_handle_open(
     const system_character_t *source )
{
	char narrow_source[ 256 ];

	libcerror_error_t *error = NULL;
	libvmdk_handle_t *handle = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = vmdk_test_handle_get_narrow_source(
	          source,
	          narrow_source,
	          256,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        VMDK_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libvmdk_handle_initialize(
	          &handle,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        VMDK_TEST_ASSERT_IS_NOT_NULL(
         "handle",
         handle );

        VMDK_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test open
	 */
	result = libvmdk_handle_open(
	          handle,
	          narrow_source,
	          LIBVMDK_OPEN_READ,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        VMDK_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libvmdk_handle_open(
	          handle,
	          narrow_source,
	          LIBVMDK_OPEN_READ,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        VMDK_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libvmdk_handle_free(
	          &handle,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        VMDK_TEST_ASSERT_IS_NULL(
         "handle",
         handle );

        VMDK_TEST_ASSERT_IS_NULL(
         "error",
         error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		libvmdk_handle_free(
		 &handle,
		 NULL );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libvmdk_handle_open_wide function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_handle_open_wide(
     const system_character_t *source )
{
	wchar_t wide_source[ 256 ];

	libcerror_error_t *error = NULL;
	libvmdk_handle_t *handle = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = vmdk_test_handle_get_wide_source(
	          source,
	          wide_source,
	          256,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        VMDK_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libvmdk_handle_initialize(
	          &handle,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        VMDK_TEST_ASSERT_IS_NOT_NULL(
         "handle",
         handle );

        VMDK_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test open
	 */
	result = libvmdk_handle_open_wide(
	          handle,
	          wide_source,
	          LIBVMDK_OPEN_READ,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        VMDK_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libvmdk_handle_open_wide(
	          handle,
	          wide_source,
	          LIBVMDK_OPEN_READ,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        VMDK_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libvmdk_handle_free(
	          &handle,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        VMDK_TEST_ASSERT_IS_NULL(
         "handle",
         handle );

        VMDK_TEST_ASSERT_IS_NULL(
         "error",
         error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		libvmdk_handle_free(
		 &handle,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libvmdk_handle_close function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_handle_close(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libvmdk_handle_close(
	          NULL,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        VMDK_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libvmdk_handle_open and libvmdk_handle_close functions
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_handle_open_close(
     const system_character_t *source )
{
	libcerror_error_t *error = NULL;
	libvmdk_handle_t *handle = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = libvmdk_handle_initialize(
	          &handle,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        VMDK_TEST_ASSERT_IS_NOT_NULL(
         "handle",
         handle );

        VMDK_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test open and close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libvmdk_handle_open_wide(
	          handle,
	          source,
	          LIBVMDK_OPEN_READ,
	          &error );
#else
	result = libvmdk_handle_open(
	          handle,
	          source,
	          LIBVMDK_OPEN_READ,
	          &error );
#endif

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        VMDK_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libvmdk_handle_close(
	          handle,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

        VMDK_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test open and close a second time to validate clean up on close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libvmdk_handle_open_wide(
	          handle,
	          source,
	          LIBVMDK_OPEN_READ,
	          &error );
#else
	result = libvmdk_handle_open(
	          handle,
	          source,
	          LIBVMDK_OPEN_READ,
	          &error );
#endif

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        VMDK_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libvmdk_handle_close(
	          handle,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

        VMDK_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Clean up
	 */
	result = libvmdk_handle_free(
	          &handle,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        VMDK_TEST_ASSERT_IS_NULL(
         "handle",
         handle );

        VMDK_TEST_ASSERT_IS_NULL(
         "error",
         error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		libvmdk_handle_free(
		 &handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libvmdk_handle_signal_abort function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_handle_signal_abort(
     libvmdk_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libvmdk_handle_signal_abort(
	          handle,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        VMDK_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libvmdk_handle_signal_abort(
	          NULL,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        VMDK_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libvmdk_handle_get_offset function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_handle_get_offset(
     libvmdk_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	off64_t offset           = 0;
	int offset_is_set        = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libvmdk_handle_get_offset(
	          handle,
	          &offset,
	          &error );

	VMDK_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	offset_is_set = result;

	/* Test error cases
	 */
	result = libvmdk_handle_get_offset(
	          NULL,
	          &offset,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( offset_is_set != 0 )
	{
		result = libvmdk_handle_get_offset(
		          handle,
		          NULL,
		          &error );

		VMDK_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		VMDK_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libvmdk_handle_get_disk_type function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_handle_get_disk_type(
     libvmdk_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	int disk_type            = 0;
	int disk_type_is_set     = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libvmdk_handle_get_disk_type(
	          handle,
	          &disk_type,
	          &error );

	VMDK_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	disk_type_is_set = result;

	/* Test error cases
	 */
	result = libvmdk_handle_get_disk_type(
	          NULL,
	          &disk_type,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( disk_type_is_set != 0 )
	{
		result = libvmdk_handle_get_disk_type(
		          handle,
		          NULL,
		          &error );

		VMDK_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		VMDK_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libvmdk_handle_get_media_size function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_handle_get_media_size(
     libvmdk_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	size64_t media_size      = 0;
	int media_size_is_set    = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libvmdk_handle_get_media_size(
	          handle,
	          &media_size,
	          &error );

	VMDK_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	media_size_is_set = result;

	/* Test error cases
	 */
	result = libvmdk_handle_get_media_size(
	          NULL,
	          &media_size,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( media_size_is_set != 0 )
	{
		result = libvmdk_handle_get_media_size(
		          handle,
		          NULL,
		          &error );

		VMDK_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		VMDK_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libvmdk_handle_get_content_identifier function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_handle_get_content_identifier(
     libvmdk_handle_t *handle )
{
	libcerror_error_t *error      = NULL;
	uint32_t content_identifier   = 0;
	int content_identifier_is_set = 0;
	int result                    = 0;

	/* Test regular cases
	 */
	result = libvmdk_handle_get_content_identifier(
	          handle,
	          &content_identifier,
	          &error );

	VMDK_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	content_identifier_is_set = result;

	/* Test error cases
	 */
	result = libvmdk_handle_get_content_identifier(
	          NULL,
	          &content_identifier,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( content_identifier_is_set != 0 )
	{
		result = libvmdk_handle_get_content_identifier(
		          handle,
		          NULL,
		          &error );

		VMDK_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		VMDK_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libvmdk_handle_get_parent_content_identifier function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_handle_get_parent_content_identifier(
     libvmdk_handle_t *handle )
{
	libcerror_error_t *error             = NULL;
	uint32_t parent_content_identifier   = 0;
	int parent_content_identifier_is_set = 0;
	int result                           = 0;

	/* Test regular cases
	 */
	result = libvmdk_handle_get_parent_content_identifier(
	          handle,
	          &parent_content_identifier,
	          &error );

	VMDK_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	parent_content_identifier_is_set = result;

	/* Test error cases
	 */
	result = libvmdk_handle_get_parent_content_identifier(
	          NULL,
	          &parent_content_identifier,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( parent_content_identifier_is_set != 0 )
	{
		result = libvmdk_handle_get_parent_content_identifier(
		          handle,
		          NULL,
		          &error );

		VMDK_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		VMDK_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libvmdk_handle_get_utf8_parent_filename_size function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_handle_get_utf8_parent_filename_size(
     libvmdk_handle_t *handle )
{
	libcerror_error_t *error             = NULL;
	size_t utf8_parent_filename_size     = 0;
	int result                           = 0;
	int utf8_parent_filename_size_is_set = 0;

	/* Test regular cases
	 */
	result = libvmdk_handle_get_utf8_parent_filename_size(
	          handle,
	          &utf8_parent_filename_size,
	          &error );

	VMDK_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_parent_filename_size_is_set = result;

	/* Test error cases
	 */
	result = libvmdk_handle_get_utf8_parent_filename_size(
	          NULL,
	          &utf8_parent_filename_size,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( utf8_parent_filename_size_is_set != 0 )
	{
		result = libvmdk_handle_get_utf8_parent_filename_size(
		          handle,
		          NULL,
		          &error );

		VMDK_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		VMDK_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libvmdk_handle_get_utf8_parent_filename function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_handle_get_utf8_parent_filename(
     libvmdk_handle_t *handle )
{
	uint8_t utf8_parent_filename[ 512 ];

	libcerror_error_t *error        = NULL;
	int result                      = 0;
	int utf8_parent_filename_is_set = 0;

	/* Test regular cases
	 */
	result = libvmdk_handle_get_utf8_parent_filename(
	          handle,
	          utf8_parent_filename,
	          512,
	          &error );

	VMDK_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_parent_filename_is_set = result;

	/* Test error cases
	 */
	result = libvmdk_handle_get_utf8_parent_filename(
	          NULL,
	          utf8_parent_filename,
	          512,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( utf8_parent_filename_is_set != 0 )
	{
		result = libvmdk_handle_get_utf8_parent_filename(
		          handle,
		          NULL,
		          512,
		          &error );

		VMDK_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		VMDK_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );

		result = libvmdk_handle_get_utf8_parent_filename(
		          handle,
		          utf8_parent_filename,
		          0,
		          &error );

		VMDK_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

	        VMDK_TEST_ASSERT_IS_NOT_NULL(
	         "error",
	         error );

		libcerror_error_free(
		 &error );

		result = libvmdk_handle_get_utf8_parent_filename(
		          handle,
		          utf8_parent_filename,
		          (size_t) SSIZE_MAX + 1,
		          &error );

		VMDK_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		VMDK_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libvmdk_handle_get_utf16_parent_filename_size function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_handle_get_utf16_parent_filename_size(
     libvmdk_handle_t *handle )
{
	libcerror_error_t *error              = NULL;
	size_t utf16_parent_filename_size     = 0;
	int result                            = 0;
	int utf16_parent_filename_size_is_set = 0;

	/* Test regular cases
	 */
	result = libvmdk_handle_get_utf16_parent_filename_size(
	          handle,
	          &utf16_parent_filename_size,
	          &error );

	VMDK_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_parent_filename_size_is_set = result;

	/* Test error cases
	 */
	result = libvmdk_handle_get_utf16_parent_filename_size(
	          NULL,
	          &utf16_parent_filename_size,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( utf16_parent_filename_size_is_set != 0 )
	{
		result = libvmdk_handle_get_utf16_parent_filename_size(
		          handle,
		          NULL,
		          &error );

		VMDK_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		VMDK_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libvmdk_handle_get_utf16_parent_filename function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_handle_get_utf16_parent_filename(
     libvmdk_handle_t *handle )
{
	uint16_t utf16_parent_filename[ 512 ];

	libcerror_error_t *error         = NULL;
	int result                       = 0;
	int utf16_parent_filename_is_set = 0;

	/* Test regular cases
	 */
	result = libvmdk_handle_get_utf16_parent_filename(
	          handle,
	          utf16_parent_filename,
	          512,
	          &error );

	VMDK_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_parent_filename_is_set = result;

	/* Test error cases
	 */
	result = libvmdk_handle_get_utf16_parent_filename(
	          NULL,
	          utf16_parent_filename,
	          512,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( utf16_parent_filename_is_set != 0 )
	{
		result = libvmdk_handle_get_utf16_parent_filename(
		          handle,
		          NULL,
		          512,
		          &error );

		VMDK_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		VMDK_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );

		result = libvmdk_handle_get_utf16_parent_filename(
		          handle,
		          utf16_parent_filename,
		          0,
		          &error );

		VMDK_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

	        VMDK_TEST_ASSERT_IS_NOT_NULL(
	         "error",
	         error );

		libcerror_error_free(
		 &error );

		result = libvmdk_handle_get_utf16_parent_filename(
		          handle,
		          utf16_parent_filename,
		          (size_t) SSIZE_MAX + 1,
		          &error );

		VMDK_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		VMDK_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libvmdk_handle_get_number_of_extents function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_handle_get_number_of_extents(
     libvmdk_handle_t *handle )
{
	libcerror_error_t *error     = NULL;
	int number_of_extents        = 0;
	int number_of_extents_is_set = 0;
	int result                   = 0;

	/* Test regular cases
	 */
	result = libvmdk_handle_get_number_of_extents(
	          handle,
	          &number_of_extents,
	          &error );

	VMDK_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_extents_is_set = result;

	/* Test error cases
	 */
	result = libvmdk_handle_get_number_of_extents(
	          NULL,
	          &number_of_extents,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( number_of_extents_is_set != 0 )
	{
		result = libvmdk_handle_get_number_of_extents(
		          handle,
		          NULL,
		          &error );

		VMDK_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		VMDK_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc,
     wchar_t * const argv[] )
#else
int main(
     int argc,
     char * const argv[] )
#endif
{
	libcerror_error_t *error   = NULL;
	libvmdk_handle_t *handle   = NULL;
	system_character_t *source = NULL;
	system_integer_t option    = 0;
	int result                 = 0;

	while( ( option = libcsystem_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind - 1 ] );

				return( EXIT_FAILURE );
		}
	}
	if( optind < argc )
	{
		source = argv[ optind ];
	}
#if defined( HAVE_DEBUG_OUTPUT ) && defined( VMDK_TEST_HANDLE_VERBOSE )
	libvmdk_notify_set_verbose(
	 1 );
	libvmdk_notify_set_stream(
	 stderr,
	 NULL );
#endif

	VMDK_TEST_RUN(
	 "libvmdk_handle_initialize",
	 vmdk_test_handle_initialize );

	VMDK_TEST_RUN(
	 "libvmdk_handle_free",
	 vmdk_test_handle_free );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	if( source != NULL )
	{
		VMDK_TEST_RUN_WITH_ARGS(
		 "libvmdk_handle_open",
		 vmdk_test_handle_open,
		 source );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		VMDK_TEST_RUN_WITH_ARGS(
		 "libvmdk_handle_open_wide",
		 vmdk_test_handle_open_wide,
		 source );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBVMDK_HAVE_BFIO )

		/* TODO add test for libvmdk_handle_open_file_io_handle */

#endif /* defined( LIBVMDK_HAVE_BFIO ) */

		VMDK_TEST_RUN(
		 "libvmdk_handle_close",
		 vmdk_test_handle_close );

		VMDK_TEST_RUN_WITH_ARGS(
		 "libvmdk_handle_open_close",
		 vmdk_test_handle_open_close,
		 source );

		/* Initialize test
		 */
		result = vmdk_test_handle_open_source(
		          &handle,
		          source,
		          &error );

		VMDK_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        VMDK_TEST_ASSERT_IS_NOT_NULL(
	         "handle",
	         handle );

	        VMDK_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		VMDK_TEST_RUN_WITH_ARGS(
		 "libvmdk_handle_signal_abort",
		 vmdk_test_handle_signal_abort,
		 handle );

		/* TODO: add tests for libvmdk_handle_open_extent_data_files */

		/* TODO: add tests for libvmdk_handle_open_extent_data_files_file_io_pool */

#if defined( __GNUC__ )

		/* TODO: add tests for libvmdk_handle_open_extent_data_file */

		/* TODO: add tests for libvmdk_handle_open_extent_data_file_wide */

		/* TODO: add tests for libvmdk_handle_open_read_grain_table */

		/* TODO: add tests for libvmdk_handle_open_read_signature */

#endif /* defined( __GNUC__ ) */

		/* TODO: add tests for libvmdk_handle_read_buffer */

		/* TODO: add tests for libvmdk_handle_read_buffer_at_offset */

		/* TODO: add tests for libvmdk_handle_write_buffer */

		/* TODO: add tests for libvmdk_handle_write_buffer_at_offset */

		/* TODO: add tests for libvmdk_handle_seek_offset */

		VMDK_TEST_RUN_WITH_ARGS(
		 "libvmdk_handle_get_offset",
		 vmdk_test_handle_get_offset,
		 handle );

		/* TODO: add tests for libvmdk_handle_set_maximum_number_of_open_handles */

		/* TODO: add tests for libvmdk_handle_set_parent_handle */

		VMDK_TEST_RUN_WITH_ARGS(
		 "libvmdk_handle_get_disk_type",
		 vmdk_test_handle_get_disk_type,
		 handle );

		VMDK_TEST_RUN_WITH_ARGS(
		 "libvmdk_handle_get_media_size",
		 vmdk_test_handle_get_media_size,
		 handle );

		VMDK_TEST_RUN_WITH_ARGS(
		 "libvmdk_handle_get_content_identifier",
		 vmdk_test_handle_get_content_identifier,
		 handle );

		VMDK_TEST_RUN_WITH_ARGS(
		 "libvmdk_handle_get_parent_content_identifier",
		 vmdk_test_handle_get_parent_content_identifier,
		 handle );

		VMDK_TEST_RUN_WITH_ARGS(
		 "libvmdk_handle_get_utf8_parent_filename_size",
		 vmdk_test_handle_get_utf8_parent_filename_size,
		 handle );

		VMDK_TEST_RUN_WITH_ARGS(
		 "libvmdk_handle_get_utf8_parent_filename",
		 vmdk_test_handle_get_utf8_parent_filename,
		 handle );

		VMDK_TEST_RUN_WITH_ARGS(
		 "libvmdk_handle_get_utf16_parent_filename_size",
		 vmdk_test_handle_get_utf16_parent_filename_size,
		 handle );

		VMDK_TEST_RUN_WITH_ARGS(
		 "libvmdk_handle_get_utf16_parent_filename",
		 vmdk_test_handle_get_utf16_parent_filename,
		 handle );

		VMDK_TEST_RUN_WITH_ARGS(
		 "libvmdk_handle_get_number_of_extents",
		 vmdk_test_handle_get_number_of_extents,
		 handle );

		/* TODO: add tests for libvmdk_handle_get_extent_descriptor */

		/* Clean up
		 */
		result = vmdk_test_handle_close_source(
		          &handle,
		          &error );

		VMDK_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );

		VMDK_TEST_ASSERT_IS_NULL(
	         "handle",
	         handle );

	        VMDK_TEST_ASSERT_IS_NULL(
	         "error",
	         error );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		vmdk_test_handle_close_source(
		 &handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

