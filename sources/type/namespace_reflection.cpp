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
	// Namespace::ReflectiveIterator
	class Namespace::ReflectiveIterator : public reflective::AbstractIterator
	{
	private:
		
	public:

		ReflectiveIterator( const reflective::Namespace & namespace_obj )
				: m_type_iterator( namespace_obj ), m_namespace_iterator( namespace_obj )
		{
			if( m_type_iterator )
			{
				const reflective::Type & type = *m_type_iterator;
				set_item( ObjectPointerWrapper::from_pointer(&type), &type.name() );
			}
			else if( m_namespace_iterator )
			{
				const reflective::Namespace & namespace_obj = *m_namespace_iterator;
				set_item( ObjectPointerWrapper::from_pointer(&namespace_obj), &namespace_obj.name() );
			}
			else
			{
				set_item_null();
			}
		}

		void move_next()
		{
			if( m_type_iterator && ++m_type_iterator )
			{			
				const reflective::Type & type = *m_type_iterator;
				set_item( ObjectPointerWrapper::from_pointer(&type), &type.name() );
			}
			else if( m_namespace_iterator && ++m_namespace_iterator )
			{
				const reflective::Namespace & namespace_obj = *m_namespace_iterator;
				set_item( ObjectPointerWrapper::from_pointer(&namespace_obj), &namespace_obj.name() );
			}
			else
			{
				set_item_null();
			}
		}

	private: // data members
		reflective::Namespace::TypeIterator m_type_iterator;
		reflective::Namespace::NamespaceIterator m_namespace_iterator;
	};

	// Namespace::CollectionHandler
	class Namespace::CollectionHandler : public reflective::CollectionHandler
	{
	public:

		CollectionHandler() : reflective::CollectionHandler( sizeof(ReflectiveIterator), alignment_of(ReflectiveIterator) ) {}

		Flags get_flags( const void * /*i_colection_object*/ ) const
			{ return eSupportPositionalIndex | eSupportGetCount | eAllowDerivedItems; }

		QualifiedType get_key_type( const void * /*i_collection_object*/ ) const
			{ return safe_get_qualified_type<SymbolName>(); }

		QualifiedType get_item_type( const void * /*i_collection_object*/ ) const
			{ return safe_get_qualified_type<Symbol>(); }


		size_t get_count( const void * i_collection_object ) const
		{
			const reflective::Namespace & namespace_obj = 
				*static_cast<const reflective::Namespace*>( i_collection_object );

			const size_t type_count = namespace_obj.type_count();
			const size_t namespace_count = namespace_obj.namespace_count();

			return type_count + namespace_count;
		}

		ObjectPointerWrapper get_at_key( const void * i_collection_object, const void * i_key ) const
		{
			const SymbolName & key = *static_cast<const SymbolName*>( i_key );
			const reflective::Namespace & namespace_obj = 
				*static_cast<const reflective::Namespace*>( i_collection_object );
			
			const Type * child_type = namespace_obj.find_child_type(key);
			if( child_type != nullptr )
			{
				return ObjectPointerWrapper::from_pointer(child_type);
			}

			const Namespace * child_namespace = namespace_obj.find_child_namespace(key);
			if( child_namespace != nullptr )
			{
				return ObjectPointerWrapper::from_pointer(child_namespace);
			}

			return ObjectPointerWrapper();
		}

		AbstractIterator * construct_iterator( void * i_collection_object, void * i_iterator_buffer ) const
		{
			const reflective::Namespace & namespace_obj = 
				*static_cast<const reflective::Namespace*>( i_collection_object );
			return new( i_iterator_buffer ) ReflectiveIterator( namespace_obj );
		}
	};

	// Namespace::_collection_handler
	Namespace::CollectionHandler Namespace::_collection_handler;

	// Namespace::access_collection_handler
	Namespace::CollectionHandler & Namespace::access_collection_handler()
	{
		return _collection_handler;
	}
}

namespace reflective_externals
{
	// reflection of reflective::Namespace
	void init_type( reflective::Type * volatile * o_result,
		reflective::Namespace * null_pointer_1,
		reflective::Namespace * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::Namespace ThisClass;
		typedef reflective::Symbol BaseClass;
		
		if( *o_result != nullptr )
			return;
		
		// class object
		Class * class_object = new_class<ThisClass>( "reflective", "Namespace" );
		*o_result = class_object;
		class_object->set_base_type( BaseType::from_types<ThisClass,BaseClass>() );
		
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "type_count", &ThisClass::type_count ),
			new_property<ThisClass>( "namespace_count", &ThisClass::namespace_count ),
		};
		
		// actions
		const Action * actions[] = 
		{
			//new_action<ThisClass>( "find_or_add_child_namespace", &ThisClass::find_or_add_child_namespace, "path" ),
			new_action<ThisClass>( "add_child_type", &ThisClass::add_child_type, "type"),
			new_action<ThisClass>( "remove_child_type", &ThisClass::remove_child_type, "type"),
			new_action<ThisClass>( "add_child_namespace", &ThisClass::add_child_namespace, "namespace_obj"),
		};
		
		// assign members
		class_object->assign_properties( properties );
		class_object->edit_actions().assign( actions );

		class_object->set_collection_handler( &ThisClass::access_collection_handler() );
	}

} // namespace reflective_externals
