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
	// Action
	class Action : public ClassMember
	{
	public:

		// qualified return type
		const Type & return_type() const;
		const TypeQualification & return_type_qualification() const;

		// parameter list
		ParameterList & edit_parameter_list();
		const ParameterList & parameter_list() const;

		/* invoke - invokes the action - the buffer pointed by actual_parameters must contain the 
			actual paramerters. ParameterList::total_alignment() and ParameterList::total_size()
			tells the required size and alignment for this buffer, and ParameterList::Walker may
			be used to construct the values in the buffer. The lifetime of this values is up to
			the caller, that should destroy them and deallocate the buffer (see ParameterList). 
			return_value parameter must point to a buffer, allocated according to the size and
			alignment of the return type, in which the return value is constructed.
			The return value is true if the action has been invoked, and false if the action could
			not be invoked. In the latter case the memory pointed by return_value has an indefinite 
			content, and no return value is constructed on it. can_invoke() can be used to know the 
			reason of the failure. */
		bool invoke( void * object, const void * actual_parameters, void * return_value ) const;

		/* can_invoke - tells whether this action can be called on the specified object. Depending 
			on the type and the state of the object the action may be disabled. Before invoking 
			the action, invoke() internally calls can_invoke(). If the latter returns false, invoke()
			does not invoke the action and returns false.
			If can_invoke() returns false, reason_text is filled with a human readable text which
			explain the reason why the action cannot be invoked. If can_invoke() returns true, the
			content of the out_reason_text after the call should be ignored.
			can_invoke() may be called even after invoke() has returned false, to get the human 
			readable reason. 
			Note: If the same object is used with adjacent calls to inkoke\can_invoke, to ensure that 
			the calls results are consistent, the object should not change its state . */
		virtual bool can_invoke( const void * object, StringOutputStream & out_reason_text ) const	
			{ REFLECTIVE_UNUSED_2( object, out_reason_text );  return true; }

		// invoke
		bool invoke( void * object, FromStringBuffer & parameters_string_buffer, 
			StringOutputStream & out_return_value_text, StringOutputStream & out_error_text ) const;

	protected:

		Action( SymbolName name, Attributes attributes, const Type & return_type );

		Action( SymbolName name, Attributes attributes,
			const Type & return_type, const TypeQualification & return_type_qualification );

		virtual void on_invoke( void * object, const void * actual_parameters, 
			void * return_value ) const = 0;

	private:
		Action & operator = ( const Action & ); // not supported

	private: // data members
		const Type & _return_type;
		TypeQualification _return_type_qualification;
		ParameterList _parameter_list;
	};

	// ActionList
	typedef SymbolList<Action, false, ClassMember::Comparer > ActionList;

} // namespace reflective


namespace reflective_externals
{
	// reflection of reflective::Action
	void init_type( reflective::Type * volatile * o_result,
		reflective::Action * null_pointer_1,
		reflective::Action * null_pointer_2 );
}
