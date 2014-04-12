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
	// ParameterStringizer
	class ParameterStringizer
	{
	public:

		ParameterStringizer();
		
		// parse_actual_list
		bool parse_actual_list( const ParameterList & parameter_list,
			FromStringBuffer & source_string_buffer, void * destination,
			StringOutputStream & error_message, size_t * out_processed_parameters_count );

	private:
		ParameterStringizer & operator = ( const ParameterStringizer & ); // not supported
	};

	// ActionInvoker
	class ActionInvoker
	{
	public:

		ActionInvoker();

		~ActionInvoker();

		void setup( const Action * action );

		bool setup( const Action * action, 
			FromStringBuffer & parameters_string_buffer,
			StringOutputStream & out_error_message, 
			size_t * out_processed_parameters_count );

		bool invoke( void * object, 
			StringOutputStream * out_return_value,
			StringOutputStream * out_reason_text ) const;

		void close();

		const Action * action() const
			{ return _action; }

	private:
		ActionInvoker & operator = ( const ActionInvoker & ); // not supported

	private: // data members
		const Action * _action;
		void * _parameters;
		void * _return_value;
	};


} // namespace reflective


namespace reflective_externals
{
	
	// reflection of reflective::ActionInvoker
	void init_type( reflective::Type * volatile * o_result,
		reflective::ActionInvoker * null_pointer_1,
		reflective::ActionInvoker * null_pointer_2 );
	
	// reflection of reflective::ParameterStringizer
	void init_type( reflective::Type * volatile * o_result,
		reflective::ParameterStringizer * null_pointer_1,
		reflective::ParameterStringizer * null_pointer_2 );

} // namespace reflective_externals





