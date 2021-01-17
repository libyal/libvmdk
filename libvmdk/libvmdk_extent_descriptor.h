/*
 * Extent descriptor functions
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

#if !defined( _LIBVMDK_INTERNAL_EXTENT_DESCRIPTOR_H )
#define _LIBVMDK_INTERNAL_EXTENT_DESCRIPTOR_H

#include <common.h>
#include <types.h>

#include "libvmdk_extern.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libcsplit.h"
#include "libvmdk_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libvmdk_internal_extent_descriptor libvmdk_internal_extent_descriptor_t;

struct libvmdk_internal_extent_descriptor
{
	/* The filename
	 */
	system_character_t *filename;

	/* The filename size
	 */
	size_t filename_size;

	/* The alternate filename
	 */
	system_character_t *alternate_filename;

	/* The alternate filename size
	 */
	size_t alternate_filename_size;

	/* The extent offset
	 */
	off64_t offset;

	/* The extent size
	 */
	size64_t size;

	/* The type
	 */
	int type;

	/* The access
	 */
	int access;
};

int libvmdk_extent_descriptor_initialize(
     libvmdk_extent_descriptor_t **extent_descriptor,
     libcerror_error_t **error );

LIBVMDK_EXTERN \
int libvmdk_extent_descriptor_free(
     libvmdk_extent_descriptor_t **extent_descriptor,
     libcerror_error_t **error );

int libvmdk_internal_extent_descriptor_free(
     libvmdk_internal_extent_descriptor_t **internal_extent_descriptor,
     libcerror_error_t **error );

int libvmdk_extent_descriptor_read(
     libvmdk_extent_descriptor_t *extent_descriptor,
     char *value_string,
     size_t value_string_size,
     int encoding,
     libcerror_error_t **error );

LIBVMDK_EXTERN \
int libvmdk_extent_descriptor_get_type(
     libvmdk_extent_descriptor_t *extent_descriptor,
     int *type,
     libcerror_error_t **error );

LIBVMDK_EXTERN \
int libvmdk_extent_descriptor_get_range(
     libvmdk_extent_descriptor_t *extent_descriptor,
     off64_t *offset,
     size64_t *size,
     libcerror_error_t **error );

LIBVMDK_EXTERN \
int libvmdk_extent_descriptor_get_utf8_filename_size(
     libvmdk_extent_descriptor_t *extent_descriptor,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBVMDK_EXTERN \
int libvmdk_extent_descriptor_get_utf8_filename(
     libvmdk_extent_descriptor_t *extent_descriptor,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBVMDK_EXTERN \
int libvmdk_extent_descriptor_get_utf16_filename_size(
     libvmdk_extent_descriptor_t *extent_descriptor,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBVMDK_EXTERN \
int libvmdk_extent_descriptor_get_utf16_filename(
     libvmdk_extent_descriptor_t *extent_descriptor,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBVMDK_INTERNAL_EXTENT_DESCRIPTOR_H ) */

