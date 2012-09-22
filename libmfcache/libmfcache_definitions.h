/*
 * The internal definitions
 *
 * Copyright (c) 2010-2012, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBMFCACHE_INTERNAL_DEFINITIONS_H )
#define _LIBMFCACHE_INTERNAL_DEFINITIONS_H

#include <common.h>

/* Define HAVE_LOCAL_LIBMFCACHE for local use of libmfcache
 */
#if !defined( HAVE_LOCAL_LIBMFCACHE )
#include <libmfcache/definitions.h>

/* The definitions in <libmfcache/definitions.h> are copied here
 * for local use of libmfcache
 */
#else
#define LIBMFCACHE_VERSION					20120701

/* The libmfcache version string
 */
#define LIBMFCACHE_VERSION_STRING				"20120701"

/* The cache value flags definitions
 */
enum LIBMFCACHE_CACHE_VALUE_FLAGS
{
	/* The cache value is not managed by the library
	 */
	LIBMFCACHE_CACHE_VALUE_FLAG_NON_MANAGED			= 0x00,

	/* The cache value is managed by the library
	 */
	LIBMFCACHE_CACHE_VALUE_FLAG_MANAGED			= 0x01
};

#endif

#endif

