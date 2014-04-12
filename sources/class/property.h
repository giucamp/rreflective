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
	// Property
	class Property : public ClassMember
	{
	public:

		// qualified_type
		const QualifiedType & qualified_type() const;
		
		// type - type of the property
		const Type & type() const;

		const TypeQualification & qualification() const; /* the qualfication gives
			additional infos about the type, such the indirection level and the pointed
			type (see TypeQualification) */

		bool get_value( const void * object, void * value ) const; /* gets a copy of
			the value of the property for the object pointed by 'object'. 
			The value is copy-constructed in the buffer, which must be large enough. 
			type().size() should be used to determine the required size. 
			After using the value the caller must destroy it. type().destroy() method 
			may be used.
			If the method returns false, the property could not be read (maybe it's a 
			write-only property), and no operation has been performed. */

		bool set_value( void * object, const void * value ) const; /* sets the value
			of the property for the object pointed by 'object'. The buffer pointed
			by 'value' must contain a valid object of the type of the property.
			The method may use the type assignment operator (if any) to set the value,
			or may destroy and copy-construct it. 
			The object pointed by 'value' is not modified by set_value(), and the
			caller is responsible for destroying it.
			If the method returns false, the property could not be written (maybe it's a 
			read-only property, or maybe the value was not suitable), and no operation 
			has been performed. */

		bool set_value_from_string( void * object, const char * value, StringOutputStream & error_buffer ) const;

		bool type_has_tostring() const;

	protected:

		// constructor
		Property( SymbolName name, 
			const Type & type, 
			const TypeQualification & type_qualification,
			ClassMember::Attributes attributes );

		// overridable
		virtual bool on_get_value( const void * object, void * value ) const = 0;
		virtual bool on_set_value( void * object, const void * value ) const = 0;	

	private: // data members
		QualifiedType _qualified_type;
	};

	// PropertyList
	typedef SymbolList<Property, false, ClassMember::Comparer > PropertyList;

} // namespace reflective

namespace reflective_externals
{
	// reflection of reflective::Property
	void init_type( reflective::Type * volatile * o_result,
		reflective::Property * null_pointer_1,
		reflective::Property * null_pointer_2 );
}
