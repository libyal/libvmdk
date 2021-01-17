/*
 * Shows information obtained from a VMware Virtual Disk (VMDK) file
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
#include <memory.h>
#include <system_string.h>
#include <types.h>

#include <stdio.h>

#if defined( HAVE_IO_H ) || defined( WINAPI )
#include <io.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#include "info_handle.h"
#include "vmdktools_getopt.h"
#include "vmdktools_libcerror.h"
#include "vmdktools_libclocale.h"
#include "vmdktools_libcnotify.h"
#include "vmdktools_libvmdk.h"
#include "vmdktools_output.h"
#include "vmdktools_signal.h"
#include "vmdktools_unused.h"

info_handle_t *vmdkinfo_info_handle = NULL;
int vmdkinfo_abort                  = 0;

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use vmdkinfo to determine information about a VMware Virtual Disk (VMDK)\n"
	                 "image file.\n\n" );

	fprintf( stream, "Usage: vmdkinfo [ -hvV ] source\n\n" );

	fprintf( stream, "\tsource: the source file(s)\n\n" );

	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
}

/* Signal handler for vmdkinfo
 */
void vmdkinfo_signal_handler(
      vmdktools_signal_t signal VMDKTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function   = "vmdkinfo_signal_handler";

	VMDKTOOLS_UNREFERENCED_PARAMETER( signal )

	vmdkinfo_abort = 1;

	if( vmdkinfo_info_handle != NULL )
	{
		if( info_handle_signal_abort(
		     vmdkinfo_info_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal info handle to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	if( _close(
	     0 ) != 0 )
#else
	if( close(
	     0 ) != 0 )
#endif
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	system_character_t * const *source_filenames = NULL;
	libvmdk_error_t *error                       = NULL;
	char *program                                = "vmdkinfo";
	system_integer_t option                      = 0;
	int number_of_source_filenames               = 0;
	int verbose                                  = 0;

	libcnotify_stream_set(
	 stderr,
	 NULL );
	libcnotify_verbose_set(
	 1 );

	if( libclocale_initialize(
             "vmdktools",
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize locale values.\n" );

		goto on_error;
	}
        if( vmdktools_output_initialize(
             _IONBF,
             &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize output settings.\n" );

		goto on_error;
	}
	vmdktools_output_version_fprint(
	 stdout,
	 program );

	while( ( option = vmdktools_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "hvV" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				vmdktools_output_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file(s).\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	source_filenames           = &( argv[ optind ] );
	number_of_source_filenames = argc - optind;

	libcnotify_verbose_set(
	 verbose );
	libvmdk_notify_set_stream(
	 stderr,
	 NULL );
	libvmdk_notify_set_verbose(
	 verbose );

	if( info_handle_initialize(
	     &vmdkinfo_info_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize info handle.\n" );

		goto on_error;
	}
	if( info_handle_open_input(
	     vmdkinfo_info_handle,
	     source_filenames,
	     number_of_source_filenames,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open source file(s).\n" );

		goto on_error;
	}
	if( info_handle_file_fprint(
	     vmdkinfo_info_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to print file information.\n" );

		goto on_error;
	}
	if( info_handle_close(
	     vmdkinfo_info_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close info handle.\n" );

		goto on_error;
	}
	if( info_handle_free(
	     &vmdkinfo_info_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free info handle.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( vmdkinfo_info_handle != NULL )
	{
		info_handle_free(
		 &vmdkinfo_info_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

