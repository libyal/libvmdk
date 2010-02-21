/*
 * The file header definition of a sparse VMware Virtual Disk (VMDK) file
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

#if !defined( _VMDK_SPARSE_FILE_HEADER_H )
#define _VMDK_SPARSE_FILE_HEADER_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct vmdk_sparse_file_header vmdk_sparse_file_header_t;

struct vmdk_sparse_file_header
{
	/* The signature
	 * Consists of 4 bytes
	 * Contains: "KDMV"
	 */
	uint8_t signature[ 4 ];

	/* The version
	 * Consists of 4 bytes
	 */
	uint8_t version[ 4 ];

	/* The flags
	 * Consists of 4 bytes
	 */
	uint8_t flags[ 4 ];

	/* The maximum data size (capacity)
	 * Consists of 8 bytes
	 * Contains a value in sectors
	 */
	uint8_t maximum_data_size[ 8 ];

	/* The grain size
	 * Consists of 8 bytes
	 * Contains a value in sectors
	 */
	uint8_t grain_size[ 8 ];

	/* The descriptor offset
	 * Consists of 8 bytes
	 * Contains a value in sectors
	 */
	uint8_t descriptor_offset[ 8 ];

	/* The descriptor size
	 * Consists of 8 bytes
	 * Contains a value in sectors
	 */
	uint8_t descriptor_size[ 8 ];

	/* The amount of grain table entries
	 * Consists of 4 bytes
	 */
	uint8_t amount_of_grain_table_entries[ 4 ];

	/* The secondary grain directory offset
	 * Consists of 8 bytes
	 * Contains a value in sectors
	 */
	uint8_t secondary_grain_directory_offset[ 8 ];

	/* The grain directory offset
	 * Consists of 8 bytes
	 * Contains a value in sectors
	 */
	uint8_t grain_directory_offset[ 8 ];

	/* The metadata (overhead) size
	 * Consists of 8 bytes
	 * Contains a value in sectors
	 */
	uint8_t metadata_size[ 8 ];

	/* Value to indicate the VMDK was cleanly closed
	 * Consists of 1 byte
	 */
	uint8_t is_dirty;

	/* The single end of line character
	 * Consists of 1 byte
	 * Contains: '\n'
	 */
	uint8_t single_end_of_line_character;

	/* A non end of line character
	 * Consists of 1 byte
	 * Contains: ' '
	 */
	uint8_t non_end_of_line_character;

	/* The first double end of line character
	 * Consists of 1 byte
	 * Contains: '\r'
	 */
	uint8_t first_double_end_of_line_character;

	/* The second double end of line character
	 * Consists of 1 byte
	 * Contains: '\n'
	 */
	uint8_t second_double_end_of_line_character;

	/* The compression method
	 * Consists of 2 bytes
	 */
	uint8_t compression_method[ 2 ];

	/* Padding
	 * Consists of 433 bytes
	 */
	uint8_t padding[ 433 ];
};

#if defined( __cplusplus )
}
#endif

#endif

