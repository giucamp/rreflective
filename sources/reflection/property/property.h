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
		This class does not store the value of the property, but it describes the signature of a property
		(like a declaration would do), and exposes methods to get or set the value given an instance of the
		owning class. */
	class Property : public ClassMember
	{
	public:

		/** Retrieves the type of the property. This information is very important to use the getter and the 
			setters methods. */
		const QualifiedTypePtr & type() const	{ return m_type; }

		/** Retrieves the value of the property for a given object. The value is constructed in the specified buffer.
			@param i_owner_object object for which the property is requested. The specified ObjPtr must not be 
				empty (i.e. null) and must point to an object whose type is exatcly the owning class of the 
				property (not a derived class or un unrelated type).
			@param i_dest memory buffer in which the value of the property should be constructed. The size of this
				buffer must be (at least) the size of the type of property, and it must respect its alignment. A fail
				to meet these requirement will result in undefined behaviour.
			@return true if the value of property has been constructed in the buffer, false otherwise. If the 
				result is true, an object whose type is the type of the property has been constructed in the buffer,
				and the caller is resposable of destroying it. If the result is false, the buffer is left unchanged.*/
		bool get_value(ObjPtr i_owner_object, void * i_dest) const;
		
		/** Sets the value of the property for a given object. 
			@param i_owner_object object for which the property should be set. The specified ObjPtr must not be 
				empty (i.e. null) and must point to an object whose type is exatcly the owning class of the 
				property (not a derived class or un unrelated type).
			@param i_dest memory buffer containing an object whoose type is exactly the type of the property.
			@return true if the value of property has been set, false otherwise. */
		bool set_value(ObjPtr i_owner_object, const void * i_source) const;

		/** Retrieves a readonly pointer to the value of the property. This method is more efficient than get_value,
			because the value is not copied, but may fail in cases when get_value would succeed (for example in case
			of properties associated to a getter method).
			@param i_owner_object object for which the property is requested. The specified ObjPtr must not be 
				empty (i.e. null) and must point to an object whose type is exatcly the owning class of the 
				property (not a derived class or un unrelated type).
			@return a pointer to the value of the property, or nullptr if the method fails. In no case the content of
				the object can be altered. The returned pointer is valid as long as the owner object is alive. */
		const void * get_value_inplace(ObjPtr i_owner_object) const;

		/** Retrieves a pointer to the value of the property, which can be used to read and motify.
			Using this method allows to edit the value of the property more efficiently than using set_value, because 
			the value is not copied, but it may fail in cases when set_value would succeed (for example in case of
			properties associated to a setter method).
			@param i_owner_object object for which the property is requested. The specified ObjPtr must not be 
				empty (i.e. null) and must point to an object whose type is exatcly the owning class of the 
				property (not a derived class or un unrelated type).
			@return a pointer to the value of the property, or nullptr if the method fails. The content of the object
				may be altered. The returned pointer is valid as long as the owner object is alive. */
		void * get_editable_value_inplace(ObjPtr i_owner_object) const;

	protected:

		/** Constructs a property. All the parameters are used to initialize immutable data of the property.
			@param i_name name of the property, which must be unique in the class and in all the base classes.
			@param i_type qualified type of the property.
			@param i_flags access control and other flags. See ClassMember::Flags. */
		Property(SymbolName i_name, QualifiedTypePtr i_type, ClassMember::Flags i_flags )
			: ClassMember(std::move(i_name), i_flags), m_type(i_type) { }

	private:

		virtual void * get_value_inplace_impl(ObjPtr i_owner_object) const = 0;

		virtual bool get_value_impl(ObjPtr i_owner_object, void * i_dest) const = 0;

		virtual bool set_value_impl(ObjPtr i_owner_object, const void * i_source) const = 0;
		
	private:
		const QualifiedTypePtr m_type;
	};
}
