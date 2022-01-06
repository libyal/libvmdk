/*
 * Library to access the VMware Virtual Disk (VMDK) format
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

#if !defined( _LIBVMDK_H )
#define _LIBVMDK_H

#include <libvmdk/definitions.h>
#include <libvmdk/error.h>
#include <libvmdk/extern.h>
#include <libvmdk/features.h>
#include <libvmdk/types.h>

#include <stdio.h>

#if defined( LIBVMDK_HAVE_BFIO )
#include <libbfio.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Support functions
 * ------------------------------------------------------------------------- */

/* Returns the library version
 */
LIBVMDK_EXTERN \
const char *libvmdk_get_version(
             void );

/* Returns the access flags for reading
 */
LIBVMDK_EXTERN \
int libvmdk_get_access_flags_read(
     void );

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_get_codepage(
     int *codepage,
     libvmdk_error_t **error );

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_set_codepage(
     int codepage,
     libvmdk_error_t **error );

/* Determines if a file contains a VMDK file signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_check_file_signature(
     const char *filename,
     libvmdk_error_t **error );

#if defined( LIBVMDK_HAVE_WIDE_CHARACTER_TYPE )

/* Determines if a file contains a VMDK file signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_check_file_signature_wide(
     const wchar_t *filename,
     libvmdk_error_t **error );

#endif /* defined( LIBVMDK_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBVMDK_HAVE_BFIO )

/* Determines if a file contains a VMDK file signature using a Basic File IO (bfio) handle
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libvmdk_error_t **error );

#endif /* defined( LIBVMDK_HAVE_BFIO ) */

/* -------------------------------------------------------------------------
 * Notify functions
 * ------------------------------------------------------------------------- */

/* Sets the verbose notification
 */
LIBVMDK_EXTERN \
void libvmdk_notify_set_verbose(
      int verbose );

/* Sets the notification stream
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_notify_set_stream(
     FILE *stream,
     libvmdk_error_t **error );

/* Opens the notification stream using a filename
 * The stream is opened in append mode
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_notify_stream_open(
     const char *filename,
     libvmdk_error_t **error );

/* Closes the notification stream if opened using a filename
 * Returns 0 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_notify_stream_close(
     libvmdk_error_t **error );

/* -------------------------------------------------------------------------
 * Error functions
 * ------------------------------------------------------------------------- */

/* Frees an error
 */
LIBVMDK_EXTERN \
void libvmdk_error_free(
      libvmdk_error_t **error );

/* Prints a descriptive string of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_error_fprint(
     libvmdk_error_t *error,
     FILE *stream );

/* Prints a descriptive string of the error to the string
 * Returns the number of printed characters if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_error_sprint(
     libvmdk_error_t *error,
     char *string,
     size_t size );

/* Prints a backtrace of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_error_backtrace_fprint(
     libvmdk_error_t *error,
     FILE *stream );

/* Prints a backtrace of the error to the string
 * Returns the number of printed characters if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_error_backtrace_sprint(
     libvmdk_error_t *error,
     char *string,
     size_t size );

/* -------------------------------------------------------------------------
 * Handle functions
 * ------------------------------------------------------------------------- */

/* Creates a handle
 * Make sure the value handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_initialize(
     libvmdk_handle_t **handle,
     libvmdk_error_t **error );

/* Frees a handle
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_free(
     libvmdk_handle_t **handle,
     libvmdk_error_t **error );

/* Signals the handle to abort its current activity
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_signal_abort(
     libvmdk_handle_t *handle,
     libvmdk_error_t **error );

/* Opens a set of VMDK file(s)
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_open(
     libvmdk_handle_t *handle,
     const char *filename,
     int access_flags,
     libvmdk_error_t **error );

#if defined( LIBVMDK_HAVE_WIDE_CHARACTER_TYPE )

/* Opens a set of VMDK file(s)
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_open_wide(
     libvmdk_handle_t *handle,
     const wchar_t *filename,
     int access_flags,
     libvmdk_error_t **error );

#endif /* defined( LIBVMDK_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBVMDK_HAVE_BFIO )

/* Opens a set of VMDK file(s) using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_open_file_io_handle(
     libvmdk_handle_t *handle,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libvmdk_error_t **error );

#endif /* defined( LIBVMDK_HAVE_BFIO ) */

/* Opens the extent data files
 * If the extent data filenames were not set explicitly this function assumes the extent data files
 * are in the same location as the descriptor file
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_open_extent_data_files(
     libvmdk_handle_t *handle,
     libvmdk_error_t **error );

#if defined( LIBVMDK_HAVE_BFIO )

/* Opens the extent data files using a Basic File IO (bfio) pool
 * This function assumes the extent data files are in same order as defined by the descriptor file
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_open_extent_data_files_file_io_pool(
     libvmdk_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     libvmdk_error_t **error );

#endif /* defined( LIBVMDK_HAVE_BFIO ) */

/* Closes the VMDK file(s)
 * Returns 0 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_close(
     libvmdk_handle_t *handle,
     libvmdk_error_t **error );

/* Reads (media) data at the current offset
 * Returns the number of bytes read or -1 on error
 */
