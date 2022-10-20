/*
 * Python file objects IO pool functions
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

#if !defined( _PYVMDK_FILE_OBJECTS_IO_POOL_H )
#define _PYVMDK_FILE_OBJECTS_IO_POOL_H

#include <common.h>
#include <types.h>

#include "pyvmdk_libbfio.h"
#include "pyvmdk_libcerror.h"
#include "pyvmdk_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

int pyvmdk_file_objects_pool_initialize(
     libbfio_pool_t **pool,
     PyObject *sequence_object,
     int access_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYVMDK_FILE_OBJECTS_IO_POOL_H ) */

