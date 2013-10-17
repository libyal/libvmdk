/*
 * Python object definition of the libvmdk handle
 *
 * Copyright (c) 2009-2013, Joachim Metz <joachim.metz@gmail.com>
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

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyvmdk.h"
#include "pyvmdk_file_object_io_handle.h"
#include "pyvmdk_handle.h"
#include "pyvmdk_integer.h"
#include "pyvmdk_libbfio.h"
#include "pyvmdk_libcerror.h"
#include "pyvmdk_libclocale.h"
#include "pyvmdk_libcstring.h"
#include "pyvmdk_libvmdk.h"
#include "pyvmdk_python.h"
#include "pyvmdk_unused.h"

#if !defined( LIBVMDK_HAVE_BFIO )
LIBVMDK_EXTERN \
int libvmdk_handle_open_file_io_handle(
     libvmdk_handle_t *handle,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libvmdk_error_t **error );
#endif

PyMethodDef pyvmdk_handle_object_methods[] = {

	{ "signal_abort",
	  (PyCFunction) pyvmdk_handle_signal_abort,
	  METH_NOARGS,
	  "signal_abort() -> None\n"
	  "\n"
	  "Signals the handle to abort the current activity." },

	/* Functions to access the handle */

	{ "open",
	  (PyCFunction) pyvmdk_handle_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, mode='r') -> None\n"
	  "\n"
	  "Opens a VMDK image handle using the descriptor file." },

	{ "open_file_object",
	  (PyCFunction) pyvmdk_handle_open_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_object(file_object, mode='r') -> None\n"
	  "\n"
	  "Opens a VMDK image handle using a file-like object of the descriptor file." },

	{ "close",
	  (PyCFunction) pyvmdk_handle_close,
	  METH_NOARGS,
	  "close() -> None\n"
	  "\n"
	  "Closes a VMDK image handle." },

	{ "read_buffer",
	  (PyCFunction) pyvmdk_handle_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer(size) -> String\n"
	  "\n"
	  "Reads a buffer of data." },

	{ "read_random",
	  (PyCFunction) pyvmdk_handle_read_random,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_random(size, offset) -> String\n"
	  "\n"
	  "Reads a buffer of data at a specific offset." },

	{ "seek_offset",
	  (PyCFunction) pyvmdk_handle_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek_offset(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the data." },

	{ "get_offset",
	  (PyCFunction) pyvmdk_handle_get_offset,
	  METH_NOARGS,
	  "get_offset() -> Integer\n"
	  "\n"
	  "Retrieved the current offset within the data." },

	/* Some Pythonesque aliases */

	{ "read",
	  (PyCFunction) pyvmdk_handle_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read(size) -> String\n"
	  "\n"
	  "Reads a buffer of data." },

	{ "seek",
	  (PyCFunction) pyvmdk_handle_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the data." },

	{ "tell",
	  (PyCFunction) pyvmdk_handle_get_offset,
	  METH_NOARGS,
	  "tell() -> Integer\n"
	  "\n"
	  "Retrieves the current offset within the data." },

	/* Functions to access the handle values */

	{ "get_media_size",
	  (PyCFunction) pyvmdk_handle_get_media_size,
	  METH_NOARGS,
	  "get_media_size() -> Integer\n"
	  "\n"
	  "Retrieves the size of the media data." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyvmdk_handle_object_get_set_definitions[] = {

	{ "size",
	  (getter) pyvmdk_handle_get_media_size,
	  (setter) 0,
	  "The media size.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyvmdk_handle_type_object = {
	PyObject_HEAD_INIT( NULL )

	/* ob_size */
	0,
	/* tp_name */
	"pyvmdk.handle",
	/* tp_basicsize */
	sizeof( pyvmdk_handle_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyvmdk_handle_free,
	/* tp_print */
	0,
	/* tp_getattr */
	0,
	/* tp_setattr */
	0,
	/* tp_compare */
	0,
	/* tp_repr */
	0,
	/* tp_as_number */
	0,
	/* tp_as_sequence */
	0,
	/* tp_as_mapping */
	0,
	/* tp_hash */
	0,
	/* tp_call */
	0,
	/* tp_str */
	0,
	/* tp_getattro */
	0,
	/* tp_setattro */
	0,
	/* tp_as_buffer */
	0,
        /* tp_flags */
	Py_TPFLAGS_DEFAULT,
	/* tp_doc */
	"pyvmdk handle object (wraps libvmdk_handle_t)",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	0,
	/* tp_iternext */
	0,
	/* tp_methods */
	pyvmdk_handle_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyvmdk_handle_object_get_set_definitions,
	/* tp_base */
	0,
	/* tp_dict */
	0,
	/* tp_descr_get */
	0,
	/* tp_descr_set */
	0,
	/* tp_dictoffset */
	0,
	/* tp_init */
	(initproc) pyvmdk_handle_init,
	/* tp_alloc */
	0,
	/* tp_new */
	0,
	/* tp_free */
	0,
	/* tp_is_gc */
	0,
	/* tp_bases */
	NULL,
	/* tp_mro */
	NULL,
	/* tp_cache */
	NULL,
	/* tp_subclasses */
	NULL,
	/* tp_weaklist */
	NULL,
	/* tp_del */
	0
};

/* Creates a new handle object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_new(
           void )
{
	pyvmdk_handle_t *pyvmdk_handle = NULL;
	static char *function          = "pyvmdk_handle_new";

	pyvmdk_handle = PyObject_New(
	                 struct pyvmdk_handle,
	                 &pyvmdk_handle_type_object );

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize handle.",
		 function );

		goto on_error;
	}
	if( pyvmdk_handle_init(
	     pyvmdk_handle ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize handle.",
		 function );

		goto on_error;
	}
	return( (PyObject *) pyvmdk_handle );

on_error:
	if( pyvmdk_handle != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyvmdk_handle );
	}
	return( NULL );
}

/* Creates a new handle object and opens it
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_new_open(
           PyObject *self PYVMDK_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *pyvmdk_handle = NULL;

	PYVMDK_UNREFERENCED_PARAMETER( self )

	pyvmdk_handle = pyvmdk_handle_new();

	pyvmdk_handle_open(
	 (pyvmdk_handle_t *) pyvmdk_handle,
	 arguments,
	 keywords );

	return( pyvmdk_handle );
}

/* Creates a new handle object and opens it
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_new_open_file_object(
           PyObject *self PYVMDK_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *pyvmdk_handle = NULL;

	PYVMDK_UNREFERENCED_PARAMETER( self )

	pyvmdk_handle = pyvmdk_handle_new();

	pyvmdk_handle_open_file_object(
	 (pyvmdk_handle_t *) pyvmdk_handle,
	 arguments,
	 keywords );

	return( pyvmdk_handle );
}

/* Intializes a handle object
 * Returns 0 if successful or -1 on error
 */
int pyvmdk_handle_init(
     pyvmdk_handle_t *pyvmdk_handle )
{
	char error_string[ PYVMDK_ERROR_STRING_SIZE ];

	static char *function    = "pyvmdk_handle_init";
	libcerror_error_t *error = NULL;

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	/* Make sure libvmdk handle is set to NULL
	 */
	pyvmdk_handle->handle = NULL;

	if( libvmdk_handle_initialize(
	     &( pyvmdk_handle->handle ),
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYVMDK_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to initialize handle.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to initialize handle.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 0 );
}

/* Frees a handle object
 */
void pyvmdk_handle_free(
      pyvmdk_handle_t *pyvmdk_handle )
{
	char error_string[ PYVMDK_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function    = "pyvmdk_handle_free";
	int result               = 0;

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid handle.",
		 function );

		return;
	}
	if( pyvmdk_handle->ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid handle - missing ob_type.",
		 function );

		return;
	}
	if( pyvmdk_handle->ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid handle - invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	if( pyvmdk_handle->handle == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid handle - missing libvmdk handle.",
		 function );

		return;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libvmdk_handle_free(
	          &( pyvmdk_handle->handle ),
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYVMDK_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to free libvmdk handle.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to free libvmdk handle.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );
	}
	pyvmdk_handle->ob_type->tp_free(
	 (PyObject*) pyvmdk_handle );
}

/* Signals the handle to abort the current activity
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_signal_abort(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments PYVMDK_ATTRIBUTE_UNUSED )
{
	char error_string[ PYVMDK_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function    = "pyvmdk_handle_signal_abort";
	int result               = 0;

	PYVMDK_UNREFERENCED_PARAMETER( arguments )

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libvmdk_handle_signal_abort(
	          pyvmdk_handle->handle,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYVMDK_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to signal abort.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to signal abort.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Opens a handle
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_open(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYVMDK_ERROR_STRING_SIZE ];

	libcerror_error_t *error    = NULL;
	char *filename              = NULL;
	char *mode                  = NULL;
	static char *keyword_list[] = { "filename", "mode", NULL };
	static char *function       = "pyvmdk_handle_open";
	int result                  = 0;

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s|s",
	     keyword_list,
	     &filename,
	     &mode ) == 0 )
        {
                return( NULL );
        }
	if( ( mode != NULL )
	 && ( mode[ 0 ] != 'r' ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported mode: %s.",
		 function,
		 mode );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libvmdk_handle_open(
	          pyvmdk_handle->handle,
                  filename,
                  LIBVMDK_OPEN_READ,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYVMDK_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to open handle.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to open handle.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Opens a handle using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_open_file_object(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYVMDK_ERROR_STRING_SIZE ];

	PyObject *file_object            = NULL;
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	char *mode                       = NULL;
	static char *keyword_list[]      = { "file_object", "mode", NULL };
	static char *function            = "pyvmdk_handle_open_file_object";
	int result                       = 0;

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|s",
	     keyword_list,
	     &file_object,
	     &mode ) == 0 )
        {
                return( NULL );
        }
	if( ( mode != NULL )
	 && ( mode[ 0 ] != 'r' ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported mode: %s.",
		 function,
		 mode );

		return( NULL );
	}
	if( pyvmdk_file_object_initialize(
	     &file_io_handle,
	     file_object,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYVMDK_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to initialize file IO handle.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to initialize file IO handle.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libvmdk_handle_open_file_io_handle(
	          pyvmdk_handle->handle,
                  file_io_handle,
                  LIBVMDK_OPEN_READ,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYVMDK_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to open handle.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to open handle.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( NULL );
}

/* Closes a handle
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_close(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments PYVMDK_ATTRIBUTE_UNUSED )
{
	char error_string[ PYVMDK_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function    = "pyvmdk_handle_close";
	int result               = 0;

	PYVMDK_UNREFERENCED_PARAMETER( arguments )

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libvmdk_handle_close(
	          pyvmdk_handle->handle,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 0 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYVMDK_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to close handle.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to close handle.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Reads data at the current offset into a buffer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_read_buffer(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYVMDK_ERROR_STRING_SIZE ];

	libcerror_error_t *error    = NULL;
	PyObject *result_data       = NULL;
	static char *function       = "pyvmdk_handle_read_buffer";
	static char *keyword_list[] = { "size", NULL };
	ssize_t read_count          = 0;
	int read_size               = -1;

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|i",
	     keyword_list,
	     &read_size ) == 0 )
	{
		return( NULL );
	}
	if( read_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into the memory buffer
	 */
	if( read_size > INT_MAX )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value exceeds maximum.",
		 function );

		return( NULL );
	}
	result_data = PyString_FromStringAndSize(
	               NULL,
	               read_size );

	Py_BEGIN_ALLOW_THREADS

	read_count = libvmdk_handle_read_buffer(
	              pyvmdk_handle->handle,
	              PyString_AsString(
	               result_data ),
	              (size_t) read_size,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count != (ssize_t) read_size )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYVMDK_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to read data.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to read data.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	return( result_data );
}

/* Reads data at a specific offset
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_read_random(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYVMDK_ERROR_STRING_SIZE ];

	libcerror_error_t *error    = NULL;
	PyObject *result_data       = NULL;
	static char *function       = "pyvmdk_handle_read_random";
	static char *keyword_list[] = { "size", "offset", NULL };
	off64_t read_offset         = 0;
	ssize_t read_count          = 0;
	int read_size               = 0;

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i|L",
	     keyword_list,
	     &read_size,
	     &read_offset ) == 0 )
	{
		return( NULL );
	}
	if( read_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into the memory buffer
	 */
	if( read_size > INT_MAX )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value exceeds maximum.",
		 function );

		return( NULL );
	}
	if( read_offset < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read offset value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into the memory buffer
	 */
	result_data = PyString_FromStringAndSize(
	               NULL,
	               read_size );

	Py_BEGIN_ALLOW_THREADS

	read_count = libvmdk_handle_read_random(
	              pyvmdk_handle->handle,
	              PyString_AsString(
	               result_data ),
	              (size_t) read_size,
	              (off64_t) read_offset,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count != (ssize_t) read_size )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYVMDK_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to read data.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to read data.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	return( result_data );
}

