/*
 * libvmdk file
 *
 * Copyright (c) 2008-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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

#if !defined( _LIBVMDK_INTERNAL_FILE_H )
#define _LIBVMDK_INTERNAL_FILE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libvmdk_extern.h"
#include "libvmdk_io_handle.h"
#include "libvmdk_offset_table.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libvmdk_internal_file libvmdk_internal_file_t;

struct libvmdk_internal_file
{
	/* The grain offset table
	 */
	libvmdk_offset_table_t *offset_table;

	/* The io handle
	 */
	libvmdk_io_handle_t *io_handle;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

LIBVMDK_EXTERN int libvmdk_file_initialize(
                    libvmdk_file_t **file,
                    liberror_error_t **error );

LIBVMDK_EXTERN int libvmdk_file_free(
                    libvmdk_file_t **file,
                    liberror_error_t **error );

LIBVMDK_EXTERN int libvmdk_file_signal_abort(
                    libvmdk_file_t *file,
                    liberror_error_t **error );

LIBVMDK_EXTERN int libvmdk_file_open(
                    libvmdk_file_t *file,
                    const char *filename,
                    int flags,
                    liberror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBVMDK_EXTERN int libvmdk_file_open_wide(
                    libvmdk_file_t *file,
                    const wchar_t *filename,
                    int flags,
                    liberror_error_t **error );
#endif

LIBVMDK_EXTERN int libvmdk_file_open_file_io_handle(
                    libvmdk_file_t *file,
                    libbfio_handle_t *file_io_handle,
                    int flags,
                    liberror_error_t **error );

LIBVMDK_EXTERN int libvmdk_file_close(
                    libvmdk_file_t *file,
                    liberror_error_t **error );

int libvmdk_file_open_read(
     libvmdk_internal_file_t *internal_file,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

