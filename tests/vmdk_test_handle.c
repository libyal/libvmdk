/*
 * Library handle type test program
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
#include <file_stream.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "vmdk_test_functions.h"
#include "vmdk_test_getopt.h"
#include "vmdk_test_libbfio.h"
#include "vmdk_test_libcerror.h"
#include "vmdk_test_libvmdk.h"
#include "vmdk_test_macros.h"
#include "vmdk_test_memory.h"
#include "vmdk_test_unused.h"

#include "../libvmdk/libvmdk_handle.h"

#if !defined( LIBVMDK_HAVE_BFIO )

LIBVMDK_EXTERN \
int libvmdk_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBVMDK_EXTERN \
int libvmdk_handle_open_file_io_handle(
     libvmdk_handle_t *handle,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libvmdk_error_t **error );

#endif /* !defined( LIBVMDK_HAVE_BFIO ) */

#if defined( HAVE_WIDE_SYSTEM_CHARACTER ) && SIZEOF_WCHAR_T != 2 && SIZEOF_WCHAR_T != 4
#error Unsupported size of wchar_t
#endif

/* Define to make vmdk_test_handle generate verbose output
#define VMDK_TEST_HANDLE_VERBOSE
 */

/* Creates and opens a source handle
 * Returns 1 if successful or -1 on error
 */
