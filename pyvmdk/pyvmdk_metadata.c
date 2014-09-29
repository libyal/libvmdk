/*
 *  Metadata functions for the Python object definition of the libvmdk handle
 *
 * Copyright (c) 2009-2014, Joachim Metz <joachim.metz@gmail.com>
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

#include <common.h>
#include <types.h>

#include "pyvmdk_error.h"
#include "pyvmdk_extent_descriptor.h"
#include "pyvmdk_extent_descriptors.h"
#include "pyvmdk_handle.h"
#include "pyvmdk_integer.h"
#include "pyvmdk_libcerror.h"
#include "pyvmdk_libvmdk.h"
#include "pyvmdk_python.h"
#include "pyvmdk_unused.h"

/* Retrieves the disk type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_get_disk_type(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments PYVMDK_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyvmdk_handle_get_disk_type";
	int result               = 0;
	int disk_type            = 0;

	PYVMDK_UNREFERENCED_PARAMETER( arguments )

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libvmdk_handle_get_disk_type(
	          pyvmdk_handle->handle,
	          &disk_type,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyvmdk_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve disk type.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	return( PyInt_FromLong(
	         (long) disk_type ) );
}

/* Retrieves the media size
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_get_media_size(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments PYVMDK_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyvmdk_handle_get_media_size";
	size64_t media_size      = 0;
	int result               = 0;

	PYVMDK_UNREFERENCED_PARAMETER( arguments )

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libvmdk_handle_get_media_size(
	          pyvmdk_handle->handle,
	          &media_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyvmdk_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: failed to retrieve media size.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyvmdk_integer_unsigned_new_from_64bit(
	                  (uint64_t) media_size );

	return( integer_object );
}

/* Retrieves the content identifier
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_get_content_identifier(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments PYVMDK_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error    = NULL;
	PyObject *integer_object    = NULL;
	static char *function       = "pyvmdk_handle_get_content_identifier";
	uint32_t content_identifier = 0;
	int result                  = 0;

	PYVMDK_UNREFERENCED_PARAMETER( arguments )

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libvmdk_handle_get_content_identifier(
	          pyvmdk_handle->handle,
	          &content_identifier,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyvmdk_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve content identifier.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyvmdk_integer_unsigned_new_from_64bit(
	                  content_identifier );

	return( integer_object );
}

/* Retrieves the parent content identifier
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_get_parent_content_identifier(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments PYVMDK_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error           = NULL;
	PyObject *integer_object           = NULL;
	static char *function              = "pyvmdk_handle_get_parent_content_identifier";
	uint32_t parent_content_identifier = 0;
	int result                         = 0;

	PYVMDK_UNREFERENCED_PARAMETER( arguments )

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libvmdk_handle_get_parent_content_identifier(
	          pyvmdk_handle->handle,
	          &parent_content_identifier,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyvmdk_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve parent content identifier.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyvmdk_integer_unsigned_new_from_64bit(
	                  parent_content_identifier );

	return( integer_object );
}

/* Retrieves the parent filename
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_get_parent_filename(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments PYVMDK_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	const char *errors          = NULL;
	uint8_t *parent_filename    = NULL;
	static char *function       = "pyvmdk_handle_get_parent_filename";
	size_t parent_filename_size = 0;
	int result                  = 0;

	PYVMDK_UNREFERENCED_PARAMETER( arguments )

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libvmdk_handle_get_utf8_parent_filename_size(
	          pyvmdk_handle->handle,
	          &parent_filename_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyvmdk_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve parent filename size.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( parent_filename_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	parent_filename = (uint8_t *) PyMem_Malloc(
	                               sizeof( uint8_t ) * parent_filename_size );

	if( parent_filename == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to create parent filename.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libvmdk_handle_get_utf8_parent_filename(
		  pyvmdk_handle->handle,
		  parent_filename,
		  parent_filename_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyvmdk_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve parent filename.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	string_object = PyUnicode_DecodeUTF8(
			 (char *) parent_filename,
			 (Py_ssize_t) parent_filename_size - 1,
			 errors );

	PyMem_Free(
	 parent_filename );

	return( string_object );

on_error:
	if( parent_filename != NULL )
	{
		PyMem_Free(
		 parent_filename );
	}
	return( NULL );
}

/* Retrieves the number of extents
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_get_number_of_extents(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments PYVMDK_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyvmdk_handle_get_number_of_extents";
	int number_of_extents    = 0;
	int result               = 0;

	PYVMDK_UNREFERENCED_PARAMETER( arguments )

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libvmdk_handle_get_number_of_extents(
	          pyvmdk_handle->handle,
	          &number_of_extents,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyvmdk_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of extents.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	return( PyInt_FromLong(
	         (long) number_of_extents ) );
}

/* Retrieves a specific extent descriptor by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_get_extent_descriptor_by_index(
           pyvmdk_handle_t *pyvmdk_handle,
           int extent_index )
{
	libcerror_error_t *error                       = NULL;
	libvmdk_extent_descriptor_t *extent_descriptor = NULL;
	PyObject *extent_descriptor_object             = NULL;
	static char *function                          = "pyvmdk_handle_get_extent_descriptor_by_index";
	int result                                     = 0;

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libvmdk_handle_get_extent_descriptor(
	          pyvmdk_handle->handle,
	          extent_index,
	          &extent_descriptor,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyvmdk_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve extent: %d descriptor.",
		 function,
		 extent_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	extent_descriptor_object = pyvmdk_extent_descriptor_new(
	                            extent_descriptor,
	                            pyvmdk_handle );

	if( extent_descriptor_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create extent descriptor object.",
		 function );

		goto on_error;
	}
	return( extent_descriptor_object );

on_error:
	if( extent_descriptor != NULL )
	{
		libvmdk_extent_descriptor_free(
		 &extent_descriptor,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific extent descriptor
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_get_extent_descriptor(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *extent_descriptor_object = NULL;
	static char *keyword_list[]        = { "extent_index", NULL };
	int extent_index                   = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &extent_index ) == 0 )
	{
		return( NULL );
	}
	extent_descriptor_object = pyvmdk_handle_get_extent_descriptor_by_index(
	                            pyvmdk_handle,
	                            extent_index );

	return( extent_descriptor_object );
}

/* Retrieves an extent descriptors sequence and iterator object for the extent descriptors
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_get_extent_descriptors(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments PYVMDK_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error            = NULL;
	PyObject *extent_descriptors_object = NULL;
	static char *function               = "pyvmdk_handle_get_extent_descriptors";
	int number_of_extent_descriptors    = 0;
	int result                          = 0;

	PYVMDK_UNREFERENCED_PARAMETER( arguments )

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libvmdk_handle_get_number_of_extents(
	          pyvmdk_handle->handle,
	          &number_of_extent_descriptors,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyvmdk_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of extents.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	extent_descriptors_object = pyvmdk_extent_descriptors_new(
	                             pyvmdk_handle,
	                             &pyvmdk_handle_get_extent_descriptor_by_index,
	                             number_of_extent_descriptors );

	if( extent_descriptors_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create extent descriptors object.",
		 function );

		return( NULL );
	}
	return( extent_descriptors_object );
}

