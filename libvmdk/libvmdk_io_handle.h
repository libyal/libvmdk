/*
 * Input/Output (IO) handle
 *
 * Copyright (C) 2009-2022, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBVMDK_IO_HANDLE_H )
#define _LIBVMDK_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libvmdk_libbfio.h"
#include "libvmdk_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libvmdk_io_handle libvmdk_io_handle_t;

struct libvmdk_io_handle
{
	/* The (storage) media size
	 */
	size64_t media_size;

	/* The file type
	 */
	uint8_t file_type;

	/* The maximum data size
	 */
	size64_t maximum_data_size;

	/* The grain size
	 */
	size64_t grain_size;

	/* Value to indicate one or more extent files is are dirty
	 */
	uint8_t is_dirty;

	/* The compression method
	 */
	uint16_t compression_method;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int libvmdk_io_handle_initialize(
     libvmdk_io_handle_t **io_handle,
     libcerror_error_t **error );

int libvmdk_io_handle_free(
     libvmdk_io_handle_t **io_handle,
     libcerror_error_t **error );

int libvmdk_io_handle_clear(
     libvmdk_io_handle_t *io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBVMDK_IO_HANDLE_H ) */

