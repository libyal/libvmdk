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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libmfcache_cache_value.h"
#include "libmfcache_date_time.h"
#include "libmfcache_definitions.h"
#include "libmfcache_libcerror.h"

/* Creates a cache value
 * Returns 1 if successful or -1 on error
 */
int libmfcache_cache_value_initialize(
     libmfcache_cache_value_t **cache_value,
     libcerror_error_t **error )
{
	static char *function = "libmfcache_cache_value_initialize";

	if( cache_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cache value.",
		 function );

		return( -1 );
	}
	if( *cache_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid cache value value already set.",
		 function );

		return( -1 );
	}
	*cache_value = memory_allocate_structure(
	                libmfcache_cache_value_t );

	if( *cache_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create cache value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *cache_value,
	     0,
	     sizeof( libmfcache_cache_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear cache value.",
		 function );

		goto on_error;
	}
	( *cache_value )->file_io_pool_entry = -1;
	( *cache_value )->offset             = (off64_t) -1;

	return( 1 );

on_error:
	if( *cache_value != NULL )
	{
		memory_free(
		 *cache_value );

		*cache_value = NULL;
	}
	return( -1 );
}

/* Frees the cache value
 * Returns 1 if successful or -1 on error
 */
int libmfcache_cache_value_free(
     libmfcache_cache_value_t **cache_value,
     libcerror_error_t **error )
{
	static char *function = "libmfcache_cache_value_free";
	int result            = 1;

	if( cache_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cache value.",
		 function );

		return( -1 );
	}
	if( *cache_value != NULL )
	{
		if( ( ( *cache_value )->flags & LIBMFCACHE_CACHE_VALUE_FLAG_MANAGED ) != 0 )
		{
			if( ( *cache_value )->value != NULL )
			{
				if( ( *cache_value )->free_value == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: invalid cache value - missing free value function.",
					 function );

					result = -1;
				}
				else if( ( *cache_value )->free_value(
				          &( ( *cache_value )->value ),
				          error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free value.",
					 function );

					result = -1;
				}
			}
		}
		memory_free(
		 *cache_value );

		*cache_value = NULL;
	}
	return( result );
}

/* Retrieves the cache value identifier
 * Returns 1 if successful or -1 on error
 */
int libmfcache_cache_value_get_identifier(
     libmfcache_cache_value_t *cache_value,
     int *file_io_pool_entry,
     off64_t *offset,
     time_t *timestamp,
     libcerror_error_t **error )
{
	static char *function = "libmfcache_cache_value_get_identifier";

	if( cache_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cache value.",
		 function );

		return( -1 );
	}
	if( file_io_pool_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO pool entry.",
		 function );

		return( -1 );
	}
	if( offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset.",
		 function );

		return( -1 );
	}
	if( timestamp == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid timestamp.",
		 function );

		return( -1 );
	}
	*file_io_pool_entry = cache_value->file_io_pool_entry;
	*offset             = cache_value->offset;
	*timestamp          = cache_value->timestamp;

	return( 1 );
}

/* Sets the cache value identifier
 * Returns 1 if successful or -1 on error
 */
int libmfcache_cache_value_set_identifier(
     libmfcache_cache_value_t *cache_value,
     int file_io_pool_entry,
     off64_t offset,
     time_t timestamp,
     libcerror_error_t **error )
{
	static char *function = "libmfcache_cache_value_set_identifier";

	if( cache_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cache value.",
		 function );

		return( -1 );
	}
	cache_value->file_io_pool_entry = file_io_pool_entry;
	cache_value->offset             = offset;
	cache_value->timestamp          = timestamp;

	return( 1 );
}

/* Retrieves the cache value
 * Returns 1 if successful or -1 on error
 */
int libmfcache_cache_value_get_value(
     libmfcache_cache_value_t *cache_value,
     intptr_t **value,
     libcerror_error_t **error )
{
	static char *function = "libmfcache_cache_value_get_value";

	if( cache_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cache value.",
		 function );

		return( -1 );
	}
	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	*value = cache_value->value;

	return( 1 );
}

/* Sets the cache value
 * Returns 1 if successful or -1 on error
 */
int libmfcache_cache_value_set_value(
     libmfcache_cache_value_t *cache_value,
     intptr_t *value,
     int (*free_value)(
            intptr_t **value,
            libcerror_error_t **error ),
     uint8_t flags,
     libcerror_error_t **error )
{
	static char *function = "libmfcache_cache_value_set_value";

	if( cache_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cache value.",
		 function );

		return( -1 );
	}
	if( free_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid free value function.",
		 function );

		return( -1 );
	}
	if( ( cache_value->flags & LIBMFCACHE_CACHE_VALUE_FLAG_MANAGED ) != 0 )
	{
		if( cache_value->value != NULL )
		{
			if( cache_value->free_value == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid cache value - missing free value function.",
				 function );

				return( -1 );
			}
			if( cache_value->free_value(
			     &( cache_value->value ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free value.",
				 function );

				return( -1 );
			}
		}
		cache_value->flags &= ~( LIBMFCACHE_CACHE_VALUE_FLAG_MANAGED );
	}
	cache_value->value      = value;
	cache_value->free_value = free_value;
	cache_value->flags     |= flags;

	return( 1 );
}