LIBVMDK_EXTERN \
ssize_t libvmdk_handle_read_buffer(
         libvmdk_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         libvmdk_error_t **error );

/* Reads (media) data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
LIBVMDK_EXTERN \
ssize_t libvmdk_handle_read_buffer_at_offset(
         libvmdk_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libvmdk_error_t **error );

/* Seeks a certain offset of the (media) data
 * Returns the offset if seek is successful or -1 on error
 */
LIBVMDK_EXTERN \
off64_t libvmdk_handle_seek_offset(
         libvmdk_handle_t *handle,
         off64_t offset,
         int whence,
         libvmdk_error_t **error );

/* Retrieves the current offset of the (media) data
 * Returns the offset if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_get_offset(
     libvmdk_handle_t *handle,
     off64_t *offset,
     libvmdk_error_t **error );

/* Sets the maximum number of (concurrent) open file handles
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_set_maximum_number_of_open_handles(
     libvmdk_handle_t *handle,
     int maximum_number_of_open_handles,
     libvmdk_error_t **error );

/* Sets the parent handle
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_set_parent_handle(
     libvmdk_handle_t *handle,
     libvmdk_handle_t *parent_handle,
     libvmdk_error_t **error );

/* Sets the path to the extend data files
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_set_extent_data_files_path(
     libvmdk_handle_t *handle,
     const char *path,
     size_t path_length,
     libvmdk_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Sets the path to the extend data files
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_set_extent_data_files_path_wide(
     libvmdk_handle_t *handle,
     const wchar_t *path,
     size_t path_length,
     libvmdk_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* -------------------------------------------------------------------------
 * Meta data functions
 * ------------------------------------------------------------------------- */

/* Retrieves the disk type
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_get_disk_type(
     libvmdk_handle_t *handle,
     int *disk_type,
     libvmdk_error_t **error );

/* Retrieves the media size
 * Returns the 1 if succesful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_get_media_size(
     libvmdk_handle_t *handle,
     size64_t *media_size,
     libvmdk_error_t **error );

/* Retrieves the content identifier
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_get_content_identifier(
     libvmdk_handle_t *handle,
     uint32_t *content_identifier,
     libvmdk_error_t **error );

/* Retrieves the parent content identifier
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_get_parent_content_identifier(
     libvmdk_handle_t *handle,
     uint32_t *parent_content_identifier,
     libvmdk_error_t **error );

/* Retrieves the size of the UTF-8 encoded parent filename
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_get_utf8_parent_filename_size(
     libvmdk_handle_t *handle,
     size_t *utf8_string_size,
     libvmdk_error_t **error );

/* Retrieves the UTF-8 encoded parent filename
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_get_utf8_parent_filename(
     libvmdk_handle_t *handle,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libvmdk_error_t **error );

/* Retrieves the size of the UTF-16 encoded parent filename
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_get_utf16_parent_filename_size(
     libvmdk_handle_t *handle,
     size_t *utf16_string_size,
     libvmdk_error_t **error );

/* Retrieves the UTF-16 encoded parent filename
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_get_utf16_parent_filename(
     libvmdk_handle_t *handle,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libvmdk_error_t **error );

/* Retrieves the number of extents
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_get_number_of_extents(
     libvmdk_handle_t *handle,
     int *number_of_extents,
     libvmdk_error_t **error );

/* Retrieves a specific extent descriptor
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_handle_get_extent_descriptor(
     libvmdk_handle_t *handle,
     int extent_index,
     libvmdk_extent_descriptor_t **extent_descriptor,
     libvmdk_error_t **error );

/* -------------------------------------------------------------------------
 * Extent descriptor functions
 * ------------------------------------------------------------------------- */

/* Frees an extent descriptor
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_extent_descriptor_free(
     libvmdk_extent_descriptor_t **extent_descriptor,
     libvmdk_error_t **error );

/* Retrieves the extent type
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_extent_descriptor_get_type(
     libvmdk_extent_descriptor_t *extent_descriptor,
     int *type,
     libvmdk_error_t **error );

/* Retrieves the extent range (offset and size)
 * Returns 1 if successful or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_extent_descriptor_get_range(
     libvmdk_extent_descriptor_t *extent_descriptor,
     off64_t *offset,
     size64_t *size,
     libvmdk_error_t **error );

/* Retrieves the size of the UTF-8 encoded filename
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_extent_descriptor_get_utf8_filename_size(
     libvmdk_extent_descriptor_t *extent_descriptor,
     size_t *utf8_string_size,
     libvmdk_error_t **error );

/* Retrieves the UTF-8 encoded filename
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_extent_descriptor_get_utf8_filename(
     libvmdk_extent_descriptor_t *extent_descriptor,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libvmdk_error_t **error );

/* Retrieves the size of the UTF-16 encoded filename
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_extent_descriptor_get_utf16_filename_size(
     libvmdk_extent_descriptor_t *extent_descriptor,
     size_t *utf16_string_size,
     libvmdk_error_t **error );

/* Retrieves the UTF-16 encoded filename
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBVMDK_EXTERN \
int libvmdk_extent_descriptor_get_utf16_filename(
     libvmdk_extent_descriptor_t *extent_descriptor,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libvmdk_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBVMDK_H ) */

