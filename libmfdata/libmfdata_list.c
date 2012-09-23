/*
 * The list functions
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

#include "libmfdata_date_time.h"
#include "libmfdata_definitions.h"
#include "libmfdata_libbfio.h"
#include "libmfdata_libcdata.h"
#include "libmfdata_libcerror.h"
#include "libmfdata_libcnotify.h"
#include "libmfdata_libmfcache.h"
#include "libmfdata_list.h"
#include "libmfdata_list_element.h"
#include "libmfdata_range.h"
#include "libmfdata_types.h"

#define libmfdata_list_calculate_cache_entry_index( element_index, number_of_cache_entries ) \
	element_index % number_of_cache_entries

/* Initializes the list
 *
 * If the flag LIBMFDATA_FLAG_IO_HANDLE_MANAGED is set the list
 * takes over management of the IO handle and the IO handle is freed when
 * no longer needed
 *
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_initialize(
     libmfdata_list_t **list,
     intptr_t *io_handle,
     int (*free_io_handle)(
            intptr_t **io_handle,
            libcerror_error_t **error ),
     int (*clone_io_handle)(
            intptr_t **destination_io_handle,
            intptr_t *source_io_handle,
            libcerror_error_t **error ),
     int (*read_element_data)(
            intptr_t *io_handle,
            libbfio_pool_t *file_io_pool,
            libmfdata_list_element_t *list_element,
            libmfcache_cache_t *cache,
            int file_io_pool_entry,
            off64_t element_data_offset,
            size64_t element_data_size,
            uint32_t element_data_flags,
            uint8_t read_flags,
            libcerror_error_t **error ),
     int (*read_element_group)(
            intptr_t *io_handle,
            libbfio_pool_t *file_io_pool,
            libmfdata_list_t *list,
            int element_index,
            int number_of_elements,
            libmfcache_cache_t *cache,
            int file_io_pool_entry,
            off64_t element_group_offset,
            size64_t element_group_size,
            uint32_t element_group_flags,
            uint8_t read_flags,
            libcerror_error_t **error ),
     uint8_t flags,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_list = NULL;
	static char *function                    = "libmfdata_list_initialize";

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	if( *list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid list value already set.",
		 function );

		return( -1 );
	}
	if( read_element_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid read element data function.",
		 function );

		return( -1 );
	}
	internal_list = memory_allocate_structure(
	                 libmfdata_internal_list_t );

	if( internal_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create list.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_list,
	     0,
	     sizeof( libmfdata_internal_list_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear list.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( internal_list->elements ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create elements array.",
		 function );

		goto on_error;
	}
	internal_list->flags             |= flags;
	internal_list->io_handle          = io_handle;
	internal_list->free_io_handle     = free_io_handle;
	internal_list->clone_io_handle    = clone_io_handle;
	internal_list->read_element_data  = read_element_data;
	internal_list->read_element_group = read_element_group;

	*list = (libmfdata_list_t *) internal_list;

	return( 1 );

on_error:
	if( internal_list != NULL )
	{
		memory_free(
		 internal_list );
	}
	return( -1 );
}

/* Frees the list
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_free(
     libmfdata_list_t **list,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_list = NULL;
	static char *function                    = "libmfdata_list_free";
	int result                               = 1;

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	if( *list != NULL )
	{
		internal_list = (libmfdata_internal_list_t *) *list;
		*list         = NULL;

		if( libcdata_array_free(
		     &( internal_list->elements ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libmfdata_list_element_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free the elements array.",
			 function );

			result = -1;
		}
		if( ( internal_list->flags & LIBMFDATA_FLAG_IO_HANDLE_MANAGED ) != 0 )
		{
			if( internal_list->io_handle != NULL )
			{
				if( internal_list->free_io_handle != NULL )
				{
					if( internal_list->free_io_handle(
					     &( internal_list->io_handle ),
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
						 "%s: unable to free IO handle.",
						 function );

						result = -1;
					}
				}
			}
		}
		memory_free(
		 internal_list );
	}
	return( result );
}

/* Clones (duplicates) the list
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_clone(
     libmfdata_list_t **destination_list,
     libmfdata_list_t *source_list,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_source_list = NULL;
	intptr_t *destination_io_handle                 = NULL;
	static char *function                           = "libmfdata_list_clone";

	if( destination_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination list.",
		 function );

		return( -1 );
	}
	if( *destination_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: destination list already set.",
		 function );

		return( -1 );
	}
	if( source_list == NULL )
	{
		*destination_list = NULL;

		return( 1 );
	}
	internal_source_list = (libmfdata_internal_list_t *) source_list;

	if( internal_source_list->io_handle != NULL )
	{
		if( internal_source_list->free_io_handle == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid source list - missing free IO handle function.",
			 function );

			goto on_error;
		}
		if( internal_source_list->clone_io_handle == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid source list - missing clone IO handle function.",
			 function );

			goto on_error;
		}
		if( internal_source_list->clone_io_handle(
		     &destination_io_handle,
		     internal_source_list->io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to clone IO handle.",
			 function );

			goto on_error;
		}
	}
	if( libmfdata_list_initialize(
	     destination_list,
	     destination_io_handle,
	     internal_source_list->free_io_handle,
	     internal_source_list->clone_io_handle,
	     internal_source_list->read_element_data,
	     internal_source_list->read_element_group,
	     LIBMFDATA_FLAG_IO_HANDLE_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination list.",
		 function );

		goto on_error;
	}
	if( *destination_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing destination list.",
		 function );

		goto on_error;
	}
	destination_io_handle = NULL;

	if( libmfdata_list_clone_elements(
	     *destination_list,
	     source_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy list elements.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_list != NULL )
	{
		libmfdata_list_free(
		 destination_list,
		 NULL );
	}
	if( destination_io_handle != NULL )
	{
		internal_source_list->free_io_handle(
		 &destination_io_handle,
		 NULL );
	}
	return( -1 );
}

/* Clones (duplicates) the elements of the list
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_clone_elements(
     libmfdata_list_t *destination_list,
     libmfdata_list_t *source_list,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_destination_list = NULL;
	libmfdata_internal_list_t *internal_source_list      = NULL;
	libmfdata_list_element_t *destination_group_element  = NULL;
	libmfdata_list_element_t *destination_list_element   = NULL;
	libmfdata_list_element_t *source_list_element        = NULL;
	static char *function                                = "libmfdata_list_clone_elements";
	int element_index                                    = 0;
	int is_group                                         = 0;
	int number_of_elements                               = 0;
	int number_of_group_elements                         = 0;

	if( destination_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination list.",
		 function );

		return( -1 );
	}
	internal_destination_list = (libmfdata_internal_list_t *) destination_list;

	if( source_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source list.",
		 function );

		return( -1 );
	}
	internal_source_list = (libmfdata_internal_list_t *) source_list;

	if( libcdata_array_get_number_of_entries(
	     internal_source_list->elements,
	     &number_of_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from source elements array.",
		 function );

		goto on_error;
	}
	if( libcdata_array_empty(
	     internal_destination_list->elements,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libmfdata_list_element_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty destination elements array.",
		 function );

		goto on_error;
	}
	if( libcdata_array_resize(
	     internal_destination_list->elements,
	     number_of_elements,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libmfdata_list_element_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize destination elements array.",
		 function );

		goto on_error;
	}
	for( element_index = 0;
	     element_index < number_of_elements;
	     element_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_source_list->elements,
		     element_index,
		     (intptr_t **) &source_list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve list element: %d from source elements array.",
			 function,
			 element_index );

			goto on_error;
		}
		if( source_list_element == NULL )
		{
			continue;
		}
		is_group = libmfdata_list_element_is_group(
		            source_list_element,
		            error );

		if( is_group == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if source list element: %d is a group.",
			 function,
			 element_index );

			goto on_error;
		}
		else if( is_group != 0 )
		{
			destination_list_element = destination_group_element;
		}
		if( destination_list_element == NULL )
		{
			if( libmfdata_list_element_clone(
			     &destination_list_element,
			     source_list_element,
			     destination_list,
			     element_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create destination list element: %d.",
				 function,
				 element_index );

				goto on_error;
			}
			if( is_group != 0 )
			{
				if( libmfdata_list_element_get_group_values(
				     source_list_element,
				     &number_of_group_elements,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve group values of source list element: %d.",
					 function,
					 element_index );

					goto on_error;
				}
				destination_group_element = destination_list_element;
			}
		}
		if( libcdata_array_set_entry_by_index(
		     internal_destination_list->elements,
		     element_index,
		     (intptr_t *) destination_list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set list element: %d in destination elements array.",
			 function,
			 element_index );

			goto on_error;
		}
		destination_list_element = NULL;

		if( is_group != 0 )
		{
			number_of_group_elements -= 1;

			if( number_of_group_elements == 0 )
			{
				destination_group_element = NULL;
			}
		}
	}
	internal_destination_list->value_size = internal_source_list->value_size;

	return( 1 );

on_error:
	if( ( destination_list_element != NULL )
	 && ( destination_list_element != destination_group_element ) )
	{
		libmfdata_list_element_free(
		 &destination_list_element,
		 NULL );
	}
	return( -1 );
}

/* Resizes the elements
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_resize(
     libmfdata_list_t *list,
     int number_of_elements,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_list = NULL;
	static char *function                    = "libmfdata_list_resize";

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libmfdata_internal_list_t *) list;

	if( libcdata_array_resize(
	     internal_list->elements,
	     number_of_elements,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libmfdata_list_element_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize elements array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of elements of the list
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_get_number_of_elements(
     libmfdata_list_t *list,
     int *number_of_elements,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_list = NULL;
	static char *function                    = "libmfdata_list_get_number_of_elements";

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libmfdata_internal_list_t *) list;

	if( libcdata_array_get_number_of_entries(
	     internal_list->elements,
	     number_of_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from elements array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific element
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_get_element_by_index(
     libmfdata_list_t *list,
     int element_index,
     libmfdata_list_element_t **element,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_list = NULL;
	static char *function                    = "libmfdata_list_get_element_by_index";

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libmfdata_internal_list_t *) list;

	if( libcdata_array_get_entry_by_index(
	     internal_list->elements,
	     element_index,
	     (intptr_t **) element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve list element: %d from elements array.",
		 function,
		 element_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves an element for the specified offset
 * Returns 1 if successful, 0 if no matching element was found or -1 on error
 */