int vmdk_test_handle_open_source(
     libvmdk_handle_t **handle,
     const system_character_t *source,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "vmdk_test_handle_open_source";
	size_t string_length  = 0;
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
	result = libvmdk_handle_open_file_io_handle(
	          *handle,
	          file_io_handle,
	          LIBVMDK_OPEN_READ,
	          error );

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
	string_length = system_string_length(
	                 source );

	while( string_length > 0 )
	{
		if( source[ string_length - 1 ] == '/' )
		{
			break;
		}
		string_length--;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libvmdk_handle_set_extent_data_files_path(
	          *handle,
	          source,
	          string_length,
	          error );
#else
	result = libvmdk_handle_set_extent_data_files_path(
	          *handle,
	          source,
	          string_length,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set extent data files path.",
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
	result = vmdk_test_get_narrow_source(
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
	          NULL,
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

	result = libvmdk_handle_open(
	          handle,
	          NULL,
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

	result = libvmdk_handle_open(
	          handle,
	          narrow_source,
	          -1,
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

	/* Test open when already opened
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
	result = vmdk_test_get_wide_source(
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
	          NULL,
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

	result = libvmdk_handle_open_wide(
	          handle,
	          NULL,
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

	result = libvmdk_handle_open_wide(
	          handle,
	          wide_source,
	          -1,
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

	/* Test open when already opened
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

/* Tests the libvmdk_handle_open_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_handle_open_file_io_handle(
     const system_character_t *source )
{
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	libvmdk_handle_t *handle         = NULL;
	size_t string_length             = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libbfio_file_initialize(
	          &file_io_handle,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        VMDK_TEST_ASSERT_IS_NOT_NULL(
         "file_io_handle",
         file_io_handle );

        VMDK_TEST_ASSERT_IS_NULL(
         "error",
         error );

	string_length = system_string_length(
	                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libbfio_file_set_name_wide(
	          file_io_handle,
	          source,
	          string_length,
	          &error );
#else
	result = libbfio_file_set_name(
	          file_io_handle,
	          source,
	          string_length,
	          &error );
#endif
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
	result = libvmdk_handle_open_file_io_handle(
	          handle,
	          file_io_handle,
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
	result = libvmdk_handle_open_file_io_handle(
	          NULL,
	          file_io_handle,
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

	result = libvmdk_handle_open_file_io_handle(
	          handle,
	          NULL,
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

	result = libvmdk_handle_open_file_io_handle(
	          handle,
	          file_io_handle,
	          -1,
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

	/* Test open when already opened
	 */
	result = libvmdk_handle_open_file_io_handle(
	          handle,
	          file_io_handle,
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

	result = libbfio_handle_free(
	          &file_io_handle,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
         "file_io_handle",
         file_io_handle );

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
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( 0 );
}

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

/* Tests the libvmdk_handle_read_buffer function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_handle_read_buffer(
     libvmdk_handle_t *handle )
{
	uint8_t buffer[ 16 ];

	libcerror_error_t *error = NULL;
	size64_t media_size      = 0;
	ssize_t read_count       = 0;
	off64_t offset           = 0;
	int result               = 0;

	/* Determine size
	 */
	result = libvmdk_handle_get_media_size(
	          handle,
	          &media_size,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Reset offset to 0
	 */
	offset = libvmdk_handle_seek_offset(
	          handle,
	          0,
	          SEEK_SET,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	if( media_size > 16 )
	{
		read_count = libvmdk_handle_read_buffer(
		              handle,
		              buffer,
		              16,
		              &error );

		VMDK_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 16 );

		VMDK_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Set offset to media_size - 8
		 */
		offset = libvmdk_handle_seek_offset(
		          handle,
		          -8,
		          SEEK_END,
		          &error );

		VMDK_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 offset,
		 (int64_t) media_size - 8 );

		VMDK_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer on media_size boundary
		 */
		read_count = libvmdk_handle_read_buffer(
		              handle,
		              buffer,
		              16,
		              &error );

		VMDK_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 8 );

		VMDK_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer beyond media_size boundary
		 */
		read_count = libvmdk_handle_read_buffer(
		              handle,
		              buffer,
		              16,
		              &error );

		VMDK_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 0 );

		VMDK_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Reset offset to 0
		 */
		offset = libvmdk_handle_seek_offset(
		          handle,
		          0,
		          SEEK_SET,
		          &error );

		VMDK_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 offset,
		 (int64_t) 0 );

		VMDK_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	read_count = libvmdk_handle_read_buffer(
	              NULL,
	              buffer,
	              16,
	              &error );

	VMDK_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libvmdk_handle_read_buffer(
	              handle,
	              NULL,
	              16,
	              &error );

	VMDK_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libvmdk_handle_read_buffer(
	              handle,
	              buffer,
	              (size_t) SSIZE_MAX + 1,
	              &error );

	VMDK_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

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

/* Tests the libvmdk_handle_read_buffer_at_offset function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_handle_read_buffer_at_offset(
     libvmdk_handle_t *handle )
{
	uint8_t buffer[ 16 ];

	libcerror_error_t *error = NULL;
	size64_t media_size      = 0;
	ssize_t read_count       = 0;
	int result               = 0;

	/* Determine size
	 */
	result = libvmdk_handle_get_media_size(
	          handle,
	          &media_size,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	if( media_size > 16 )
	{
		read_count = libvmdk_handle_read_buffer_at_offset(
		              handle,
		              buffer,
		              16,
		              0,
		              &error );

		VMDK_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 16 );

		VMDK_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer on media_size boundary
		 */
		read_count = libvmdk_handle_read_buffer_at_offset(
		              handle,
		              buffer,
		              16,
		              media_size - 8,
		              &error );

		VMDK_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 8 );

		VMDK_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer beyond media_size boundary
		 */
		read_count = libvmdk_handle_read_buffer_at_offset(
		              handle,
		              buffer,
		              16,
		              media_size + 8,
		              &error );

		VMDK_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 0 );

		VMDK_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	read_count = libvmdk_handle_read_buffer_at_offset(
	              NULL,
	              buffer,
	              16,
	              0,
	              &error );

	VMDK_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libvmdk_handle_read_buffer_at_offset(
	              handle,
	              NULL,
	              16,
	              0,
	              &error );

	VMDK_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libvmdk_handle_read_buffer_at_offset(
	              handle,
	              buffer,
	              (size_t) SSIZE_MAX + 1,
	              0,
	              &error );

	VMDK_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libvmdk_handle_read_buffer_at_offset(
	              handle,
	              buffer,
	              16,
	              -1,
	              &error );

	VMDK_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

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

/* Tests the libvmdk_handle_seek_offset function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_handle_seek_offset(
     libvmdk_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	size64_t size            = 0;
	off64_t offset           = 0;

	/* Test regular cases
	 */
	offset = libvmdk_handle_seek_offset(
	          handle,
	          0,
	          SEEK_END,
	          &error );

	VMDK_TEST_ASSERT_NOT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	size = (size64_t) offset;

	offset = libvmdk_handle_seek_offset(
	          handle,
	          1024,
	          SEEK_SET,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 1024 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	offset = libvmdk_handle_seek_offset(
	          handle,
	          -512,
	          SEEK_CUR,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 512 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	offset = libvmdk_handle_seek_offset(
	          handle,
	          (off64_t) ( size + 512 ),
	          SEEK_SET,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) ( size + 512 ) );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Reset offset to 0
	 */
	offset = libvmdk_handle_seek_offset(
	          handle,
	          0,
	          SEEK_SET,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	offset = libvmdk_handle_seek_offset(
	          NULL,
	          0,
	          SEEK_SET,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	offset = libvmdk_handle_seek_offset(
	          handle,
	          -1,
	          SEEK_SET,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	offset = libvmdk_handle_seek_offset(
	          handle,
	          -1,
	          SEEK_CUR,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	offset = libvmdk_handle_seek_offset(
	          handle,
	          (off64_t) ( -1 * ( size + 1 ) ),
	          SEEK_END,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

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
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	libvmdk_handle_t *handle         = NULL;
	system_character_t *source       = NULL;
	system_integer_t option          = 0;
	size_t string_length             = 0;
	int result                       = 0;

	while( ( option = vmdk_test_getopt(
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
		result = libbfio_file_initialize(
		          &file_io_handle,
		          &error );

		VMDK_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        VMDK_TEST_ASSERT_IS_NOT_NULL(
	         "file_io_handle",
	         file_io_handle );

	        VMDK_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		string_length = system_string_length(
		                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbfio_file_set_name_wide(
		          file_io_handle,
		          source,
		          string_length,
		          &error );
#else
		result = libbfio_file_set_name(
		          file_io_handle,
		          source,
		          string_length,
		          &error );
#endif
		VMDK_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        VMDK_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		result = libvmdk_check_file_signature_file_io_handle(
		          file_io_handle,
		          &error );

		VMDK_TEST_ASSERT_NOT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		VMDK_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	if( result != 0 )
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

		VMDK_TEST_RUN_WITH_ARGS(
		 "libvmdk_handle_open_file_io_handle",
		 vmdk_test_handle_open_file_io_handle,
		 source );

		VMDK_TEST_RUN(
		 "libvmdk_handle_close",
		 vmdk_test_handle_close );

		VMDK_TEST_RUN_WITH_ARGS(
		 "libvmdk_handle_open_close",
		 vmdk_test_handle_open_close,
		 source );

		/* Initialize handle for tests
		 */
		result = vmdk_test_handle_open_source(
		          &handle,
		          source,
		          file_io_handle,
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

#if defined( __GNUC__ ) && !defined( LIBVMDK_DLL_IMPORT )

		/* TODO: add tests for libvmdk_handle_open_extent_data_file */

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		/* TODO: add tests for libvmdk_handle_open_extent_data_file_wide */

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

		/* TODO: add tests for libvmdk_internal_handle_open_read_extent_data_files */

		/* TODO: add tests for libvmdk_internal_handle_open_read_signature */

#endif /* defined( __GNUC__ ) && !defined( LIBVMDK_DLL_IMPORT ) */

		VMDK_TEST_RUN_WITH_ARGS(
		 "libvmdk_handle_read_buffer",
		 vmdk_test_handle_read_buffer,
		 handle );

		VMDK_TEST_RUN_WITH_ARGS(
		 "libvmdk_handle_read_buffer_at_offset",
		 vmdk_test_handle_read_buffer_at_offset,
		 handle );

		/* TODO: add tests for libvmdk_handle_write_buffer */

		/* TODO: add tests for libvmdk_handle_write_buffer_at_offset */

		VMDK_TEST_RUN_WITH_ARGS(
		 "libvmdk_handle_seek_offset",
		 vmdk_test_handle_seek_offset,
		 handle );

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

		result = libbfio_handle_free(
		          &file_io_handle,
		          &error );

		VMDK_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		VMDK_TEST_ASSERT_IS_NULL(
	         "file_io_handle",
	         file_io_handle );

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
		libvmdk_handle_free(
		 &handle,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

