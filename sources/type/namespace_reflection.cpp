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
	class Namespace::ReflectiveIterator : public reflective::IIterator
	{
	public:
		ReflectiveIterator( const reflective::Namespace & namespace_obj )
				: _types( namespace_obj ), _namespaces( namespace_obj )
		{
		}

		void first_group( ConstGroup & out_group )
		{
			if( _types )
			{
				const reflective::Type & type = *_types;
				out_group.type = &type_of( type );
				out_group.curr_in_group = &type;
				out_group.end_of_group = ( &type ) + 1;
			}
			else if( _namespaces )
			{
				const reflective::Namespace & namespace_obj = *_namespaces;
				out_group.type = &type_of( namespace_obj );
				out_group.curr_in_group = &namespace_obj;
				out_group.end_of_group = ( &namespace_obj ) + 1;
			}
			else
			{
				out_group.curr_in_group = nullptr;
				out_group.end_of_group = nullptr;
			}
		}

		void next_group( ConstGroup & out_group )
		{
			if( _types && ++_types )
			{			
				const reflective::Type & type = *_types;
				out_group.type = &type_of( type );
				out_group.curr_in_group = &type;
				out_group.end_of_group = ( &type ) + 1;
			}
			else if( _namespaces && ++_namespaces )
			{
				const reflective::Namespace & namespace_obj = *_namespaces;
				out_group.type = &type_of( namespace_obj );
				out_group.curr_in_group = &namespace_obj;
				out_group.end_of_group = ( &namespace_obj ) + 1;
			}
			else
			{
				out_group.curr_in_group = nullptr;
				out_group.end_of_group = nullptr;
			}
		}

		void first_group( Group & out_group )
		{
			out_group.curr_in_group = nullptr;
			out_group.end_of_group = nullptr;
			out_group.type = nullptr;
		}

		void next_group( Group & out_group )
		{
			out_group.curr_in_group = nullptr;
			out_group.end_of_group = nullptr;
			out_group.type = nullptr;
		}

		bool insert( 
			size_t group_offset_index, const Type & items_type, 
			const void * source_object, size_t item_count,
			const void * i_key_value, Group & out_curr_group )
		{
			REFLECTIVE_UNUSED_4( group_offset_index, items_type, source_object, item_count );
			REFLECTIVE_UNUSED_2( out_curr_group, i_key_value );
			return false; // unsupported
		}

		bool remove( size_t offset_index, size_t item_count, Group & out_curr_group )
		{
			REFLECTIVE_UNUSED_2( offset_index, item_count );
			REFLECTIVE_UNUSED( out_curr_group );
			return false; // unsupported
		}
			

	private: // data members
		reflective::Namespace::TypeIterator _types;
		reflective::Namespace::NamespaceIterator _namespaces;
	};

	// Namespace::ICollectionHandler
	class Namespace::ICollectionHandler : public reflective::ICollectionHandler
	{
	public:

		// query_item_count
		bool query_item_count( const void * collection_object, size_t * out_count ) const
		{
			const reflective::Namespace & namespace_obj = 
				*static_cast<const reflective::Namespace*>( collection_object );

			const size_t type_count = namespace_obj.type_count();
			const size_t namespace_count = namespace_obj.namespace_count();

			*out_count = type_count + namespace_count;

			return true;
		}

		// query_item_base_type
		const reflective::Type * query_item_base_type( const void * collection_object,
			bool * out_allow_derived_types ) const
		{
			REFLECTIVE_UNUSED( collection_object );

			*out_allow_derived_types = true;
			return &safe_get_type<reflective::Symbol>();
		}

		// create_iterator
		reflective::IIterator * create_iterator( void * i_collection_object, const void * i_key_value ) const
		{
			if( i_key_value != 0 )
				return nullptr;
				
			const reflective::Namespace & namespace_obj = 
				*static_cast<const reflective::Namespace*>( i_collection_object );

			ReflectiveIterator * result = REFLECTIVE_LIFO_NEW( ReflectiveIterator, namespace_obj );

			return result;
		}

		// resize
		bool resize( void * collection_object, size_t new_size ) const
		{
			REFLECTIVE_UNUSED_2( collection_object, new_size );
			return false;
		}

		// get_indices_info
		virtual bool get_key_type( const void * i_collection_object, uint32_t * o_flags, QualifiedType * o_index_type ) const
		{
			REFLECTIVE_UNUSED( i_collection_object );
			*o_flags = 0;
			*o_index_type = safe_get_qualified_type<SymbolName>();
			return true;
		}

		bool register_watch( Watch * watch, void * collection_object ) const
		{
			REFLECTIVE_UNUSED_2( watch, collection_object );
			return false;
		}

		void unregister_watch( Watch * watch, void * collection_object ) const
		{
			REFLECTIVE_UNUSED_2( watch, collection_object );
		}
	};

	// Namespace::_collection_handler
	Namespace::ICollectionHandler Namespace::_collection_handler;

	// Namespace::access_collection_handler
	Namespace::ICollectionHandler & Namespace::access_collection_handler()
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
