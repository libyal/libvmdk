/*
 * The cache functions
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

#if !defined( _LIBMFCACHE_INTERNAL_CACHE_H )
#define _LIBMFCACHE_INTERNAL_CACHE_H

#include <common.h>
#include <types.h>

#include "libmfcache_date_time.h"
#include "libmfcache_cache_value.h"
#include "libmfcache_extern.h"
#include "libmfcache_libcdata.h"
#include "libmfcache_libcerror.h"
#include "libmfcache_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libmfcache_internal_cache libmfcache_internal_cache_t;

struct libmfcache_internal_cache
{
	/* The cache entries (array)
	 */
	libcdata_array_t *entries;

	/* The number of cache values
	 */
	int number_of_cache_values;
};

LIBMFCACHE_EXTERN \
int libmfcache_cache_initialize(
     libmfcache_cache_t **cache,
     int maximum_cache_entries,
     libcerror_error_t **error );

LIBMFCACHE_EXTERN \
int libmfcache_cache_free(
     libmfcache_cache_t **cache,
     libcerror_error_t **error );

LIBMFCACHE_EXTERN \
int libmfcache_cache_clone(
     libmfcache_cache_t **destination_cache,
     libmfcache_cache_t *source_cache,
     libcerror_error_t **error );

LIBMFCACHE_EXTERN \
int libmfcache_cache_clear(
     libmfcache_cache_t *cache,
     libcerror_error_t **error );

int libmfcache_cache_get_number_of_entries(
     libmfcache_cache_t *cache,
     int *number_of_entries,
     libcerror_error_t **error );

int libmfcache_cache_get_number_of_cache_values(
     libmfcache_cache_t *cache,
     int *number_of_values,
     libcerror_error_t **error );

/* Cache value functions
 */
int libmfcache_cache_get_value_by_index(
     libmfcache_cache_t *cache,
     int cache_entry_index,
     libmfcache_cache_value_t **cache_value,
     libcerror_error_t **error );

int libmfcache_cache_set_value_by_index(
     libmfcache_cache_t *cache,
     int cache_entry_index,
     int file_io_pool_entry,
     off64_t offset,
     time_t timestamp,
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

