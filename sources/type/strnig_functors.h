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
	class Type;

	// Stringizer
	class Stringizer
	{
	public:

		// to_string - gives a string representation of an object
		virtual void to_string( ToStringBuffer & dest_buffer,
			const Type & type, const void * object ) const = 0;

		// assign_from_string
		virtual bool assign_from_string( FromStringBuffer & source_buffer, 
			const Type & type, void * object, 
			ToStringBuffer & error_buffer ) const = 0;

		// virtual destructor
		virtual ~Stringizer() { }
	};

	// GlobalFunctionsStringizer
	class GlobalFunctionsStringizer : public Stringizer
	{
	public:

		// function types
		typedef void (*ToStringDumper)( ToStringBuffer & dest_buffer, 
			const Type & type, const void * object );
		typedef bool (*FromStringAssigner)( FromStringBuffer & source_buffer,
			const Type & type, void * object, ToStringBuffer & error_buffer );

		// lifo_create
		static GlobalFunctionsStringizer * lifo_create( ToStringDumper to_string, FromStringAssigner from_string );

		// constructor
		GlobalFunctionsStringizer( ToStringDumper to_string, FromStringAssigner from_string );

		// to_string - gives a string representation of an object
		void to_string( ToStringBuffer & dest_buffer,
			const Type & type, const void * object ) const;

		// assign_from_string
		bool assign_from_string( FromStringBuffer & source_buffer, 
			const Type & type, void * object, 
			ToStringBuffer & error_buffer ) const;

		// function getters
		FromStringAssigner from_string() const;
		ToStringDumper to_string() const;

	private: // data members
		ToStringDumper _to_string;
		FromStringAssigner _from_string;		
	};

	// MethodsStringizer
	template <class TYPE>
		class MethodsStringizer : public Stringizer
	{
	public:

		// function types
		typedef void (TYPE::*ToStringDumper)( ToStringBuffer & dest_buffer ) const;
		typedef bool (TYPE::*FromStringAssigner)( FromStringBuffer & source_buffer,
			ToStringBuffer & error_buffer );

		// lifo_create
		static MethodsStringizer * lifo_create( ToStringDumper to_string, FromStringAssigner from_string );

		// constructor
		MethodsStringizer( ToStringDumper to_string, FromStringAssigner from_string );

		// to_string - gives a string representation of an object
		void to_string( ToStringBuffer & dest_buffer,
			const Type & type, const void * object ) const;

		// assign_from_string
		bool assign_from_string( FromStringBuffer & source_buffer, 
			const Type & type, void * object, 
			ToStringBuffer & error_buffer ) const;

		// function getters
		FromStringAssigner from_string() const;
		ToStringDumper to_string() const;

	private: // data members		
		ToStringDumper _to_string;
		FromStringAssigner _from_string;
	};

} // namespace reflective
