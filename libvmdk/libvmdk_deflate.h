/*
 * Deflate (zlib) (un)compression functions
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

#if !defined( _LIBVMDK_DEFLATE_COMPRESSION_H )
#define _LIBVMDK_DEFLATE_COMPRESSION_H

#include <common.h>
#include <types.h>

#include "libvmdk_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

/* The block types
 */
enum LIBVMDK_DEFLATE_BLOCK_TYPES
{
	LIBVMDK_DEFLATE_BLOCK_TYPE_UNCOMPRESSED		= 0x00,
	LIBVMDK_DEFLATE_BLOCK_TYPE_HUFFMAN_FIXED	= 0x01,
	LIBVMDK_DEFLATE_BLOCK_TYPE_HUFFMAN_DYNAMIC	= 0x02,
	LIBVMDK_DEFLATE_BLOCK_TYPE_RESERVED		= 0x03
};

typedef struct libvmdk_deflate_bit_stream libvmdk_deflate_bit_stream_t;

struct libvmdk_deflate_bit_stream
{
	/* The byte stream
	 */
	const uint8_t *byte_stream;

	/* The byte stream size
	 */
	size_t byte_stream_size;

	/* The byte stream offset
	 */
	size_t byte_stream_offset;

	/* The bit buffer
	 */
	uint32_t bit_buffer;

	/* The number of bits remaining in the bit buffer
	 */
	uint8_t bit_buffer_size;
};

typedef struct libvmdk_deflate_huffman_table libvmdk_deflate_huffman_table_t;

struct libvmdk_deflate_huffman_table
{
	/* The maximum number of bits representable by the Huffman table
	 */
	uint8_t maximum_number_of_bits;

/* TODO create initialize function that sets the size of codes array? */
	/* The codes array
	 */
	int codes_array[ 288 ];

	/* The code counts array
	 */
	int code_counts_array[ 16 ];

	/* The number of codes
	 */
	int number_of_codes;
};

int libvmdk_deflate_bit_stream_get_value(
     libvmdk_deflate_bit_stream_t *bit_stream,
     uint8_t number_of_bits,
     uint32_t *value_32bit,
     libcerror_error_t **error );

int libvmdk_deflate_huffman_table_construct(
     libvmdk_deflate_huffman_table_t *table,
     const uint16_t *code_sizes_array,
     int number_of_code_sizes,
     libcerror_error_t **error );

int libvmdk_deflate_bit_stream_get_huffman_encoded_value(
     libvmdk_deflate_bit_stream_t *bit_stream,
     libvmdk_deflate_huffman_table_t *table,
     uint32_t *value_32bit,
     libcerror_error_t **error );

int libvmdk_deflate_initialize_dynamic_huffman_tables(
     libvmdk_deflate_bit_stream_t *bit_stream,
     libvmdk_deflate_huffman_table_t *literals_table,
     libvmdk_deflate_huffman_table_t *distances_table,
     libcerror_error_t **error );

int libvmdk_deflate_initialize_fixed_huffman_tables(
     libvmdk_deflate_huffman_table_t *literals_table,
     libvmdk_deflate_huffman_table_t *distances_table,
     libcerror_error_t **error );

int libvmdk_deflate_decode_huffman(
     libvmdk_deflate_bit_stream_t *bit_stream,
     libvmdk_deflate_huffman_table_t *literals_table,
     libvmdk_deflate_huffman_table_t *distances_table,
     uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     size_t *uncompressed_data_offset,
     libcerror_error_t **error );

int libvmdk_deflate_calculate_adler32(
     uint32_t *checksum_value,
     const uint8_t *buffer,
     size_t size,
     uint32_t initial_value,
     libcerror_error_t **error );

int libvmdk_deflate_read_data_header(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     size_t *compressed_data_offset,
     libcerror_error_t **error );

int libvmdk_deflate_read_block(
     libvmdk_deflate_bit_stream_t *bit_stream,
     uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     size_t *uncompressed_data_offset,
     uint8_t *last_block_flag,
     libcerror_error_t **error );

int libvmdk_deflate_decompress(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *uncompressed_data,
     size_t *uncompressed_data_size,
     libcerror_error_t **error );

int libvmdk_deflate_decompress_zlib(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *uncompressed_data,
     size_t *uncompressed_data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBVMDK_DEFLATE_COMPRESSION_H ) */

