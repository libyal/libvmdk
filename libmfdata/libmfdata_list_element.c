/*
 * The list element functions
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
#include "libmfdata_group.h"
#include "libmfdata_libbfio.h"
#include "libmfdata_libcerror.h"
#include "libmfdata_libmfcache.h"
#include "libmfdata_list.h"
#include "libmfdata_list_element.h"
#include "libmfdata_range.h"
#include "libmfdata_types.h"

/* Initializes the element
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_element_initialize(
     libmfdata_list_element_t **element,
     libmfdata_list_t *list,
     int element_index,
     libcerror_error_t **error )
{
	libmfdata_internal_list_element_t *internal_element = NULL;
	static char *function                               = "libmfdata_list_element_initialize";

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
	if( *element != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid element value already set.",
		 function );

		return( -1 );
	}
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
	if( element_index < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid element index value less than zero.",
		 function );

		return( -1 );
	}
	internal_element = memory_allocate_structure(
	                    libmfdata_internal_list_element_t );

	if( internal_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create element.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_element,
	     0,
	     sizeof( libmfdata_internal_list_element_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear element.",
		 function );

		goto on_error;
	}
	if( libmfdata_range_initialize(
	     &( internal_element->data_range ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data range.",
		 function );

		goto on_error;
	}
	internal_element->list          = list;
	internal_element->element_index = element_index;
	internal_element->timestamp     = libmfdata_date_time_get_timestamp();

	*element = (libmfdata_list_element_t *) internal_element;

	return( 1 );

on_error:
	if( internal_element != NULL )
	{
		memory_free(
		 internal_element );
	}
	return( -1 );
}

/* Frees the element
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_element_free(
     libmfdata_list_element_t **element,
     libcerror_error_t **error )
{
	libmfdata_internal_list_element_t *internal_element = NULL;
	static char *function                               = "libmfdata_list_element_free";
	int result                                          = 1;

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
	if( *element != NULL )
	{
		internal_element = (libmfdata_internal_list_element_t *) *element;
		*element         = NULL;

		if( ( internal_element->flags & LIBMFDATA_LIST_ELEMENT_FLAG_IS_GROUP ) != 0 )
		{
			if( internal_element->group_values != NULL )
			{
				if( internal_element->group_values->number_of_elements == 1 )
				{
					if( libmfdata_group_free(
					     &( internal_element->group_values ),
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
						 "%s: unable to free group values.",
						 function );

						result = -1;
					}
					internal_element->flags &= ~( LIBMFDATA_LIST_ELEMENT_FLAG_IS_GROUP );
				}
				else
				{
					internal_element->group_values->number_of_elements -= 1;
				}
			}
		}
		if( ( internal_element->flags & LIBMFDATA_LIST_ELEMENT_FLAG_IS_GROUP ) == 0 )
		{
			if( internal_element->data_range != NULL )
			{
				if( libmfdata_range_free(
				     &( internal_element->data_range ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free data range.",
					 function );

					result = -1;
				}
			}
			if( internal_element->backup_data_range != NULL )
			{
				if( libmfdata_range_free(
				     &( internal_element->backup_data_range ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free backup data range.",
					 function );

					result = -1;
				}
			}
			memory_free(
			 internal_element );
		}
	}
	return( result );
}

/* Clones (duplicates) the element
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_element_clone(
     libmfdata_list_element_t **destination_element,
     libmfdata_list_element_t *source_element,
     libmfdata_list_t *list,
     int element_index,
     libcerror_error_t **error )
{
	libmfdata_internal_list_element_t *internal_destination_element = NULL;
	libmfdata_internal_list_element_t *internal_source_element      = NULL;
	static char *function                                           = "libmfdata_list_element_clone";

	if( destination_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination element.",
		 function );

		return( -1 );
	}
	if( *destination_element != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: destination element already set.",
		 function );

		return( -1 );
	}
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
	if( element_index < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid element index value less than zero.",
		 function );

		return( -1 );
	}
	if( source_element == NULL )
	{
		*destination_element = NULL;

		return( 1 );
	}
	internal_source_element = (libmfdata_internal_list_element_t *) source_element;

	internal_destination_element = memory_allocate_structure(
	                                libmfdata_internal_list_element_t );

	if( internal_destination_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination element.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_destination_element,
	     0,
	     sizeof( libmfdata_internal_list_element_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear destination element.",
		 function );

		memory_free(
		 internal_destination_element );

		return( -1 );
	}
	if( libmfdata_range_clone(
	     &( internal_destination_element->data_range ),
	     internal_source_element->data_range,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination data range.",
		 function );

		goto on_error;
	}
	if( libmfdata_range_clone(
	     &( internal_destination_element->backup_data_range ),
	     internal_source_element->backup_data_range,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination backup data range.",
		 function );

		goto on_error;
	}
	if( libmfdata_group_clone(
	     &( internal_destination_element->group_values ),
	     internal_source_element->group_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination group values.",
		 function );

		goto on_error;
	}
	internal_destination_element->list          = list;
	internal_destination_element->element_index = element_index;
	internal_destination_element->value_offset  = internal_source_element->value_offset;
	internal_destination_element->value_size    = internal_source_element->value_size;
	internal_destination_element->timestamp     = libmfdata_date_time_get_timestamp();

	*destination_element = (libmfdata_list_element_t *) internal_destination_element;

	return( 1 );

on_error:
	if( internal_destination_element != NULL )
	{
		if( internal_destination_element->group_values != NULL )
		{
			libmfdata_group_free(
			 &( internal_destination_element->group_values ),
			 NULL );
		}
		if( internal_destination_element->backup_data_range != NULL )
		{
			libmfdata_range_free(
			 &( internal_destination_element->backup_data_range ),
			 NULL );
		}
		if( internal_destination_element->data_range != NULL )
		{
			libmfdata_range_free(
			 &( internal_destination_element->data_range ),
			 NULL );
		}
		memory_free(
		 internal_destination_element );
	}
	return( -1 );
}

/* Retrieves the element index
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_element_get_element_index(
     libmfdata_list_element_t *element,
     int *element_index,
     libcerror_error_t **error )
{
	libmfdata_internal_list_element_t *internal_element = NULL;
	static char *function                               = "libmfdata_list_element_get_element_index";

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
	internal_element = (libmfdata_internal_list_element_t *) element;

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
	*element_index = internal_element->element_index;

	return( 1 );
}

/* Sets the element index
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_element_set_element_index(
     libmfdata_list_element_t *element,
     int element_index,
     libcerror_error_t **error )
{
	libmfdata_internal_list_element_t *internal_element = NULL;
	static char *function                               = "libmfdata_list_element_set_element_index";

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
	internal_element = (libmfdata_internal_list_element_t *) element;

	if( element_index < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid element index value less than zero.",
		 function );

		return( -1 );
	}
	internal_element->element_index = element_index;

	return( 1 );
}

/* Retrieves the time stamp
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_element_get_timestamp(
     libmfdata_list_element_t *element,
     time_t *timestamp,
     libcerror_error_t **error )
{
	libmfdata_internal_list_element_t *internal_element = NULL;
	static char *function                               = "libmfdata_list_element_get_timestamp";

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
	internal_element = (libmfdata_internal_list_element_t *) element;

	if( timestamp == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid time stamp.",
		 function );

		return( -1 );
	}
	*timestamp = internal_element->timestamp;

	return( 1 );
}

/* Retrieves the value offset
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_element_get_value_offset(
     libmfdata_list_element_t *element,
     off64_t *value_offset,
     libcerror_error_t **error )
{
	libmfdata_internal_list_element_t *internal_element = NULL;
	static char *function                               = "libmfdata_list_element_get_value_offset";

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
	internal_element = (libmfdata_internal_list_element_t *) element;

	if( value_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value offset.",
		 function );

		return( -1 );
	}
	*value_offset = internal_element->value_offset;

	return( 1 );
}

/* Sets the value offset
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_element_set_value_offset(
     libmfdata_list_element_t *element,
     off64_t value_offset,
     libcerror_error_t **error )
{
	libmfdata_internal_list_element_t *internal_element = NULL;
	static char *function                               = "libmfdata_list_element_set_value_offset";

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
	internal_element = (libmfdata_internal_list_element_t *) element;

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
	internal_element->value_offset = value_offset;

	return( 1 );
}

/* Retrieves the value size
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_element_get_value_size(
     libmfdata_list_element_t *element,
     size64_t *value_size,
     libcerror_error_t **error )
{
	libmfdata_internal_list_element_t *internal_element = NULL;
	static char *function                               = "libmfdata_list_element_get_value_size";

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
	internal_element = (libmfdata_internal_list_element_t *) element;

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
	*value_size = internal_element->value_size;

	return( 1 );
}

/* Sets the value size
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_element_set_value_size(
     libmfdata_list_element_t *element,
     size64_t value_size,
     libcerror_error_t **error )
{
	libmfdata_internal_list_element_t *internal_element = NULL;
	static char *function                               = "libmfdata_list_element_set_value_size";

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
	internal_element = (libmfdata_internal_list_element_t *) element;

	if( value_size > (size64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid value size value exceeds maximum.",
		 function );

		return( -1 );
	}
	internal_element->value_size = value_size;

	return( 1 );
}

/* Group functions
 */

