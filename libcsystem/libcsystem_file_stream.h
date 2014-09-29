/*
 * File stream functions
 *
 * Copyright (c) 2008-2014, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBCSYSTEM_FILE_STREAM_H )
#define _LIBCSYSTEM_FILE_STREAM_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "libcsystem_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
#define libcsystem_file_stream_open( filename, mode ) \
	file_stream_open_wide( filename, mode )

#else
#define libcsystem_file_stream_open( filename, mode ) \
	file_stream_open( filename, mode )

#endif

#define libcsystem_file_stream_close( stream ) \
	file_stream_close( stream )

#define libcsystem_file_stream_read( stream, data, size ) \
	file_stream_read( stream, data, size )

#define libcsystem_file_stream_write( stream, data, size ) \
	file_stream_write( stream, data, size )

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
#define libcsystem_file_stream_get_string( stream, string, size ) \
	file_stream_get_string_wide( stream, string, size )

#else
#define libcsystem_file_stream_get_string( stream, string, size ) \
	file_stream_get_string( stream, string, size )

#endif

#if defined( __cplusplus )
}
#endif

#endif

