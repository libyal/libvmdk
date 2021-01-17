/*
 * The file header definition of a sparse Copy-On-Write Disk (COWD) file
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

#if !defined( _COWD_SPARSE_FILE_HEADER_H )
#define _COWD_SPARSE_FILE_HEADER_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct cowd_sparse_file_header cowd_sparse_file_header_t;

struct cowd_sparse_file_header
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
	 * Consists of 4 bytes
	 * Contains a value in sectors
	 */
	uint8_t maximum_data_number_of_sectors[ 4 ];

	/* The grain number of sectors
	 * Consists of 4 bytes
	 * Contains a value in sectors
	 */
	uint8_t grain_number_of_sectors[ 4 ];

	/* The primary grain directory sector number
	 * Consists of 4 bytes
	 * Contains a value in sectors
	 */
	uint8_t primary_grain_directory_sector_number[ 4 ];

	/* The number of grain directory entries
	 * Consists of 4 bytes
	 */
	uint8_t number_of_grain_directory_entries[ 4 ];

	/* The next free grain
	 * Consists of 4 bytes
	 */
	uint8_t next_free_grain[ 4 ];

	union
	{
		struct
		{
			/* The number of cylinders
			 * Consists of 4 bytes
			 */
			uint8_t number_of_cylinders[ 4 ];

			/* The number of heads
			 * Consists of 4 bytes
			 */
			uint8_t number_of_heads[ 4 ];

			/* The number of sectors
			 * Consists of 4 bytes
			 */
			uint8_t number_of_sectors[ 4 ];
		};
		struct
		{
			/* The parent filename
			 * Consists of 1024 bytes
			 */
			uint8_t parent_filename[ 1024 ];

			/* The parent generation
			 * Consists of 4 bytes
			 */
			uint8_t parent_generation[ 4 ];

		};
	};

	/* The generation
	 * Consists of 4 bytes
	 */
	uint8_t generation[ 4 ];

	/* The name
	 * Consists of 60 bytes
	 */
	uint8_t name[ 60 ];

	/* The description
	 * Consists of 512 bytes
	 */
	uint8_t description[ 512 ];

	/* The saved generation
	 * Consists of 4 bytes
	 */
	uint8_t saved_generation[ 4 ];

	/* Reserved
	 * Consists of 8 bytes
	 */
	uint8_t reserved[ 8 ];

	/* Value to indicate the COWD was cleanly closed
	 * Consists of 1 byte
	 */
	uint8_t is_dirty;

	/* Padding
	 * Consists of 396 bytes
	 */
	uint8_t padding[ 396 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _COWD_SPARSE_FILE_HEADER_H ) */

