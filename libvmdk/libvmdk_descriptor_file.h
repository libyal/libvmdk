/*
 * Descriptor file functions
 *
 * Copyright (c) 2009-2012 Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBVMDK_DESCRIPTOR_FILE_H )
#define _LIBVMDK_DESCRIPTOR_FILE_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "libvmdk_libbfio.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libcfile.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libvmdk_descriptor_file libvmdk_descriptor_file_t;

struct libvmdk_descriptor_file
{
	/* The filename
	 */
	libcstring_system_character_t *name;

	/* The size of the filename
	 */
	size_t name_size;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;
};

int libvmdk_descriptor_file_initialize(
     libvmdk_descriptor_file_t **descriptor_file,
     libcerror_error_t **error );

int libvmdk_descriptor_file_free(
     libvmdk_descriptor_file_t **descriptor_file,
     libcerror_error_t **error );

int libvmdk_descriptor_file_set_name(
     libvmdk_descriptor_file_t *descriptor_file,
     const libcstring_system_character_t *name,
     size_t name_length,
     libcerror_error_t **error );

int libvmdk_descriptor_file_open(
     libvmdk_descriptor_file_t *descriptor_file,
     const libcstring_system_character_t *mode,
     libcerror_error_t **error );

int libvmdk_descriptor_file_close(
     libvmdk_descriptor_file_t *descriptor_file,
     libcerror_error_t **error );

int libvmdk_descriptor_file_read_header(
     libvmdk_descriptor_file_t *descriptor_file,
     libcerror_error_t **error );

int libvmdk_descriptor_file_read_extents(
     libvmdk_descriptor_file_t *descriptor_file,
     libcerror_error_t **error );

int libvmdk_descriptor_file_read_disk_database(
     libvmdk_descriptor_file_t *descriptor_file,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

