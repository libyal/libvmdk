/*
 * Cache value functions
 *
 * Copyright (c) 2010-2012, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBMFCACHE_CACHE_VALUE_H )
#define _LIBMFCACHE_CACHE_VALUE_H

#include <common.h>
#include <types.h>

#include "libmfcache_date_time.h"
#include "libmfcache_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libmfcache_cache_value libmfcache_cache_value_t;

struct libmfcache_cache_value
{
	/* The file IO pool entry
	 */
	int file_io_pool_entry;

	/* The offset
	 */
	off64_t offset;

	/* The time stamp
	 */
	time_t timestamp;

	/* The value
	 */
	intptr_t *value;

	/* The free value function
	 */
	int (*free_value)(
	       intptr_t **value,
	       libcerror_error_t **error );

	/* The flags
	 */
	uint8_t flags;
};

int libmfcache_cache_value_initialize(
     libmfcache_cache_value_t **cache_value,
     libcerror_error_t **error );

int libmfcache_cache_value_free(
     libmfcache_cache_value_t **cache_value,
     libcerror_error_t **error );

int libmfcache_cache_value_get_identifier(
     libmfcache_cache_value_t *cache_value,
     int *file_io_pool_entry,
     off64_t *offset,
     time_t *timestamp,
     libcerror_error_t **error );

int libmfcache_cache_value_set_identifier(
     libmfcache_cache_value_t *cache_value,
     int file_io_pool_entry,
     off64_t offset,
     time_t timestamp,
     libcerror_error_t **error );

int libmfcache_cache_value_get_value(
     libmfcache_cache_value_t *cache_value,
     intptr_t **value,
     libcerror_error_t **error );

int libmfcache_cache_value_set_value(
     libmfcache_cache_value_t *cache_value,
     intptr_t *value,
     int (*free_value)(
            intptr_t **value,
            libcerror_error_t **error ),
     uint8_t flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