/* Determines if the element is a group (element)
 * Returns 1 if element is set, 0 if not or -1 on error
 */
int libmfdata_list_element_is_group(
     libmfdata_list_element_t *element,
     libcerror_error_t **error )
{
	libmfdata_internal_list_element_t *internal_element = NULL;
	static char *function                               = "libmfdata_list_element_is_group";

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
	internal_element = (libmfdata_internal_list_element_t *) element;

	if( ( internal_element->flags & LIBMFDATA_LIST_ELEMENT_FLAG_IS_GROUP ) != 0 )
	{
		return( 1 );
	}
	return( 0 );
}

/* Retrieves the group values
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_element_get_group_values(
     libmfdata_list_element_t *element,
     int *number_of_elements,
     libcerror_error_t **error )
{
	libmfdata_internal_list_element_t *internal_element = NULL;
	static char *function                               = "libmfdata_list_element_get_group_values";

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
	internal_element = (libmfdata_internal_list_element_t *) element;

	if( ( internal_element->flags & LIBMFDATA_LIST_ELEMENT_FLAG_IS_GROUP ) == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: element is not a group.",
		 function );

		return( -1 );
	}
	if( libmfdata_group_get_values(
	     internal_element->group_values,
	     number_of_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve group values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the group values
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_element_set_group_values(
     libmfdata_list_element_t *element,
     int number_of_elements,
     libcerror_error_t **error )
{
	libmfdata_internal_list_element_t *internal_element = NULL;
	static char *function                               = "libmfdata_list_element_set_group_values";

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
	internal_element = (libmfdata_internal_list_element_t *) element;

	if( ( internal_element->flags & LIBMFDATA_LIST_ELEMENT_FLAG_IS_GROUP ) == 0 )
	{
		if( internal_element->group_values == NULL )
		{
			if( libmfdata_group_initialize(
			     &( internal_element->group_values ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create group values.",
				 function );

				return( -1 );
			}
		}
		internal_element->flags |= LIBMFDATA_LIST_ELEMENT_FLAG_IS_GROUP;
	}
	if( libmfdata_group_set_values(
	     internal_element->group_values,
	     number_of_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set group values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Data range functions
 */

