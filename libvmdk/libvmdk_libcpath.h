/*
 * The internal libcpath header
 *
 * Copyright (C) 2009-2022, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBVMDK_LIBCPATH_H )
#define _LIBVMDK_LIBCPATH_H

#include <common.h>

/* Define HAVE_LOCAL_LIBCPATH for local use of libcpath
 */
#if defined( HAVE_LOCAL_LIBCPATH )

#include <libcpath_definitions.h>
#include <libcpath_path.h>

#else

/* If libtool DLL support is enabled set LIBCPATH_DLL_IMPORT
 * before including libcpath.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBCPATH_DLL_IMPORT
#endif

#include <libcpath.h>

#endif /* defined( HAVE_LOCAL_LIBCPATH ) */

#endif /* !defined( _LIBVMDK_LIBCPATH_H ) */

