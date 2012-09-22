/*
 * The internal type definitions
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

#if !defined( _LIBMFDATA_INTERNAL_TYPES_H )
#define _LIBMFDATA_INTERNAL_TYPES_H

#include <common.h>
#include <types.h>

/* Define HAVE_LOCAL_LIBMFDATA for local use of libmfdata
 * The definitions in <libmfdata/types.h> are copied here
 * for local use of libmfdata
 */
#if defined( HAVE_LOCAL_LIBMFDATA )

/* The following type definitions hide internal data structures
 */
#if defined( HAVE_DEBUG_OUTPUT ) && !defined( WINAPI )
typedef struct libmfdata_file {}		libmfdata_file_t;
typedef struct libmfdata_file_list {}		libmfdata_file_list_t;
typedef struct libmfdata_list {}		libmfdata_list_t;
typedef struct libmfdata_list_element {}	libmfdata_list_element_t;
typedef struct libmfdata_segment {}		libmfdata_segment_t;
typedef struct libmfdata_segment_table {}	libmfdata_segment_table_t;

#else
typedef intptr_t libmfdata_file_t;
typedef intptr_t libmfdata_file_list_t;
typedef intptr_t libmfdata_list_t;
typedef intptr_t libmfdata_list_element_t;
typedef intptr_t libmfdata_segment_t;
typedef intptr_t libmfdata_segment_table_t;

#endif

#endif

#endif

