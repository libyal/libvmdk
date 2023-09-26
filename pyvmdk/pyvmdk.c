/*
 * Python bindings module for libvmdk (pyvmdk)
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
#include <narrow_string.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyvmdk.h"
#include "pyvmdk_disk_types.h"
#include "pyvmdk_error.h"
#include "pyvmdk_extent_descriptor.h"
#include "pyvmdk_extent_descriptors.h"
#include "pyvmdk_extent_types.h"
#include "pyvmdk_file_object_io_handle.h"
#include "pyvmdk_handle.h"
#include "pyvmdk_libbfio.h"
#include "pyvmdk_libcerror.h"
#include "pyvmdk_libvmdk.h"
#include "pyvmdk_python.h"
#include "pyvmdk_unused.h"

#if !defined( LIBVMDK_HAVE_BFIO )

LIBVMDK_EXTERN \
int libvmdk_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libvmdk_error_t **error );

#endif /* !defined( LIBVMDK_HAVE_BFIO ) */

/* The pyvmdk module methods
 */
PyMethodDef pyvmdk_module_methods[] = {
	{ "get_version",
	  (PyCFunction) pyvmdk_get_version,
	  METH_NOARGS,
	  "get_version() -> String\n"
	  "\n"
	  "Retrieves the version." },

	{ "check_file_signature",
	  (PyCFunction) pyvmdk_check_file_signature,
	  METH_VARARGS | METH_KEYWORDS,
	  "check_file_signature(filename) -> Boolean\n"
	  "\n"
	  "Checks if a file has a VMware Virtual Disk (VMDK) file signature." },

	{ "check_file_signature_file_object",
	  (PyCFunction) pyvmdk_check_file_signature_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "check_file_signature_file_object(file_object) -> Boolean\n"
	  "\n"
	  "Checks if a file has a VMware Virtual Disk (VMDK) file signature using a file-like object." },

	{ "open",
	  (PyCFunction) pyvmdk_open_new_handle,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, mode='r') -> Object\n"
	  "\n"
	  "Opens VMDK image handle using the descriptor file." },

	{ "open_file_object",
	  (PyCFunction) pyvmdk_open_new_handle_with_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_object(file_object, mode='r') -> Object\n"
	  "\n"
	  "Opens a VMDK image handle using a file-like object of the descriptor file." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

/* Retrieves the pyvmdk/libvmdk version
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_get_version(
           PyObject *self PYVMDK_ATTRIBUTE_UNUSED,
           PyObject *arguments PYVMDK_ATTRIBUTE_UNUSED )
{
	const char *version_string   = NULL;
	size_t version_string_length = 0;

	PYVMDK_UNREFERENCED_PARAMETER( self )
	PYVMDK_UNREFERENCED_PARAMETER( arguments )

	Py_BEGIN_ALLOW_THREADS

	version_string = libvmdk_get_version();

	Py_END_ALLOW_THREADS

	version_string_length = narrow_string_length(
	                         version_string );

	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	return( PyUnicode_DecodeUTF8(
	         version_string,
	         (Py_ssize_t) version_string_length,
	         NULL ) );
}

/* Checks if a file has a VMware Virtual Disk (VMDK) file signature
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_check_file_signature(
           PyObject *self PYVMDK_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object     = NULL;
	libcerror_error_t *error    = NULL;
	const char *filename_narrow = NULL;
	static char *function       = "pyvmdk_check_file_signature";
	static char *keyword_list[] = { "filename", NULL };
	int result                  = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	const wchar_t *filename_wide = NULL;
#else
	PyObject *utf8_string_object = NULL;
#endif

	PYVMDK_UNREFERENCED_PARAMETER( self )

	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. This will also fail if the default encoding is not set correctly. We cannot use "u" here either since that
	 * does not allow us to pass non Unicode string objects and Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|",
	     keyword_list,
	     &string_object ) == 0 )
	{
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
		 "%s: unable to determine if string object is of type Unicode.",
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

		result = libvmdk_check_file_signature_wide(
		          filename_wide,
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
			 "%s: unable to convert Unicode string to UTF-8.",
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

		result = libvmdk_check_file_signature(
		          filename_narrow,
		          &error );

		Py_END_ALLOW_THREADS

		Py_DecRef(
		 utf8_string_object );

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

		if( result == -1 )
		{
			pyvmdk_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to check file signature.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		if( result != 0 )
		{
			Py_IncRef(
			 (PyObject *) Py_True );

			return( Py_True );
		}
		Py_IncRef(
		 (PyObject *) Py_False );

		return( Py_False );
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

		result = libvmdk_check_file_signature(
		          filename_narrow,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
		{
			pyvmdk_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to check file signature.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		if( result != 0 )
		{
			Py_IncRef(
			 (PyObject *) Py_True );

			return( Py_True );
		}
		Py_IncRef(
		 (PyObject *) Py_False );

		return( Py_False );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type.",
	 function );

	return( NULL );
}

/* Checks if a file has a VMware Virtual Disk (VMDK) file signature using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_check_file_signature_file_object(
           PyObject *self PYVMDK_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_object            = NULL;
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	static char *function            = "pyvmdk_check_file_signature_file_object";
	static char *keyword_list[]      = { "file_object", NULL };
	int result                       = 0;

	PYVMDK_UNREFERENCED_PARAMETER( self )

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|",
	     keyword_list,
	     &file_object ) == 0 )
	{
		return( NULL );
	}
	if( pyvmdk_file_object_initialize(
	     &file_io_handle,
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

	result = libvmdk_check_file_signature_file_io_handle(
	          file_io_handle,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyvmdk_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to check file signature.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     &error ) != 1 )
	{
		pyvmdk_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to free file IO handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	if( result != 0 )
	{
		Py_IncRef(
		 (PyObject *) Py_True );

		return( Py_True );
	}
	Py_IncRef(
	 (PyObject *) Py_False );

	return( Py_False );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( NULL );
}

/* Creates a new handle object and opens it
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_open_new_handle(
           PyObject *self PYVMDK_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	pyvmdk_handle_t *pyvmdk_handle = NULL;
	static char *function          = "pyvmdk_open_new_handle";

	PYVMDK_UNREFERENCED_PARAMETER( self )

	/* PyObject_New does not invoke tp_init
	 */
	pyvmdk_handle = PyObject_New(
	                 struct pyvmdk_handle,
	                 &pyvmdk_handle_type_object );

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create handle.",
		 function );

		goto on_error;
	}
	if( pyvmdk_handle_init(
	     pyvmdk_handle ) != 0 )
	{
		goto on_error;
	}
	if( pyvmdk_handle_open(
	     pyvmdk_handle,
	     arguments,
	     keywords ) == NULL )
	{
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

/* Creates a new handle object and opens it using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_open_new_handle_with_file_object(
           PyObject *self PYVMDK_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	pyvmdk_handle_t *pyvmdk_handle = NULL;
	static char *function          = "pyvmdk_open_new_handle_with_file_object";

	PYVMDK_UNREFERENCED_PARAMETER( self )

	/* PyObject_New does not invoke tp_init
	 */
	pyvmdk_handle = PyObject_New(
	                 struct pyvmdk_handle,
	                 &pyvmdk_handle_type_object );

	if( pyvmdk_handle == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create handle.",
		 function );

		goto on_error;
	}
	if( pyvmdk_handle_init(
	     pyvmdk_handle ) != 0 )
	{
		goto on_error;
	}
	if( pyvmdk_handle_open_file_object(
	     pyvmdk_handle,
	     arguments,
	     keywords ) == NULL )
	{
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

#if PY_MAJOR_VERSION >= 3

/* The pyvmdk module definition
 */
PyModuleDef pyvmdk_module_definition = {
	PyModuleDef_HEAD_INIT,

	/* m_name */
	"pyvmdk",
	/* m_doc */
	"Python libvmdk module (pyvmdk).",
	/* m_size */
	-1,
	/* m_methods */
	pyvmdk_module_methods,
	/* m_reload */
	NULL,
	/* m_traverse */
	NULL,
	/* m_clear */
	NULL,
	/* m_free */
	NULL,
};

#endif /* PY_MAJOR_VERSION >= 3 */

/* Initializes the pyvmdk module
 */
#if PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC PyInit_pyvmdk(
                void )