/* Retrieves the data range
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_element_get_data_range(
     libmfdata_list_element_t *element,
     int *file_io_pool_entry,
     off64_t *offset,
     size64_t *size,
     uint32_t *flags,
     libcerror_error_t **error )
{
	libmfdata_internal_list_element_t *internal_element = NULL;
	static char *function                               = "libmfdata_list_element_get_data_range";

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
	internal_element = (libmfdata_internal_list_element_t *) element;

	if( libmfdata_range_get_values(
	     internal_element->data_range,
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
		 "%s: unable to retrieve data range.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the data range
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_element_set_data_range(
     libmfdata_list_element_t *element,
     int file_io_pool_entry,
     off64_t offset,
     size64_t size,
     uint32_t flags,
     libcerror_error_t **error )
{
	libmfdata_internal_list_element_t *internal_element = NULL;
	static char *function                               = "libmfdata_list_element_set_data_range";

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
	internal_element = (libmfdata_internal_list_element_t *) element;

	if( libmfdata_range_set_values(
	     internal_element->data_range,
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
		 "%s: unable to set data range.",
		 function );

		return( -1 );
	}
	internal_element->timestamp = libmfdata_date_time_get_timestamp();

	return( 1 );
}

/* Backup data range functions
 */

/* Retrieves the backup data range
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libmfdata_list_element_get_backup_data_range(
     libmfdata_list_element_t *element,
     int *file_io_pool_entry,
     off64_t *offset,
     size64_t *size,
     uint32_t *flags,
     libcerror_error_t **error )
{
	libmfdata_internal_list_element_t *internal_element = NULL;
	static char *function                               = "libmfdata_list_element_get_backup_data_range";

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
	internal_element = (libmfdata_internal_list_element_t *) element;

	if( internal_element->backup_data_range == NULL )
	{
		return( 0 );
	}
	if( libmfdata_range_get_values(
	     internal_element->backup_data_range,
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
		 "%s: unable to retrieve backup data range.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the backup data range
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_element_set_backup_data_range(
     libmfdata_list_element_t *element,
     int file_io_pool_entry,
     off64_t offset,
     size64_t size,
     uint32_t flags,
     libcerror_error_t **error )
{
	libmfdata_internal_list_element_t *internal_element = NULL;
	static char *function                               = "libmfdata_list_element_set_backup_data_range";

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
	internal_element = (libmfdata_internal_list_element_t *) element;

	if( internal_element->backup_data_range == NULL )
	{
		if( libmfdata_range_initialize(
		     &( internal_element->backup_data_range ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create backup data range.",
			 function );

			return( 1 );
		}
	}
	if( libmfdata_range_set_values(
	     internal_element->backup_data_range,
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
		 "%s: unable to set backup data range.",
		 function );

		return( -1 );
	}
	internal_element->timestamp = libmfdata_date_time_get_timestamp();

	return( 1 );
}

/* Element value functions
 */

