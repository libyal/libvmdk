/*
 * Python object definition of the sequence and iterator object of extent descriptors
 *
 * Copyright (C) 2009-2023, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYVMDK_EXTENT_DESCRIPTORS_H )
#define _PYVMDK_EXTENT_DESCRIPTORS_H

#include <common.h>
#include <types.h>

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

	/* The parent object
	 */
	PyObject *parent_object;

	/* The get item by index callback function
	 */
	PyObject* (*get_item_by_index)(
	             PyObject *parent_object,
	             int index );

	/* The current index
	 */
	int current_index;

	/* The number of items
	 */
	int number_of_items;
};

extern PyTypeObject pyvmdk_extent_descriptors_type_object;

PyObject *pyvmdk_extent_descriptors_new(
           PyObject *parent_object,
           PyObject* (*get_item_by_index)(
                        PyObject *parent_object,
                        int index ),
           int number_of_items );

int pyvmdk_extent_descriptors_init(
     pyvmdk_extent_descriptors_t *sequence_object );

void pyvmdk_extent_descriptors_free(
      pyvmdk_extent_descriptors_t *sequence_object );

Py_ssize_t pyvmdk_extent_descriptors_len(
            pyvmdk_extent_descriptors_t *sequence_object );

PyObject *pyvmdk_extent_descriptors_getitem(
           pyvmdk_extent_descriptors_t *sequence_object,
           Py_ssize_t item_index );

PyObject *pyvmdk_extent_descriptors_iter(
           pyvmdk_extent_descriptors_t *sequence_object );

PyObject *pyvmdk_extent_descriptors_iternext(
           pyvmdk_extent_descriptors_t *sequence_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYVMDK_EXTENT_DESCRIPTORS_H ) */

