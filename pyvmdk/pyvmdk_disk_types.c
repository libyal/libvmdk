/*
 * Python object definition of the libvmdk disk types
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

#include "pyvmdk_disk_types.h"
#include "pyvmdk_libvmdk.h"
#include "pyvmdk_python.h"
#include "pyvmdk_unused.h"

PyTypeObject pyvmdk_disk_types_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyvmdk.disk_types",
	/* tp_basicsize */
	sizeof( pyvmdk_disk_types_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyvmdk_disk_types_free,
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
	"pyvmdk disk types object (wraps LIBVMDK_DISK_TYPES)",
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
	0,
	/* tp_members */
	0,
	/* tp_getset */
	0,
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
	(initproc) pyvmdk_disk_types_init,
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

/* Initializes the type object
 * Returns 1 if successful or -1 on error
 */
int pyvmdk_disk_types_init_type(
     PyTypeObject *type_object )
{
	PyObject *value_object = NULL;

	if( type_object == NULL )
	{
		return( -1 );
	}
	type_object->tp_dict = PyDict_New();

	if( type_object->tp_dict == NULL )
	{
		return( -1 );
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBVMDK_DISK_TYPE_FLAT_2GB_EXTENT );
#else
	value_object = PyInt_FromLong(
	                LIBVMDK_DISK_TYPE_FLAT_2GB_EXTENT );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "FLAT_2GB_EXTENT",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBVMDK_DISK_TYPE_SPARSE_2GB_EXTENT );
#else
	value_object = PyInt_FromLong(
	                LIBVMDK_DISK_TYPE_SPARSE_2GB_EXTENT );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "SPARSE_2GB_EXTENT",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBVMDK_DISK_TYPE_CUSTOM );
#else
	value_object = PyInt_FromLong(
	                LIBVMDK_DISK_TYPE_CUSTOM );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "CUSTOM",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBVMDK_DISK_TYPE_DEVICE );
#else
	value_object = PyInt_FromLong(
	                LIBVMDK_DISK_TYPE_DEVICE );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "DEVICE",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBVMDK_DISK_TYPE_DEVICE_PARITIONED );
#else
	value_object = PyInt_FromLong(
	                LIBVMDK_DISK_TYPE_DEVICE_PARITIONED );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "DEVICE_PARITIONED",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBVMDK_DISK_TYPE_MONOLITHIC_FLAT );
#else
	value_object = PyInt_FromLong(
	                LIBVMDK_DISK_TYPE_MONOLITHIC_FLAT );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "MONOLITHIC_FLAT",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBVMDK_DISK_TYPE_MONOLITHIC_SPARSE );
#else
	value_object = PyInt_FromLong(
	                LIBVMDK_DISK_TYPE_MONOLITHIC_SPARSE );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "MONOLITHIC_SPARSE",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBVMDK_DISK_TYPE_STREAM_OPTIMIZED );
#else
	value_object = PyInt_FromLong(
	                LIBVMDK_DISK_TYPE_STREAM_OPTIMIZED );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "STREAM_OPTIMIZED",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBVMDK_DISK_TYPE_VMFS_FLAT );
#else
	value_object = PyInt_FromLong(
	                LIBVMDK_DISK_TYPE_VMFS_FLAT );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "VMFS_FLAT",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBVMDK_DISK_TYPE_VMFS_FLAT_PRE_ALLOCATED );
#else
	value_object = PyInt_FromLong(
	                LIBVMDK_DISK_TYPE_VMFS_FLAT_PRE_ALLOCATED );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "VMFS_FLAT_PRE_ALLOCATED",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBVMDK_DISK_TYPE_VMFS_FLAT_ZEROED );
#else
	value_object = PyInt_FromLong(
	                LIBVMDK_DISK_TYPE_VMFS_FLAT_ZEROED );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "VMFS_FLAT_ZEROED",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBVMDK_DISK_TYPE_VMFS_RAW );
#else
	value_object = PyInt_FromLong(
	                LIBVMDK_DISK_TYPE_VMFS_RAW );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "VMFS_RAW",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBVMDK_DISK_TYPE_VMFS_RDM );
#else
	value_object = PyInt_FromLong(
	                LIBVMDK_DISK_TYPE_VMFS_RDM );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "VMFS_RDM",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBVMDK_DISK_TYPE_VMFS_RDMP );
#else
	value_object = PyInt_FromLong(
	                LIBVMDK_DISK_TYPE_VMFS_RDMP );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "VMFS_RDMP",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBVMDK_DISK_TYPE_VMFS_SPARSE );
#else
	value_object = PyInt_FromLong(
	                LIBVMDK_DISK_TYPE_VMFS_SPARSE );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "VMFS_SPARSE",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBVMDK_DISK_TYPE_VMFS_SPARSE_THIN );
#else
	value_object = PyInt_FromLong(
	                LIBVMDK_DISK_TYPE_VMFS_SPARSE_THIN );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "VMFS_SPARSE_THIN",
	     value_object ) != 0 )
	{
		goto on_error;
	}
	return( 1 );

on_error:
	if( type_object->tp_dict != NULL )
	{
		Py_DecRef(
		 type_object->tp_dict );

		type_object->tp_dict = NULL;
	}
	return( -1 );
}

/* Creates a new disk types object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvmdk_disk_types_new(
           void )
{
	pyvmdk_disk_types_t *definitions_object = NULL;
	static char *function                   = "pyvmdk_disk_types_new";

	definitions_object = PyObject_New(
	                      struct pyvmdk_disk_types,
	                      &pyvmdk_disk_types_type_object );

	if( definitions_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create definitions object.",
		 function );

		goto on_error;
	}
	if( pyvmdk_disk_types_init(
	     definitions_object ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize definitions object.",
		 function );

		goto on_error;
	}
	return( (PyObject *) definitions_object );

on_error:
	if( definitions_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) definitions_object );
	}
	return( NULL );
}

/* Initializes a disk types object
 * Returns 0 if successful or -1 on error
 */
int pyvmdk_disk_types_init(
     pyvmdk_disk_types_t *definitions_object )
{
	static char *function = "pyvmdk_disk_types_init";

	if( definitions_object == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid definitions object.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Frees a disk types object
 */
void pyvmdk_disk_types_free(
      pyvmdk_disk_types_t *definitions_object )
{
	struct _typeobject *ob_type = NULL;
	static char *function       = "pyvmdk_disk_types_free";

	if( definitions_object == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid definitions object.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           definitions_object );

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
	ob_type->tp_free(
	 (PyObject*) definitions_object );
}

