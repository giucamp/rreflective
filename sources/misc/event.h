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

	/* AbstractEvent and Event<PARAM> - provide an easy-to-use ay to notify events to 
		objects that want to be notified. The event object allows attaching\detachhing of 
		AbstractHandler objects. When the event is raised  with the raise() method, 
		the invoke function is called on the attached handlers.
		Listeners can derive a class from AbstractHandler to implement the invoke, or 
		can use derived classes like Handler, whitch redirects the invoke to the listener 
		by calling a method on it (see Handler<TYPE> class). 
		Events can be used (and handlers attached\detached) also in a reflective way, 
		that is without compile-time knowlege of the owner type or params type,
		given that the Event object is exposed by the owning class with an 
		EventDescriptor within the reflective::Class (see EventDescriptor class). 
		This is the reason of the existince of the non-template AbstractEvent class,
		which handles event parameter of unknown type. */

	// fowrards
	class AbstractEvent;
		template <class PARAMS> class Event;
	class AbstractHandler;

	// AbstractEvent
	class AbstractEvent
	{
	public:

		// constructor \  destructor
		AbstractEvent();
		~AbstractEvent();

		// raise - raises the event, calling invoke( params ) on every attached handler
		void raise( void * params );
		void raise( const void * params );

		// handlers methods
		void add_handler( AbstractHandler & handler );
		void remove_handler( AbstractHandler & handler );

		// handlers operator - alias for add_handler \ remove_handler
		void operator += ( AbstractHandler & handler );
		void operator -= ( AbstractHandler & handler );

	private: // data members
		AbstractHandler * _first_handler;
	};

	// Event<PARAMS>
	template <class PARAMS>
		class Event : public AbstractEvent
	{
	public:

		// raise - raises the event, calling invoke( params ) on every attached handler
		void raise( PARAMS & params );
	};

} // namespace reflective