int libmfdata_list_get_element_by_offset(
     libmfdata_list_t *list,
     off64_t value_offset,
     int *element_index,
     libmfdata_list_element_t **element,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_list = NULL;
	static char *function                    = "libmfdata_list_get_element_by_offset";
	off64_t range_offset                     = 0;
	size64_t range_size                      = 0;
	uint32_t range_flags                     = 0;
	int range_file_io_pool_entry             = 0;
	int number_of_elements                   = 0;

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libmfdata_internal_list_t *) list;

	if( element_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid element index.",
		 function );

		return( -1 );
	}
	if( element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid element.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     internal_list->elements,
	     &number_of_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from elements array.",
		 function );

		return( -1 );
	}
	for( *element_index = 0;
	     *element_index < number_of_elements;
	     *element_index += 1 )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_list->elements,
		     *element_index,
		     (intptr_t **) element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve list element: %d from elements array.",
			 function,
			 *element_index );

			return( -1 );
		}
		if( libmfdata_list_element_get_data_range(
		     *element,
		     &range_file_io_pool_entry,
		     &range_offset,
		     &range_size,
		     &range_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data range of list element: %d.",
			 function,
			 *element_index );

			return( -1 );
		}
		if( ( value_offset >= range_offset )
		 && ( value_offset < (off64_t) ( range_offset + range_size ) ) )
		{
			return( 1 );
		}
	}
	*element = NULL;

	return( 0 );
}

