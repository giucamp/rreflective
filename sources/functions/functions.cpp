/***********************************************************************************

Copyright 2011-2012 Giuseppe Campana - giu.campana@gmail.com
All rights reserved

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list
      of conditions and the following disclaimer in the documentation and/or other materials
      provided with the distribution.

THIS SOFTWARE IS PROVIDED BY GIUSEPPE CAMPANA ''AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL GIUSEPPE CAMPANA OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***********************************************************************************/


namespace reflective
{
	// count_inhertied_properties
	size_t count_inhertied_properties( const Class & class_object )
	{
		size_t result = 0;
		const Class * curr_class = &class_object;
		do {
			
			#if REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE
				unsigned base_type_count = curr_class->base_types_count();
				if( base_type_count > 1 )
				{
					unsigned base_type_index = 1;
					do {
						const Type * base_type = curr_class->base_type( base_type_index );
						REFLECTIVE_ASSERT( base_type != nullptr );

						REFLECTIVE_ASSERT( is_instance_of<Class>( *base_type ) ); /* non-class
							types cannot be base of class types. */

						const Class * base_class = static_cast<const Class*>( base_type );

						result += count_inhertied_properties( *base_class );

					} while( ++base_type_index < base_type_count );
				}
			#endif

			result += curr_class->properties().count();

			const Type * next_type = curr_class->base_type( 0 );
			REFLECTIVE_ASSERT( !next_type || is_instance_of<Class>( *next_type ) ); /* non-class
				types cannot be base of class types. */
			curr_class = static_cast<const Class*>( next_type );

		} while( curr_class != nullptr );

		// done
		return result;
	}

	// enum_inhertied_properties
	size_t _enum_inhertied_properties( const Class & class_object, const void * object,
		EnumPropertiesCallback callback, void * user_data, size_t * inout_index )
	{
		const size_t invalid_uint_address = ~size_t( 0 );

		unsigned base_type_count = class_object.base_types_count();
		if( base_type_count > 0 )
		{
			unsigned base_type_index = 0;
			do {
				const Type * base_type = class_object.base_type( base_type_index );
				REFLECTIVE_ASSERT( base_type != nullptr );

				REFLECTIVE_ASSERT( is_instance_of<Class>( *base_type ) ); /* non-class
					types cannot be base of class types. */

				const Class & base_class = *static_cast<const Class*>( base_type );

				const void * base_object_address = nullptr;
				if( object )
					base_object_address = class_object.pointer_to_base_type( object, base_type_index );

				const size_t recursion_result = _enum_inhertied_properties( base_class, 
					base_object_address, callback, user_data, inout_index );
				if( recursion_result == invalid_uint_address )
					return invalid_uint_address;

			} while( ++base_type_index < base_type_count );
		}

		const PropertyList & properties = class_object.properties();
		const size_t property_count = properties.count();
		if( property_count > 0 )
		{
			size_t property_index = 0;
			do {
				const Property & property = properties[ property_index ];

				bool callback_result = (*callback)( class_object, property, object, *inout_index, user_data );
				if( !callback_result )
					return *inout_index = invalid_uint_address;

				++*inout_index;

			} while( ++property_index < property_count );
		}

		return *inout_index;
	}

	// get_inhertied_properties
	size_t get_inhertied_properties( const Class & class_,
		PropertyAndClass * result, size_t result_max_size )
	{
		PropertyAndClass * next_result = result;
		size_t curr_required_size = 0;

		const Class * curr_class = &class_;
		do {

			#if REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE
				unsigned base_type_count = curr_class->base_types_count();
				if( base_type_count > 1 )
				{
					unsigned base_type_index = 1;
					do {
						const Type * base_type = curr_class->base_type( base_type_index );
						REFLECTIVE_ASSERT( base_type != nullptr );

						REFLECTIVE_ASSERT( is_instance_of<Class>( *base_type ) ); /* non-class
							types cannot be base of class types. */

						const Class & base_class = *static_cast<const Class*>( base_type );

						size_t max_size = 0;

						if( result_max_size > curr_required_size )
							max_size = result_max_size - curr_required_size;

						const size_t recursion_result = get_inhertied_properties( base_class, 
							 next_result, max_size );

						curr_required_size += recursion_result;

					} while( ++base_type_index < base_type_count );
				}				
			#endif

			const PropertyList & properties = curr_class->properties();
			const size_t property_count = properties.count();
			if( property_count > 0 )
			{
				size_t property_index = 0;
				do {

					if( ++curr_required_size <= result_max_size )
					{					
						next_result->property = &properties[ property_index ];
						next_result->owning_class = curr_class;
						next_result++;
					}

				} while( ++property_index < property_count );
			}
			else
			{
				if( ++curr_required_size <= result_max_size )
				{					
					next_result->property = nullptr;
					next_result->owning_class = curr_class;
					next_result++;
				}
			}

			const Type * next_type = curr_class->base_type( 0 );
			REFLECTIVE_ASSERT( !next_type || is_instance_of<Class>( *next_type ) ); /* non-class
				types cannot be base of class types. */

			curr_class = static_cast<const Class*>( next_type );

		} while( curr_class != nullptr );

		return curr_required_size;
	}

