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
					/**** AbstractHandler ****/

	// AbstractHandler::constructor
	inline AbstractHandler::AbstractHandler()
			: _next_handler( nullptr ), _event( nullptr )
	{
	}

	// AbstractHandler::destructor
	inline AbstractHandler::~AbstractHandler()
	{
		if( _event )
			(*_event) -= *this;
	}

					/**** Handler ****/

	// Handler<ROOT_CLASS, PARAM>::constructor
	template <class ROOT_CLASS, class PARAM>
		inline Handler<ROOT_CLASS, PARAM>::Handler()
			: _object( NULL ), _handler_method( NULL )
	{

	}

	// Handler::constructor
	template <class ROOT_CLASS, class PARAM>
		template <class OBJECT>
			inline void Handler<ROOT_CLASS, PARAM>::init( OBJECT & object, 
				void (OBJECT::*handler_method)( PARAM & ) )
	{
		ROOT_CLASS * just_a_type_check = static_cast<OBJECT*>( nullptr ); // ROOT_CLASS must be an accessible base of OBJECT
		REFLECTIVE_UNUSED( just_a_type_check );

		typedef void (ROOT_CLASS::*RootHandlerMethod)( PARAM & );

		_object = &object;
		_handler_method = static_cast<RootHandlerMethod>( handler_method );
	}

	// Handler::invoke
	template <class ROOT_CLASS, class PARAM>
		inline void Handler<ROOT_CLASS, PARAM>::invoke( void * param )
	{
		PARAM & typed_param = *static_cast< PARAM * >( param );
		(_object->*_handler_method)( typed_param );
	}

	// Handler::invoke
	template <class ROOT_CLASS, class PARAM>
		inline void Handler<ROOT_CLASS, PARAM>::invoke( const void * param )
	{
		REFLECTIVE_UNUSED( param );
		REFLECTIVE_ASSERT( false ); // const invoke not supported on Handler with a non-const PARAM
	}


				/**** Handler (const PARAM) ****/

	// Handler<ROOT_CLASS, PARAM>::constructor
	template <class ROOT_CLASS, class PARAM>
		inline Handler<ROOT_CLASS, const PARAM>::Handler()
			: _object( NULL ), _handler_method( NULL )
	{

	}

	// Handler::constructor
	template <class ROOT_CLASS, class PARAM>
		template <class OBJECT>
			inline void Handler< ROOT_CLASS, const PARAM >::init( OBJECT & object, 
					void (OBJECT::*handler_method)( const PARAM & ) )
	{
		ROOT_CLASS * just_a_type_check = static_cast<OBJECT*>( nullptr ); // ROOT_CLASS must be an accessible base of OBJECT

		typedef void (ROOT_CLASS::*RootHandlerMethod)( const PARAM & );

		_object = &object;
		_handler_method = static_cast<RootHandlerMethod>( handler_method );
	}

	// Handler::invoke
	template <class ROOT_CLASS, class PARAM>
		inline void Handler<ROOT_CLASS, const PARAM>::invoke( void * param )
	{
		const PARAM & typed_param = *static_cast< const PARAM * >( param );
		(_object->*_handler_method)( typed_param );
	}

	// Handler::invoke
	template <class ROOT_CLASS, class PARAM>
		inline void Handler<ROOT_CLASS, const PARAM>::invoke( const void * param )
	{
		const PARAM & typed_param = *static_cast< const PARAM * >( param );
		(_object->*_handler_method)( typed_param );
	}

} // namespace reflective