/* Sets a specific element
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_set_element_by_index(
     libmfdata_list_t *list,
     int element_index,
     int file_io_pool_entry,
     off64_t offset,
     size64_t size,
     uint32_t flags,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_list = NULL;
	libmfdata_list_element_t *list_element   = NULL;
	static char *function                    = "libmfdata_list_set_element_by_index";
	off64_t previous_offset                  = 0;
	size64_t previous_size                   = 0;
	uint32_t previous_flags                  = 0;
	int previous_file_io_pool_entry          = 0;
	int result                               = 0;

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libmfdata_internal_list_t *) list;

	if( libcdata_array_get_entry_by_index(
	     internal_list->elements,
	     element_index,
	     (intptr_t **) &list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve list element: %d from elements array.",
		 function,
		 element_index );

		return( -1 );
	}
	if( list_element != NULL )
	{
		if( libmfdata_list_element_get_data_range(
		     list_element,
		     &previous_file_io_pool_entry,
		     &previous_offset,
		     &previous_size,
		     &previous_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data range of list element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
		internal_list->value_size -= previous_size;

		result = libmfdata_list_element_is_group(
		          list_element,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if list element: %d is a group.",
			 function,
			 element_index );

			return( -1 );
		}
		else if( result != 0 )
		{
			if( libcdata_array_set_entry_by_index(
			     internal_list->elements,
			     element_index,
			     NULL,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set list element: %d in elements array.",
				 function,
				 element_index );

				libmfdata_list_element_free(
				 &list_element,
				 NULL );

				return( -1 );
			}
			if( libmfdata_list_element_free(
			     &list_element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free list element.",
				 function );

				return( -1 );
			}
		}
	}
	if( list_element == NULL )
	{
		if( libmfdata_list_element_initialize(
		     &list_element,
		     list,
		     element_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create list element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
		if( libcdata_array_set_entry_by_index(
		     internal_list->elements,
		     element_index,
		     (intptr_t *) list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set list element: %d in elements array.",
			 function,
			 element_index );

			libmfdata_list_element_free(
			 &list_element,
			 NULL );

			return( -1 );
		}
	}
	if( libmfdata_list_element_set_data_range(
	     list_element,
	     file_io_pool_entry,
	     offset,
	     size,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set data range of list element: %d.",
		 function,
		 element_index );

		return( -1 );
	}
	internal_list->value_size += size;
	internal_list->flags      |= LIBMFDATA_LIST_FLAG_CALCULATE_VALUE_OFFSETS;

	return( 1 );
}

/* Appends an element
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_append_element(
     libmfdata_list_t *list,
     int *element_index,
     int file_io_pool_entry,
     off64_t offset,
     size64_t size,
     uint32_t flags,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_list = NULL;
	libmfdata_list_element_t *list_element   = NULL;
	static char *function                    = "libmfdata_list_append_element";

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libmfdata_internal_list_t *) list;

	if( libmfdata_list_element_initialize(
	     &list_element,
	     list,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create list element.",
		 function );

		goto on_error;
	}
	if( libcdata_array_append_entry(
	     internal_list->elements,
	     element_index,
	     (intptr_t *) list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append list element to elements array.",
		 function );

		goto on_error;
	}
	if( libmfdata_list_element_set_data_range(
	     list_element,
	     file_io_pool_entry,
	     offset,
	     size,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set data range of list element: %d.",
		 function,
		 element_index );

		goto on_error;
	}
	if( libmfdata_list_element_set_element_index(
	     list_element,
	     *element_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set list element index.",
		 function );

		list_element = NULL;

		goto on_error;
	}
	internal_list->value_size += size;
	internal_list->flags      |= LIBMFDATA_LIST_FLAG_CALCULATE_VALUE_OFFSETS;

	return( 1 );

on_error:
	if( list_element != NULL )
	{
		libmfdata_list_element_free(
		 &list_element,
		 NULL );
	}
	return( -1 );
}

/* Determines if a specific element is set
 * Returns 1 if element is set, 0 if not or -1 on error
 */
int libmfdata_list_is_set(
     libmfdata_list_t *list,
     int element_index,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_list = NULL;
	libmfdata_list_element_t *element        = NULL;
	static char *function                    = "libmfdata_list_is_set";
	int number_of_elements                   = 0;

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libmfdata_internal_list_t *) list;

	if( libcdata_array_get_number_of_entries(
	     internal_list->elements,
	     &number_of_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from elements array.",
		 function );

		return( -1 );
	}
	if( ( number_of_elements == 0 )
	 || ( element_index >= number_of_elements ) )
	{
		return( 0 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_list->elements,
	     element_index,
	     (intptr_t **) &element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve list element: %d from elements array.",
		 function,
		 element_index );

		return( -1 );
	}
	if( element != NULL )
	{
		return( 1 );
	}
	return( 0 );
}

/* Determines if a specific element is a group
 * Returns 1 if element is set, 0 if not or -1 on error
 */
int libmfdata_list_is_group(
     libmfdata_list_t *list,
     int element_index,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_list = NULL;
	libmfdata_list_element_t *element        = NULL;
	static char *function                    = "libmfdata_list_is_group";
	int result                               = 0;

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libmfdata_internal_list_t *) list;

	if( libcdata_array_get_entry_by_index(
	     internal_list->elements,
	     element_index,
	     (intptr_t **) &element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve list element: %d from elements array.",
		 function,
		 element_index );

		return( -1 );
	}
	result = libmfdata_list_element_is_group(
		  element,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if list element: %d is a group.",
		 function,
		 element_index );

		return( -1 );
	}
	return( result );
}

/* Group functions
 */

