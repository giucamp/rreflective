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
	// AbstractHandler
	class AbstractHandler
	{
	public:
		AbstractHandler();
		virtual void invoke( void * param ) = 0;
		virtual void invoke( const void * param ) = 0;
		virtual ~AbstractHandler();

	private: // data members
		AbstractHandler * _next_handler;
		AbstractEvent * _event;
		friend class AbstractEvent;
	};

	// Handler
	template <class ROOT_CLASS, class PARAM>
		class Handler : public AbstractHandler
	{
	public:

		typedef void (ROOT_CLASS::*Action)( PARAM & param );

		Handler();

		template <class OBJECT>
			Handler & operator () ( OBJECT & object, void (OBJECT::*handler_method)( PARAM & ) );

		void invoke( void * param );
		void invoke( const void * param );

	private: // data members
		ROOT_CLASS * _object;
		Action _handler_method;
	};

	// Handler (const PARAM)
	template <class ROOT_CLASS, class PARAM>
		class Handler<ROOT_CLASS, const PARAM> : public AbstractHandler
	{
	public:

		typedef void (ROOT_CLASS::*Action)( const PARAM & param );

		Handler();

		template <class OBJECT>
			Handler & operator () ( OBJECT & object, void (OBJECT::*handler_method)( const PARAM & ) );

		void invoke( void * param );
		void invoke( const void * param );

	private: // data members
		ROOT_CLASS * _object;
		Action _handler_method;
	};
}
