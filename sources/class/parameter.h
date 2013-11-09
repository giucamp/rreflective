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
	// Parameter
	class Parameter : public Symbol
	{
	public:
		Parameter( SymbolName name, const Type & type );
		Parameter( SymbolName name, const Type & type, const TypeQualification & type_qualification );
		Parameter( SymbolName name, const QualifiedType & qualified_type );

		// type
		const Type & type() const;
		const TypeQualification & qualification() const;

		// stringizer
		void to_string( ToStringBuffer & dest_buffer ) const;

	private:
		Parameter & operator = ( const Parameter & ); // not supported

	private: // data members		
		const QualifiedType _qualified_type;
	};

	/* ParameterList - list of formal parameters. Each parameter has a name and a
		qualified type. Actual parameters (i.e. values) can be allocated in one
		buffer.
		Usually the user allocates a buffer (see total_size() and total_alignment() ),
		then uses a ParameterList:Walker to construct the actual parameters in the buffer.
		When the actual parameters are no more needed, the user can use destroy_values()
		to call the destructor, and finally deallocates the buffer. */
	class ParameterList : public SymbolList< Parameter, false >
	{
	public:

		// total_alignment \ total_size
		size_t total_alignment() const;
		size_t total_size() const; /* returns the required alignment
			and size of a buffer to be used to construct and pass parameter
			actual values. If the parameter list is empty, the size is zero
			and the alignment is 1. */

		/* destroy_values - destroys the objects in the specified buffer. The objects must
			match exactly this parameter list, otherwise memory corruprion\fault may occurr. */
		void destroy_values( void * actual_parameters );

		// actual_params_to_string
		void actual_params_to_string( ToStringBuffer & dest, const void * actual_parameters );

		/* parse_names - parses the string pointed by the first parameter as a comma separamed list of
				parameter names and fills names with them. count is the maximum number of names. The
				actual parsed count is retured. */
		static unsigned parse_names( const char * string, SymbolName * names, unsigned count );

		// Walker
		template <class POINTER>
			class Walker
		{
		public:
			Walker( const ParameterList & parameter_list, POINTER buffer );

			// parameter_list
			const ParameterList & parameter_list() const;

			/* is_not_over - returns true if the iteration is not over. It should
				be called after constructing the Walker and after each call to move_next(). */
			bool is_not_over() const;
			operator bool () const;

			// parameter \ value - can be called only if is_not_over returns true
			const Parameter & parameter() const;
			POINTER value() const;

			// move next - returns false if the iteration is over
			bool move_next();
			bool operator ++ ();

		private:
			Walker & operator = ( const Walker & ); // not supported

		private: // data members
			const ParameterList & _parameter_list;
			POINTER _curr_value;
			size_t _curr_parameter_index;
		};

		// assign
		void assign( const Parameter * * items, size_t count );
		template < size_t ITEM_COUNT >
			void assign( const Parameter * (&items_array)[ITEM_COUNT] );

	private: // internal services
		void _compute_totals();

	private: // data members
		size_t _total_alignment;
		size_t _total_size;
	};

	// new_parameter
	template <class TYPE> Parameter * new_parameter( SymbolName name );
	Parameter * new_parameter( SymbolName name, const Type & type, const TypeQualification & type_qualification );

} // namespace reflective


namespace reflective_externals
{
	// reflection of reflective::Parameter
	reflective::Class * init_type(
		reflective::Parameter * null_pointer_1,
		reflective::Parameter * null_pointer_2 );

	// reflection of reflective::ParameterList
	reflective::Class * init_type(
		reflective::ParameterList * null_pointer_1,
		reflective::ParameterList * null_pointer_2 );
	
} // namespace reflective_externals





