/*
 * Library read testing program
 *
 * Copyright (c) 2009-2014, Joachim Metz <joachim.metz@gmail.com>
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

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

#include "vmdk_test_libcstring.h"
#include "vmdk_test_libcerror.h"
#include "vmdk_test_libvmdk.h"
#include "vmdk_test_unused.h"

/* Define to make vmdk_test_read generate verbose output
#define VMDK_TEST_READ_VERBOSE
 */

#define VMDK_TEST_READ_BUFFER_SIZE	4096

/* Tests libvmdk_handle_seek_offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int vmdk_test_seek_offset(
     libvmdk_handle_t *handle,
     off64_t input_offset,
     int input_whence,
     off64_t expected_offset )
{
	libvmdk_error_t *error = NULL;
	off64_t result_offset  = 0;
	int result             = 0;

	if( handle == NULL )
	{
		return( -1 );
	}
	result_offset = libvmdk_handle_seek_offset(
	                 handle,
	                 input_offset,
	                 input_whence,
	                 &error );

	if( expected_offset != result_offset )
	{
		fprintf(
		 stderr,
		 "Unexpected result offset: %" PRIi64 "\n",
		 result_offset );
	}
	else
	{
		result = 1;
	}
	if( error != NULL )
	{
		if( result != 1 )
		{
			libvmdk_error_backtrace_fprint(
			 error,
			 stderr );
		}
		libvmdk_error_free(
		 &error );
	}
	return( result );
}

/* Tests libvmdk_handle_read_buffer
 * Returns 1 if successful, 0 if not or -1 on error
 */
int vmdk_test_read_buffer(
     libvmdk_handle_t *handle,
     size64_t input_size,
     size64_t expected_size )
{
	uint8_t buffer[ VMDK_TEST_READ_BUFFER_SIZE ];

	libvmdk_error_t *error  = NULL;
	size64_t remaining_size = 0;
	size64_t result_size    = 0;
	size_t read_size        = 0;
	ssize_t read_count      = 0;
	int result              = 0;

	if( handle == NULL )
	{
		return( -1 );
	}
	remaining_size = input_size;

	while( remaining_size > 0 )
	{
		read_size = VMDK_TEST_READ_BUFFER_SIZE;

		if( remaining_size < (size64_t) read_size )
		{
			read_size = (size_t) remaining_size;
		}
		read_count = libvmdk_handle_read_buffer(
			      handle,
			      buffer,
			      read_size,
			      &error );

		if( read_count < 0 )
		{
			break;
		}
		remaining_size -= (size64_t) read_count;
		result_size    += (size64_t) read_count;

		if( read_count != (ssize_t) read_size )
		{
			break;
		}
	}
	if( expected_size != result_size )
	{
		fprintf(
		 stderr,
		 "Unexpected read count: %" PRIu64 "\n",
		 result_size );
	}
	else
	{
		result = 1;
	}
	if( error != NULL )
	{
		if( result != 1 )
		{
			libvmdk_error_backtrace_fprint(
			 error,
			 stderr );
		}
		libvmdk_error_free(
		 &error );
	}
	return( result );
}

