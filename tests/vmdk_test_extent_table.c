/*
 * Library extent_table type test program
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

#include "../libvmdk/libvmdk_extent_table.h"
#include "../libvmdk/libvmdk_io_handle.h"

#if defined( __GNUC__ ) && !defined( LIBVMDK_DLL_IMPORT )

/* Tests the libvmdk_extent_table_initialize function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_extent_table_initialize(
     void )
{
	libcerror_error_t *error             = NULL;
	libvmdk_extent_table_t *extent_table = NULL;
	libvmdk_io_handle_t *io_handle       = NULL;
	int result                           = 0;

#if defined( HAVE_VMDK_TEST_MEMORY )
	int number_of_malloc_fail_tests      = 1;
	int number_of_memset_fail_tests      = 1;
	int test_number                      = 0;
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
	result = libvmdk_extent_table_initialize(
	          &extent_table,
	          io_handle,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "extent_table",
	 extent_table );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libvmdk_extent_table_free(
	          &extent_table,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "extent_table",
	 extent_table );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libvmdk_extent_table_initialize(
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

	extent_table = (libvmdk_extent_table_t *) 0x12345678UL;

	result = libvmdk_extent_table_initialize(
	          &extent_table,
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

	extent_table = NULL;

	result = libvmdk_extent_table_initialize(
	          &extent_table,
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
		/* Test libvmdk_extent_table_initialize with malloc failing
		 */
		vmdk_test_malloc_attempts_before_fail = test_number;

		result = libvmdk_extent_table_initialize(
		          &extent_table,
		          io_handle,
		          &error );

		if( vmdk_test_malloc_attempts_before_fail != -1 )
		{
			vmdk_test_malloc_attempts_before_fail = -1;

			if( extent_table != NULL )
			{
				libvmdk_extent_table_free(
				 &extent_table,
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
			 "extent_table",
			 extent_table );

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
		/* Test libvmdk_extent_table_initialize with memset failing
		 */
		vmdk_test_memset_attempts_before_fail = test_number;

		result = libvmdk_extent_table_initialize(
		          &extent_table,
		          io_handle,
		          &error );

		if( vmdk_test_memset_attempts_before_fail != -1 )
		{
			vmdk_test_memset_attempts_before_fail = -1;

			if( extent_table != NULL )
			{
				libvmdk_extent_table_free(
				 &extent_table,
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
			 "extent_table",
			 extent_table );

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
	if( extent_table != NULL )
	{
		libvmdk_extent_table_free(
		 &extent_table,
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

/* Tests the libvmdk_extent_table_free function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_extent_table_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libvmdk_extent_table_free(
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

/* Tests the libvmdk_extent_table_clear function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_extent_table_clear(
     void )
{
	libcerror_error_t *error             = NULL;
	libvmdk_extent_table_t *extent_table = NULL;
	libvmdk_io_handle_t *io_handle       = NULL;
	int result                           = 0;

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

	result = libvmdk_extent_table_initialize(
	          &extent_table,
	          io_handle,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "extent_table",
	 extent_table );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libvmdk_extent_table_clear(
	          extent_table,
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
	result = libvmdk_extent_table_clear(
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

	/* Clean up
	 */
	result = libvmdk_extent_table_free(
	          &extent_table,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "extent_table",
	 extent_table );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

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
	if( extent_table != NULL )
	{
		libvmdk_extent_table_free(
		 &extent_table,
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

/* Tests the libvmdk_extent_table_clone function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_extent_table_clone(
     void )
{
	libcerror_error_t *error                         = NULL;
	libvmdk_extent_table_t *destination_extent_table = NULL;
	libvmdk_extent_table_t *source_extent_table      = NULL;
	libvmdk_io_handle_t *io_handle                   = NULL;
	int result                                       = 0;

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

	result = libvmdk_extent_table_initialize(
	          &source_extent_table,
	          io_handle,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "source_extent_table",
	 source_extent_table );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libvmdk_extent_table_clone(
	          &destination_extent_table,
	          source_extent_table,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "destination_extent_table",
	 destination_extent_table );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libvmdk_extent_table_free(
	          &destination_extent_table,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "destination_extent_table",
	 destination_extent_table );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libvmdk_extent_table_clone(
	          &destination_extent_table,
	          NULL,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "destination_extent_table",
	 destination_extent_table );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libvmdk_extent_table_clone(
	          NULL,
	          source_extent_table,
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
	result = libvmdk_extent_table_free(
	          &source_extent_table,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "source_extent_table",
	 source_extent_table );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

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
	if( destination_extent_table != NULL )
	{
		libvmdk_extent_table_free(
		 &destination_extent_table,
		 NULL );
	}
	if( source_extent_table != NULL )
	{
		libvmdk_extent_table_free(
		 &source_extent_table,
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

/* Tests the libvmdk_extent_table_get_data_files_path_size function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_extent_table_get_data_files_path_size(
     void )
{
	libcerror_error_t *error             = NULL;
	libvmdk_extent_table_t *extent_table = NULL;
	libvmdk_io_handle_t *io_handle       = NULL;
	size_t data_files_path_size          = 0;
	int data_files_path_size_is_set      = 0;
	int result                           = 0;

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

	result = libvmdk_extent_table_initialize(
	          &extent_table,
	          io_handle,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "extent_table",
	 extent_table );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libvmdk_extent_table_get_data_files_path_size(
	          extent_table,
	          &data_files_path_size,
	          &error );

	VMDK_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	data_files_path_size_is_set = result;

	/* Test error cases
	 */
	result = libvmdk_extent_table_get_data_files_path_size(
	          NULL,
	          &data_files_path_size,
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

	if( data_files_path_size_is_set != 0 )
	{
		result = libvmdk_extent_table_get_data_files_path_size(
		          extent_table,
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
	result = libvmdk_extent_table_free(
	          &extent_table,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "extent_table",
	 extent_table );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
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
	if( extent_table != NULL )
	{
		libvmdk_extent_table_free(
		 &extent_table,
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

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libvmdk_extent_table_get_data_files_path_size_wide function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_extent_table_get_data_files_path_size_wide(
     void )
{
	libcerror_error_t *error             = NULL;
	libvmdk_extent_table_t *extent_table = NULL;
	libvmdk_io_handle_t *io_handle       = NULL;
	size_t data_files_path_size_wide     = 0;
	int data_files_path_size_wide_is_set = 0;
	int result                           = 0;

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

	result = libvmdk_extent_table_initialize(
	          &extent_table,
	          io_handle,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "extent_table",
	 extent_table );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libvmdk_extent_table_get_data_files_path_size_wide(
	          extent_table,
	          &data_files_path_size_wide,
	          &error );

	VMDK_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	data_files_path_size_wide_is_set = result;

	/* Test error cases
	 */
	result = libvmdk_extent_table_get_data_files_path_size_wide(
	          NULL,
	          &data_files_path_size_wide,
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

	if( data_files_path_size_wide_is_set != 0 )
	{
		result = libvmdk_extent_table_get_data_files_path_size_wide(
		          extent_table,
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
	result = libvmdk_extent_table_free(
	          &extent_table,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "extent_table",
	 extent_table );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
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
	if( extent_table != NULL )
	{
		libvmdk_extent_table_free(
		 &extent_table,
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

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libvmdk_extent_table_get_number_of_extents function
 * Returns 1 if successful or 0 if not
 */
int vmdk_test_extent_table_get_number_of_extents(
     void )
{
	libcerror_error_t *error             = NULL;
	libvmdk_extent_table_t *extent_table = NULL;
	libvmdk_io_handle_t *io_handle       = NULL;
	int number_of_extents                = 0;
	int number_of_extents_is_set         = 0;
	int result                           = 0;

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

	result = libvmdk_extent_table_initialize(
	          &extent_table,
	          io_handle,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NOT_NULL(
	 "extent_table",
	 extent_table );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libvmdk_extent_table_get_number_of_extents(
	          extent_table,
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
	result = libvmdk_extent_table_get_number_of_extents(
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
		result = libvmdk_extent_table_get_number_of_extents(
		          extent_table,
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
	result = libvmdk_extent_table_free(
	          &extent_table,
	          &error );

	VMDK_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	VMDK_TEST_ASSERT_IS_NULL(
	 "extent_table",
	 extent_table );

	VMDK_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
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
	if( extent_table != NULL )
	{
		libvmdk_extent_table_free(
		 &extent_table,
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
	 "libvmdk_extent_table_initialize",
	 vmdk_test_extent_table_initialize );

	VMDK_TEST_RUN(
	 "libvmdk_extent_table_free",
	 vmdk_test_extent_table_free );

	VMDK_TEST_RUN(
	 "libvmdk_extent_table_clear",
	 vmdk_test_extent_table_clear );

	VMDK_TEST_RUN(
	 "libvmdk_extent_table_clone",
	 vmdk_test_extent_table_clone );

	VMDK_TEST_RUN(
	 "libvmdk_extent_table_get_data_files_path_size",
	 vmdk_test_extent_table_get_data_files_path_size );

	/* TODO: add tests for libvmdk_extent_table_get_data_files_path */

	/* TODO: add tests for libvmdk_extent_table_set_data_files_path */

#if defined( HAVE_WIDE_CHARACTER_TYPE )

	VMDK_TEST_RUN(
	 "libvmdk_extent_table_get_data_files_path_size_wide",
	 vmdk_test_extent_table_get_data_files_path_size_wide );

	/* TODO: add tests for libvmdk_extent_table_get_data_files_path_wide */

	/* TODO: add tests for libvmdk_extent_table_set_data_files_path_wide */

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

	/* TODO: add tests for libvmdk_extent_table_initialize_extents */

	VMDK_TEST_RUN(
	 "libvmdk_extent_table_get_number_of_extents",
	 vmdk_test_extent_table_get_number_of_extents );

	/* TODO: add tests for libvmdk_extent_table_get_extent_by_index */

	/* TODO: add tests for libvmdk_extent_table_get_extent_at_offset */

	/* TODO: add tests for libvmdk_extent_table_set_extent_storage_media_size_by_index */

	/* TODO: add tests for libvmdk_extent_table_get_extent_file_by_index */

	/* TODO: add tests for libvmdk_extent_table_get_extent_file_at_offset */

	/* TODO: add tests for libvmdk_extent_table_set_extent_file_by_index */

	/* TODO: add tests for libvmdk_extent_table_set_extent_by_extent_descriptor */

#endif /* defined( __GNUC__ ) && !defined( LIBVMDK_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