/* Retrieves a specific group
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_get_group_by_index(
     libmfdata_list_t *list,
     int element_index,
     int *number_of_elements,
     int *file_io_pool_entry,
     off64_t *offset,
     size64_t *size,
     uint32_t *flags,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_list = NULL;
	libmfdata_list_element_t *list_element   = NULL;
	static char *function                    = "libmfdata_list_get_group_by_index";
	int result                               = 0;

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libmfdata_internal_list_t *) list;

	if( libcdata_array_get_entry_by_index(
	     internal_list->elements,
	     element_index,
	     (intptr_t **) &list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve list element: %d from elements array.",
		 function,
		 element_index );

		return( -1 );
	}
	result = libmfdata_list_element_is_group(
	          list_element,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if list element: %d is a group.",
		 function,
		 element_index );

		return( -1 );
	}
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value list element: %d is not a group.",
		 function,
		 element_index );

		return( -1 );
	}
	if( libmfdata_list_element_get_data_range(
	     list_element,
	     file_io_pool_entry,
	     offset,
	     size,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data range from list element: %d.",
		 function,
		 element_index );

		return( -1 );
	}
	if( libmfdata_list_element_get_group_values(
	     list_element,
	     number_of_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve group values from list element: %d.",
		 function,
		 element_index );

		return( -1 );
	}
	return( 1 );
}

/* Sets a specific group
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_set_group_by_index(
     libmfdata_list_t *list,
     int element_index,
     int number_of_elements,
     int file_io_pool_entry,
     off64_t offset,
     size64_t size,
     uint32_t flags,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_list     = NULL;
	libmfdata_list_element_t *group_list_element = NULL;
	libmfdata_list_element_t *list_element       = NULL;
	static char *function                        = "libmfdata_list_set_group_by_index";
	off64_t previous_offset                      = 0;
	size64_t previous_size                       = 0;
	uint32_t previous_flags                      = 0;
	int group_element_set                        = 0;
	int previous_file_io_pool_entry              = 0;

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libmfdata_internal_list_t *) list;

	if( number_of_elements <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid number of elements value zero or less.",
		 function );

		return( -1 );
	}
	if( libmfdata_list_element_initialize(
	     &group_list_element,
	     list,
	     element_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create group list element: %d.",
		 function,
		 element_index );

		goto on_error;
	}
	if( libmfdata_list_element_set_data_range(
	     group_list_element,
	     file_io_pool_entry,
	     offset,
	     size,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set data range of group list element: %d.",
		 function,
		 element_index );

		goto on_error;
	}
	if( libmfdata_list_element_set_group_values(
	     group_list_element,
	     number_of_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set group values.",
		 function );

		goto on_error;
	}
	while( number_of_elements > 0 )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_list->elements,
		     element_index,
		     (intptr_t **) &list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve list element: %d from elements array.",
			 function,
			 element_index );

			goto on_error;
		}
		if( list_element != NULL )
		{
			if( libmfdata_list_element_get_data_range(
			     list_element,
			     &previous_file_io_pool_entry,
			     &previous_offset,
			     &previous_size,
			     &previous_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve data range of list element: %d.",
				 function,
				 element_index );

				goto on_error;
			}
			internal_list->value_size -= previous_size;
		}
		if( libcdata_array_set_entry_by_index(
		     internal_list->elements,
		     element_index,
		     (intptr_t *) group_list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set group list element: %d in elements array.",
			 function,
			 element_index );

			goto on_error;
		}
		group_element_set = 1;

		if( list_element != NULL )
		{
			if( libmfdata_list_element_free(
			     &list_element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free list element.",
				 function );

				goto on_error;
			}
		}
		element_index++;
		number_of_elements--;
	}
	internal_list->flags |= LIBMFDATA_LIST_FLAG_CALCULATE_VALUE_OFFSETS;

	return( 1 );

on_error:
	if( group_element_set == 0 )
	{
		if( group_list_element != NULL )
		{
			libmfdata_list_element_free(
			 &group_list_element,
			 NULL );
		}
	}
	return( -1 );
}

/* Appends a group
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_append_group(
     libmfdata_list_t *list,
     int *element_index,
     int number_of_elements,
     int file_io_pool_entry,
     off64_t offset,
     size64_t size,
     uint32_t flags,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_list     = NULL;
	libmfdata_list_element_t *group_list_element = NULL;
	static char *function                        = "libmfdata_list_append_group";
	int first_element_index                      = -1;
	int group_element_set                        = 0;

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libmfdata_internal_list_t *) list;

	if( element_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid element index.",
		 function );

		return( -1 );
	}
	if( number_of_elements <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid number of elements value zero or less.",
		 function );

		return( -1 );
	}
	if( libmfdata_list_element_initialize(
	     &group_list_element,
	     list,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create group list element.",
		 function );

		goto on_error;
	}
	if( libmfdata_list_element_set_data_range(
	     group_list_element,
	     file_io_pool_entry,
	     offset,
	     size,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set data range of group list element.",
		 function );

		goto on_error;
	}
	if( libmfdata_list_element_set_group_values(
	     group_list_element,
	     number_of_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set group values.",
		 function );

		goto on_error;
	}
	while( number_of_elements > 0 )
	{
		if( libcdata_array_append_entry(
		     internal_list->elements,
		     element_index,
		     (intptr_t *) group_list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append group list element to elements array.",
			 function );

			goto on_error;
		}
		group_element_set = 1;

		if( first_element_index == -1 )
		{
			first_element_index = *element_index;
		}
		number_of_elements--;
	}
	*element_index = first_element_index;

	if( libmfdata_list_element_set_element_index(
	     group_list_element,
	     *element_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set group list element index.",
		 function );

		goto on_error;
	}
	internal_list->flags |= LIBMFDATA_LIST_FLAG_CALCULATE_VALUE_OFFSETS;

	return( 1 );

on_error:
	if( group_element_set == 0 )
	{
		if( group_list_element != NULL )
		{
			libmfdata_list_element_free(
			 &group_list_element,
			 NULL );
		}
	}
	return( -1 );
}

/* Expands an element group
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_expand_group(
     libmfdata_list_t *list,
     libbfio_pool_t *file_io_pool,
     libmfcache_cache_t *cache,
     libmfdata_list_element_t *group_list_element,
     int element_index,
     uint8_t read_flags,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_list    = NULL;
	static char *function                       = "libmfdata_list_expand_group";
	off64_t backup_element_group_offset         = 0;
	off64_t element_group_offset                = 0;
	size64_t backup_element_group_size          = 0;
	size64_t element_group_size                 = 0;
	uint32_t backup_element_group_data_flags    = 0;
	uint32_t element_group_data_flags           = 0;
	int backup_element_group_file_io_pool_entry = 0;
	int element_group_file_io_pool_entry        = 0;
	int group_element_index                     = 0;
	int number_of_elements                      = 0;
	int has_backup_data_range                   = 0;
	int result                                  = 0;

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libmfdata_internal_list_t *) list;

	if( group_list_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid group list element.",
		 function );

		return( -1 );
	}
	if( libmfdata_list_element_get_data_range(
	     group_list_element,
	     &element_group_file_io_pool_entry,
	     &element_group_offset,
	     &element_group_size,
	     &element_group_data_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data range of list element: %d.",
		 function,
		 element_index );

		return( -1 );
	}
	/* Safe guard the backup data range in case the read element group function
	 * overwrites these values
	 */
	result = libmfdata_list_element_get_backup_data_range(
	          group_list_element,
	          &backup_element_group_file_io_pool_entry,
	          &backup_element_group_offset,
	          &backup_element_group_size,
	          &backup_element_group_data_flags,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve backup data range of list element: %d.",
		 function,
		 element_index );

		return( -1 );
	}
	else if( result != 0 )
	{
		has_backup_data_range = 1;
	}
	if( libmfdata_list_element_get_group_values(
	     group_list_element,
	     &number_of_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve group values of list element: %d.",
		 function,
		 element_index );

		return( -1 );
	}
	if( libmfdata_list_element_get_element_index(
	     group_list_element,
	     &group_element_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve group element index from list element: %d.",
		 function,
		 element_index );

		return( -1 );
	}
	if( internal_list->read_element_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid list - missing read element group function.",
		 function );

		return( -1 );
	}
	result = internal_list->read_element_group(
	          internal_list->io_handle,
	          file_io_pool,
	          list,
	          group_element_index,
	          number_of_elements,
	          cache,
	          element_group_file_io_pool_entry,
	          element_group_offset,
	          element_group_size,
	          element_group_data_flags,
	          read_flags,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read element group: %d.",
		 function,
		 group_element_index );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( has_backup_data_range == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: error reading element group: %d and no backup data range to compensate.",
			 function,
			 group_element_index );

			return( -1 );
		}
		if( internal_list->read_element_group(
		     internal_list->io_handle,
		     file_io_pool,
		     list,
		     group_element_index,
		     number_of_elements,
		     cache,
		     backup_element_group_file_io_pool_entry,
		     backup_element_group_offset,
		     backup_element_group_size,
		     backup_element_group_data_flags,
		     read_flags | LIBMFDATA_READ_FLAG_IS_BACKUP_RANGE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read element group: %d using backup range.",
			 function,
			 group_element_index );

			return( -1 );
		}
	}
	return( 1 );
}

