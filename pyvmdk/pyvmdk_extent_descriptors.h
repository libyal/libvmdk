/*
 * Python object definition of the extent descriptors sequence and iterator
 *
 * Copyright (C) 2009-2015, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYVMDK_EXTENT_DESCRIPTORS_H )
#define _PYVMDK_EXTENT_DESCRIPTORS_H

#include <common.h>
#include <types.h>

#include "pyvmdk_handle.h"
#include "pyvmdk_libvmdk.h"
#include "pyvmdk_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyvmdk_extent_descriptors pyvmdk_extent_descriptors_t;

struct pyvmdk_extent_descriptors
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The handle object
	 */
	pyvmdk_handle_t *handle_object;

	/* The get extent descriptor by index callback function
	 */
	PyObject* (*get_extent_descriptor_by_index)(
	             pyvmdk_handle_t *handle_object,
	             int extent_index );

	/* The (current) extent index
	 */
	int extent_index;

	/* The number of extents
	 */
	int number_of_extents;
};

extern PyTypeObject pyvmdk_extent_descriptors_type_object;

PyObject *pyvmdk_extent_descriptors_new(
           pyvmdk_handle_t *handle_object,
           PyObject* (*get_extent_descriptor_by_index)(
                        pyvmdk_handle_t *handle_object,
                        int extent_index ),
           int number_of_extents );

int pyvmdk_extent_descriptors_init(
     pyvmdk_extent_descriptors_t *pyvmdk_extent_descriptors );

void pyvmdk_extent_descriptors_free(
      pyvmdk_extent_descriptors_t *pyvmdk_extent_descriptors );

Py_ssize_t pyvmdk_extent_descriptors_len(
            pyvmdk_extent_descriptors_t *pyvmdk_extent_descriptors );

PyObject *pyvmdk_extent_descriptors_getitem(
           pyvmdk_extent_descriptors_t *pyvmdk_extent_descriptors,
           Py_ssize_t item_index );

PyObject *pyvmdk_extent_descriptors_iter(
           pyvmdk_extent_descriptors_t *pyvmdk_extent_descriptors );

PyObject *pyvmdk_extent_descriptors_iternext(
           pyvmdk_extent_descriptors_t *pyvmdk_extent_descriptors );

#if defined( __cplusplus )
}
#endif

#endif

