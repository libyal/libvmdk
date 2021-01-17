/*
 * Python object wrapper of libvmdk_handle_t
 *
 * Copyright (C) 2009-2021, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYVMDK_HANDLE_H )
#define _PYVMDK_HANDLE_H

#include <common.h>
#include <types.h>

#include "pyvmdk_libbfio.h"
#include "pyvmdk_libvmdk.h"
#include "pyvmdk_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyvmdk_handle pyvmdk_handle_t;

struct pyvmdk_handle
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libvmdk handle
	 */
	libvmdk_handle_t *handle;

	/* The libbfio file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The file IO pool
	 */
	libbfio_pool_t *file_io_pool;
};

extern PyMethodDef pyvmdk_handle_object_methods[];
extern PyTypeObject pyvmdk_handle_type_object;

PyObject *pyvmdk_handle_new(
           void );

PyObject *pyvmdk_handle_new_open(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyvmdk_handle_new_open_file_object(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

int pyvmdk_handle_init(
     pyvmdk_handle_t *pyvmdk_handle );

void pyvmdk_handle_free(
      pyvmdk_handle_t *pyvmdk_handle );

PyObject *pyvmdk_handle_signal_abort(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments );

PyObject *pyvmdk_handle_open(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyvmdk_handle_open_file_object(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyvmdk_handle_open_extent_data_files(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments );

PyObject *pyvmdk_handle_open_extent_data_files_as_file_objects(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyvmdk_handle_set_parent(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyvmdk_handle_close(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments );

PyObject *pyvmdk_handle_read_buffer(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyvmdk_handle_read_buffer_at_offset(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyvmdk_handle_seek_offset(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyvmdk_handle_get_offset(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments );

PyObject *pyvmdk_handle_get_disk_type(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments );

PyObject *pyvmdk_handle_get_media_size(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments );

PyObject *pyvmdk_handle_get_content_identifier(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments );

PyObject *pyvmdk_handle_get_parent_content_identifier(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments );

PyObject *pyvmdk_handle_get_parent_filename(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments );

PyObject *pyvmdk_handle_get_number_of_extents(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments );

PyObject *pyvmdk_handle_get_extent_descriptor_by_index(
           PyObject *pyvmdk_handle,
           int extent_index );

PyObject *pyvmdk_handle_get_extent_descriptor(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyvmdk_handle_get_extent_descriptors(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYVMDK_HANDLE_H ) */

