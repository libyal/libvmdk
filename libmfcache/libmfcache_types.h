/*
 * The internal type definitions
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

#if !defined( _LIBMFCACHE_INTERNAL_TYPES_H )
#define _LIBMFCACHE_INTERNAL_TYPES_H

#include <common.h>
#include <types.h>

/* Define HAVE_LOCAL_LIBMFCACHE for local use of libmfcache
 * The definitions in <libmfcache/types.h> are copied here
 * for local use of libmfcache
 */
#if defined( HAVE_LOCAL_LIBMFCACHE )

/* The following type definitions hide internal data structures
 */
#if defined( HAVE_DEBUG_OUTPUT ) && !defined( WINAPI )
typedef struct libmfcache_cache {}	libmfcache_cache_t;

#else
typedef intptr_t libmfcache_cache_t;

#endif

#endif /* defined( HAVE_LOCAL_LIBMFCACHE ) */

#endif