	// get_inhertied_properties_count
	size_t get_inhertied_properties_count( const Class & class_ )
	{
		size_t curr_required_size = 0;

		const Class * curr_class = &class_;
		do {

			#if REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE
				unsigned base_type_count = curr_class->base_types_count();
				if( base_type_count > 1 )
				{
					unsigned base_type_index = 1;
					do {
						const Type * base_type = curr_class->base_type( base_type_index );
						REFLECTIVE_ASSERT( base_type != nullptr );

						REFLECTIVE_ASSERT( is_instance_of<Class>( *base_type ) ); /* non-class
							types cannot be base of class types. */

						const Class & base_class = *static_cast<const Class*>( base_type );

						// size_t max_size = 0;

						const size_t recursion_result = get_inhertied_properties_count( base_class );

						curr_required_size += recursion_result;

					} while( ++base_type_index < base_type_count );
				}				
			#endif

			const PropertyList & properties = curr_class->properties();
			const size_t property_count = properties.count();
			if( property_count > 0 )
			{
				curr_required_size += property_count;
			}
			else
			{
				curr_required_size++;
			}

			const Type * next_type = curr_class->base_type( 0 );
			REFLECTIVE_ASSERT( !next_type || is_instance_of<Class>( *next_type ) ); /* non-class
				types cannot be base of class types. */

			curr_class = static_cast<const Class*>( next_type );

		} while( curr_class != nullptr );

		return curr_required_size;
	}

	// find_inhertied_property
	const Property * find_inhertied_property( const Class & class_obj, 
		SymbolName name, const Class * * out_owning_class,
		void * * inout_object )
	{
		*out_owning_class = nullptr;

		const Property * result;

		const Class * curr_class = &class_obj;
		do {
			
			#if REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE
				unsigned base_type_count = curr_class->base_types_count();
				if( base_type_count > 1 )
				{
					unsigned base_type_index = 1;
					do {
						const Type * base_type = curr_class->base_type( base_type_index );
						REFLECTIVE_ASSERT( base_type != nullptr );

						REFLECTIVE_ASSERT( is_instance_of<Class>( *base_type ) ); /* non-class
							types cannot be base of class types. */

						const Class * base_class = static_cast<const Class*>( base_type );

						if( inout_object )
							*inout_object = curr_class->pointer_to_base_type( *inout_object, base_type_index );

						result = find_inhertied_property( *base_class, 
							name, out_owning_class, inout_object );
						if( result )
							return result;

					} while( ++base_type_index < base_type_count );
				}
			#endif

			result = curr_class->properties().find( name );
			if( result )
			{
				*out_owning_class = curr_class;
				return result;
			}

			const Type * next_type = curr_class->base_type( 0 );
			REFLECTIVE_ASSERT( !next_type || is_instance_of<Class>( *next_type ) ); /* non-class
				types cannot be base of class types. */

			if( inout_object && next_type )
				*inout_object = curr_class->pointer_to_base_type( *inout_object, 0 );

			curr_class = static_cast<const Class*>( next_type );

		} while( curr_class != nullptr );

		// not found
		return nullptr;
	}

	// find_inhertied_action
	const Action * find_inhertied_action( const Class & class_obj, 
		SymbolName name, const Class * * out_owning_class )
	{
		*out_owning_class = nullptr;

		const Action * result;

		const Class * curr_class = &class_obj;
		do {
			
			#if REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE
				unsigned base_type_count = curr_class->base_types_count();
				if( base_type_count > 1 )
				{
					unsigned base_type_index = 1;
					do {
						const Type * base_type = curr_class->base_type( base_type_index );
						REFLECTIVE_ASSERT( base_type != nullptr );

						REFLECTIVE_ASSERT( is_instance_of<Class>( *base_type ) ); /* non-class
							types cannot be base of class types. */

						const Class * base_class = static_cast<const Class*>( base_type );

						result = find_inhertied_action( *base_class, 
							name, out_owning_class );
						if( result )
							return result;

					} while( ++base_type_index < base_type_count );
				}
			#endif

			result = curr_class->actions().find( name );
			if( result )
			{
				*out_owning_class = curr_class;
				return result;
			}

			const Type * next_type = curr_class->base_type( 0 );
			REFLECTIVE_ASSERT( !next_type || is_instance_of<Class>( *next_type ) ); /* non-class
				types cannot be base of class types. */
			curr_class = static_cast<const Class*>( next_type );

		} while( curr_class != nullptr );

		// not found
		return nullptr;
	}

