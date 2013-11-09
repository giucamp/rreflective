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
	/* EventDescriptor - provides runtime info about an event, an allow dynamic 
		attaching\detaching of handlers. */
	class EventDescriptor : public ClassMember
	{
	public:

		// constructor
		EventDescriptor( SymbolName name, ClassMember::Attributes attributes,
			const Type & argument_type );

		// argument_type
		const Type & argument_type() const;

		// handlers
		virtual void add_handler( void * object, AbstractHandler * handler ) = 0;
		virtual void remove_handler( void * object, AbstractHandler * handler ) = 0;

	private:
		EventDescriptor & operator = ( const EventDescriptor & ); // not supported

	private: // data members		
		const Type & _argument_type;
	};

	// TypedEventDescriptor
	template <class BASE_OBJECT, class EVENT_PARAMS>
		class TypedEventDescriptor : public EventDescriptor
	{
	public:

		// constructor
		TypedEventDescriptor( SymbolName name, ClassMember::Attributes attributes,
			Event<EVENT_PARAMS> (BASE_OBJECT::*event_member) );

		// handlers
		void add_handler( void * object, AbstractHandler * handler );
		void remove_handler( void * object, AbstractHandler * handler );

	private: // data members
		Event<EVENT_PARAMS> (BASE_OBJECT::*_event_member);
	};

	// EventDescriptorList
	typedef SymbolList<EventDescriptor, false, ClassMember::Comparer > EventDescriptorList;

	
} // namespace reflective

namespace reflective_externals
{
	// reflection of reflective::EventDescriptor
	reflective::Class * init_type(
		reflective::EventDescriptor * null_pointer_1,
		reflective::EventDescriptor * null_pointer_2 );
}

