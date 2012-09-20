/*
 * Virtual Hard Disk (VHD) library read testing program
 *
 * Copyright (c) 2012, Joachim Metz <joachim.metz@gmail.com>
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

#include "vhdi_test_libcstring.h"
#include "vhdi_test_libvhdi.h"

#define VHDI_TEST_READ_BUFFER_SIZE	4096

/* Tests libvhdi_file_seek_offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int vhdi_test_seek_offset(
     libvhdi_file_t *file,
     off64_t input_offset,
     int input_whence,
     off64_t output_offset )
{
	libvhdi_error_t *error = NULL;
	off64_t result_offset   = 0;
	int result              = 0;

	if( file == NULL )
	{
		return( -1 );
	}
	result_offset = libvhdi_file_seek_offset(
	                 file,
	                 input_offset,
	                 input_whence,
	                 &error );

	if( result_offset == -1 )
	{
		libvhdi_error_backtrace_fprint(
		 error,
		 stderr );

		libvhdi_error_free(
		 &error );
	}
	if( result_offset == -1 )
	{
		libvhdi_error_backtrace_fprint(
		 error,
		 stderr );

		libvhdi_error_free(
		 &error );
	}
	if( result_offset != output_offset )
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
	return( result );
}

/* Tests libvhdi_file_read_buffer
 * Returns 1 if successful, 0 if not or -1 on error
 */
int vhdi_test_read_buffer(
     libvhdi_file_t *file,
     size64_t input_size,
     size64_t output_size )
{
	uint8_t buffer[ VHDI_TEST_READ_BUFFER_SIZE ];

	libvhdi_error_t *error = NULL;
	size64_t remaining_size = 0;
	size64_t result_size    = 0;
	size_t read_size        = 0;
	ssize_t read_count      = 0;
	int result              = 0;

	if( file == NULL )
	{
		return( -1 );
	}
	remaining_size = input_size;

	while( remaining_size > 0 )
	{
		read_size = VHDI_TEST_READ_BUFFER_SIZE;

		if( remaining_size < (size64_t) read_size )
		{
			read_size = (size_t) remaining_size;
		}
		read_count = libvhdi_file_read_buffer(
			      file,
			      buffer,
			      read_size,
			      &error );

		if( read_count < 0 )
		{
			libvhdi_error_backtrace_fprint(
			 error,
			 stderr );

			libvhdi_error_free(
			 &error );

			break;
		}
		remaining_size -= (size64_t) read_count;
		result_size    += (size64_t) read_count;

		if( read_count != (ssize_t) read_size )
		{
			break;
		}
	}
	if( output_size == result_size )
	{
		result = 1;
	}
	else
	{
		fprintf(
		 stderr,
		 "Unexpected read count: %" PRIu64 "\n",
		 result_size );
	}
	return( result );
}

/* Tests reading data at a specific offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int vhdi_test_read(
     libvhdi_file_t *file,
     off64_t input_offset,
     int input_whence,
     size64_t input_size,
     off64_t output_offset,
     size64_t output_size )
{
	const char *whence_string = NULL;
	int result                = 0;

	if( file == NULL )
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
	 "Testing reading range with offset: %" PRIi64 ", whence: %s and size: %" PRIu64 "\t",
	 input_offset,
	 whence_string,
	 input_size );

	result = vhdi_test_seek_offset(
	          file,
	          input_offset,
	          input_whence,
	          output_offset );

	if( result == 1 )
	{
		if( input_offset >= 0 )
		{
			result = vhdi_test_read_buffer(
				  file,
				  input_size,
				  output_size );
		}
	}
	if( result != 0 )
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

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libvhdi_error_t *error = NULL;
	libvhdi_file_t *file   = NULL;
	size64_t media_size    = 0;

	if( argc < 2 )
	{
		fprintf(
		 stderr,
		 "Missing filename.\n" );

		return( EXIT_FAILURE );
	}
	/* Initialization
	 */
	if( libvhdi_file_initialize(
	     &file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create file.\n" );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libvhdi_file_open_wide(
	     file,
	     argv[ 1 ],
	     LIBVHDI_OPEN_READ,
	     &error ) != 1 )
