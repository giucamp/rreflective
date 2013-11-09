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

#ifndef _INCLUDING_REFLECTIVE
	#error include "reflective.h" instead of including this file
#endif

namespace reflective
{
	// EventDescriptor::constructor
	inline EventDescriptor::EventDescriptor( SymbolName name, 
		ClassMember::Attributes attributes, const Type & argument_type )
			: ClassMember( name, attributes ), 
			  _argument_type( argument_type )
	{
		set_type_id( eTypeAction );
	}

	// EventDescriptor::argument_type
	inline const Type & EventDescriptor::argument_type() const
	{
		return _argument_type;
	}

	// TypedEventDescriptor::constructor
	template <class BASE_OBJECT, class EVENT_PARAMS>
		inline TypedEventDescriptor<BASE_OBJECT, EVENT_PARAMS>::TypedEventDescriptor( 
			SymbolName name, ClassMember::Attributes attributes,
			Event<EVENT_PARAMS> (BASE_OBJECT::*event_member) )
				: EventDescriptor( name, attributes, event_member ), 
				  _event_member( event_member )
	{

	}

	// TypedEventDescriptor::add_handler
	template <class BASE_OBJECT, class EVENT_PARAMS>
		inline void TypedEventDescriptor<BASE_OBJECT, EVENT_PARAMS>::add_handler( 
			void * object, AbstractHandler * handler )
	{
		BASE_OBJECT & typed_object = *static_cast<BASE_OBJECT*>( object );
		(typed_object.*_event_member).add_handler( handler );
	}

	// TypedEventDescriptor::remove_handler
	template <class BASE_OBJECT, class EVENT_PARAMS>
		inline void TypedEventDescriptor<BASE_OBJECT, EVENT_PARAMS>::remove_handler(
			void * object, AbstractHandler * handler )
	{
		BASE_OBJECT & typed_object = *static_cast<BASE_OBJECT*>( object );
		(typed_object.*_event_member).remove_handler( handler );
	}

} // namespace reflective
