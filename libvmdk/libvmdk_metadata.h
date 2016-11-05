/*
 * Metadata functions
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

#if !defined( _LIBVMDK_METADATA_H )
#define _LIBVMDK_METADATA_H

#include <common.h>
#include <types.h>

#include "libvmdk_extern.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

LIBVMDK_EXTERN \
int libvmdk_handle_get_disk_type(
     libvmdk_handle_t *handle,
     int *disk_type,
     libcerror_error_t **error );

LIBVMDK_EXTERN \
int libvmdk_handle_get_media_size(
     libvmdk_handle_t *handle,
     size64_t *media_size,
     libcerror_error_t **error );

LIBVMDK_EXTERN \
int libvmdk_handle_get_content_identifier(
     libvmdk_handle_t *handle,
     uint32_t *content_identifier,
     libcerror_error_t **error );

LIBVMDK_EXTERN \
int libvmdk_handle_get_parent_content_identifier(
     libvmdk_handle_t *handle,
     uint32_t *parent_content_identifier,
     libcerror_error_t **error );

LIBVMDK_EXTERN \
int libvmdk_handle_get_utf8_parent_filename_size(
     libvmdk_handle_t *handle,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBVMDK_EXTERN \
int libvmdk_handle_get_utf8_parent_filename(
     libvmdk_handle_t *handle,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBVMDK_EXTERN \
int libvmdk_handle_get_utf16_parent_filename_size(
     libvmdk_handle_t *handle,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBVMDK_EXTERN \
int libvmdk_handle_get_utf16_parent_filename(
     libvmdk_handle_t *handle,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBVMDK_EXTERN \
int libvmdk_handle_get_number_of_extents(
     libvmdk_handle_t *handle,
     int *number_of_extents,
     libcerror_error_t **error );

LIBVMDK_EXTERN \
int libvmdk_handle_get_extent_descriptor(
     libvmdk_handle_t *handle,
     int extent_index,
     libvmdk_extent_descriptor_t **extent_descriptor,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBVMDK_METADATA_H ) */

