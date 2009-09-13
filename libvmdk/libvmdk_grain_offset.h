/*
 * Grain offset functions
 *
 * Copyright (c) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _LIBVMDK_GRAIN_OFFSET_H )
#define _LIBVMDK_GRAIN_OFFSET_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

#define LIBVMDK_GRAIN_OFFSET_FLAGS_COMPRESSED	0x01
#define LIBVMDK_GRAIN_OFFSET_FLAGS_CORRUPTED	0x04
#define LIBVMDK_GRAIN_OFFSET_FLAGS_DELTA_GRAIN	0x08

typedef struct libvmdk_grain_offset libvmdk_grain_offset_t;

struct libvmdk_grain_offset
{
	/* The file offset of the grain in the segment file
	 */
	off64_t file_offset;

	/* The size of the grain
	 */
	size_t size;

	/* Flags
	 * 0x01 indicates if the grain is compressed
	 * 0x04 indicates if the grain is corrupted
	 * 0x80 indicates if the grain is a delta grain
	 */
	uint8_t flags;
};

#if defined( __cplusplus )
}
#endif

#endif

