/*
 * The group functions
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

#if !defined( _LIBMFDATA_GROUP_H )
#define _LIBMFDATA_GROUP_H

#include <common.h>
#include <types.h>

#include "libmfdata_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libmfdata_group libmfdata_group_t;

struct libmfdata_group
{
	/* The number of elements in the group
	 */
	int number_of_elements;
};

int libmfdata_group_initialize(
     libmfdata_group_t **group,
     libcerror_error_t **error );

int libmfdata_group_free(
     libmfdata_group_t **group,
     libcerror_error_t **error );

int libmfdata_group_clone(
     libmfdata_group_t **destination_group,
     libmfdata_group_t *source_group,
     libcerror_error_t **error );

int libmfdata_group_get_values(
     libmfdata_group_t *group,
     int *number_of_elements,
     libcerror_error_t **error );

int libmfdata_group_set_values(
     libmfdata_group_t *group,
     int number_of_elements,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

