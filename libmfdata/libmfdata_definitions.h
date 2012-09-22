/*
 * The internal definitions
 *
 * Copyright (c) 2010-2012, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBMFDATA_INTERNAL_DEFINITIONS_H )
#define _LIBMFDATA_INTERNAL_DEFINITIONS_H

#include <common.h>

/* Define HAVE_LOCAL_LIBMFDATA for local use of libmfdata
 */
#if !defined( HAVE_LOCAL_LIBMFDATA )
#include <libmfdata/definitions.h>

/* The definitions in <libmfdata/definitions.h> are copied here
 * for local use of libmfdata
 */
#else
#define LIBMFDATA_VERSION						20120724

/* The libmfdata version string
 */
#define LIBMFDATA_VERSION_STRING					"20120724"

/* The library flags definitions
 */
enum LIBMFDATA_FLAGS
{
	/* The IO handle is not managed by the library
	 */
        LIBMFDATA_FLAG_IO_HANDLE_NON_MANAGED				= 0x00,

	/* The IO handle is managed by the library
	 */
        LIBMFDATA_FLAG_IO_HANDLE_MANAGED				= 0x01,

	/* The IO handle is cloned by the function
	 */
        LIBMFDATA_FLAG_IO_HANDLE_CLONE_BY_FUNCTION			= 0x00,

	/* The IO handle is not cloned, but passed as a reference
	 */
        LIBMFDATA_FLAG_IO_HANDLE_CLONE_BY_REFERENCE			= 0x02,
};

/* The cache value flags definitions
 */
enum LIBMFDATA_CACHE_VALUE_FLAGS
{
	LIBMFDATA_CACHE_VALUE_FLAG_NON_MANAGED				= LIBMFDATA_FLAG_IO_HANDLE_NON_MANAGED,
	LIBMFDATA_CACHE_VALUE_FLAG_MANAGED				= LIBMFDATA_FLAG_IO_HANDLE_MANAGED
};

/* The file value flags definitions
 */
enum LIBMFDATA_FILE_VALUE_FLAGS
{
	LIBMFDATA_FILE_VALUE_FLAG_NON_MANAGED				= LIBMFDATA_FLAG_IO_HANDLE_NON_MANAGED,
	LIBMFDATA_FILE_VALUE_FLAG_MANAGED				= LIBMFDATA_FLAG_IO_HANDLE_MANAGED
};

/* The list flags definitions
 */
enum LIBMFDATA_LIST_FLAGS
{
	/* The value offsets need to be calculated
	 */
        LIBMFDATA_LIST_FLAG_CALCULATE_VALUE_OFFSETS			= 0x10,
};

/* The list element flags definitions
 */
enum LIBMFDATA_LIST_ELEMENT_FLAGS
{
	/* The list element is a group (element)
	 */
	LIBMFDATA_LIST_ELEMENT_FLAG_IS_GROUP				= 0x01,
};

/* The list element value flags definitions
 */
enum LIBMFDATA_LIST_ELEMENT_VALUE_FLAGS
{
	LIBMFDATA_LIST_ELEMENT_VALUE_FLAG_NON_MANAGED			= LIBMFDATA_FLAG_IO_HANDLE_NON_MANAGED,
	LIBMFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED			= LIBMFDATA_FLAG_IO_HANDLE_MANAGED
};

/* The range flags definitions
 */
enum LIBMFDATA_RANGE_FLAGS
{
	/* The range is sparse
	 */
        LIBMFDATA_RANGE_FLAG_IS_SPARSE					= 0x00000001UL,

	/* The range is compressed
	 */
        LIBMFDATA_RANGE_FLAG_IS_COMPRESSED				= 0x00000002UL,

	/* User defined flags
	 */
        LIBMFDATA_RANGE_FLAG_USER_DEFINED_1				= 0x00000100UL,
        LIBMFDATA_RANGE_FLAG_USER_DEFINED_2				= 0x00000200UL,
        LIBMFDATA_RANGE_FLAG_USER_DEFINED_3				= 0x00000400UL,
        LIBMFDATA_RANGE_FLAG_USER_DEFINED_4				= 0x00000800UL,
        LIBMFDATA_RANGE_FLAG_USER_DEFINED_5				= 0x00001000UL,
        LIBMFDATA_RANGE_FLAG_USER_DEFINED_6				= 0x00002000UL,
        LIBMFDATA_RANGE_FLAG_USER_DEFINED_7				= 0x00004000UL,
        LIBMFDATA_RANGE_FLAG_USER_DEFINED_8				= 0x00008000UL,
};

#define LIBMFDATA_RANGE_FLAG_MAXIMUM \
	LIBMFDATA_RANGE_FLAG_USER_DEFINED_8

/* The read flags definitions
 */
enum LIBMFDATA_READ_FLAGS
{
	/* Ignore data cached by the library
	 */
        LIBMFDATA_READ_FLAG_IGNORE_CACHE				= 0x01,

	/* Is backup data range
	 */
        LIBMFDATA_READ_FLAG_IS_BACKUP_RANGE				= 0x02
};

/* The segment table flags definitions
 */
enum LIBMFDATA_SEGMENT_TABLE_FLAGS
{
	/* The value offsets need to be calculated
	 */
        LIBMFDATA_SEGMENT_TABLE_FLAG_CALCULATE_VALUE_OFFSETS		= 0x10,
};

#endif

#endif

