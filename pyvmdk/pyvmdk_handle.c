/*
 * Python object wrapper of libvmdk_handle_t
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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyvmdk_error.h"
#include "pyvmdk_extent_descriptor.h"
#include "pyvmdk_extent_descriptors.h"
#include "pyvmdk_file_object_io_handle.h"
#include "pyvmdk_file_objects_io_pool.h"
#include "pyvmdk_handle.h"
#include "pyvmdk_integer.h"
#include "pyvmdk_libbfio.h"
#include "pyvmdk_libcerror.h"
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

LIBVMDK_EXTERN \
int libvmdk_handle_open_extent_data_files_file_io_pool(
     libvmdk_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     libvmdk_error_t **error );

#endif /* !defined( LIBVMDK_HAVE_BFIO ) */

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
	  "Opens a handle using the descriptor file." },

	{ "open_file_object",
	  (PyCFunction) pyvmdk_handle_open_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_object(file_object, mode='r') -> None\n"
	  "\n"
	  "Opens a handle using a file-like object of the descriptor file." },

	{ "open_extent_data_files",
	  (PyCFunction) pyvmdk_handle_open_extent_data_files,
	  METH_NOARGS,
	  "open_extent_data_files() -> None\n"
	  "\n"
	  "Opens the extent data files." },

	{ "open_extent_data_files_as_file_objects",
	  (PyCFunction) pyvmdk_handle_open_extent_data_files_as_file_objects,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_extent_data_files_as_file_objects(file_objects) -> None\n"
	  "\n"
	  "Opens extent data files using a list of file-like objects." },

	{ "set_parent",
	  (PyCFunction) pyvmdk_handle_set_parent,
	  METH_VARARGS | METH_KEYWORDS,
	  "set_parent(parent_handle) -> None\n"
	  "\n"
	  "Sets the parent handle." },

	{ "close",
	  (PyCFunction) pyvmdk_handle_close,
	  METH_NOARGS,
	  "close() -> None\n"
	  "\n"
	  "Closes a handle." },

	{ "read_buffer",
	  (PyCFunction) pyvmdk_handle_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer(size) -> String\n"
	  "\n"
	  "Reads a buffer of data." },

	{ "read_buffer_at_offset",
	  (PyCFunction) pyvmdk_handle_read_buffer_at_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer_at_offset(size, offset) -> String\n"
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

	/* Functions to access the metadata */

	{ "get_disk_type",
	  (PyCFunction) pyvmdk_handle_get_disk_type,
	  METH_NOARGS,
	  "get_type() -> Integer\n"
	  "\n"
	  "Retrieves the disk type." },

	{ "get_media_size",
	  (PyCFunction) pyvmdk_handle_get_media_size,
	  METH_NOARGS,
	  "get_media_size() -> Integer\n"
	  "\n"
	  "Retrieves the size of the media data." },

	{ "get_content_identifier",
	  (PyCFunction) pyvmdk_handle_get_content_identifier,
	  METH_NOARGS,
	  "get_type() -> Integer\n"
	  "\n"
	  "Retrieves the content identifier." },

	{ "get_parent_content_identifier",
	  (PyCFunction) pyvmdk_handle_get_parent_content_identifier,
	  METH_NOARGS,
	  "get_type() -> Integer\n"
	  "\n"
	  "Retrieves the parent content identifier." },

	{ "get_parent_filename",
	  (PyCFunction) pyvmdk_handle_get_parent_filename,
	  METH_NOARGS,
	  "get_parent_filename() -> Unicode string or None\n"
	  "\n"
	  "Retrieves the parent filename." },

	/* Functions to access the extent descriptors */

	{ "get_number_of_extents",
	  (PyCFunction) pyvmdk_handle_get_number_of_extents,
	  METH_NOARGS,
	  "get_number_of_extents() -> Integer\n"
	  "\n"
	  "Retrieves the number of extents" },

	{ "get_extent_descriptor",
	  (PyCFunction) pyvmdk_handle_get_extent_descriptor,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_extent_descriptor(extent_index) -> Object or None\n"
	  "\n"
	  "Retrieves a specific extent descriptor" },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyvmdk_handle_object_get_set_definitions[] = {

	{ "disk_type",
	  (getter) pyvmdk_handle_get_disk_type,
	  (setter) 0,
	  "The disk type.",
	  NULL },

	{ "media_size",
	  (getter) pyvmdk_handle_get_media_size,
	  (setter) 0,
	  "The media size.",
	  NULL },

	{ "content_identifier",
	  (getter) pyvmdk_handle_get_content_identifier,
	  (setter) 0,
	  "The content identifier.",
	  NULL },

	{ "parent_content_identifier",
	  (getter) pyvmdk_handle_get_parent_content_identifier,
	  (setter) 0,
	  "The parent content identifier.",
	  NULL },

	{ "parent_filename",
	  (getter) pyvmdk_handle_get_parent_filename,
	  (setter) 0,
	  "The parent filename.",
	  NULL },

	{ "number_of_extents",
	  (getter) pyvmdk_handle_get_number_of_extents,
	  (setter) 0,
	  "The number of extents",
	  NULL },

	{ "extent_descriptors",
	  (getter) pyvmdk_handle_get_extent_descriptors,
	  (setter) 0,
	  "The extent descriptors",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyvmdk_handle_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

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

/* Initializes a handle object
 * Returns 0 if successful or -1 on error
 */
int pyvmdk_handle_init(
     pyvmdk_handle_t *pyvmdk_handle )
{
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
	pyvmdk_handle->handle         = NULL;
	pyvmdk_handle->file_io_handle = NULL;
	pyvmdk_handle->file_io_pool   = NULL;

	if( libvmdk_handle_initialize(
	     &( pyvmdk_handle->handle ),
	     &error ) != 1 )
	{
		pyvmdk_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize handle.",
		 function );

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
	libcerror_error_t *error    = NULL;
	struct _typeobject *ob_type = NULL;
	static char *function       = "pyvmdk_handle_free";
	int result                  = 0;

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid handle.",
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
	ob_type = Py_TYPE(
	           pyvmdk_handle );

	if( ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: missing ob_type.",
		 function );

		return;
	}
	if( ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	if( ( pyvmdk_handle->file_io_handle != NULL )
	 || ( pyvmdk_handle->file_io_pool != NULL ) )
	{
		if( pyvmdk_handle_close(
		     pyvmdk_handle,
		     NULL ) == NULL )
		{
			return;
		}
	}
	if( pyvmdk_handle->handle != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libvmdk_handle_free(
		          &( pyvmdk_handle->handle ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyvmdk_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libvmdk handle.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	if( pyvmdk_handle->parent_handle_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyvmdk_handle->parent_handle_object );
	}
	ob_type->tp_free(
	 (PyObject*) pyvmdk_handle );
}

/* Signals the handle to abort the current activity
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_signal_abort(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments PYVMDK_ATTRIBUTE_UNUSED )
{
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
		pyvmdk_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to signal abort.",
		 function );

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
	PyObject *string_object      = NULL;
	libcerror_error_t *error     = NULL;
	static char *function        = "pyvmdk_handle_open";
	static char *keyword_list[]  = { "filename", "mode", NULL };
	const char *filename_narrow  = NULL;
	char *mode                   = NULL;
	int result                   = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	const wchar_t *filename_wide = NULL;
#else
	PyObject *utf8_string_object = NULL;
#endif

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. This will also fail if the default encoding is not set correctly. We cannot use "u" here either since that
	 * does not allow us to pass non Unicode string objects and Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|s",
	     keyword_list,
	     &string_object,
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
	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		pyvmdk_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type unicode.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
#if PY_MAJOR_VERSION >= 3 && PY_MINOR_VERSION >= 3
		filename_wide = (wchar_t *) PyUnicode_AsWideCharString(
		                             string_object,
		                             NULL );
#else
		filename_wide = (wchar_t *) PyUnicode_AsUnicode(
		                             string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libvmdk_handle_open_wide(
		          pyvmdk_handle->handle,
	                  filename_wide,
		          LIBVMDK_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

#if PY_MAJOR_VERSION >= 3 && PY_MINOR_VERSION >= 3
		PyMem_Free(
		 filename_wide );
#endif
#else
		utf8_string_object = PyUnicode_AsUTF8String(
		                      string_object );

		if( utf8_string_object == NULL )
		{
			pyvmdk_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to convert unicode string to UTF-8.",
			 function );

			return( NULL );
		}
#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
				   utf8_string_object );
#else
		filename_narrow = PyString_AsString(
				   utf8_string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libvmdk_handle_open(
		          pyvmdk_handle->handle,
	                  filename_narrow,
		          LIBVMDK_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

		Py_DecRef(
		 utf8_string_object );
#endif
		if( result != 1 )
		{
			pyvmdk_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to open handle.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
	result = PyObject_IsInstance(
		  string_object,
		  (PyObject *) &PyBytes_Type );
#else
	result = PyObject_IsInstance(
		  string_object,
		  (PyObject *) &PyString_Type );
#endif
	if( result == -1 )
	{
		pyvmdk_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type string.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
				   string_object );
#else
		filename_narrow = PyString_AsString(
				   string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libvmdk_handle_open(
		          pyvmdk_handle->handle,
	                  filename_narrow,
		          LIBVMDK_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyvmdk_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to open handle.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type.",
	 function );

	return( NULL );
}

/* Opens a handle using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_open_file_object(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_object       = NULL;
	libcerror_error_t *error    = NULL;
	char *mode                  = NULL;
	static char *keyword_list[] = { "file_object", "mode", NULL };
	static char *function       = "pyvmdk_handle_open_file_object";
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
	PyErr_Clear();

	result = PyObject_HasAttrString(
	          file_object,
	          "read" );

	if( result != 1 )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported file object - missing read attribute.",
		 function );

		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_HasAttrString(
	          file_object,
	          "seek" );

	if( result != 1 )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported file object - missing seek attribute.",
		 function );

		return( NULL );
	}
	if( pyvmdk_handle->file_io_handle != NULL )
	{
		pyvmdk_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: invalid handle - file IO handle already set.",
		 function );

		goto on_error;
	}
	if( pyvmdk_file_object_initialize(
	     &( pyvmdk_handle->file_io_handle ),
	     file_object,
	     &error ) != 1 )
	{
		pyvmdk_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize file IO handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libvmdk_handle_open_file_io_handle(
	          pyvmdk_handle->handle,
	          pyvmdk_handle->file_io_handle,
	          LIBVMDK_OPEN_READ,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyvmdk_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to open handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );

on_error:
	if( pyvmdk_handle->file_io_handle  != NULL )
	{
		libbfio_handle_free(
		 &( pyvmdk_handle->file_io_handle ),
		 NULL );
	}
	return( NULL );
}

/* Opens the extent data files
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_open_extent_data_files(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments PYVMDK_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyvmdk_handle_open_extent_data_files";
	int result               = 0;

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libvmdk_handle_open_extent_data_files(
	          pyvmdk_handle->handle,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyvmdk_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to open extent data files.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Opens extent data files using a list of file-like objects
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_open_extent_data_files_as_file_objects(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_objects      = NULL;
	libcerror_error_t *error    = NULL;
	static char *keyword_list[] = { "file_object", NULL };
	static char *function       = "pyvmdk_handle_open_extent_data_files_as_file_objects";
	int result                  = 0;

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O",
	     keyword_list,
	     &file_objects ) == 0 )
	{
		return( NULL );
	}
	if( pyvmdk_file_objects_pool_initialize(
	     &( pyvmdk_handle->file_io_pool ),
	     file_objects,
	     LIBBFIO_OPEN_READ,
	     &error ) != 1 )
	{
		pyvmdk_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize file IO pool.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libvmdk_handle_open_extent_data_files_file_io_pool(
	          pyvmdk_handle->handle,
	          pyvmdk_handle->file_io_pool,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyvmdk_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to open extent data files.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );

on_error:
	if( pyvmdk_handle->file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &( pyvmdk_handle->file_io_pool ),
		 NULL );
	}
	return( NULL );
}

/* Sets the parent handle
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_set_parent(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	pyvmdk_handle_t *pyvmdk_parent_handle = NULL;
	libcerror_error_t *error              = NULL;
	static char *keyword_list[]           = { "parent_handle", NULL };
	static char *function                 = "pyvmdk_handle_set_parent";
	int result                            = 0;

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
	     "O!",
	     keyword_list,
	     &pyvmdk_handle_type_object,
	     &pyvmdk_parent_handle ) == 0)
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libvmdk_handle_set_parent_handle(
	          pyvmdk_handle->handle,
	          pyvmdk_parent_handle->handle,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyvmdk_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to set parent handle.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	pyvmdk_handle->parent_handle_object = pyvmdk_parent_handle;

	Py_IncRef(
	 pyvmdk_handle->parent_handle_object );

	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Closes a handle
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_close(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments PYVMDK_ATTRIBUTE_UNUSED )
{
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
		pyvmdk_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to close handle.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( pyvmdk_handle->file_io_handle != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libbfio_handle_free(
		          &( pyvmdk_handle->file_io_handle ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyvmdk_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to free libbfio file IO handle.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	if( pyvmdk_handle->file_io_pool != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libbfio_pool_free(
		          &( pyvmdk_handle->file_io_pool ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyvmdk_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to free libbfio file IO pool.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
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
	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	static char *function       = "pyvmdk_handle_read_buffer";
	static char *keyword_list[] = { "size", NULL };
	char *buffer                = NULL;
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
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
	                 NULL,
	                 read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libvmdk_handle_read_buffer(
	              pyvmdk_handle->handle,
	              (uint8_t *) buffer,
	              (size_t) read_size,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count <= -1 )
	{
		pyvmdk_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to read data.",
		 function );

		libcerror_error_free(
		 &error );

		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	/* Need to resize the string here in case read_size was not fully read.
	 */
#if PY_MAJOR_VERSION >= 3
	if( _PyBytes_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#else
	if( _PyString_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#endif
	{
		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	return( string_object );
}

/* Reads data at a specific offset
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_read_buffer_at_offset(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	static char *function       = "pyvmdk_handle_read_buffer_at_offset";
	static char *keyword_list[] = { "size", "offset", NULL };
	char *buffer                = NULL;
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
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
	                 NULL,
	                 read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libvmdk_handle_read_buffer_at_offset(
	              pyvmdk_handle->handle,
	              (uint8_t *) buffer,
	              (size_t) read_size,
	              (off64_t) read_offset,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count <= -1 )
	{
		pyvmdk_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to read data.",
		 function );

		libcerror_error_free(
		 &error );

		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	/* Need to resize the string here in case read_size was not fully read.
	 */
#if PY_MAJOR_VERSION >= 3
	if( _PyBytes_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#else
	if( _PyString_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#endif
	{
		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	return( string_object );
}

/* Seeks a certain offset in the data
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_seek_offset(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments,
           PyObject *keywords )
{
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
		pyvmdk_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to seek offset.",
		 function );

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
		pyvmdk_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve offset.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyvmdk_integer_signed_new_from_64bit(
	                  (int64_t) current_offset );

	return( integer_object );
}

/* Retrieves the disk type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_get_disk_type(
           pyvmdk_handle_t *pyvmdk_handle,
           PyObject *arguments PYVMDK_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
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
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) disk_type );
#else
	integer_object = PyInt_FromLong(
	                  (long) disk_type );
#endif
	return( integer_object );
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
		 PyExc_MemoryError,
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
			 NULL );

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
	PyObject *integer_object = NULL;
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
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_extents );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_extents );
#endif
	return( integer_object );
}

/* Retrieves a specific extent descriptor by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_handle_get_extent_descriptor_by_index(
           PyObject *pyvmdk_handle,
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
	          ( ( pyvmdk_handle_t *) pyvmdk_handle )->handle,
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
	                            (PyObject *) pyvmdk_handle,
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
	                             (PyObject *) pyvmdk_handle,
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

