
namespace reflective_externals
{
	template< typename TYPE, typename ALLOCATOR >
		class StdVectorIterator : public reflective::AbstractIterator
	{
	private: // data members
		typedef typename std::vector< TYPE, ALLOCATOR > Vector;
		typedef typename std::vector< TYPE, ALLOCATOR >::iterator Iterator;
		typedef typename std::vector< TYPE, ALLOCATOR >::iterator ConstIterator;
		Vector & m_vector;
		Iterator m_iterator;

	private: // unsupported
		StdVectorIterator( const StdVectorIterator & );
		StdVectorIterator & operator = ( const StdVectorIterator & );

	public:

		StdVectorIterator( Vector & i_vector )
			: m_vector( i_vector ) 
		{
			m_iterator = m_vector.begin();
			if( m_iterator != m_vector.end() )
				set_item( &*m_iterator, reflective::get_type<TYPE>(), nullptr );
			else
				set_item_null();
		}

		void move_next()
		{
			m_iterator++;
			if( m_iterator != m_vector.end() )
				set_item( &*m_iterator, reflective::get_type<TYPE>(), nullptr );
			else
				set_item_null();
		}
	};

	template< typename TYPE, typename ALLOCATOR >
		class StdVectorCollectionHandler : public CollectionHandler
	{
	public:

		typedef std::vector< TYPE, ALLOCATOR > Vector;
		typedef typename std::vector< TYPE, ALLOCATOR >::iterator VectorIterator;
		typedef typename std::vector< TYPE, ALLOCATOR >::const_iterator VectorConstIterator;

		StdVectorCollectionHandler()
			: CollectionHandler( sizeof(StdVectorIterator< TYPE, ALLOCATOR >), alignment_of(StdVectorIterator< TYPE, ALLOCATOR >) ) {}

		Flags get_flags( const void * /*i_colection_object*/ ) const
			{ return eSupportPositionalIndex | eSupportGetCount | eAllowEdit; }
		
		QualifiedType get_item_type( const void * /*i_collection_object*/ ) const
			{ return safe_get_qualified_type<TYPE>(); }
				
		size_t get_count( const void * i_collection_object ) const
			{ return static_cast<const Vector*>( i_collection_object )->size(); }

		ObjectPointerWrapper get_at_index( const void * i_collection_object, size_t i_index ) const
		{
			const Vector & vector = *static_cast<const Vector*>( i_collection_object );
			return ObjectPointerWrapper::from_pointer( &vector[i_index] );
		}

		void * insert_item_at_index( void * i_collection_object, size_t i_index, const ObjectPointerWrapper & i_source_object ) const
		{
			if( i_source_object.type() == safe_get_qualified_type<TYPE>() )
			{
				Vector & vector = *static_cast<Vector*>( i_collection_object );
				
				if( i_source_object.object() == nullptr )
				{
					vector.insert( vector.begin() + i_index, TYPE() );
				}
				else
				{
					vector.insert( vector.begin() + i_index, *static_cast<const TYPE*>(i_source_object.object()) );
				}
				return &vector[i_index];
			}
			else
			{
				return nullptr;
			}
		}
		
		void reserve( void * i_collection_object, size_t i_total_item_count ) const
		{ 
			Vector & vector = *static_cast<Vector*>( i_collection_object );
			vector.reserve(i_total_item_count);
		}

		bool delete_item_at_index( void * i_collection_object, size_t i_index ) const
		{
			Vector & vector = *static_cast<Vector*>( i_collection_object );
			vector.erase( vector.begin() + i_index );
			return true;
		}

		AbstractIterator * construct_iterator( void * i_collection_object, void * i_iterator_buffer ) const
		{
			Vector & vector = *static_cast<Vector*>( i_collection_object );
			return new( i_iterator_buffer ) StdVectorIterator< TYPE, ALLOCATOR >( vector );
		}
	};

	// reflection of std::vector
	template< typename TYPE, typename ALLOCATOR >
		void init_type( reflective::Type * volatile * o_result,
			std::vector< TYPE, ALLOCATOR > * null_pointer_1,
			std::vector< TYPE, ALLOCATOR > * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef std::vector< TYPE, ALLOCATOR > ThisClass;

		if( *o_result != nullptr )
			return;

		// template parameters
		const TemplateParameter template_parameters[] = 
		{
			TemplateParameter( "TYPE", safe_get_qualified_type<TYPE>() ),
			TemplateParameter( "ALLOCATOR", safe_get_qualified_type<ALLOCATOR>() ),
		};

		// class object
		Class * class_object = new_class<ThisClass>( "std", "vector", template_parameters );
		*o_result = class_object;
		class_object->set_no_base_type();
		class_object->set_life_functions( LifeFunctions::from_type<ThisClass>( eConstructorCopyAssignmentDestructor ) );

		// properties
		const Property * properties[] = 
		{
			new_property( "size", &ThisClass::size ),
		};

		// actions
		const Action * actions[] = 
		{
			new_action<ThisClass>( "clear", &ThisClass::clear ),
		};

		// assign members
		class_object->assign_properties( properties );
		class_object->edit_actions().assign( actions );

		// assign collection handler
		static StdVectorCollectionHandler< TYPE, ALLOCATOR > s_collection_handler;
		class_object->set_collection_handler( &s_collection_handler );
	}
}


