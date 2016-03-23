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
	/** Abstract class representing a property of a class. A property is a generalization of a data member,
		and may be a getter-setter pair, or just a getter (for a read-only property) or even just a setter
		(for a write-only property).
		This class does not store the value of the property, but it describes the signature of a property
		(like a declaration would do), and exposes methods to get or set the value given an instance of the
		owning class. 
		Depending on the type (of the value) of the property, getters and setters can be done with copy or move semantics.
		Depending on how the property is exposed (getter\setter\member variable), the property may be read or edited
		in place (without any copy or move operation). */
	class Property : public ClassMember
	{
	public:

		/** Retrieves the type of the property. This information is very important to use the getter and the 
			setters methods. To construct, destroy, get the size and alignment of values of the property use 
			type().primary_type(). */
		const QualifiedTypePtr & type() const REFLECTIVE_NOEXCEPT	{ return m_type; }

		/** Retrieves the value of the property for a given owner object, using copy semantics. On success the value 
				is copy-constructed in the specified buffer. The owner object is not atered in any way.
			@param i_owner_object object for which the property is requested. The specified ObjPtr must not be 
				empty (i.e. null) and must point to an object whose type is exactly the owning class of the 
				property (not a derived class or an unrelated type). If the type of i_owner_object does not match exactly
				the class owning the property, the deriveds class shoud call reflective::on_invaid_operation(),
				that may throw an exception, abort the program, depending on the implementation provided by the user.
			@param i_dest memory buffer in which the value of the property should be copy-constructed. The size of this
				buffer must be (at least) the size of the type of property, and it must respect its alignment. A fail
				to meet these requirement will result in undefined behavior.
			@param i_fail_reason string buffer to which a human readable description of the error is appended in
				case of failure (i.e. the function returns false). Otherwise is should remain unchanged.
			@return true if the value of property has been copy-constructed in the buffer, false otherwise. If the 
				result is true, an object whose type is the type of the property has been constructed in the buffer,
				and the caller is responsible of destroying it. If the result is false, the buffer is left unchanged.*/
		bool get_value_by_copy(ObjPtr i_owner_object, void * i_dest, OutBufferTextStream & o_fail_reason) const;

		/** Moves the value of the property from a given owner object. On success the value is move-constructed in the specified 
				buffer. This method alters the owner object, as the move constructor of the type of the value is invoked.
			@param i_owner_object object for which the property is requested. The specified ObjPtr must not be 
				empty (i.e. null) and must point to an object whose type is exactly the owning class of the 
				property (not a derived class or an unrelated type). If the type of i_owner_object does not match exactly
				the class owning the property, the deriveds class shoud call reflective::on_invaid_operation(),
				that may throw an exception, abort the program, depending on the implementation provided by the user.
			@param i_dest memory buffer in which the value of the property should be move-constructed. The size of this
				buffer must be (at least) the size of the type of property, and it must respect its alignment. A fail
				to meet these requirement will result in undefined behavior.
			@param i_fail_reason string buffer to which a human readable description of the error is appended in
				case of failure (i.e. the function returns false). Otherwise is should remain unchanged.
			@return true if the value of property has been move-constructed in the buffer, false otherwise. If the 
				result is true, an object whose type is the type of the property has been constructed in the buffer,
				and the caller is responsible of destroying it. If the result is false, the buffer is left unchanged.*/
		bool get_value_by_move(ObjPtr i_owner_object, void * i_dest, OutBufferTextStream & o_fail_reason) const;
		
		/** Sets the value of the property for a given object using copy semantics. The source value is not altered 
				in any way.
			@param i_owner_object object for which the property should be set. The specified ObjPtr must not be 
				empty (i.e. null) and must point to an object whose type is exactly the owning class of the 
				property (not a derived class or un unrelated type). If the type of i_owner_object does not match exactly
				the class owning the property, the deriveds class shoud call reflective::on_invaid_operation(),
				that may throw an exception, abort the program, depending on the implementation provided by the user.
			@param i_source memory buffer containing an object whose type is exactly the type of the property.
			@param i_fail_reason string buffer to which a human readable description of the error is appended in
				case of failure (i.e. the function returns false). Otherwise is should remain unchanged.
			@return true if the value of property has been set, false otherwise. */
		bool set_value_by_copy(ObjPtr i_owner_object, const void * i_source, OutBufferTextStream & o_fail_reason) const;

		/** Sets the value of the property for a given object using move semantics. On success the source value is 
			altered, as it is the destination of a move assignment.
			@param i_owner_object object for which the property should be set. The specified ObjPtr must not be 
				empty (i.e. null) and must point to an object whose type is exactly the owning class of the 
				property (not a derived class or un unrelated type). If the type of i_owner_object does not match exactly
				the class owning the property, the deriveds class shoud call reflective::on_invaid_operation(),
				that may throw an exception, abort the program, depending on the implementation provided by the user.
			@param i_source memory buffer containing an object whose type is exactly the type of the property.
			@param i_fail_reason string buffer to which a human readable description of the error is appended in
				case of failure (i.e. the function returns false). Otherwise is should remain unchanged.
			@return true if the value of property has been set, false otherwise. */
		bool set_value_by_move(ObjPtr i_owner_object, void * i_source, OutBufferTextStream & o_fail_reason) const;

		/** Retrieves a read-only pointer to the value of the property. This method is more efficient than get_value,
			because the value is not copied, but may fail in cases when get_value would succeed (for example in case
			of properties associated to a getter method).
			@param i_owner_object object for which the property is requested. The specified ObjPtr must not be 
				empty (i.e. null) and must point to an object whose type is exactly the owning class of the 
				property (not a derived class or un unrelated type).
			@return a pointer to the value of the property, or nullptr if the method fails. In no case the content of
				the object can be altered. The returned pointer is valid as long as the owner object is alive. */
		const void * get_value_inplace(ObjPtr i_owner_object) const;

		/** Retrieves a pointer to the value of the property, which can be used to read and modify.
			Using this method allows to edit the value of the property more efficiently than using set_value, because 
			the value is not copied, but it may fail in cases when set_value would succeed (for example in case of
			properties associated to a setter method).
			@param i_owner_object object for which the property is requested. The specified ObjPtr must not be 
				empty (i.e. null) and must point to an object whose type is exactly the owning class of the 
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

		/** derived class must implement this function returning a pointer to the value
			of the property for the given object. If this is not possible, the implementation
			should return nullptr. */
		virtual void * get_value_inplace_impl(ObjPtr i_owner_object) const = 0;

		virtual bool get_value_by_copy_impl(ObjPtr i_owner_object, void * i_dest, OutBufferTextStream & o_fail_reason) const = 0;

		virtual bool get_value_by_move_impl(ObjPtr i_owner_object, void * i_dest, OutBufferTextStream & o_fail_reason) const = 0;

		virtual bool set_value_by_copy_impl(ObjPtr i_owner_object, const void * i_source, OutBufferTextStream & o_fail_reason) const = 0;
		
		virtual bool set_value_by_move_impl(ObjPtr i_owner_object, void * i_source, OutBufferTextStream & o_fail_reason) const = 0;

	private:
		const QualifiedTypePtr m_type;
	};
}
