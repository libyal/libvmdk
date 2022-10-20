/*
 * The file header definition of a sparse VMware Virtual Disk (VMDK) file
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

	/* The maximum data number of sectors (capacity)
	 * Consists of 8 bytes
	 * Contains a value in sectors
	 */
	uint8_t maximum_data_number_of_sectors[ 8 ];

	/* The grain number of sectors
	 * Consists of 8 bytes
	 * Contains a value in sectors
	 */
	uint8_t grain_number_of_sectors[ 8 ];

	/* The descriptor sector number
	 * Consists of 8 bytes
	 * Contains a value in sectors
	 */
	uint8_t descriptor_sector_number[ 8 ];

	/* The descriptor number of sectors
	 * Consists of 8 bytes
	 * Contains a value in sectors
	 */
	uint8_t descriptor_number_of_sectors[ 8 ];

	/* The number of grain table entries
	 * Consists of 4 bytes
	 */
	uint8_t number_of_grain_table_entries[ 4 ];

	/* The secondary grain directory sector number
	 * Consists of 8 bytes
	 * Contains a value in sectors
	 */
	uint8_t secondary_grain_directory_sector_number[ 8 ];

	/* The primary grain directory sector number
	 * Consists of 8 bytes
	 * Contains a value in sectors
	 */
	uint8_t primary_grain_directory_sector_number[ 8 ];

	/* The metadata (overhead) number of sectors
	 * Consists of 8 bytes
	 * Contains a value in sectors
	 */
	uint8_t metadata_number_of_sectors[ 8 ];

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

#endif /* !defined( _VMDK_SPARSE_FILE_HEADER_H ) */

