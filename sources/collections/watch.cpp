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

	// register_watch
	bool register_watch( Watch * watch, const Type & collection_type, void * collection )
	{
		#if REFLECTIVE_ENABLE_ASSERT
			void * prev_collection = watch->collection();
			REFLECTIVE_ASSERT( prev_collection == nullptr ); // call unregister_watch first
		#endif

		const CollectionHandler * collection_handler = collection_type.collection_handler();
		if( !collection_handler )
			return false;

		const bool result = collection_handler->register_watch( watch, collection );
		if( result )
		{
			REFLECTIVE_ASSERT( watch->collection() == collection );
			REFLECTIVE_ASSERT( collection_type.can_cast_to( *watch->collection_type() ) );
		}
		else
		{
			REFLECTIVE_ASSERT( watch->collection() == nullptr );
			REFLECTIVE_ASSERT( watch->collection_type() == nullptr );
		}
		return result;
	}

	// unregister_watch
	void unregister_watch( Watch * watch )
	{
		const Type * collection_type = watch->collection_type();
		void * collection = watch->collection();
		if( collection == nullptr )
		{
			REFLECTIVE_ASSERT( collection_type == nullptr );
			return;
		}
		else
		{
			REFLECTIVE_ASSERT( collection_type != nullptr );
		}

		const CollectionHandler * collection_handler = collection_type->collection_handler();
		if( !collection_handler )
			return;

		collection_handler->unregister_watch( watch, collection );
	}

} // namespace reflective


namespace reflective_externals
{
	// reflection of reflective::Watch
	void init_type( reflective::Type * volatile * o_result, reflective::Watch * null_pointer_1, reflective::Watch * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::Watch ThisClass;
	
		if( *o_result != nullptr )
			return;
	
		// class object
		Class * class_object = new_class<ThisClass>( "reflective", "Watch" );
		*o_result = class_object;
		class_object->set_no_base_type();
		class_object->set_life_functions( LifeFunctions::from_type<ThisClass>(
			eNoLifeFunctions ) );
	
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "collection_type", &ThisClass::collection_type ),
			new_property<ThisClass>( "collection", &ThisClass::collection ),
		};
	
		// actions
		const Action * actions[] = 
		{
			new_action<ThisClass>( "notify_added_objects", &ThisClass::notify_added_objects, "notify_params" ),
			new_action<ThisClass>( "notify_changed_objects", &ThisClass::notify_changed_objects, "notify_params" ),
			new_action<ThisClass>( "notify_removing_objects", &ThisClass::notify_removing_objects, "notify_params" ),
		};
	
		// assign members
		class_object->assign_properties( properties );
		class_object->assign_actions( actions );
	}
}