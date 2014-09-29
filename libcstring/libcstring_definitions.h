/*
 * The internal definitions
 *
 * Copyright (c) 2010-2014, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBCSTRING_INTERNAL_DEFINITIONS_H )
#define _LIBCSTRING_INTERNAL_DEFINITIONS_H

#include <common.h>

/* Define HAVE_LOCAL_LIBCSTRING for local use of libcstring
 */
#if !defined( HAVE_LOCAL_LIBCSTRING )
#include <libcstring/definitions.h>

/* The definitions in <libcstring/definitions.h> are copied here
 * for local use of libcstring
 */
#else

/* Detect if the code is being compiled with Windows Unicode support
 */
#if defined( WINAPI ) && ( defined( _UNICODE ) || defined( UNICODE ) )
#define LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER           1
#endif

#define LIBCSTRING_VERSION				20140505

/* The libcstring version string
 */
#define LIBCSTRING_VERSION_STRING			"20140505"

#if defined( WINAPI ) && ( defined( _UNICODE ) || defined( UNICODE ) )
#define PRIc_LIBCSTRING_SYSTEM				"lc"
#define PRIs_LIBCSTRING_SYSTEM				"ls"

#else
#define PRIc_LIBCSTRING_SYSTEM				"c"
#define PRIs_LIBCSTRING_SYSTEM				"s"

#endif

#endif

#endif

