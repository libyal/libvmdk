/*
 * Integer functions
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

#if !defined( _PYVMDK_INTEGER_H )
#define _PYVMDK_INTEGER_H

#include <common.h>
#include <types.h>

#include "pyvmdk_libcerror.h"
#include "pyvmdk_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

PyObject *pyvmdk_integer_signed_new_from_64bit(
           int64_t value_64bit );

PyObject *pyvmdk_integer_unsigned_new_from_64bit(
           uint64_t value_64bit );

int pyvmdk_integer_signed_copy_to_64bit(
     PyObject *integer_object,
     int64_t *value_64bit,
     libcerror_error_t **error );

int pyvmdk_integer_unsigned_copy_to_64bit(
     PyObject *integer_object,
     uint64_t *value_64bit,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYVMDK_INTEGER_H ) */