/* Data range functions
 */

/* Retrieves the data range of a specific element
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_get_data_range_by_index(
     libmfdata_list_t *list,
     int element_index,
     int *file_io_pool_entry,
     off64_t *offset,
     size64_t *size,
     uint32_t *flags,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_list = NULL;
	libmfdata_list_element_t *list_element   = NULL;
	static char *function                    = "libmfdata_list_get_data_range_by_index";

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libmfdata_internal_list_t *) list;

	if( libcdata_array_get_entry_by_index(
	     internal_list->elements,
	     element_index,
	     (intptr_t **) &list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve list element: %d from elements array.",
		 function,
		 element_index );

		return( -1 );
	}
	if( libmfdata_list_element_get_data_range(
	     list_element,
	     file_io_pool_entry,
	     offset,
	     size,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data range from list element: %d.",
		 function,
		 element_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the data range of the element at the specified offset
 * Returns 1 if successful, 0 if no matching element was found or -1 on error
 */
int libmfdata_list_get_data_range_by_offset(
     libmfdata_list_t *list,
     off64_t value_offset,
     int *element_index,
     int *file_io_pool_entry,
     off64_t *offset,
     size64_t *size,
     uint32_t *flags,
     libcerror_error_t **error )
{
	libmfdata_list_element_t *list_element   = NULL;
	libmfdata_internal_list_t *internal_list = NULL;
	static char *function                    = "libmfdata_list_get_data_range_by_offset";
	int number_of_elements                   = 0;

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libmfdata_internal_list_t *) list;

	if( value_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid value offset value less than zero.",
		 function );

		return( -1 );
	}
	if( element_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid element index.",
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
	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     internal_list->elements,
	     &number_of_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from elements array.",
		 function );

		return( -1 );
	}
	for( *element_index = 0;
	     *element_index < number_of_elements;
	     *element_index += 1 )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_list->elements,
		     *element_index,
		     (intptr_t **) &list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve list element: %d from elements array.",
			 function,
			 *element_index );

			return( -1 );
		}
		if( libmfdata_list_element_get_data_range(
		     list_element,
		     file_io_pool_entry,
		     offset,
		     size,
		     flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data range of list element: %d.",
			 function,
			 *element_index );

			return( -1 );
		}
		if( ( value_offset >= *offset )
		 && ( value_offset < (off64_t) ( *offset + *size ) ) )
		{
			return( 1 );
		}
	}
	return( 0 );
}

/* Sets the data range of a specific element
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_set_data_range_by_index(
     libmfdata_list_t *list,
     int element_index,
     int file_io_pool_entry,
     off64_t offset,
     size64_t size,
     uint32_t flags,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_list = NULL;
	libmfdata_list_element_t *list_element   = NULL;
	static char *function                    = "libmfdata_list_set_data_range_by_index";
	off64_t previous_offset                  = 0;
	size64_t previous_size                   = 0;
	uint32_t previous_flags                  = 0;
	int previous_file_io_pool_entry          = 0;

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libmfdata_internal_list_t *) list;

	if( libcdata_array_get_entry_by_index(
	     internal_list->elements,
	     element_index,
	     (intptr_t **) &list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve list element: %d from elements array.",
		 function,
		 element_index );

		return( -1 );
	}
	if( libmfdata_list_element_get_data_range(
	     list_element,
	     &previous_file_io_pool_entry,
	     &previous_offset,
	     &previous_size,
	     &previous_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data range of list element: %d.",
		 function,
		 element_index );

		return( -1 );
	}
	internal_list->value_size -= previous_size;

	if( libmfdata_list_element_set_data_range(
	     list_element,
	     file_io_pool_entry,
	     offset,
	     size,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set data range of list element: %d.",
		 function,
		 element_index );

		return( -1 );
	}
	internal_list->value_size += size;
	internal_list->flags      |= LIBMFDATA_LIST_FLAG_CALCULATE_VALUE_OFFSETS;

	return( 1 );
}

/* Backup data range functions
 */

/* Retrieves the backup data range of a specific element
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_get_backup_data_range_by_index(
     libmfdata_list_t *list,
     int element_index,
     int *file_io_pool_entry,
     off64_t *offset,
     size64_t *size,
     uint32_t *flags,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_list = NULL;
	libmfdata_list_element_t *list_element   = NULL;
	static char *function                    = "libmfdata_list_get_backup_data_range_by_index";

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libmfdata_internal_list_t *) list;

	if( libcdata_array_get_entry_by_index(
	     internal_list->elements,
	     element_index,
	     (intptr_t **) &list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve list element: %d from elements array.",
		 function,
		 element_index );

		return( -1 );
	}
	if( libmfdata_list_element_get_backup_data_range(
	     list_element,
	     file_io_pool_entry,
	     offset,
	     size,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve backup data range from list element: %d.",
		 function,
		 element_index );

		return( -1 );
	}
	return( 1 );
}

/* Sets the backup data range of a specific element
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_set_backup_data_range_by_index(
     libmfdata_list_t *list,
     int element_index,
     int file_io_pool_entry,
     off64_t offset,
     size64_t size,
     uint32_t flags,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_list = NULL;
	libmfdata_list_element_t *list_element   = NULL;
	static char *function                    = "libmfdata_list_set_backup_data_range_by_index";

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libmfdata_internal_list_t *) list;

	if( libcdata_array_get_entry_by_index(
	     internal_list->elements,
	     element_index,
	     (intptr_t **) &list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve list element: %d from elements array.",
		 function,
		 element_index );

		return( -1 );
	}
	if( libmfdata_list_element_set_backup_data_range(
	     list_element,
	     file_io_pool_entry,
	     offset,
	     size,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set backup data range of list element: %d.",
		 function,
		 element_index );

		return( -1 );
	}
	return( 1 );
}

/* List element value functions
 */

