/*
 * Library descriptor_file type test program
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

#include "../libvmdk/libvmdk_descriptor_file.h"

#if defined( __GNUC__ ) && !defined( LIBVMDK_DLL_IMPORT )

/* Tests the libvmdk_descriptor_file_initialize function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_descriptor_file_initialize(
     void )
{
	libcerror_error_t *error                   = NULL;
	libvmdk_descriptor_file_t *descriptor_file = NULL;
	int result                                 = 0;

#if defined( HAVE_VMDK_TEST_MEMORY )
	int number_of_malloc_fail_tests            = 1;
	int number_of_memset_fail_tests            = 1;
	int test_number                            = 0;
#endif

	/* Test regular cases
	 */
	result = libvmdk_descriptor_file_initialize(
	          &descriptor_file,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "descriptor_file",
	 descriptor_file );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libvmdk_descriptor_file_free(
	          &descriptor_file,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "descriptor_file",
	 descriptor_file );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libvmdk_descriptor_file_initialize(
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

	descriptor_file = (libvmdk_descriptor_file_t *) 0x12345678UL;

	result = libvmdk_descriptor_file_initialize(
	          &descriptor_file,
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

	descriptor_file = NULL;

#if defined( HAVE_VMDK_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libvmdk_descriptor_file_initialize with malloc failing
		 */
		vmdk_test_malloc_attempts_before_fail = test_number;

		result = libvmdk_descriptor_file_initialize(
		          &descriptor_file,
		          &error );

		if( vmdk_test_malloc_attempts_before_fail != -1 )
		{
			vmdk_test_malloc_attempts_before_fail = -1;

			if( descriptor_file != NULL )
			{
				libvmdk_descriptor_file_free(
				 &descriptor_file,
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
			 "descriptor_file",
			 descriptor_file );

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
		/* Test libvmdk_descriptor_file_initialize with memset failing
		 */
		vmdk_test_memset_attempts_before_fail = test_number;

		result = libvmdk_descriptor_file_initialize(
		          &descriptor_file,
		          &error );

		if( vmdk_test_memset_attempts_before_fail != -1 )
		{
			vmdk_test_memset_attempts_before_fail = -1;

			if( descriptor_file != NULL )
			{
				libvmdk_descriptor_file_free(
				 &descriptor_file,
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
			 "descriptor_file",
			 descriptor_file );

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
	if( descriptor_file != NULL )
	{
		libvmdk_descriptor_file_free(
		 &descriptor_file,
		 NULL );
	}
	return( 0 );
}

/* Tests the libvmdk_descriptor_file_free function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_descriptor_file_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libvmdk_descriptor_file_free(
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

/* Tests the libvmdk_descriptor_file_get_number_of_extents function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_descriptor_file_get_number_of_extents(
     void )
{
	libcerror_error_t *error                   = NULL;
	libvmdk_descriptor_file_t *descriptor_file = NULL;
	int number_of_extents                      = 0;
	int number_of_extents_is_set               = 0;
	int result                                 = 0;

	/* Initialize test
	 */
	result = libvmdk_descriptor_file_initialize(
	          &descriptor_file,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "descriptor_file",
	 descriptor_file );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libvmdk_descriptor_file_get_number_of_extents(
	          descriptor_file,
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
	result = libvmdk_descriptor_file_get_number_of_extents(
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
		result = libvmdk_descriptor_file_get_number_of_extents(
		          descriptor_file,
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
	result = libvmdk_descriptor_file_free(
	          &descriptor_file,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "descriptor_file",
	 descriptor_file );

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
	if( descriptor_file != NULL )
	{
		libvmdk_descriptor_file_free(
		 &descriptor_file,
		 NULL );
	}
	return( 0 );
}

/* Tests the libvmdk_descriptor_file_get_parent_content_identifier function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_descriptor_file_get_parent_content_identifier(
     void )
{
	libcerror_error_t *error                   = NULL;
	libvmdk_descriptor_file_t *descriptor_file = NULL;
	uint32_t parent_content_identifier         = 0;
	int parent_content_identifier_is_set       = 0;
	int result                                 = 0;

	/* Initialize test
	 */
	result = libvmdk_descriptor_file_initialize(
	          &descriptor_file,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "descriptor_file",
	 descriptor_file );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libvmdk_descriptor_file_get_parent_content_identifier(
	          descriptor_file,
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
	result = libvmdk_descriptor_file_get_parent_content_identifier(
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
		result = libvmdk_descriptor_file_get_parent_content_identifier(
		          descriptor_file,
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
	result = libvmdk_descriptor_file_free(
	          &descriptor_file,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "descriptor_file",
	 descriptor_file );

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
	if( descriptor_file != NULL )
	{
		libvmdk_descriptor_file_free(
		 &descriptor_file,
		 NULL );
	}
	return( 0 );
}

/* Tests the libvmdk_descriptor_file_get_utf8_parent_filename_size function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_descriptor_file_get_utf8_parent_filename_size(
     void )
{
	libcerror_error_t *error                   = NULL;
	libvmdk_descriptor_file_t *descriptor_file = NULL;
	size_t utf8_parent_filename_size           = 0;
	int result                                 = 0;
	int utf8_parent_filename_size_is_set       = 0;

	/* Initialize test
	 */
	result = libvmdk_descriptor_file_initialize(
	          &descriptor_file,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "descriptor_file",
	 descriptor_file );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libvmdk_descriptor_file_get_utf8_parent_filename_size(
	          descriptor_file,
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
	result = libvmdk_descriptor_file_get_utf8_parent_filename_size(
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
		result = libvmdk_descriptor_file_get_utf8_parent_filename_size(
		          descriptor_file,
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
	result = libvmdk_descriptor_file_free(
	          &descriptor_file,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "descriptor_file",
	 descriptor_file );

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
	if( descriptor_file != NULL )
	{
		libvmdk_descriptor_file_free(
		 &descriptor_file,
		 NULL );
	}
	return( 0 );
}

/* Tests the libvmdk_descriptor_file_get_utf8_parent_filename function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_descriptor_file_get_utf8_parent_filename(
     void )
{
	uint8_t utf8_parent_filename[ 512 ];

	libcerror_error_t *error                   = NULL;
	libvmdk_descriptor_file_t *descriptor_file = NULL;
	int result                                 = 0;
	int utf8_parent_filename_is_set            = 0;

	/* Initialize test
	 */
	result = libvmdk_descriptor_file_initialize(
	          &descriptor_file,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "descriptor_file",
	 descriptor_file );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libvmdk_descriptor_file_get_utf8_parent_filename(
	          descriptor_file,
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
	result = libvmdk_descriptor_file_get_utf8_parent_filename(
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
		result = libvmdk_descriptor_file_get_utf8_parent_filename(
		          descriptor_file,
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

		result = libvmdk_descriptor_file_get_utf8_parent_filename(
		          descriptor_file,
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

		result = libvmdk_descriptor_file_get_utf8_parent_filename(
		          descriptor_file,
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
	/* Clean up
	 */
	result = libvmdk_descriptor_file_free(
	          &descriptor_file,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "descriptor_file",
	 descriptor_file );

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
	if( descriptor_file != NULL )
	{
		libvmdk_descriptor_file_free(
		 &descriptor_file,
		 NULL );
	}
	return( 0 );
}

/* Tests the libvmdk_descriptor_file_get_utf16_parent_filename_size function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_descriptor_file_get_utf16_parent_filename_size(
     void )
{
	libcerror_error_t *error                   = NULL;
	libvmdk_descriptor_file_t *descriptor_file = NULL;
	size_t utf16_parent_filename_size          = 0;
	int result                                 = 0;
	int utf16_parent_filename_size_is_set      = 0;

	/* Initialize test
	 */
	result = libvmdk_descriptor_file_initialize(
	          &descriptor_file,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "descriptor_file",
	 descriptor_file );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libvmdk_descriptor_file_get_utf16_parent_filename_size(
	          descriptor_file,
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
	result = libvmdk_descriptor_file_get_utf16_parent_filename_size(
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
		result = libvmdk_descriptor_file_get_utf16_parent_filename_size(
		          descriptor_file,
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
	result = libvmdk_descriptor_file_free(
	          &descriptor_file,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "descriptor_file",
	 descriptor_file );

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
	if( descriptor_file != NULL )
	{
		libvmdk_descriptor_file_free(
		 &descriptor_file,
		 NULL );
	}
	return( 0 );
}

/* Tests the libvmdk_descriptor_file_get_utf16_parent_filename function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_descriptor_file_get_utf16_parent_filename(
     void )
{
	uint16_t utf16_parent_filename[ 512 ];

	libcerror_error_t *error                   = NULL;
	libvmdk_descriptor_file_t *descriptor_file = NULL;
	int result                                 = 0;
	int utf16_parent_filename_is_set           = 0;

	/* Initialize test
	 */
	result = libvmdk_descriptor_file_initialize(
	          &descriptor_file,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "descriptor_file",
	 descriptor_file );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libvmdk_descriptor_file_get_utf16_parent_filename(
	          descriptor_file,
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
	result = libvmdk_descriptor_file_get_utf16_parent_filename(
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
		result = libvmdk_descriptor_file_get_utf16_parent_filename(
		          descriptor_file,
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

		result = libvmdk_descriptor_file_get_utf16_parent_filename(
		          descriptor_file,
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

		result = libvmdk_descriptor_file_get_utf16_parent_filename(
		          descriptor_file,
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
	/* Clean up
	 */
	result = libvmdk_descriptor_file_free(
	          &descriptor_file,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "descriptor_file",
	 descriptor_file );

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
	if( descriptor_file != NULL )
	{
		libvmdk_descriptor_file_free(
		 &descriptor_file,
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
	 "libvmdk_descriptor_file_initialize",
	 vmdk_test_descriptor_file_initialize );

	VMDK_TEST_RUN(
	 "libvmdk_descriptor_file_free",
	 vmdk_test_descriptor_file_free );

	/* TODO: add tests for libvmdk_descriptor_file_read */

	/* TODO: add tests for libvmdk_descriptor_file_read_string */

	/* TODO: add tests for libvmdk_descriptor_file_read_signature */

	/* TODO: add tests for libvmdk_descriptor_file_read_header */

	/* TODO: add tests for libvmdk_descriptor_file_read_extents */

	/* TODO: add tests for libvmdk_descriptor_file_read_change_tracking_file */

	/* TODO: add tests for libvmdk_descriptor_file_read_disk_database */

	VMDK_TEST_RUN(
	 "libvmdk_descriptor_file_get_number_of_extents",
	 vmdk_test_descriptor_file_get_number_of_extents );

	/* TODO: add tests for libvmdk_descriptor_file_get_extent_by_index */

	VMDK_TEST_RUN(
	 "libvmdk_descriptor_file_get_parent_content_identifier",
	 vmdk_test_descriptor_file_get_parent_content_identifier );

	VMDK_TEST_RUN(
	 "libvmdk_descriptor_file_get_utf8_parent_filename_size",
	 vmdk_test_descriptor_file_get_utf8_parent_filename_size );

	VMDK_TEST_RUN(
	 "libvmdk_descriptor_file_get_utf8_parent_filename",
	 vmdk_test_descriptor_file_get_utf8_parent_filename );

	VMDK_TEST_RUN(
	 "libvmdk_descriptor_file_get_utf16_parent_filename_size",
	 vmdk_test_descriptor_file_get_utf16_parent_filename_size );

	VMDK_TEST_RUN(
	 "libvmdk_descriptor_file_get_utf16_parent_filename",
	 vmdk_test_descriptor_file_get_utf16_parent_filename );

#endif /* defined( __GNUC__ ) && !defined( LIBVMDK_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

