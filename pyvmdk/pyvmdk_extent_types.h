/*
 * Python object definition of the libvmdk extent types
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

#if !defined( _PYVMDK_EXTENT_TYPES_H )
#define _PYVMDK_EXTENT_TYPES_H

#include <common.h>
#include <types.h>

#include "pyvmdk_libvmdk.h"
#include "pyvmdk_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyvmdk_extent_types pyvmdk_extent_types_t;

struct pyvmdk_extent_types
{
	/* Python object initialization
	 */
	PyObject_HEAD
};

extern PyTypeObject pyvmdk_extent_types_type_object;

int pyvmdk_extent_types_init_type(
     PyTypeObject *type_object );

PyObject *pyvmdk_extent_types_new(
           void );

int pyvmdk_extent_types_init(
     pyvmdk_extent_types_t *definitions_object );

void pyvmdk_extent_types_free(
      pyvmdk_extent_types_t *definitions_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYVMDK_EXTENT_TYPES_H ) */