/* Retrieves the element index for a specific value offset
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_get_element_index_at_value_offset(
     libmfdata_list_t *list,
     libbfio_pool_t *file_io_pool,
     libmfcache_cache_t *cache,
     off64_t value_offset,
     int *element_index,
     size_t *element_offset,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_list = NULL;
	libmfdata_list_element_t *list_element   = NULL;
	static char *function                    = "libmfdata_list_get_element_index_at_value_offset";
	off64_t calculated_value_offset          = 0;
	off64_t element_data_offset              = 0;
	size64_t element_data_size               = 0;
	uint32_t element_data_flags              = 0;
	int calculated_element_index             = 0;
	int element_file_io_pool_entry           = 0;
	int initial_element_index                = 0;
	int number_of_elements                   = 0;
	int result                               = 0;

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libmfdata_internal_list_t *) list;

	if( internal_list->value_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid list - value size value out of bounds.",
		 function );

		return( -1 );
	}
	if( value_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid value offset value less than zero.",
		 function );

		return( -1 );
	}
	if( element_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid element index.",
		 function );

		return( -1 );
	}
	if( element_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid element offset.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     internal_list->elements,
	     &number_of_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from elements array.",
		 function );

		return( -1 );
	}
	if( ( internal_list->flags & LIBMFDATA_LIST_FLAG_CALCULATE_VALUE_OFFSETS ) != 0 )
	{
		for( calculated_element_index = 0;
		     calculated_element_index < number_of_elements;
		     calculated_element_index++ )
		{
			if( libcdata_array_get_entry_by_index(
			     internal_list->elements,
			     calculated_element_index,
			     (intptr_t **) &list_element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve list element: %d from elements array.",
				 function,
				 calculated_element_index );

				return( -1 );
			}
			if( libmfdata_list_element_get_data_range(
			     list_element,
			     &element_file_io_pool_entry,
			     &element_data_offset,
			     &element_data_size,
			     &element_data_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve data range of list element: %d.",
				 function,
				 calculated_element_index );

				return( -1 );
			}
			if( libmfdata_list_element_set_value_offset(
			     list_element,
			     calculated_value_offset,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set data offset of list element: %d.",
				 function,
				 calculated_element_index );

				return( -1 );
			}
			if( value_offset < calculated_value_offset )
			{
				initial_element_index = calculated_element_index;
			}
			calculated_value_offset += (off64_t) element_data_size;
		}
		internal_list->flags &= ~( LIBMFDATA_LIST_FLAG_CALCULATE_VALUE_OFFSETS );
	}
	else
	{
		/* This assumes a fairly even distribution of the sizes of the list elements
		 */
		initial_element_index = (int) ( ( number_of_elements * value_offset ) / internal_list->value_size );
	}
	/* Look for the corresponding list element upwards in the list
	 */
	for( calculated_element_index = initial_element_index;
	     calculated_element_index < number_of_elements;
	     calculated_element_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_list->elements,
		     calculated_element_index,
		     (intptr_t **) &list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve list element: %d from elements array.",
			 function,
			 calculated_element_index );

			return( -1 );
		}
		result = libmfdata_list_element_is_group(
			  list_element,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if list element: %d is a group.",
			 function,
			 calculated_element_index );

			return( -1 );
		}
		else if( result != 0 )
		{
			if( libmfdata_list_expand_group(
			     list,
			     file_io_pool,
			     cache,
			     list_element,
			     calculated_element_index,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to expand element group: %d.",
				 function,
				 calculated_element_index );

				return( -1 );
			}
			if( libmfdata_list_get_element_by_index(
			     list,
			     calculated_element_index,
			     &list_element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve list element: %d.",
				 function,
				 calculated_element_index );

				return( -1 );
			}
			result = libmfdata_list_element_is_group(
				  list_element,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine if list element: %d is a group.",
				 function,
				 calculated_element_index );

				return( -1 );
			}
			else if( result != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported value list element: %d is a group.",
				 function,
				 calculated_element_index );

				return( -1 );
			}
		}
		if( libmfdata_list_element_get_data_range(
		     list_element,
		     &element_file_io_pool_entry,
		     &element_data_offset,
		     &element_data_size,
		     &element_data_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data range of list element: %d.",
			 function,
			 calculated_element_index );

			return( -1 );
		}
		if( libmfdata_list_element_get_value_offset(
		     list_element,
		     &calculated_value_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to retrieve data offset of list element: %d.",
			 function,
			 calculated_element_index );

			return( -1 );
		}
		/* Check if the data offset is in the list element
		 */
		if( ( value_offset >= calculated_value_offset )
		 && ( value_offset < ( calculated_value_offset + (off64_t) element_data_size ) ) )
		{
			value_offset -= calculated_value_offset;

			break;
		}
		/* Check if the data offset is out of bounds
		 */
		if( value_offset < calculated_value_offset )
		{
			calculated_element_index = number_of_elements;

			break;
		}
	}
	if( calculated_element_index >= number_of_elements )
	{
		/* Look for the corresponding list element downwards in the list
		 */
		for( calculated_element_index = initial_element_index;
		     calculated_element_index >= 0;
		     calculated_element_index-- )
		{
			if( libcdata_array_get_entry_by_index(
			     internal_list->elements,
			     calculated_element_index,
			     (intptr_t **) &list_element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve list element: %d from elements array.",
				 function,
				 calculated_element_index );

				return( -1 );
			}
			result = libmfdata_list_element_is_group(
				  list_element,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine if list element: %d is a group.",
				 function,
				 calculated_element_index );

				return( -1 );
			}
			else if( result != 0 )
			{
				if( libmfdata_list_expand_group(
				     list,
				     file_io_pool,
				     cache,
				     list_element,
				     calculated_element_index,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to expand element group: %d.",
					 function,
					 calculated_element_index );

					return( -1 );
				}
				if( libmfdata_list_get_element_by_index(
				     list,
				     calculated_element_index,
				     &list_element,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve list element: %d.",
					 function,
					 calculated_element_index );

					return( -1 );
				}
				result = libmfdata_list_element_is_group(
					  list_element,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to determine if list element: %d is a group.",
					 function,
					 calculated_element_index );

					return( -1 );
				}
				else if( result != 0 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported value list element: %d is a group.",
					 function,
					 calculated_element_index );

					return( -1 );
				}
			}
			if( libmfdata_list_element_get_data_range(
			     list_element,
			     &element_file_io_pool_entry,
			     &element_data_offset,
			     &element_data_size,
			     &element_data_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve data range of list element: %d.",
				 function,
				 calculated_element_index );

				return( -1 );
			}
			if( libmfdata_list_element_get_value_offset(
			     list_element,
			     &calculated_value_offset,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to retrieve data offset of list element: %d.",
				 function,
				 calculated_element_index );

				return( -1 );
			}
			/* Check if the data offset is in the list element
			 */
			if( ( value_offset >= calculated_value_offset )
			 && ( value_offset < ( calculated_value_offset + (off64_t) element_data_size ) ) )
			{
				value_offset -= calculated_value_offset;

				break;
			}
			/* Check if the data offset is out of bounds
			 */
			if( value_offset > calculated_value_offset )
			{
				calculated_element_index = -1;

				break;
			}
		}
	}
	if( calculated_element_index < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid element index value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( value_offset < 0 )
	 || ( value_offset > (off64_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid element offset value out of bounds.",
		 function );

		return( -1 );
	}
	*element_index  = calculated_element_index;
	*element_offset = (size_t) value_offset;

	return( 1 );
}

