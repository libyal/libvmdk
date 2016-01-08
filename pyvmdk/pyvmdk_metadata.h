/*
 * Metadata functions for the Python object definition of the libvmdk handle
 *
 * Copyright (C) 2009-2016, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYVMDK_METADATA_H )
#define _PYVMDK_METADATA_H

#include <common.h>
#include <types.h>

#include "pyvmdk_handle.h"
#include "pyvmdk_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

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
           pyvmdk_handle_t *pyvmdk_handle,
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

#endif

