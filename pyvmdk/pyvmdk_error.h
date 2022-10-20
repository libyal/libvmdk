/*
 * Error functions
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

#if !defined( _PYVMDK_ERROR_H )
#define _PYVMDK_ERROR_H

#include <common.h>
#include <types.h>

#include "pyvmdk_libcerror.h"
#include "pyvmdk_python.h"

#define PYVMDK_ERROR_STRING_SIZE	2048

#if defined( __cplusplus )
extern "C" {
#endif

void pyvmdk_error_fetch(
      libcerror_error_t **error,
      int error_domain,
      int error_code,
      const char *format_string,
      ... );

void pyvmdk_error_fetch_and_raise(
      PyObject *exception_object,
      const char *format_string,
      ... );

void pyvmdk_error_raise(
      libcerror_error_t *error,
      PyObject *exception_object,
      const char *format_string,
      ... );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYVMDK_ERROR_H ) */