/* Retrieves the element value
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_element_get_element_value(
     libmfdata_list_element_t *element,
     libbfio_pool_t *file_io_pool,
     libmfcache_cache_t *cache,
     intptr_t **element_value,
     uint8_t read_flags,
     libcerror_error_t **error )
{
	libmfdata_internal_list_element_t *internal_element = NULL;
	static char *function                               = "libmfdata_list_element_get_element_value";

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
	internal_element = (libmfdata_internal_list_element_t *) element;

	if( libmfdata_list_get_element_value(
	     internal_element->list,
	     file_io_pool,
	     cache,
	     element,
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

/* Sets the element value
 *
 * If the flag LIBMFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED is set the list element
 * takes over management of the value and the value is freed when
 * no longer needed.
 *
 * Returns 1 if successful or -1 on error
 */
int libmfdata_list_element_set_element_value(
     libmfdata_list_element_t *element,
     libmfcache_cache_t *cache,
     intptr_t *element_value,
     int (*free_element_value)(
            intptr_t **element_value,
            libcerror_error_t **error ),
     uint8_t flags,
     libcerror_error_t **error )
{
	libmfdata_internal_list_element_t *internal_element = NULL;
	static char *function                               = "libmfdata_list_element_set_element_value";

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
	internal_element = (libmfdata_internal_list_element_t *) element;

	if( libmfdata_list_set_element_value(
	     internal_element->list,
	     cache,
	     element,
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

