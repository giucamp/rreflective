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

#pragma once
#ifndef INCLUDING_REFLECTIVE
	#error "cant't include this header directly, include reflective.h instead"
#endif

namespace reflective
{
	/** This struct can be used as hasher for a standard unordered container that use SymbolName as key */
	struct SymbolNameHasher
	{
		uint32_t operator () (const SymbolName & i_source) const
			{ return i_source.hash(); }
	};

	class Attribute
	{
	public:

		Attribute(const Class & i_type) : m_type(i_type) { }

		Attribute(const Symbol &) = delete;

		Attribute & operator = (const Attribute &) = delete;

	private:
		const Class & m_type;
	};

	/** A symbol is a named object, with a (possibly empty) list of attributes. It may be a type (a class, an enum), a parameter,
		a property, etc. Many classes of reflective derive (directly or indirectly) from Symbol.
		The name of a Symbol is an object of type SymbolName, which is an alias of a reflective::Identifier. The name may be a pair
		(string, hash of the string). The type of the string may be std::string, reflective::StringView, or any suitable string type.
		The name of the symbol does not include scope specifiers. For example, the name of a class A in the namespace B is "A", not "B::A". 
		The name of a symbol is immutable (can be specified only to the constructor), and can't be empty. Furthermore,
		it follows the syntax of identifiers in C++: the first character must be alphabetic or an underscore, and the characters
		from the second on must be alphanumeric or underscores. No spaces or special symbols are allowed.
		The class Symbol is not polymorphic. This implies that from a pointer or reference to a Symbol there is no way to know the complete
		type (that is whether it is a reflective::Class, reflective::Namespace, or whatever. */
	class Symbol
	{
	protected:		
		
		/** Constructs a Symbol, providing a name. If REFLECTIVE_ASSERT_ENABLED is defined as non-zero, the name is checked
			to detect an invalid format. This is just a debug check: the caller is responsible to specify a valid string. */
		Symbol(SymbolName i_name)
			: m_name(std::move(i_name))
		{
			// check the character of the name, if REFLECTIVE_ASSERT_ENABLED is non-zero and SymbolName includes the string
			#if REFLECTIVE_ASSERT_ENABLED
				check_name(i_name);
			#endif
		}

	public:

		// reflection
		using ThisClass = Symbol;
		static void setup_class(Class & i_class);

		Symbol(const Symbol &) = delete; // copy construction not allowed
		Symbol & operator = (const Symbol &) = delete; // copy assignment not allowed

		/** Returns the name of the symbol, as a SymbolName. */
		const SymbolName & name() const			{ return m_name; }

		/** Retrieves a read-only list of the attributes associated to this symbol */
		const List<Attribute> & attributes() const	{ return m_attributes; }

		/** Sets the attributes associated to this type
			@param i_attributes list of attributes to assign. Since this parameter is an r-value reference, 
				the list must be a temporary object or the result of a std::move call. */
		void set_attributes(List<Attribute> && i_attributes) { m_attributes = std::move(i_attributes); }

	private:
		const SymbolName m_name;
		List<Attribute> m_attributes;

	private:

		#if REFLECTIVE_ASSERT_ENABLED

			template < typename HASHER, typename STRING >
				static void check_name(const Identifier<HASHER, STRING> & i_name)
			{
				const STRING & string = i_name.string();
				const size_t length = string.length();
				REFLECTIVE_ASSERT(length > 0, "empty symbol name");
				REFLECTIVE_ASSERT(isalpha(string[0]) != 0 || string[0] == '_', "the first character in a symbol name must be alphabetic or an underscore");
				
				for (size_t index = 1; index < length; index++)
				{
					REFLECTIVE_ASSERT(isalnum(string[index]) != 0 || string[index] == '_', "invalid character on a reflective::SymbolName");
				}
			}

			template < typename HASHER >
				static void check_name(const Identifier<HASHER, void> & /*i_name*/)
			{
			}

		#endif
	};
}

