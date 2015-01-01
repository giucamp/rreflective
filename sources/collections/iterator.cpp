
namespace reflective
{
	Iterator::Iterator( ObjectPointerWrapper i_collection )
		: m_iterator( nullptr )
	{
		if( i_collection.object() != nullptr )
		{
			const Type * const type = i_collection.type().final_type();
			void * const object = const_cast<void *>( i_collection.type().qualification().full_indirection( i_collection.object() ) );
			if( object != nullptr )
			{
				if( type->check_capabilities( Type::eHasCollectionHandler ) )
				{
					m_iterator = type->collection_handler()->create_iterator_lifo( object );
				}
			}
		}
	}

	Iterator::~Iterator() 
	{
		if( m_iterator != nullptr )
		{
			REFLECTIVE_LIFO_DELETE(m_iterator);
		}
	}
}