#else
PyMODINIT_FUNC initpyvmdk(
                void )
#endif
{
	PyObject *module           = NULL;
	PyGILState_STATE gil_state = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libvmdk_notify_set_stream(
	 stderr,
	 NULL );
	libvmdk_notify_set_verbose(
	 1 );
#endif

	/* Create the module
	 * This function must be called before grabbing the GIL
	 * otherwise the module will segfault on a version mismatch
	 */
#if PY_MAJOR_VERSION >= 3
	module = PyModule_Create(
	          &pyvmdk_module_definition );
#else
	module = Py_InitModule3(
	          "pyvmdk",
	          pyvmdk_module_methods,
	          "Python libvmdk module (pyvmdk)." );
#endif
	if( module == NULL )
	{
#if PY_MAJOR_VERSION >= 3
		return( NULL );
#else
		return;
#endif
	}
#if PY_VERSION_HEX < 0x03070000
	PyEval_InitThreads();
#endif
	gil_state = PyGILState_Ensure();

	/* Setup the disk_types type object
	 */
	pyvmdk_disk_types_type_object.tp_new = PyType_GenericNew;

	if( pyvmdk_disk_types_init_type(
	     &pyvmdk_disk_types_type_object ) != 1 )
	{
		goto on_error;
	}
	if( PyType_Ready(
	     &pyvmdk_disk_types_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyvmdk_disk_types_type_object );

	PyModule_AddObject(
	 module,
	 "disk_types",
	 (PyObject *) &pyvmdk_disk_types_type_object );

	/* Setup the extent_descriptor type object
	 */
	pyvmdk_extent_descriptor_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyvmdk_extent_descriptor_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyvmdk_extent_descriptor_type_object );

	PyModule_AddObject(
	 module,
	 "extent_descriptor",
	 (PyObject *) &pyvmdk_extent_descriptor_type_object );

	/* Setup the extent_descriptors type object
	 */
	pyvmdk_extent_descriptors_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyvmdk_extent_descriptors_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyvmdk_extent_descriptors_type_object );

	PyModule_AddObject(
	 module,
	 "extent_descriptors",
	 (PyObject *) &pyvmdk_extent_descriptors_type_object );

	/* Setup the extent_types type object
	 */
	pyvmdk_extent_types_type_object.tp_new = PyType_GenericNew;

	if( pyvmdk_extent_types_init_type(
	     &pyvmdk_extent_types_type_object ) != 1 )
	{
		goto on_error;
	}
	if( PyType_Ready(
	     &pyvmdk_extent_types_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyvmdk_extent_types_type_object );

	PyModule_AddObject(
	 module,
	 "extent_types",
	 (PyObject *) &pyvmdk_extent_types_type_object );

	/* Setup the handle type object
	 */
	pyvmdk_handle_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyvmdk_handle_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyvmdk_handle_type_object );

	PyModule_AddObject(
	 module,
	 "handle",
	 (PyObject *) &pyvmdk_handle_type_object );

	PyGILState_Release(
	 gil_state );

#if PY_MAJOR_VERSION >= 3
	return( module );
#else
	return;
#endif

on_error:
	PyGILState_Release(
	 gil_state );

#if PY_MAJOR_VERSION >= 3
	return( NULL );
#else
	return;
#endif
}