#else
	if( libvhdi_file_open(
	     file,
	     argv[ 1 ],
	     LIBVHDI_OPEN_READ,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to open file.\n" );

		goto on_error;
	}
	if( libvhdi_file_get_media_size(
	     file,
	     &media_size,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve media size.\n" );

		goto on_error;
	}
	if( media_size > (size64_t) INT64_MAX )
	{
		fprintf(
		 stderr,
		 "Media size exceeds maximum.\n" );

		goto on_error;
	}
	fprintf(
	 stdout,
	 "Media size: %" PRIu64 " bytes\n",
	 media_size );

	/* Case 0: test full read
	 */

	/* Test: offset: 0 size: <media_size>
	 * Expected result: offset: 0 size: <media_size>
	 */
	if( vhdi_test_read(
	     file,
	     0,
	     SEEK_SET,
	     media_size,
	     0,
	     media_size ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read.\n" );

		goto on_error;
	}
	/* Test: offset: 0 size: <media_size>
	 * Expected result: offset: 0 size: <media_size>
	 */
	if( vhdi_test_read(
	     file,
	     0,
	     SEEK_SET,
	     media_size,
	     0,
	     media_size ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read.\n" );

		goto on_error;
	}

	/* Case 1: test random read
	 */

	/* Test: offset: <media_size / 7> size: <media_size / 2>
	 * Expected result: offset: <media_size / 7> size: <media_size / 2>
	 */
	if( vhdi_test_read(
	     file,
	     (off64_t) ( media_size / 7 ),
	     SEEK_SET,
	     media_size / 2,
	     (off64_t) ( media_size / 7 ),
	     media_size / 2 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read.\n" );

		goto on_error;
	}
	/* Test: offset: <media_size / 7> size: <media_size / 2>
	 * Expected result: offset: <media_size / 7> size: <media_size / 2>
	 */
	if( vhdi_test_read(
	     file,
	     (off64_t) ( media_size / 7 ),
	     SEEK_SET,
	     media_size / 2,
	     (off64_t) ( media_size / 7 ),
	     media_size / 2 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read.\n" );

		goto on_error;
	}

	/* Case 2: test read beyond media size
	 */

	if( media_size < 1024 )
	{
		/* Test: offset: <media_size - 1024> size: 4096
		 * Expected result: offset: -1 size: <undetermined>
		 */
		if( vhdi_test_read(
		     file,
		     (off64_t) ( media_size - 1024 ),
		     SEEK_SET,
		     4096,
		     -1,
		     (size64_t) -1 ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read.\n" );

			goto on_error;
		}
		/* Test: offset: <media_size - 1024> size: 4096
		 * Expected result: offset: -1 size: <undetermined>
		 */
		if( vhdi_test_read(
		     file,
		     (off64_t) ( media_size - 1024 ),
		     SEEK_SET,
		     4096,
		     -1,
		     (size64_t) -1 ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read.\n" );

			goto on_error;
		}
	}
	else
	{
		/* Test: offset: <media_size - 1024> size: 4096
		 * Expected result: offset: <media_size - 1024> size: 1024
		 */
		if( vhdi_test_read(
		     file,
		     (off64_t) ( media_size - 1024 ),
		     SEEK_SET,
		     4096,
		     (off64_t) ( media_size - 1024 ),
		     1024 ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read.\n" );

			goto on_error;
		}
		/* Test: offset: <media_size - 1024> size: 4096
		 * Expected result: offset: <media_size - 1024> size: 1024
		 */
		if( vhdi_test_read(
		     file,
		     (off64_t) ( media_size - 1024 ),
		     SEEK_SET,
		     4096,
		     (off64_t) ( media_size - 1024 ),
		     1024 ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read.\n" );

			goto on_error;
		}
	}
	/* Clean up
	 */
	if( libvhdi_file_close(
	     file,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close file(s).\n" );

		goto on_error;
	}
	if( libvhdi_file_free(
	     &file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free file.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libvhdi_error_backtrace_fprint(
		 error,
		 stderr );
		libvhdi_error_free(
		 &error );
	}
	if( file != NULL )
	{
		libvhdi_file_close(
		 file,
		 NULL );
		libvhdi_file_free(
		 &file,
		 NULL );
	}
	return( EXIT_FAILURE );
}

