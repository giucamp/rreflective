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
	/** Abstract class rapresenting a property of a class. A property is a generaliaztion of a data member,
		and maybe a pair of getter-setter, or just a getter (for a readonly property) or just a even setter
		(for a writeonly property).
		Since this library is about reflection, this class does not store the value of the property, but it 
		describes the signature of a property (like a declaration would do), and exposes methods to get or set 
		the value given an instance of the owning class.
		When giving the property an instance of the owning class (with a void*), the user must be sure that 
		the pointer acually points to an object of the owning class, and not to a base o derived class. Since
		with reflection the type binding is done at runtime, there is not a general way to validate the void
		pointers passed to the property. Messing with the object will result in an undefined behaviour.  */
	class Property : public ClassMember
	{
	public:

		const QualifiedTypePtr & type() const	{ return m_type; }

		bool get_value(const void * i_owner_object, void * i_dest) const;

		bool set_value(void * i_owner_object, const void * i_source) const;

		const void * get_value_inplace(const void * i_owner_object) const;

		void * get_editable_value_inplace(void * i_owner_object) const;

	protected:

		/** Constructs a property. The overloaded function reflective::make_property offsers
			a more convenient way to construct a property.
			@param i_name name of the property, which must be unique in the class and in all the base classes. This
				parameter initializes an immutable member of the indirect base class Symbol.
			@param i_type qualified type of the property. This parameter initializes an immutable member.
			@param i_flags access control and other flags. See ClassMember::Flags. This
				parameter initializes an immutable member of the base class ClassMember.*/
		Property(SymbolName i_name, QualifiedTypePtr i_type, ClassMember::Flags i_flags )
			: ClassMember(std::move(i_name), i_flags), m_type(i_type) { }
	private:

		virtual void * get_value_inplace_impl(void * i_owner_object) const = 0;

		virtual bool get_value_impl(const void * i_owner_object, void * i_dest) const = 0;

		virtual bool set_value_impl(void * i_owner_object, const void * i_source) const = 0;
		
	private:
		const QualifiedTypePtr m_type;
	};
}
