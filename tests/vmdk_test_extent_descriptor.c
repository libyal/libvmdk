/*
 * Library extent_descriptor type test program
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
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "vmdk_test_libcerror.h"
#include "vmdk_test_libvmdk.h"
#include "vmdk_test_macros.h"
#include "vmdk_test_memory.h"
#include "vmdk_test_unused.h"

#include "../libvmdk/libvmdk_extent_descriptor.h"

#if defined( __GNUC__ ) && !defined( LIBVMDK_DLL_IMPORT )

/* Tests the libvmdk_extent_descriptor_initialize function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_extent_descriptor_initialize(
     void )
{
	libcerror_error_t *error                       = NULL;
	libvmdk_extent_descriptor_t *extent_descriptor = NULL;
	int result                                     = 0;

#if defined( HAVE_VMDK_TEST_MEMORY )
	int number_of_malloc_fail_tests                = 1;
	int number_of_memset_fail_tests                = 1;
	int test_number                                = 0;
#endif

	/* Test regular cases
	 */
	result = libvmdk_extent_descriptor_initialize(
	          &extent_descriptor,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "extent_descriptor",
	 extent_descriptor );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libvmdk_internal_extent_descriptor_free(
	          (libvmdk_internal_extent_descriptor_t **) &extent_descriptor,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "extent_descriptor",
	 extent_descriptor );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libvmdk_extent_descriptor_initialize(
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

	extent_descriptor = (libvmdk_extent_descriptor_t *) 0x12345678UL;

	result = libvmdk_extent_descriptor_initialize(
	          &extent_descriptor,
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

	extent_descriptor = NULL;

#if defined( HAVE_VMDK_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libvmdk_extent_descriptor_initialize with malloc failing
		 */
		vmdk_test_malloc_attempts_before_fail = test_number;

		result = libvmdk_extent_descriptor_initialize(
		          &extent_descriptor,
		          &error );

		if( vmdk_test_malloc_attempts_before_fail != -1 )
		{
			vmdk_test_malloc_attempts_before_fail = -1;

			if( extent_descriptor != NULL )
			{
				libvmdk_internal_extent_descriptor_free(
				 (libvmdk_internal_extent_descriptor_t **) &extent_descriptor,
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
			 "extent_descriptor",
			 extent_descriptor );

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
		/* Test libvmdk_extent_descriptor_initialize with memset failing
		 */
		vmdk_test_memset_attempts_before_fail = test_number;

		result = libvmdk_extent_descriptor_initialize(
		          &extent_descriptor,
		          &error );

		if( vmdk_test_memset_attempts_before_fail != -1 )
		{
			vmdk_test_memset_attempts_before_fail = -1;

			if( extent_descriptor != NULL )
			{
				libvmdk_internal_extent_descriptor_free(
				 (libvmdk_internal_extent_descriptor_t **) &extent_descriptor,
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
			 "extent_descriptor",
			 extent_descriptor );

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
	if( extent_descriptor != NULL )
	{
		libvmdk_internal_extent_descriptor_free(
		 (libvmdk_internal_extent_descriptor_t **) &extent_descriptor,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBVMDK_DLL_IMPORT ) */

/* Tests the libvmdk_extent_descriptor_free function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_extent_descriptor_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libvmdk_extent_descriptor_free(
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

#if defined( __GNUC__ ) && !defined( LIBVMDK_DLL_IMPORT )

/* Tests the libvmdk_extent_descriptor_get_type function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_extent_descriptor_get_type(
     void )
{
	libcerror_error_t *error                       = NULL;
	libvmdk_extent_descriptor_t *extent_descriptor = NULL;
	int result                                     = 0;
	int type                                       = 0;
	int type_is_set                                = 0;

	/* Initialize test
	 */
	result = libvmdk_extent_descriptor_initialize(
	          &extent_descriptor,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "extent_descriptor",
	 extent_descriptor );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libvmdk_extent_descriptor_get_type(
	          extent_descriptor,
	          &type,
	          &error );

	VMDK_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	type_is_set = result;

	/* Test error cases
	 */
	result = libvmdk_extent_descriptor_get_type(
	          NULL,
	          &type,
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

	if( type_is_set != 0 )
	{
		result = libvmdk_extent_descriptor_get_type(
		          extent_descriptor,
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
	/* Clean up
	 */
	result = libvmdk_internal_extent_descriptor_free(
	          (libvmdk_internal_extent_descriptor_t **) &extent_descriptor,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "extent_descriptor",
	 extent_descriptor );

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
	if( extent_descriptor != NULL )
	{
		libvmdk_internal_extent_descriptor_free(
		 (libvmdk_internal_extent_descriptor_t **) &extent_descriptor,
		 NULL );
	}
	return( 0 );
}

/* Tests the libvmdk_extent_descriptor_get_utf8_filename_size function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_extent_descriptor_get_utf8_filename_size(
     void )
{
	libcerror_error_t *error                       = NULL;
	libvmdk_extent_descriptor_t *extent_descriptor = NULL;
	size_t utf8_filename_size                      = 0;
	int result                                     = 0;
	int utf8_filename_size_is_set                  = 0;

	/* Initialize test
	 */
	result = libvmdk_extent_descriptor_initialize(
	          &extent_descriptor,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "extent_descriptor",
	 extent_descriptor );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libvmdk_extent_descriptor_get_utf8_filename_size(
	          extent_descriptor,
	          &utf8_filename_size,
	          &error );

	VMDK_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_filename_size_is_set = result;

	/* Test error cases
	 */
	result = libvmdk_extent_descriptor_get_utf8_filename_size(
	          NULL,
	          &utf8_filename_size,
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

	if( utf8_filename_size_is_set != 0 )
	{
		result = libvmdk_extent_descriptor_get_utf8_filename_size(
		          extent_descriptor,
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
	/* Clean up
	 */
	result = libvmdk_internal_extent_descriptor_free(
	          (libvmdk_internal_extent_descriptor_t **) &extent_descriptor,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "extent_descriptor",
	 extent_descriptor );

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
	if( extent_descriptor != NULL )
	{
		libvmdk_internal_extent_descriptor_free(
		 (libvmdk_internal_extent_descriptor_t **) &extent_descriptor,
		 NULL );
	}
	return( 0 );
}

/* Tests the libvmdk_extent_descriptor_get_utf8_filename function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_extent_descriptor_get_utf8_filename(
     void )
{
	uint8_t utf8_filename[ 512 ];

	libcerror_error_t *error                       = NULL;
	libvmdk_extent_descriptor_t *extent_descriptor = NULL;
	int result                                     = 0;
	int utf8_filename_is_set                       = 0;

	/* Initialize test
	 */
	result = libvmdk_extent_descriptor_initialize(
	          &extent_descriptor,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "extent_descriptor",
	 extent_descriptor );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libvmdk_extent_descriptor_get_utf8_filename(
	          extent_descriptor,
	          utf8_filename,
	          512,
	          &error );

	VMDK_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_filename_is_set = result;

	/* Test error cases
	 */
	result = libvmdk_extent_descriptor_get_utf8_filename(
	          NULL,
	          utf8_filename,
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

	if( utf8_filename_is_set != 0 )
	{
		result = libvmdk_extent_descriptor_get_utf8_filename(
		          extent_descriptor,
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

		result = libvmdk_extent_descriptor_get_utf8_filename(
		          extent_descriptor,
		          utf8_filename,
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

		result = libvmdk_extent_descriptor_get_utf8_filename(
		          extent_descriptor,
		          utf8_filename,
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
	/* Clean up
	 */
	result = libvmdk_internal_extent_descriptor_free(
	          (libvmdk_internal_extent_descriptor_t **) &extent_descriptor,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "extent_descriptor",
	 extent_descriptor );

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
	if( extent_descriptor != NULL )
	{
		libvmdk_internal_extent_descriptor_free(
		 (libvmdk_internal_extent_descriptor_t **) &extent_descriptor,
		 NULL );
	}
	return( 0 );
}

/* Tests the libvmdk_extent_descriptor_get_utf16_filename_size function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_extent_descriptor_get_utf16_filename_size(
     void )
{
	libcerror_error_t *error                       = NULL;
	libvmdk_extent_descriptor_t *extent_descriptor = NULL;
	size_t utf16_filename_size                     = 0;
	int result                                     = 0;
	int utf16_filename_size_is_set                 = 0;

	/* Initialize test
	 */
	result = libvmdk_extent_descriptor_initialize(
	          &extent_descriptor,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "extent_descriptor",
	 extent_descriptor );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libvmdk_extent_descriptor_get_utf16_filename_size(
	          extent_descriptor,
	          &utf16_filename_size,
	          &error );

	VMDK_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_filename_size_is_set = result;

	/* Test error cases
	 */
	result = libvmdk_extent_descriptor_get_utf16_filename_size(
	          NULL,
	          &utf16_filename_size,
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

	if( utf16_filename_size_is_set != 0 )
	{
		result = libvmdk_extent_descriptor_get_utf16_filename_size(
		          extent_descriptor,
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
	/* Clean up
	 */
	result = libvmdk_internal_extent_descriptor_free(
	          (libvmdk_internal_extent_descriptor_t **) &extent_descriptor,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "extent_descriptor",
	 extent_descriptor );

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
	if( extent_descriptor != NULL )
	{
		libvmdk_internal_extent_descriptor_free(
		 (libvmdk_internal_extent_descriptor_t **) &extent_descriptor,
		 NULL );
	}
	return( 0 );
}

/* Tests the libvmdk_extent_descriptor_get_utf16_filename function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_extent_descriptor_get_utf16_filename(
     void )
{
	uint16_t utf16_filename[ 512 ];

	libcerror_error_t *error                       = NULL;
	libvmdk_extent_descriptor_t *extent_descriptor = NULL;
	int result                                     = 0;
	int utf16_filename_is_set                      = 0;

	/* Initialize test
	 */
	result = libvmdk_extent_descriptor_initialize(
	          &extent_descriptor,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "extent_descriptor",
	 extent_descriptor );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libvmdk_extent_descriptor_get_utf16_filename(
	          extent_descriptor,
	          utf16_filename,
	          512,
	          &error );

	VMDK_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_filename_is_set = result;

	/* Test error cases
	 */
	result = libvmdk_extent_descriptor_get_utf16_filename(
	          NULL,
	          utf16_filename,
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

	if( utf16_filename_is_set != 0 )
	{
		result = libvmdk_extent_descriptor_get_utf16_filename(
		          extent_descriptor,
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

		result = libvmdk_extent_descriptor_get_utf16_filename(
		          extent_descriptor,
		          utf16_filename,
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

		result = libvmdk_extent_descriptor_get_utf16_filename(
		          extent_descriptor,
		          utf16_filename,
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
	/* Clean up
	 */
	result = libvmdk_internal_extent_descriptor_free(
	          (libvmdk_internal_extent_descriptor_t **) &extent_descriptor,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "extent_descriptor",
	 extent_descriptor );

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
	if( extent_descriptor != NULL )
	{
		libvmdk_internal_extent_descriptor_free(
		 (libvmdk_internal_extent_descriptor_t **) &extent_descriptor,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBVMDK_DLL_IMPORT ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc VMDK_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] VMDK_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc VMDK_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] VMDK_TEST_ATTRIBUTE_UNUSED )
#endif
{
	VMDK_TEST_UNREFERENCED_PARAMETER( argc )
	VMDK_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBVMDK_DLL_IMPORT )

	VMDK_TEST_RUN(
	 "libvmdk_extent_descriptor_initialize",
	 vmdk_test_extent_descriptor_initialize );

#endif /* defined( __GNUC__ ) && !defined( LIBVMDK_DLL_IMPORT ) */

	VMDK_TEST_RUN(
	 "libvmdk_extent_descriptor_free",
	 vmdk_test_extent_descriptor_free );

#if defined( __GNUC__ ) && !defined( LIBVMDK_DLL_IMPORT )

	/* TODO: add tests for libvmdk_extent_descriptor_read */

	VMDK_TEST_RUN(
	 "libvmdk_extent_descriptor_get_type",
	 vmdk_test_extent_descriptor_get_type );

	/* TODO: add tests for libvmdk_extent_descriptor_get_range */

	VMDK_TEST_RUN(
	 "libvmdk_extent_descriptor_get_utf8_filename_size",
	 vmdk_test_extent_descriptor_get_utf8_filename_size );

	VMDK_TEST_RUN(
	 "libvmdk_extent_descriptor_get_utf8_filename",
	 vmdk_test_extent_descriptor_get_utf8_filename );

	VMDK_TEST_RUN(
	 "libvmdk_extent_descriptor_get_utf16_filename_size",
	 vmdk_test_extent_descriptor_get_utf16_filename_size );

	VMDK_TEST_RUN(
	 "libvmdk_extent_descriptor_get_utf16_filename",
	 vmdk_test_extent_descriptor_get_utf16_filename );

#endif /* defined( __GNUC__ ) && !defined( LIBVMDK_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

