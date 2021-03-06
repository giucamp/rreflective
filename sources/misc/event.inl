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
	// AbstractEvent::constructor
	inline AbstractEvent::AbstractEvent()
			: _first_handler( NULL )
	{
	}
	
	// AbstractEvent::add_handler
	inline void AbstractEvent::add_handler( AbstractHandler & handler )
	{
		REFLECTIVE_ASSERT( handler._event == nullptr ); // this handler is already registered to an event

		handler._event = this;
		handler._next_handler = _first_handler;
		_first_handler = &handler;
	}


				/**** Event<PARAM> ****/

	// Event<PARAM>::raise
	template <class PARAM>
		inline void Event<PARAM>::raise( PARAM params )
	{
		AbstractEvent::raise( &params );
	}

	// Event<PARAM>::raise
	template <class PARAM>
		inline void Event<PARAM&>::raise( PARAM & params )
	{
		PARAM * pointer = &params;
		AbstractEvent::raise( &pointer );
	}
}
