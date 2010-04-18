/*
 * Handle functions
 *
 * Copyright (c) 2009-2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _LIBVMDK_INTERNAL_HANDLE_H )
#define _LIBVMDK_INTERNAL_HANDLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libvmdk_extern.h"
#include "libvmdk_io_handle.h"
#include "libvmdk_libbfio.h"
#include "libvmdk_offset_table.h"
#include "libvmdk_segment_table.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libvmdk_internal_handle libvmdk_internal_handle_t;

struct libvmdk_internal_handle
{
	/* The io handle
	 */
	libvmdk_io_handle_t *io_handle;

	/* The file io pool
	 */
	libbfio_pool_t *file_io_pool;

	/* Value to indicate if the pool was created inside the library
	 */
	uint8_t file_io_pool_created_in_library;

	/* The maximum amount of open handles in the pool
	 */
	int maximum_amount_of_open_handles;

	/* The list of segment files
	 */
	libvmdk_segment_table_t *segment_table;

	/* The grain offset table
	 */
	libvmdk_offset_table_t *offset_table;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

LIBVMDK_EXTERN int libvmdk_handle_initialize(
                    libvmdk_handle_t **handle,
                    liberror_error_t **error );

LIBVMDK_EXTERN int libvmdk_handle_free(
                    libvmdk_handle_t **handle,
                    liberror_error_t **error );

LIBVMDK_EXTERN int libvmdk_handle_signal_abort(
                    libvmdk_handle_t *handle,
                    liberror_error_t **error );

LIBVMDK_EXTERN int libvmdk_handle_open(
                    libvmdk_handle_t *handle,
                    char * const filenames[],
                    int amount_of_filenames,
                    int flags,
                    liberror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBVMDK_EXTERN int libvmdk_handle_open_wide(
                    libvmdk_handle_t *handle,
                    wchar_t * const filenames[],
                    int amount_of_filenames,
                    int flags,
                    liberror_error_t **error );
#endif

LIBVMDK_EXTERN int libvmdk_handle_open_file_io_pool(
                    libvmdk_handle_t *handle,
                    libbfio_pool_t *file_io_pool,
                    int flags,
                    liberror_error_t **error );

LIBVMDK_EXTERN int libvmdk_handle_close(
                    libvmdk_handle_t *handle,
                    liberror_error_t **error );

int libvmdk_handle_open_read(
     libvmdk_internal_handle_t *internal_handle,
     libvmdk_segment_table_t *segment_table,
     liberror_error_t **error );

LIBVMDK_EXTERN int libvmdk_handle_set_maximum_amount_of_open_handles(
                    libvmdk_handle_t *handle,
                    int maximum_amount_of_open_handles,
                    liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

