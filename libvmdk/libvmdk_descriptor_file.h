/*
 * Descriptor file functions
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

#if !defined( _LIBVMDK_DESCRIPTOR_FILE_H )
#define _LIBVMDK_DESCRIPTOR_FILE_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "libvmdk_extent_values.h"
#include "libvmdk_libbfio.h"
#include "libvmdk_libcdata.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libcsplit.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const char *vmdk_descriptor_file_signature;

typedef struct libvmdk_descriptor_file libvmdk_descriptor_file_t;

struct libvmdk_descriptor_file
{
	/* The format version
	 */
	int version;

	/* The encoding
	 */
	int encoding;

	/* The content identifier
	 */
	uint32_t content_identifier;

	/* The parent content identifier
	 */
	uint32_t parent_content_identifier;

	/* Value to indicate the parent content identifier has been set
	 */
	uint8_t parent_content_identifier_set;

	/* The parent filename
	 */
	uint8_t *parent_filename;

	/* The parent filename size
	 */
	size_t parent_filename_size;

	/* The disk type
	 */
	int disk_type;

	/* The (storage) media size
	 */
	size64_t media_size;
};

int libvmdk_descriptor_file_initialize(
     libvmdk_descriptor_file_t **descriptor_file,
     libcerror_error_t **error );

int libvmdk_descriptor_file_free(
     libvmdk_descriptor_file_t **descriptor_file,
     libcerror_error_t **error );

int libvmdk_descriptor_file_read_file_io_handle(
     libvmdk_descriptor_file_t *descriptor_file,
     libbfio_handle_t *file_io_handle,
     libcdata_array_t *extents_values_array,
     libcerror_error_t **error );

int libvmdk_descriptor_file_read_string(
     libvmdk_descriptor_file_t *descriptor_file,
     const char *value_string,
     size_t value_string_size,
     libcdata_array_t *extents_values_array,
     libcerror_error_t **error );

int libvmdk_descriptor_file_read_signature(
     libcsplit_narrow_split_string_t *lines,
     int number_of_lines,
     int *line_index,
     libcerror_error_t **error );

int libvmdk_descriptor_file_read_header(
     libvmdk_descriptor_file_t *descriptor_file,
     libcsplit_narrow_split_string_t *lines,
     int number_of_lines,
     int *line_index,
     libcerror_error_t **error );

int libvmdk_descriptor_file_read_extents(
     libvmdk_descriptor_file_t *descriptor_file,
     libcsplit_narrow_split_string_t *lines,
     int number_of_lines,
     int *line_index,
     libcdata_array_t *extents_values_array,
     libcerror_error_t **error );

int libvmdk_descriptor_file_read_change_tracking_file(
     libvmdk_descriptor_file_t *descriptor_file,
     libcsplit_narrow_split_string_t *lines,
     int number_of_lines,
     int *line_index,
     libcerror_error_t **error );

int libvmdk_descriptor_file_read_disk_database(
     libvmdk_descriptor_file_t *descriptor_file,
     libcsplit_narrow_split_string_t *lines,
     int number_of_lines,
     int *line_index,
     libcerror_error_t **error );

int libvmdk_descriptor_file_get_parent_content_identifier(
     libvmdk_descriptor_file_t *descriptor_file,
     uint32_t *parent_content_identifier,
     libcerror_error_t **error );

int libvmdk_descriptor_file_get_utf8_parent_filename_size(
     libvmdk_descriptor_file_t *descriptor_file,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libvmdk_descriptor_file_get_utf8_parent_filename(
     libvmdk_descriptor_file_t *descriptor_file,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libvmdk_descriptor_file_get_utf16_parent_filename_size(
     libvmdk_descriptor_file_t *descriptor_file,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libvmdk_descriptor_file_get_utf16_parent_filename(
     libvmdk_descriptor_file_t *descriptor_file,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libvmdk_descriptor_file_get_disk_type(
     libvmdk_descriptor_file_t *descriptor_file,
     int *disk_type,
     libcerror_error_t **error );

int libvmdk_descriptor_file_get_media_size(
     libvmdk_descriptor_file_t *descriptor_file,
     size64_t *media_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBVMDK_DESCRIPTOR_FILE_H ) */

