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

#ifndef _IMPLEMENTING_REFLECTIVE
	#error compile "reflective.cpp" instead of this file
#endif

namespace reflective
{

	// TypeInspector::constructor
	TypeInspector::TypeInspector(
			TypeInspector::InspectionFlags inspection_flags )
		: _inspection_flags( inspection_flags ),
			_type( null ), _clasess_and_objects( null ), _classes_count( 0 ),
			_properties( null ), _property_count( 0 ), _property_values_buffer( null )
	{

	}

	// TypeInspector::_compute_counts
	void TypeInspector::_compute_counts( const Class & class_obj, TypeInspector::Counts & inout_counts )
	{
		const Class * curr_class = &class_obj;
		for(;;)
		{
			inout_counts.class_count++;

			if( _inspection_flags & eIncludeNonPublicMembers )
			{
				inout_counts.property_count += curr_class->properties().count();
				inout_counts.action_count += curr_class->actions().count();
				inout_counts.event_count += curr_class->events().count();
			}
			else
			{
				inout_counts.property_count += reflective::count_public_members( curr_class->properties() );
				inout_counts.action_count += reflective::count_public_members( curr_class->actions() );
				inout_counts.event_count += reflective::count_public_members( curr_class->events() );
			}

			#if REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE

				unsigned base_type_count = curr_class->base_types_count();
				if( base_type_count > 1 )
				{
					unsigned base_type_index = 1;
					do {
						const Type * base_type = curr_class->base_type( base_type_index );
						REFLECTIVE_ASSERT( base_type != null );

						REFLECTIVE_ASSERT( is_instance_of<Class>( *base_type ) ); /* non-class
							types cannot be base of class types. */

						const Class * base_class = static_cast<const Class*>( base_type );

						_compute_counts( *base_class, inout_counts );

					} while( ++base_type_index < base_type_count );
				}

			#endif

			const Type * base_type = curr_class->base_type( 0 );
			if( !base_type )
				break;

			REFLECTIVE_ASSERT( is_instance_of<Class>( *base_type ) ); /* class types cannot 
				have non-class bases. */

			curr_class = static_cast<const Class *>( base_type );
		}
	}

	// TypeInspector::_gather_classes_and_objects
	TypeInspector::ClassAndObject * TypeInspector::_gather_classes_and_objects( const Class & class_obj, const void * object,
		TypeInspector::ClassAndObject * class_and_objects )
	{
		const Class * curr_class = &class_obj;
		for(;;)
		{
			class_and_objects--;
			class_and_objects->class_obj = curr_class;
			class_and_objects->object_part = object;			

			#if REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE

				unsigned base_type_count = curr_class->base_types_count();
				if( base_type_count > 1 )
				{
					unsigned base_type_index = base_type_count - 1;
					do {
						const Type * base_type = curr_class->base_type( base_type_index );
						REFLECTIVE_ASSERT( base_type != null );

						REFLECTIVE_ASSERT( is_instance_of<Class>( *base_type ) ); /* non-class
							types cannot be base of class types. */

						const Class * base_class = static_cast<const Class*>( base_type );

						const void * next_object_part = object;
						if( next_object_part != null )
							next_object_part = curr_class->pointer_to_base_type( next_object_part, base_type_index );

						class_and_objects = _gather_classes_and_objects( 
							*base_class, next_object_part, class_and_objects );

					} while( --base_type_index > 0 );
				}

			#endif

			const Type * base_type = curr_class->base_type( 0 );
			if( !base_type )
				break;

			REFLECTIVE_ASSERT( is_instance_of<Class>( *base_type ) ); /* class types cannot 
				have non-class bases. */

			if( object != null )
				object = curr_class->pointer_to_base_type( object, 0 );

			curr_class = static_cast<const Class *>( base_type );
		}

		return class_and_objects;
	}

