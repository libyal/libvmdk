/*
 * Python object wrapper of libvmdk_extent_descriptor_t
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

#if !defined( _PYVMDK_EXTENT_DESCRIPTOR_H )
#define _PYVMDK_EXTENT_DESCRIPTOR_H

#include <common.h>
#include <types.h>

#include "pyvmdk_handle.h"
#include "pyvmdk_libvmdk.h"
#include "pyvmdk_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyvmdk_extent_descriptor pyvmdk_extent_descriptor_t;

struct pyvmdk_extent_descriptor
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libvmdk extent descriptor
	 */
	libvmdk_extent_descriptor_t *extent_descriptor;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pyvmdk_extent_descriptor_object_methods[];
extern PyTypeObject pyvmdk_extent_descriptor_type_object;

PyObject *pyvmdk_extent_descriptor_new(
           libvmdk_extent_descriptor_t *extent_descriptor,
           PyObject *parent_object );

int pyvmdk_extent_descriptor_init(
     pyvmdk_extent_descriptor_t *pyvmdk_extent_descriptor );

void pyvmdk_extent_descriptor_free(
      pyvmdk_extent_descriptor_t *pyvmdk_extent_descriptor );

PyObject *pyvmdk_extent_descriptor_get_type(
           pyvmdk_extent_descriptor_t *pyvmdk_extent_descriptor,
           PyObject *arguments );

PyObject *pyvmdk_extent_descriptor_get_offset(
           pyvmdk_extent_descriptor_t *pyvmdk_extent_descriptor,
           PyObject *arguments );

PyObject *pyvmdk_extent_descriptor_get_size(
           pyvmdk_extent_descriptor_t *pyvmdk_extent_descriptor,
           PyObject *arguments );

PyObject *pyvmdk_extent_descriptor_get_filename(
           pyvmdk_extent_descriptor_t *pyvmdk_extent_descriptor,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYVMDK_EXTENT_DESCRIPTOR_H ) */