	// object_identifier_to_string
	bool object_identifier_to_string( StringOutputStream & dest_buffer, const void * object, const Type & type )
	{
		if( !is_instance_of<Class>( type ) )
			return false;

		return object_identifier_to_string( dest_buffer, object,
			static_cast<const Class &>( type ) );		
	}

	bool object_identifier_to_string( StringOutputStream & dest_buffer, const void * object, const Class & class_obj )
	{
		const void * curr_object = object;
		const Class * curr_class = &class_obj;
		do {

			const Property * identifier_property = curr_class->identifier_property();
			if( identifier_property != nullptr )
			{
				const Type & property_type = identifier_property->type();

				if( property_type.check_capabilities( Type::eHasToString ) )
				{
					AutoLifo value_buffer( property_type.alignment(),
						property_type.size() );
					
					if( identifier_property->get_value( curr_object, value_buffer.get() ) )
					{
						const void * final_value = identifier_property->qualification().full_indirection( value_buffer.get() );
						identifier_property->qualification().final_type()->to_string( dest_buffer, final_value );
						return true;
					}
				}
			}
			
			#if REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE
				unsigned base_type_count = curr_class->base_types_count();
				if( base_type_count > 1 )
				{
					unsigned base_type_index = 1;
					do {
						const Type * base_type = curr_class->base_type( base_type_index );
						REFLECTIVE_ASSERT( base_type != nullptr );

						REFLECTIVE_ASSERT( is_instance_of<Class>( *base_type ) ); /* non-class
							types cannot be base of class types. */

						const Class * base_class = static_cast<const Class*>( base_type );
						
						const void * base_object_address = nullptr;
						if( curr_object )
							base_object_address = curr_class->pointer_to_base_type( curr_object, base_type_index );

						bool result = object_identifier_to_string( dest_buffer, base_object_address, *base_class );
						if( result )
							return true;

					} while( ++base_type_index < base_type_count );
				}
			#endif

			const Type * next_type = curr_class->base_type( 0 );
			REFLECTIVE_ASSERT( !next_type || is_instance_of<Class>( *next_type ) ); /* non-class
				types cannot be base of class types. */

			if( curr_object && next_type )
				curr_object = curr_class->pointer_to_base_type( curr_object, 0 );

			curr_class = static_cast<const Class*>( next_type );	

		} while( curr_class != nullptr );

		return false;
	}



					/**** AutoLifo ****/

	// AutoLifo::constructor
	AutoLifo::AutoLifo( size_t initial_alignment, size_t initial_size )
		: _alignment( initial_alignment ), _length( initial_size )
	{
		_buffer = reflective_externals::mem_lifo_alloc( initial_alignment, initial_size );
	}

	// AutoLifo::destructor
	AutoLifo::~AutoLifo()
	{
		reflective_externals::mem_lifo_free( _buffer );
	}

	// AutoLifo::get
	void * AutoLifo::get( size_t needed_alignment, size_t needed_size )
	{
		if( _alignment < needed_alignment || needed_size < _length )
		{
			reflective_externals::mem_lifo_free( _buffer );

			_buffer = reflective_externals::mem_lifo_alloc( needed_alignment, needed_size );
			_alignment = needed_alignment;
			_length = needed_size;
		}

		return _buffer;
	}

} // namespace reflective

namespace reflective_externals
{
	// reflection of reflective::PropertyAndClass
	void init_type( reflective::Type * volatile * o_result,
		reflective::PropertyAndClass * null_pointer_1,
		reflective::PropertyAndClass * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::PropertyAndClass ThisClass;
		
		if( *o_result != nullptr )
			return;
		
		// class object
		Class * class_object = new_class<ThisClass>( "reflective", "PropertyAndClass" );
		*o_result = class_object;
		class_object->set_no_base_type();
		class_object->set_life_functions( LifeFunctions::from_type<ThisClass>(
			eConstructorCopyAssignmentDestructor ) );
		
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "property", &ThisClass::property ),
			//new_property<ThisClass>( "down_caster", &ThisClass::down_caster ),
		};
		
		// assign members
		class_object->assign_properties( properties );
	}
	
} // namespace reflective_externals
