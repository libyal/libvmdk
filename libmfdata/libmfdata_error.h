/*
 * Error functions
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

#if !defined( _LIBMFDATA_INTERNAL_ERROR_H )
#define _LIBMFDATA_INTERNAL_ERROR_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#if !defined( HAVE_LOCAL_LIBMFDATA )
#include <libmfdata/error.h>
#endif

#include "libmfdata_extern.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBMFDATA )

LIBMFDATA_EXTERN \
void libmfdata_error_free(
      libmfdata_error_t **error );

LIBMFDATA_EXTERN \
int libmfdata_error_fprint(
     libmfdata_error_t *error,
     FILE *stream );

LIBMFDATA_EXTERN \
int libmfdata_error_sprint(
     libmfdata_error_t *error,
     char *string,
     size_t size );

LIBMFDATA_EXTERN \
int libmfdata_error_backtrace_fprint(
     libmfdata_error_t *error,
     FILE *stream );

LIBMFDATA_EXTERN \
int libmfdata_error_backtrace_sprint(
     libmfdata_error_t *error,
     char *string,
     size_t size );

#endif /* !defined( HAVE_LOCAL_LIBMFDATA ) */

#if defined( __cplusplus )
}
#endif

#endif

