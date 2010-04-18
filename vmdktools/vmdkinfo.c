/*
 * Shows information obtained from a VMware Virtual Disk (VMDK) file(s)
 *
 * Copyright (c) 2009-2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <liberror.h>

#include <stdio.h>

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

/* If libtool DLL support is enabled set LIBVMDK_DLL_IMPORT
 * before including libvmdk.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBVMDK_DLL_IMPORT
#endif

#include <libvmdk.h>

#include <libsystem.h>

#include "vmdkoutput.h"

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use vmdkinfo to determine information about a VMware Virtual Disk (VMDK) file(s).\n\n" );

	fprintf( stream, "Usage: vmdkinfo [ -hvV ] vmdk_files\n\n" );

	fprintf( stream, "\tvmdk_files: the entire set of VMDK segment files or the one containing the descriptor\n\n" );

	fprintf( stream, "\t-h:         shows this help\n" );
	fprintf( stream, "\t-v:         verbose output to stderr\n" );
	fprintf( stream, "\t-V:         print version\n" );
}

/* Prints file information
 * Returns 1 if successful or -1 on error
 */
int vmdkinfo_handle_info_fprint(
     FILE *stream,
     libvmdk_handle_t *handle,
     libvmdk_error_t **error )
{
	static char *function = "vmdkinfo_handle_info_fprint";

	if( stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	fprintf(
	 stream,
	 "VMware Virtual Disk information:\n" );

	fprintf(
	 stream,
	 "\n" );

	return( 1 );
}

/* The main program
 */
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libvmdk_error_t *error                = NULL;
	libvmdk_handle_t *vmdk_handle         = NULL;
	libcstring_system_character_t *source = NULL;
	char *program                         = "vmdkinfo";
	libcstring_system_integer_t option    = 0;
	int verbose                           = 0;

	libsystem_notify_set_stream(
	 stderr,
	 NULL );
	libsystem_notify_set_verbose(
	 1 );

        if( libsystem_initialize(
             &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize system values.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	vmdkoutput_version_fprint(
	 stdout,
	 program );

	while( ( option = libsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBCSTRING_SYSTEM_STRING( "hvV" ) ) ) != (libcstring_system_integer_t) -1 )
	{
		switch( option )
		{
			case (libcstring_system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_LIBCSTRING_SYSTEM "\n",
				 argv[ optind ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (libcstring_system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libcstring_system_integer_t) 'v':
				verbose = 1;

				break;

			case (libcstring_system_integer_t) 'V':
				vmdkoutput_copyright_fprint(
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
	source = argv[ optind ];

	libsystem_notify_set_verbose(
	 verbose );
	libvmdk_notify_set_stream(
	 stderr,
	 NULL );
	libvmdk_notify_set_verbose(
	 verbose );

	if( libvmdk_handle_initialize(
	     &vmdk_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize libvmdk handle.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		libvmdk_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	/* TODO allow for multiple files */

#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
	if( libvmdk_handle_open_wide(
	     vmdk_handle,
	     source,
	     LIBVMDK_OPEN_READ,
	     &error ) != 1 )
#else
	if( libvmdk_handle_open(
	     vmdk_handle,
	     source,
	     LIBVMDK_OPEN_READ,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Error opening file(s).\n",
		 argv[ optind ] );

		libsystem_notify_print_error_backtrace(
		 error );
		libvmdk_error_free(
		 &error );

		libvmdk_handle_free(
		 &vmdk_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( vmdkinfo_handle_info_fprint(
	     stdout,
	     vmdk_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to print handle information.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		libvmdk_error_free(
		 &error );

		libvmdk_handle_free(
		 &vmdk_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( libvmdk_handle_close(
	     vmdk_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Error closing file(s).\n",
		 argv[ optind ] );

		libsystem_notify_print_error_backtrace(
		 error );
		libvmdk_error_free(
		 &error );

		libvmdk_handle_free(
		 &vmdk_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( libvmdk_handle_free(
	     &vmdk_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free libvmdk handle.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		libvmdk_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );
}

