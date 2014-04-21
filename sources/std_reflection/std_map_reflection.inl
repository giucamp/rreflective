

namespace reflective_externals
{
	template < typename KEY, typename VALUE, typename PREDICATE, typename ALLOCATOR >
		class StdMapIterator : public reflective::IIterator
	{
	private:

		typedef typename std::map< KEY, VALUE, PREDICATE, ALLOCATOR > Map;
		typedef typename std::map< KEY, VALUE, PREDICATE, ALLOCATOR >::iterator Iterator;
		typedef typename std::map< KEY, VALUE, PREDICATE, ALLOCATOR >::iterator ConstIterator;

		Map & m_map;
		Iterator m_iterator;

		void get_group( Group & out_group )
		{
			if( m_iterator != m_map.end() )
			{
				out_group.curr_in_group = &m_iterator->second;
				out_group.qualified_type = reflective::qualified_type_of( m_iterator->second );
				out_group.end_of_group = memo::address_add( out_group.curr_in_group, out_group.qualified_type.type()->size() ); 
			}
			else
			{
				out_group.curr_in_group = nullptr;
				out_group.qualified_type = QualifiedType();
				out_group.end_of_group = nullptr; 
			}
		}

		void get_group( ConstGroup & out_group )
		{
			if( m_iterator != m_map.end() )
			{
				out_group.curr_in_group = &m_iterator->second;
				out_group.qualified_type = reflective::qualified_type_of( m_iterator->second );
				out_group.end_of_group = memo::address_add( out_group.curr_in_group, out_group.qualified_type.type()->size() );
			}
			else
			{
				out_group.curr_in_group = nullptr;
				out_group.qualified_type = QualifiedType();
				out_group.end_of_group = nullptr; 
			}
		}

	public:

		StdMapIterator( Map & i_map, const void * i_key_value )
			: m_map( i_map ) 
		{
			if( i_key_value == nullptr )
				m_iterator = m_map.begin();
			else
				m_iterator = m_map.find( *static_cast<const KEY*>(i_key_value) );
		}

		void first_group( Group & out_group )
		{
			get_group( out_group );
		}

		void next_group( Group & out_group )
		{
			m_iterator++;
			get_group( out_group );
		}

		void first_group( ConstGroup & out_group )
		{
			get_group( out_group );
		}
		void next_group( ConstGroup & out_group )
		{
			m_iterator++;
			get_group( out_group );
		}

		bool insert( 
			size_t group_offset_index, const reflective::Type & items_type, 
			const void * source_object, size_t item_count,
			const void * i_key_value, Group & out_curr_group )
		{
			REFLECTIVE_ASSERT( group_offset_index == 0 );
			REFLECTIVE_UNUSED( group_offset_index );

			REFLECTIVE_ASSERT( i_key_value == nullptr );
			REFLECTIVE_UNUSED( i_key_value );
			bool result = false;

			if( items_type == reflective::get_type<VALUE>() && item_count == 1 && group_offset_index == 0 && i_key_value != nullptr )
			{
				std::pair< Iterator, bool > insert_result;
				if( source_object != nullptr )
				{
					insert_result = m_map.insert( std::make_pair( *static_cast<const KEY*>(i_key_value), *static_cast<const VALUE*>(source_object) ) );
				}
				else
				{
					insert_result = m_map.insert( std::make_pair( *static_cast<const KEY*>(i_key_value), VALUE() ) );
				}
				if( insert_result.second )
				{
					result = true;
					m_iterator = insert_result.first;
				}
			}

			get_group( out_curr_group );

			return result;
		}

		bool remove( size_t offset_index, size_t item_count, Group & out_curr_group )
		{
			REFLECTIVE_ASSERT( offset_index == 0 );
			REFLECTIVE_UNUSED( offset_index );

			bool result = false;

			if( m_iterator != m_map.end() )
			{
				result = true;
				REFLECTIVE_ASSERT( false ); // to do
				/*for( size_t item_index = 0; item_index < item_count; item_index++ )
				{
					m_map.delete_at( m_current_index );
				}*/
			}


			get_group( out_curr_group );

			return result;
		}

		 const void * get_key_value( size_t group_offset_index )
		 {
			 REFLECTIVE_ASSERT( group_offset_index == 0 );
			 REFLECTIVE_UNUSED( group_offset_index );

			 if( m_iterator != m_map.end() )
			 {
				 return &(m_iterator->first);
			 }
			 else
			 {
				 return nullptr;
			 }
		 }
	};