/* Seeks a certain offset in the data
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_seek_offset(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYVMDK_ERROR_STRING_SIZE ];

	libcerror_error_t *error    = NULL;
	static char *function       = "pyvmdk_handle_seek_offset";
	static char *keyword_list[] = { "offset", "whence", NULL };
	off64_t offset              = 0;
	int whence                  = 0;

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "L|i",
	     keyword_list,
	     &offset,
	     &whence ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	offset = libvmdk_handle_seek_offset(
	          pyvmdk_handle->handle,
	          offset,
	          whence,
	          &error );

	Py_END_ALLOW_THREADS

 	if( offset == -1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYVMDK_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to seek offset.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to seek offset.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Retrieves the current offset in the data
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_get_offset(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments PYVMDK_ATTRIBUTE_UNUSED )
{
	char error_string[ PYVMDK_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyvmdk_handle_get_offset";
	off64_t current_offset   = 0;
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

	result = libvmdk_handle_get_offset(
	          pyvmdk_handle->handle,
	          &current_offset,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYVMDK_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve offset.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve offset.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyvmdk_integer_signed_new_from_64bit(
	                  (int64_t) current_offset );

	return( integer_object );
}

/* Retrieves the media size
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_get_media_size(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments PYVMDK_ATTRIBUTE_UNUSED )
{
	char error_string[ PYVMDK_ERROR_STRING_SIZE ];

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
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYVMDK_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: failed to retrieve media size.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: failed to retrieve media size.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyvmdk_integer_unsigned_new_from_64bit(
	                  (uint64_t) media_size );

	return( integer_object );
}