	// TypeInspector::inspect
	void TypeInspector::inspect( const Type & type, const void * object,
			size_t max_property_count )
	{
		if( _type )
			uninspect();

		_type = &type;

		if( !is_instance_of<Class>( type ) )
			return;

		const Class & class_obj = static_cast< const Class & >( type );
		
		// compute counts
		Counts counts;
		counts.class_count = 0;
		counts.property_count = 0;
		counts.action_count = 0;
		counts.event_count = 0;
		_compute_counts( class_obj, counts );	

		// allocate class list
		ClassAndObject * clasess_and_objects = static_cast< ClassAndObject *>(
			reflective_externals::mem_alloc( alignment_of( ClassAndObject ), sizeof( ClassAndObject ) * counts.class_count ) );
		_clasess_and_objects = clasess_and_objects;
		_classes_count = counts.class_count;
		//TypeInspector::ClassAndObject * classes_and_objects =
			_gather_classes_and_objects( class_obj, object, 
				clasess_and_objects + counts.class_count );
		//REFLECTIVE_ASSERT( classes_and_objects == clasess_and_objects );


		size_t property_buffer_size = 0;
		size_t property_buffer_alignment = 1;
		size_t actual_total_property_count = 0;

		// iterate classes
		size_t class_index = 0;
		if( counts.class_count > 0 ) do {

			const PropertyList & properties = clasess_and_objects[ class_index ].class_obj->properties();

			const size_t property_count = properties.count();

			size_t property_index = 0;
			if( property_count > 0 ) do {	

				if( actual_total_property_count >= max_property_count )
					break;

				const Property & curr_property = properties[ property_index ];

				if( (_inspection_flags & eIncludeNonPublicMembers) == 0 &&
					!curr_property.is_public() )
						continue;

				const size_t curr_size = curr_property.type().size();
				const size_t curr_alignment = curr_property.type().alignment();

				property_buffer_size = mem::upper_align( property_buffer_size, curr_alignment );

				property_buffer_size += curr_size;

				if( curr_alignment > property_buffer_alignment )
					property_buffer_alignment = curr_alignment;

				++actual_total_property_count;

			} while( ++property_index < property_count );

		} while( ++class_index < counts.class_count );

		// allocate _properties and _property_values_buffer
		void * properties_allocation = reflective_externals::mem_alloc( 
			alignment_of( PropertyAndValue ), sizeof( PropertyAndValue ) * actual_total_property_count );		
		_property_values_buffer = reflective_externals::mem_alloc( property_buffer_alignment, property_buffer_size );
		_properties = new( properties_allocation ) PropertyAndValue[ actual_total_property_count ];	
		_property_count = actual_total_property_count;

		// iterate classes
		actual_total_property_count = 0;
		void * prop_value = _property_values_buffer;
		class_index = 0;
		if( counts.class_count > 0 ) do {

			const PropertyList & properties = clasess_and_objects[ class_index ].class_obj->properties();

			const size_t property_count = properties.count();

			size_t property_index = 0;
			if( property_count > 0 ) do {				

				if( actual_total_property_count >= max_property_count )
					break;

				const Property & curr_property = properties[ property_index ];

				if( (_inspection_flags & eIncludeNonPublicMembers) == 0 &&
					!curr_property.is_public() )
						continue;

				const Type & type = curr_property.type();
				const size_t curr_size = type.size();
				const size_t curr_alignment = type.alignment();
				
				prop_value = mem::upper_align( prop_value, curr_alignment );			

				_properties[ actual_total_property_count ].property = &curr_property;

				if( clasess_and_objects[ class_index ].object_part != null )
					curr_property.get_value( clasess_and_objects[ class_index ].object_part, prop_value );
				
				_properties[ actual_total_property_count ].value = prop_value;

				prop_value = mem::address_add( prop_value, curr_size );

				++actual_total_property_count;

			} while( ++property_index < property_count );

		} while( ++class_index < counts.class_count );
	}

	// TypeInspector::uninspect
	void TypeInspector::uninspect()
	{
		if( !_type )
			return;

		// destroy properties
		size_t property_index = 0;
		if( _property_count > 0 ) do {

			_properties[property_index].property->type().destroy( 
				_properties[property_index].value );

		} while( ++property_index < _property_count );
		
		if( _property_values_buffer )
			reflective_externals::mem_free( _property_values_buffer );
		if( _properties )
			reflective_externals::mem_free( _properties );
		if( _clasess_and_objects )
			reflective_externals::mem_free( _clasess_and_objects );
	}


} // namespace reflective

namespace reflective_externals
{
	// reflection of reflective::TypeInspector
	reflective::Class * init_type(
		reflective::TypeInspector * null_pointer_1,
		reflective::TypeInspector * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::TypeInspector ThisClass;
	
		static Class * class_object = null;
		if( class_object != null )
			return class_object;
	
		// class object
		class_object = new_class<ThisClass>( "reflective", "TypeInspector" );
		class_object->set_no_base_type();
		class_object->set_life_functions( LifeFunctions::from_type<ThisClass>(
			eCopyAssignmentDestructor ) );
	
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "property_count", &ThisClass::get_property_count ),
			new_property<ThisClass>( "classes_count", &ThisClass::get_classes_count ),
		};
	
		// actions
		const Action * actions[] = 
		{
			new_action<ThisClass>( "inspect", &ThisClass::inspect, "type, object, max_property_count" ),
			new_action<ThisClass>( "uninspect", &ThisClass::uninspect ),
			new_action<ThisClass>( "get_property", &ThisClass::get_property, "index" ),
			new_action<ThisClass>( "get_classes", &ThisClass::get_classes, "index" ),
		};
	
		// assign members
		class_object->assign_properties( properties );
		class_object->assign_actions( actions );
	
		// return type
		return class_object;
	}

} // namespace reflective_externals
