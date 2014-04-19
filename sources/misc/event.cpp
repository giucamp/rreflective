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

	// AbstractEvent::destructor
	AbstractEvent::~AbstractEvent()
	{
		AbstractHandler * handler = _first_handler;
		AbstractHandler * next_handler;
		if( handler ) do {

			handler->_event = nullptr;
			next_handler = handler->_next_handler;
			handler->_next_handler = nullptr;

			handler = next_handler;

		} while( handler != nullptr );
	}

	// AbstractEvent::raise
	void AbstractEvent::raise( void * params )
	{
		AbstractHandler * handler = _first_handler;
		if( handler ) do {

			handler->invoke( params );

			handler = handler->_next_handler;
		} while( handler != nullptr );
	}

	// AbstractEvent::raise (const params)
	void AbstractEvent::raise( const void * params )
	{
		AbstractHandler * handler = _first_handler;
		if( handler ) do {

			handler->invoke( params );

			handler = handler->_next_handler;
		} while( handler != nullptr );
	}

	// AbstractEvent::remove_handler
	void AbstractEvent::remove_handler( AbstractHandler & handler )
	{		
		AbstractHandler * curr_handler = _first_handler;
		AbstractHandler * prev_handler = nullptr;
		if( curr_handler ) do {
			
			if( curr_handler == &handler )
			{
				if( prev_handler )
				{
					REFLECTIVE_ASSERT( _first_handler != curr_handler );
					prev_handler->_next_handler = handler._next_handler;
					handler._event = nullptr;
					break;
				}
				else
				{
					REFLECTIVE_ASSERT( _first_handler == curr_handler );
					_first_handler = handler._next_handler;
					handler._event = nullptr;
					break;
				}
			}

			prev_handler = curr_handler;
			curr_handler = curr_handler->_next_handler;

		} while( curr_handler != nullptr );

		REFLECTIVE_ASSERT( curr_handler != nullptr ); // handler not found?
	}

} // namespace reflective