/* Tests libvmdk_handle_read_buffer_at_offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int vmdk_test_read_buffer_at_offset(
     libvmdk_handle_t *handle,
     off64_t input_offset,
     size64_t input_size,
     off64_t expected_offset,
     size64_t expected_size )
{
	uint8_t buffer[ VMDK_TEST_READ_BUFFER_SIZE ];

	libvmdk_error_t *error  = NULL;
	off64_t result_offset   = 0;
	size64_t remaining_size = 0;
	size64_t result_size    = 0;
	size_t read_size        = 0;
	ssize_t read_count      = 0;
	int result              = 0;

	if( handle == NULL )
	{
		return( -1 );
	}
	remaining_size = input_size;

	fprintf(
	 stdout,
	 "Testing reading buffer at offset: %" PRIi64 " and size: %" PRIu64 "\t",
	 input_offset,
	 input_size );

	while( remaining_size > 0 )
	{
		read_size = VMDK_TEST_READ_BUFFER_SIZE;

		if( remaining_size < (size64_t) read_size )
		{
			read_size = (size_t) remaining_size;
		}
		read_count = libvmdk_handle_read_buffer_at_offset(
			      handle,
			      buffer,
			      read_size,
			      input_offset,
			      &error );

		if( read_count < 0 )
		{
			break;
		}
		input_offset   += (size64_t) read_count;
		remaining_size -= (size64_t) read_count;
		result_size    += (size64_t) read_count;

		if( read_count != (ssize_t) read_size )
		{
			break;
		}
	}
	if( libvmdk_handle_get_offset(
	     handle,
	     &result_offset,
	     &error ) != 1 )
	{
		result = -1;
	}
	if( expected_offset != result_offset )
	{
		fprintf(
		 stderr,
		 "Unexpected offset: %" PRIi64 "\n",
		 result_offset );
	}
	else if( expected_size != result_size )
	{
		fprintf(
		 stderr,
		 "Unexpected read count: %" PRIu64 "\n",
		 result_size );
	}
	else
	{
		result = 1;
	}
	if( result == 1 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( error != NULL )
	{
		if( result != 1 )
		{
			libvmdk_error_backtrace_fprint(
			 error,
			 stderr );
		}
		libvmdk_error_free(
		 &error );
	}
	return( result );
}

/* Tests reading data at a specific offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int vmdk_test_seek_offset_and_read_buffer(
     libvmdk_handle_t *handle,
     off64_t input_offset,
     int input_whence,
     size64_t input_size,
     off64_t expected_offset,
     size64_t expected_size )
{
	const char *whence_string = NULL;
	int result                = 0;

	if( handle == NULL )
	{
		return( -1 );
	}
	if( input_whence == SEEK_CUR )
	{
		whence_string = "SEEK_CUR";
	}
	else if( input_whence == SEEK_END )
	{
		whence_string = "SEEK_END";
	}
	else if( input_whence == SEEK_SET )
	{
		whence_string = "SEEK_SET";
	}
	else
	{
		whence_string = "UNKNOWN";
	}
	fprintf(
	 stdout,
	 "Testing reading buffer at offset: %" PRIi64 ", whence: %s and size: %" PRIu64 "\t",
	 input_offset,
	 whence_string,
	 input_size );

	result = vmdk_test_seek_offset(
	          handle,
	          input_offset,
	          input_whence,
	          expected_offset );

	if( result == 1 )
	{
		if( input_offset >= 0 )
		{
			result = vmdk_test_read_buffer(
				  handle,
				  input_size,
				  expected_size );
		}
	}
	if( result == 1 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	return( result );
}

/* Tests reading data from a handle
 * Returns 1 if successful, 0 if not or -1 on error
 */
