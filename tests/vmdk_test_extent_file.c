/*
 * Library extent_file type test program
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
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "vmdk_test_libcerror.h"
#include "vmdk_test_libvmdk.h"
#include "vmdk_test_macros.h"
#include "vmdk_test_memory.h"
#include "vmdk_test_unused.h"

#include "../libvmdk/libvmdk_extent_file.h"
#include "../libvmdk/libvmdk_io_handle.h"

#if defined( __GNUC__ ) && !defined( LIBVMDK_DLL_IMPORT )

/* Tests the libvmdk_extent_file_initialize function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_extent_file_initialize(
     void )
{
	libcerror_error_t *error           = NULL;
	libvmdk_extent_file_t *extent_file = NULL;
	libvmdk_io_handle_t *io_handle     = NULL;
	int result                         = 0;

#if defined( HAVE_VMDK_TEST_MEMORY )
	int number_of_malloc_fail_tests    = 1;
	int number_of_memset_fail_tests    = 1;
	int test_number                    = 0;
#endif

	/* Initialize test
	 */
	result = libvmdk_io_handle_initialize(
	          &io_handle,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libvmdk_extent_file_initialize(
	          &extent_file,
	          io_handle,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "extent_file",
	 extent_file );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libvmdk_extent_file_free(
	          &extent_file,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "extent_file",
	 extent_file );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libvmdk_extent_file_initialize(
	          NULL,
	          io_handle,
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

	extent_file = (libvmdk_extent_file_t *) 0x12345678UL;

	result = libvmdk_extent_file_initialize(
	          &extent_file,
	          io_handle,
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

	extent_file = NULL;

	result = libvmdk_extent_file_initialize(
	          &extent_file,
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

#if defined( HAVE_VMDK_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libvmdk_extent_file_initialize with malloc failing
		 */
		vmdk_test_malloc_attempts_before_fail = test_number;

		result = libvmdk_extent_file_initialize(
		          &extent_file,
		          io_handle,
		          &error );

		if( vmdk_test_malloc_attempts_before_fail != -1 )
		{
			vmdk_test_malloc_attempts_before_fail = -1;

			if( extent_file != NULL )
			{
				libvmdk_extent_file_free(
				 &extent_file,
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
			 "extent_file",
			 extent_file );

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
		/* Test libvmdk_extent_file_initialize with memset failing
		 */
		vmdk_test_memset_attempts_before_fail = test_number;

		result = libvmdk_extent_file_initialize(
		          &extent_file,
		          io_handle,
		          &error );

		if( vmdk_test_memset_attempts_before_fail != -1 )
		{
			vmdk_test_memset_attempts_before_fail = -1;

			if( extent_file != NULL )
			{
				libvmdk_extent_file_free(
				 &extent_file,
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
			 "extent_file",
			 extent_file );

			VMDK_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_VMDK_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libvmdk_io_handle_free(
	          &io_handle,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

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
	if( extent_file != NULL )
	{
		libvmdk_extent_file_free(
		 &extent_file,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libvmdk_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libvmdk_extent_file_free function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_extent_file_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libvmdk_extent_file_free(
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
	 "libvmdk_extent_file_initialize",
	 vmdk_test_extent_file_initialize );

	VMDK_TEST_RUN(
	 "libvmdk_extent_file_free",
	 vmdk_test_extent_file_free );

	/* TODO: add tests for libvmdk_extent_file_check_for_empty_block */

	/* TODO: add tests for libvmdk_extent_file_read_file_header_file_io_handle */

	/* TODO: add tests for libvmdk_extent_file_read_file_header */

	/* TODO: add tests for libvmdk_extent_file_read_file_header_data */

	/* TODO: add tests for libvmdk_extent_file_read_descriptor_data_file_io_handle */

	/* TODO: add tests for libvmdk_extent_file_read_grain_directories */

	/* TODO: add tests for libvmdk_extent_file_read_grain_directory */

	/* TODO: add tests for libvmdk_extent_file_read_backup_grain_directory */

	/* TODO: add tests for libvmdk_extent_file_read_grains_scan */

	/* TODO: add tests for libvmdk_extent_file_read_element_data */

	/* TODO: add tests for libvmdk_extent_file_read_grain_group_element_data */

	/* TODO: add tests for libvmdk_extent_file_grain_group_is_sparse_at_offset */

	/* TODO: add tests for libvmdk_extent_file_get_grain_group_at_offset */

	/* TODO: add tests for libvmdk_extent_file_read_segment_data */

	/* TODO: add tests for libvmdk_extent_file_seek_segment_offset */

#endif /* defined( __GNUC__ ) && !defined( LIBVMDK_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