/* Retrieves the element index for a specific value index
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_get_element_index_at_value_index(
     libmfdata_list_t *list,
     int value_index,
     size_t value_size,
     int *element_index,
     size_t *element_offset,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_list = NULL;
	libmfdata_list_element_t *list_element   = NULL;
	static char *function                    = "libmfdata_list_get_element_index_at_value_index";
	off64_t element_data_offset              = 0;
	size64_t element_data_size               = 0;
	uint64_t number_of_values                = 0;
	uint32_t element_data_flags              = 0;
	int calculated_element_index             = 0;
	int element_file_io_pool_entry           = 0;
	int number_of_elements                   = 0;

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libmfdata_internal_list_t *) list;

	if( value_index < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid value index value less than zero.",
		 function );

		return( -1 );
	}
	if( value_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid value size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( element_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid element index.",
		 function );

		return( -1 );
	}
	if( element_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid element offset.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     internal_list->elements,
	     &number_of_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from elements array.",
		 function );

		return( -1 );
	}
	for( calculated_element_index = 0;
	     calculated_element_index < number_of_elements;
	     calculated_element_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_list->elements,
		     *element_index,
		     (intptr_t **) &list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve element: %d from elements array.",
			 function,
			 calculated_element_index );

			return( -1 );
		}
		if( libmfdata_list_element_get_data_range(
		     list_element,
		     &element_file_io_pool_entry,
		     &element_data_offset,
		     &element_data_size,
		     &element_data_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data range of list element: %d.",
			 function,
			 calculated_element_index );

			return( -1 );
		}
		number_of_values = element_data_size / value_size;

		if( number_of_values > (uint64_t) INT_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid number of values value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( value_index < (int) number_of_values )
		{
			break;
		}
		value_index -= (int) number_of_values;
	}
	if( calculated_element_index >= number_of_elements )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid element index value out of bounds.",
		 function );

		return( -1 );
	}
	if( value_index < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid value index value out of bounds.",
		 function );

		return( -1 );
	}
	*element_index  = calculated_element_index;
	*element_offset = value_size * value_index;

	return( 1 );
}

/* Retrieves the value of the element
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_get_element_value(
     libmfdata_list_t *list,
     libbfio_pool_t *file_io_pool,
     libmfcache_cache_t *cache,
     libmfdata_list_element_t *element,
     intptr_t **element_value,
     uint8_t read_flags,
     libcerror_error_t **error )
{
	libmfcache_cache_value_t *cache_value    = NULL;
	libmfdata_internal_list_t *internal_list = NULL;
	static char *function                    = "libmfdata_list_get_element_value";
        off64_t cache_value_offset               = (off64_t) -1;
	off64_t element_data_offset              = 0;
	size64_t element_data_size               = 0;
	time_t cache_value_timestamp             = 0;
	time_t element_timestamp                 = 0;
	uint32_t element_data_flags              = 0;
	int cache_entry_index                    = -1;
	int cache_value_file_io_pool_entry       = -1;
	int element_file_io_pool_entry           = 0;
	int element_index                        = -1;
	int number_of_cache_entries              = 0;
	int result                               = 0;

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libmfdata_internal_list_t *) list;

	if( libmfdata_list_element_get_element_index(
	     element,
	     &element_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element index from list element.",
		 function );

		return( -1 );
	}
	if( libmfdata_list_element_get_data_range(
	     element,
	     &element_file_io_pool_entry,
	     &element_data_offset,
	     &element_data_size,
	     &element_data_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data range from list element.",
		 function );

		return( -1 );
	}
	if( libmfcache_cache_get_number_of_entries(
	     cache,
	     &number_of_cache_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of cache entries.",
		 function );

		return( -1 );
	}
	if( number_of_cache_entries <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of cache entries value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( read_flags & LIBMFDATA_READ_FLAG_IGNORE_CACHE ) == 0 )
	{
		cache_entry_index = libmfdata_list_calculate_cache_entry_index(
		                     element_index,
		                     number_of_cache_entries );

		if( libmfcache_cache_get_value_by_index(
		     cache,
		     cache_entry_index,
		     &cache_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve cache entry: %d from cache.",
			 function,
			 cache_entry_index );

			return( -1 );
		}
		if( cache_value != NULL )
		{
			if( libmfdata_list_element_get_timestamp(
			     element,
			     &element_timestamp,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve time stamp from list element.",
				 function );

				return( -1 );
			}
			if( libmfcache_cache_value_get_identifier(
			     cache_value,
			     &cache_value_file_io_pool_entry,
			     &cache_value_offset,
			     &cache_value_timestamp,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve cache value identifier.",
				 function );

				return( -1 );
			}
		}
		if( ( element_file_io_pool_entry == cache_value_file_io_pool_entry )
		 && ( element_data_offset == cache_value_offset )
		 && ( element_timestamp == cache_value_timestamp ) )
		{
			result = 1;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( result == 0 )
			{
				libcnotify_printf(
				 "%s: cache: 0x%08" PRIjx " miss (%d out of %d)\n",
				 function,
				 (intptr_t) cache,
				 cache_entry_index,
				 number_of_cache_entries );
			}
			else
			{
				libcnotify_printf(
				 "%s: cache: 0x%08" PRIjx " hit (%d out of %d)\n",
				 function,
				 (intptr_t) cache,
				 cache_entry_index,
				 number_of_cache_entries );
			}
		}
#endif
	}
	if( result == 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: reading element data from pool entry: %d at offset: %" PRIi64 " (0x%08" PRIx64 ") of size: %" PRIu64 "\n",
			 function,
			 element_file_io_pool_entry,
			 element_data_offset,
			 element_data_offset,
			 element_data_size );
		}
#endif
		/* Read the element data from the file IO handle
		 */
		if( internal_list->read_element_data(
		     internal_list->io_handle,
		     file_io_pool,
		     element,
		     cache,
		     element_file_io_pool_entry,
		     element_data_offset,
		     element_data_size,
		     element_data_flags,
		     read_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read element data from pool entry: %d at offset: %" PRIi64 ".",
			 function,
			 element_file_io_pool_entry,
			 element_data_offset );

			return( -1 );
		}
		cache_entry_index = libmfdata_list_calculate_cache_entry_index(
		                     element_index,
		                     number_of_cache_entries );

		if( libmfcache_cache_get_value_by_index(
		     cache,
		     cache_entry_index,
		     &cache_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve cache entry: %d from cache.",
			 function,
			 cache_entry_index );

			return( -1 );
		}
		if( cache_value != NULL )
		{
			if( libmfdata_list_element_get_timestamp(
			     element,
			     &element_timestamp,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve time stamp from list element.",
				 function );

				return( -1 );
			}
			if( libmfcache_cache_value_get_identifier(
			     cache_value,
			     &cache_value_file_io_pool_entry,
			     &cache_value_offset,
			     &cache_value_timestamp,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve cache value identifier.",
				 function );

				return( -1 );
			}
		}
		if( ( element_file_io_pool_entry != cache_value_file_io_pool_entry )
		 || ( element_data_offset != cache_value_offset )
		 || ( element_timestamp != cache_value_timestamp ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing cache value.",
			 function );

			return( -1 );
		}
	}
	if( libmfcache_cache_value_get_value(
	     cache_value,
	     element_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element value from cache value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the value of a specific element
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_get_element_value_by_index(
     libmfdata_list_t *list,
     libbfio_pool_t *file_io_pool,
     libmfcache_cache_t *cache,
     int element_index,
     intptr_t **element_value,
     uint8_t read_flags,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_list = NULL;
	libmfdata_list_element_t *list_element   = NULL;
	static char *function                    = "libmfdata_list_get_element_value_by_index";
	int result                               = 0;

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libmfdata_internal_list_t *) list;

	if( libcdata_array_get_entry_by_index(
	     internal_list->elements,
	     element_index,
	     (intptr_t **) &list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve list element: %d from elements array.",
		 function,
		 element_index );

		return( -1 );
	}
	result = libmfdata_list_element_is_group(
		  list_element,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if list element: %d is a group.",
		 function,
		 element_index );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libmfdata_list_expand_group(
		     list,
		     file_io_pool,
		     cache,
		     list_element,
		     element_index,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to expand element group: %d.",
			 function,
			 element_index );

			return( -1 );
		}
		if( libmfdata_list_get_element_by_index(
		     list,
		     element_index,
		     &list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve list element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
		result = libmfdata_list_element_is_group(
		          list_element,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if list element: %d is a group.",
			 function,
			 element_index );

			return( -1 );
		}
		else if( result != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported value list element: %d is a group.",
			 function,
			 element_index );

			return( -1 );
		}
	}
	if( libmfdata_list_get_element_value(
	     list,
	     file_io_pool,
	     cache,
	     list_element,
	     element_value,
	     read_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the value an element at a specific offset
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_get_element_value_at_offset(
     libmfdata_list_t *list,
     libbfio_pool_t *file_io_pool,
     libmfcache_cache_t *cache,
     off64_t value_offset,
     intptr_t **element_value,
     uint8_t read_flags,
     libcerror_error_t **error )
{
	static char *function = "libmfdata_list_get_element_value_at_offset";
	size_t element_offset = 0;
	int element_index     = 0;

	if( libmfdata_list_get_element_index_at_value_offset(
	     list,
	     file_io_pool,
	     cache,
	     value_offset,
	     &element_index,
	     &element_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element index at offset: %" PRIi64 ".",
		 function,
		 value_offset );

		return( -1 );
	}
	if( libmfdata_list_get_element_value_by_index(
	     list,
	     file_io_pool,
	     cache,
	     element_index,
	     element_value,
	     read_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element: %d value.",
		 function,
		 element_index );

		return( -1 );
	}
	return( 1 );
}

/* Sets the value of a specific element
 *
 * If the flag LIBMFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED is set the list
 * takes over management of the value and the value is freed when
 * no longer needed.
 *
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_set_element_value(
     libmfdata_list_t *list,
     libmfcache_cache_t *cache,
     libmfdata_list_element_t *element,
     intptr_t *element_value,
     int (*free_element_value)(
            intptr_t **element_value,
            libcerror_error_t **error ),
     uint8_t flags,
     libcerror_error_t **error )
{
	static char *function          = "libmfdata_list_set_element_value";
	off64_t element_data_offset    = 0;
	size64_t element_data_size     = 0;
	time_t element_timestamp       = 0;
	uint32_t element_data_flags    = 0;
	int cache_entry_index          = -1;
	int element_index              = -1;
	int element_file_io_pool_entry = 0;
	int number_of_cache_entries    = 0;

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	if( libmfdata_list_element_get_data_range(
	     element,
	     &element_file_io_pool_entry,
	     &element_data_offset,
	     &element_data_size,
	     &element_data_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data range from list element.",
		 function );

		return( -1 );
	}
	if( libmfdata_list_element_get_timestamp(
	     element,
	     &element_timestamp,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve time stamp from list element.",
		 function );

		return( -1 );
	}
	if( libmfcache_cache_get_number_of_entries(
	     cache,
	     &number_of_cache_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of cache entries.",
		 function );

		return( -1 );
	}
	if( libmfdata_list_element_get_element_index(
	     element,
	     &element_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element index from list element.",
		 function );

		return( -1 );
	}
	if( number_of_cache_entries <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of cache entries value out of bounds.",
		 function );

		return( -1 );
	}
	cache_entry_index = libmfdata_list_calculate_cache_entry_index(
	                     element_index,
	                     number_of_cache_entries );

	if( libmfcache_cache_set_value_by_index(
	     cache,
	     cache_entry_index,
	     element_file_io_pool_entry,
	     element_data_offset,
	     element_timestamp,
	     element_value,
	     free_element_value,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set value in cache entry: %d.",
		 function,
		 cache_entry_index );

		return( -1 );
	}
	return( 1 );
}

/* Sets the value of a specific element
 *
 * If the flag LIBMFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED is set the list
 * takes over management of the value and the value is freed when
 * no longer needed.
 *
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_set_element_value_by_index(
     libmfdata_list_t *list,
     libmfcache_cache_t *cache,
     int element_index,
     intptr_t *element_value,
     int (*free_element_value)(
            intptr_t **element_value,
            libcerror_error_t **error ),
     uint8_t flags,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_list = NULL;
	libmfdata_list_element_t *list_element   = NULL;
	static char *function                    = "libmfdata_list_set_element_value_by_index";

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libmfdata_internal_list_t *) list;

	if( libcdata_array_get_entry_by_index(
	     internal_list->elements,
	     element_index,
	     (intptr_t **) &list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve list element: %d from elements array.",
		 function,
		 element_index );

		return( -1 );
	}
	if( libmfdata_list_set_element_value(
	     list,
	     cache,
	     list_element,
	     element_value,
	     free_element_value,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set element value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the value size of the list
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_get_value_size(
     libmfdata_list_t *list,
     size64_t *value_size,
     libcerror_error_t **error )
{
	libmfdata_internal_list_t *internal_list = NULL;
	static char *function                    = "libmfdata_list_get_value_size";

	if( list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	internal_list = (libmfdata_internal_list_t *) list;

	if( value_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value size.",
		 function );

		return( -1 );
	}
	*value_size = internal_list->value_size;

	return( 1 );
}

