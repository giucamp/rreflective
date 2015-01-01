

namespace reflective_externals
{
	template < typename KEY, typename VALUE, typename PREDICATE, typename ALLOCATOR >
		class StdMapIterator : public reflective::AbstractIterator
	{
	private: // data members
		typedef typename std::map< KEY, VALUE, PREDICATE, ALLOCATOR > Map;
		typedef typename std::map< KEY, VALUE, PREDICATE, ALLOCATOR >::iterator Iterator;
		typedef typename std::map< KEY, VALUE, PREDICATE, ALLOCATOR >::iterator ConstIterator;
		Map & m_map;
		Iterator m_iterator;

	private: // unsupported
		StdMapIterator( const StdMapIterator & );
		StdMapIterator & operator = ( const StdMapIterator & );

	public:

		StdMapIterator( Map & i_map )
			: m_map( i_map ) 
		{
			m_iterator = m_map.begin();
			if( m_iterator != m_map.end() )
				set_item( &m_iterator->second, reflective::get_type<VALUE>(), &m_iterator->first );
			else
				set_item_null();
		}

		void move_next()
		{
			m_iterator++;
			if( m_iterator != m_map.end() )
				set_item( &m_iterator->second, reflective::get_type<VALUE>(), &m_iterator->first );
			else
				set_item_null();
		}
	};

	template < typename KEY, typename VALUE, typename PREDICATE, typename ALLOCATOR >
		class StdMapCollectionHandler : public CollectionHandler
	{
	public:

		typedef std::map< KEY, VALUE, PREDICATE, ALLOCATOR > Map;
		typedef typename std::map< KEY, VALUE, PREDICATE, ALLOCATOR >::iterator MapIterator;
		typedef typename std::map< KEY, VALUE, PREDICATE, ALLOCATOR >::const_iterator MapConstIterator;

		StdMapCollectionHandler()
			: CollectionHandler( sizeof(StdMapIterator< KEY, VALUE, PREDICATE, ALLOCATOR >), alignment_of(StdMapIterator< KEY, VALUE, PREDICATE, ALLOCATOR >) ) {}

		Flags get_flags( const void * /*i_colection_object*/ ) const
			{ return eSupportKey | eSupportGetCount | eAllowEdit; }
		
		QualifiedType get_item_type( const void * /*i_collection_object*/ ) const
			{ return safe_get_qualified_type<VALUE>(); }

		QualifiedType get_key_type( const void * /*i_collection_object*/ ) const
			{ return safe_get_qualified_type<KEY>(); }

		size_t get_count( const void * i_collection_object ) const
			{ return static_cast<const Map*>( i_collection_object )->size(); }

		ObjectPointerWrapper get_at_key( const void * i_collection_object, const void * i_key ) const
		{
			const Map & map = *static_cast<const Map*>( i_collection_object );
			MapConstIterator it = map.find( *static_cast<const KEY*>(i_key) ); 
			if( it != map.end() )
				return ObjectPointerWrapper::from_pointer( const_cast<VALUE*>( &it->second ) );
			else
				return ObjectPointerWrapper();
		}

		void * insert_item_at_key( void * i_collection_object, const void * i_key, const ObjectPointerWrapper & i_source_object ) const
		{
			Map & map = *static_cast<Map*>( i_collection_object );

			if( i_source_object.type() == safe_get_qualified_type<VALUE>() )
			{
				const KEY & key = *static_cast<const KEY*>(i_key);
				
				if( i_source_object.object() == nullptr )
				{
					VALUE & val = map[ key ];	
					return &val;
				}
				else
				{
					std::pair<MapIterator,bool> res = map.insert( std::make_pair(key, *static_cast<VALUE*>( i_source_object.object() ) ) );
					VALUE & val = res.first->second;	
					return &val;
				}
			}
			else
			{
				return nullptr;
			}
		}
		
		bool resize( void * /*i_collection_object*/, size_t /*new_size*/ ) const
			{ return false; }

		bool delete_item_at_key( void * i_collection_object, const void * i_key_value ) const
		{
			Map & map = *static_cast<Map*>( i_collection_object );
			MapIterator it = map.find( *static_cast<const KEY*>(i_key_value) );
			if( it != map.end() )
			{
				map.erase( it );
				return true;
			}
			else
			{
				return false;
			}
		}

		AbstractIterator * construct_iterator( void * i_collection_object, void * i_iterator_buffer ) const
		{
			Map & map = *static_cast<Map*>( i_collection_object );
			return new( i_iterator_buffer ) StdMapIterator< KEY, VALUE, PREDICATE, ALLOCATOR >( map );
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


