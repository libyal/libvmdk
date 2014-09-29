/*
 * Type definitions
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

#if !defined( _LIBCSTRING_TYPES_H )
#define _LIBCSTRING_TYPES_H

#include <common.h>
#include <types.h>

#if defined( _cplusplus )
extern "C" {
#endif

/* Define HAVE_LOCAL_LIBCSTRING for local use of libcstring
 * The definitions in <libcstring/types.h> are copied here
 * for local use of libcstring
 */
#if defined( HAVE_LOCAL_LIBCSTRING )

#if defined( WINAPI ) && ( defined( _UNICODE ) || defined( UNICODE ) )

/* The system character type is wide
 * A system string contains either UTF-16 or UTF-32
 */
typedef wchar_t libcstring_system_character_t;
typedef wint_t libcstring_system_integer_t;

#else

/* The system character type is narrow
 * A system string contains either UTF-8 or extended ASCII with a codepage
 */
typedef char libcstring_system_character_t;
typedef int libcstring_system_integer_t;

#endif

#endif

#if defined( _cplusplus )
}
#endif

#endif