int vmdk_test_read_from_handle(
     libvmdk_handle_t *handle,
     size64_t media_size )
{
	int result = 0;

	if( handle == NULL )
	{
		return( -1 );
	}
	if( media_size > (size64_t) INT64_MAX )
	{
		fprintf(
		 stderr,
		 "Volume size exceeds maximum.\n" );

		return( -1 );
	}
	/* Case 0: test full read
	 */

	/* Test: offset: 0 size: <media_size>
	 * Expected result: offset: 0 size: <media_size>
	 */
	result = vmdk_test_seek_offset_and_read_buffer(
	          handle,
	          0,
	          SEEK_SET,
	          media_size,
	          0,
	          media_size );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset and read buffer.\n" );

		return( result );
	}
	/* Test: offset: 0 size: <media_size>
	 * Expected result: offset: 0 size: <media_size>
	 */
	result = vmdk_test_seek_offset_and_read_buffer(
	          handle,
	          0,
	          SEEK_SET,
	          media_size,
	          0,
	          media_size );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset and read buffer.\n" );

		return( result );
	}

	/* Case 1: test read buffer at offset
	 */

	/* Test: offset: <media_size / 7> size: <media_size / 2>
	 * Expected result: offset: <media_size / 7> size: <media_size / 2>
	 */
	result = vmdk_test_seek_offset_and_read_buffer(
	          handle,
	          (off64_t) ( media_size / 7 ),
	          SEEK_SET,
	          media_size / 2,
	          (off64_t) ( media_size / 7 ),
	          media_size / 2 );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset and read buffer.\n" );

		return( result );
	}
	/* Test: offset: <media_size / 7> size: <media_size / 2>
	 * Expected result: offset: <media_size / 7> size: <media_size / 2>
	 */
	result = vmdk_test_seek_offset_and_read_buffer(
	          handle,
	          (off64_t) ( media_size / 7 ),
	          SEEK_SET,
	          media_size / 2,
	          (off64_t) ( media_size / 7 ),
	          media_size / 2 );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset and read buffer.\n" );

		return( result );
	}

	/* Case 2: test read beyond media size
	 */

	if( media_size < 1024 )
	{
		/* Test: offset: <media_size - 1024> size: 4096
		 * Expected result: offset: -1 size: <undetermined>
		 */
		result = vmdk_test_seek_offset_and_read_buffer(
		          handle,
		          (off64_t) ( media_size - 1024 ),
		          SEEK_SET,
		          4096,
		          -1,
		          (size64_t) -1 );

		if( result != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test seek offset and read buffer.\n" );

			return( result );
		}
		/* Test: offset: <media_size - 1024> size: 4096
		 * Expected result: offset: -1 size: <undetermined>
		 */
		result = vmdk_test_seek_offset_and_read_buffer(
		          handle,
		          (off64_t) ( media_size - 1024 ),
		          SEEK_SET,
		          4096,
		          -1,
		          (size64_t) -1 );

		if( result != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test seek offset and read buffer.\n" );

			return( result );
		}
	}
	else
	{
		/* Test: offset: <media_size - 1024> size: 4096
		 * Expected result: offset: <media_size - 1024> size: 1024
		 */
		result = vmdk_test_seek_offset_and_read_buffer(
		          handle,
		          (off64_t) ( media_size - 1024 ),
		          SEEK_SET,
		          4096,
		          (off64_t) ( media_size - 1024 ),
		          1024 );

		if( result != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read.\n" );

			return( result );
		}
		/* Test: offset: <media_size - 1024> size: 4096
		 * Expected result: offset: <media_size - 1024> size: 1024
		 */
		result = vmdk_test_seek_offset_and_read_buffer(
		          handle,
		          (off64_t) ( media_size - 1024 ),
		          SEEK_SET,
		          4096,
		          (off64_t) ( media_size - 1024 ),
		          1024 );

		if( result != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test seek offset and read buffer.\n" );

			return( result );
		}
	}
	/* Case 3: test read buffer at offset
	 */

	/* Test: offset: <media_size / 7> size: <media_size / 2>
	 * Expected result: offset: < ( media_size / 7 ) + ( media_size / 2 ) > size: <media_size / 2>
	 */
	result = vmdk_test_read_buffer_at_offset(
	          handle,
	          (off64_t) ( media_size / 7 ),
	          media_size / 2,
	          (off64_t) ( media_size / 7 ) + ( media_size / 2 ),
	          media_size / 2 );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read buffer at offset.\n" );

		return( result );
	}
	/* Test: offset: <media_size / 7> size: <media_size / 2>
	 * Expected result: offset: < ( media_size / 7 ) + ( media_size / 2 ) > size: <media_size / 2>
	 */
	result = vmdk_test_read_buffer_at_offset(
	          handle,
	          (off64_t) ( media_size / 7 ),
	          media_size / 2,
	          (off64_t) ( media_size / 7 ) + ( media_size / 2 ),
	          media_size / 2 );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read buffer at offset.\n" );

		return( result );
	}
	return( 1 );
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libvmdk_error_t *error   = NULL;
	libvmdk_handle_t *handle = NULL;
	size64_t media_size      = 0;

	if( argc < 2 )
	{
		fprintf(
		 stderr,
		 "Missing filename.\n" );

		return( EXIT_FAILURE );
	}
#if defined( HAVE_DEBUG_OUTPUT ) && defined( VMDK_TEST_READ_VERBOSE )
	libvmdk_notify_set_verbose(
	 1 );
	libvmdk_notify_set_stream(
	 stderr,
	 NULL );
#endif
	/* Initialization
	 */
	if( libvmdk_handle_initialize(
	     &handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create handle.\n" );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libvmdk_handle_open_wide(
	     handle,
	     argv[ 1 ],
	     LIBVMDK_OPEN_READ,
	     &error ) != 1 )
#else
	if( libvmdk_handle_open(
	     handle,
	     argv[ 1 ],
	     LIBVMDK_OPEN_READ,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to open handle.\n" );

		goto on_error;
	}
	if( libvmdk_handle_open_extent_data_files(
	     handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open extent data files.\n" );

		goto on_error;
	}
	if( libvmdk_handle_get_media_size(
	     handle,
	     &media_size,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve media size.\n" );

		goto on_error;
	}
	fprintf(
	 stdout,
	 "Media size: %" PRIu64 " bytes\n",
	 media_size );

	if( vmdk_test_read_from_handle(
	     handle,
	     media_size ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to read from handle.\n" );

		goto on_error;
	}
	/* Clean up
	 */
	if( libvmdk_handle_close(
	     handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close handle.\n" );

		goto on_error;
	}
	if( libvmdk_handle_free(
	     &handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free handle.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libvmdk_error_backtrace_fprint(
		 error,
		 stderr );
		libvmdk_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		libvmdk_handle_close(
		 handle,
		 NULL );
		libvmdk_handle_free(
		 &handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