	template < typename KEY, typename VALUE, typename PREDICATE, typename ALLOCATOR >
		class StdMapCollectionHandler : public ICollectionHandler
	{
	public:

		typedef std::map< KEY, VALUE, PREDICATE, ALLOCATOR > Map;
		typedef typename std::map< KEY, VALUE, PREDICATE, ALLOCATOR >::iterator MapIterator;

		// query_item_count
		bool query_item_count( const void * collection_object, size_t * out_count ) const
		{
			*out_count = static_cast<const Map*>( collection_object )->size();
			return true;
		}

		// resize
		bool resize( void * collection_object, size_t new_size ) const
		{
			REFLECTIVE_UNUSED_2( collection_object, new_size );
			return false;
		}

		// query_item_base_type
		const reflective::Type * query_item_base_type( const void * collection_object,
			bool * out_allow_derived_types ) const
		{
			REFLECTIVE_UNUSED( collection_object );
			*out_allow_derived_types = false;
			return &safe_get_type<VALUE>();
		}

		// iteration
		IIterator * create_iterator( void * i_collection_object, const void * i_key_value ) const
		{
			typedef StdMapIterator<KEY, VALUE, PREDICATE, ALLOCATOR> Iterator;
			return REFLECTIVE_LIFO_NEW( Iterator, *static_cast<Map*>( i_collection_object ), i_key_value );
		}

		void * add_object( const QualifiedType & i_qualified_type, void * i_collection_object, const void * i_key_value ) const
		{
			if( i_qualified_type != get_qualified_type<VALUE>() || i_key_value == nullptr )
				return nullptr;
			Map & map = *static_cast<Map*>( i_collection_object );
			const KEY & key_value = *static_cast<const KEY*>( i_key_value );
			std::pair< MapIterator, bool > result = map.insert( std::make_pair( key_value, VALUE() ) );
			if( result.second )
			{
				return &(result.first->second);
			}
			else
			{
				return nullptr;
			}
		}

		// get_indices_info
		virtual bool get_key_type( const void * i_collection_object, uint32_t * o_flags, QualifiedType * o_index_type ) const
		{
			REFLECTIVE_UNUSED( i_collection_object );
			*o_flags = 0;
			*o_index_type = safe_get_qualified_type<KEY>();
			return true;
		}

		
				/* watching */

		bool register_watch( reflective::Watch * watch, void * collection_object ) const 
		{
			REFLECTIVE_UNUSED_2( watch, collection_object );
			return false;
		}

		void unregister_watch( reflective::Watch * watch, void * collection_object ) const
		{
			REFLECTIVE_UNUSED_2( watch, collection_object );
		}
	};

	// reflection of std::map
	template< typename KEY, typename VALUE, typename PREDICATE, typename ALLOCATOR >
		void init_type( reflective::Type * volatile * o_result,
			std::map< KEY, VALUE, PREDICATE, ALLOCATOR > * null_pointer_1,
			std::map< KEY, VALUE, PREDICATE, ALLOCATOR > * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef std::map< KEY, VALUE, PREDICATE, ALLOCATOR > ThisClass;

		if( *o_result != nullptr )
			return;

		// template parameters
		const TemplateParameter template_parameters[] = 
		{
			TemplateParameter( "KEY", safe_get_qualified_type<KEY>() ),
			TemplateParameter( "VALUE", safe_get_qualified_type<VALUE>() ),
			TemplateParameter( "PREDICATE", safe_get_qualified_type<PREDICATE>() ),
			TemplateParameter( "ALLOCATOR", safe_get_qualified_type<ALLOCATOR>() ),
		};

		// class object
		Class * class_object = new_class<ThisClass>( "std", "map", template_parameters );
		*o_result = class_object;
		class_object->set_no_base_type();
		class_object->set_life_functions( LifeFunctions::from_type<ThisClass>( eConstructorCopyAssignmentDestructor ) );

		// properties
		/*const Property * properties[] = 
		{
			new_property<ThisClass>( "size", &ThisClass::size ), // in ms std, size if a member of _Tree, the base class of map.
		};*/

		// actions
		const Action * actions[] = 
		{
			new_action<ThisClass>( "clear", &ThisClass::clear ),
		};

		// assign members
		//class_object->assign_properties( properties );
		class_object->edit_actions().assign( actions );

		// assign collection handler
		static StdMapCollectionHandler< KEY, VALUE, PREDICATE, ALLOCATOR > s_collection_handler;
		class_object->set_collection_handler( &s_collection_handler );
	}
}


