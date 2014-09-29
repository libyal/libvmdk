/*
 * Globbing functions
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

#if !defined( _LIBCSYSTEM_GLOB_H )
#define _LIBCSYSTEM_GLOB_H

#include <common.h>
#include <types.h>

#include "libcsystem_libcerror.h"
#include "libcsystem_libcstring.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_GLOB_H )
#define LIBCSYSTEM_HAVE_GLOB	1
#endif

#if !defined( LIBCSYSTEM_HAVE_GLOB )

typedef struct libcsystem_glob libcsystem_glob_t;

struct libcsystem_glob
{
	/* The number of globs resolved
	 */
	int number_of_results;

	/* The resolved globs
	 */
	libcstring_system_character_t **result;
};

int libcsystem_glob_initialize(
     libcsystem_glob_t **glob,
     libcerror_error_t **error );

int libcsystem_glob_free(
     libcsystem_glob_t **glob,
     libcerror_error_t **error );

int libcsystem_glob_resize(
     libcsystem_glob_t *glob,
     int new_number_of_results,
     libcerror_error_t **error );

int libcsystem_glob_resolve(
     libcsystem_glob_t *glob,
     libcstring_system_character_t * const patterns[],
     int number_of_patterns,
     libcerror_error_t **error );

#endif

#if defined( __cplusplus )
}
#endif

#endif

