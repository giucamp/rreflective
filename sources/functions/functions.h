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
	/* get_type<TYPE>() - returns the reflective::Type objects associated to a given C++ 
		type known at compile time. Having no parameters, the actual template argument 
		cannot be deduced, and must be explicit. */	
	template <class TYPE> const Type & get_type();
	template <class TYPE> const Class * get_class();

	template <class TYPE> const Type & safe_get_type();

	template <class TYPE> const Class * safe_get_class();

	// is_instance_of<TEST_TYPE>( object )
	template <class TEST_TYPE, class ARG_TYPE>
		inline bool is_instance_of( const ARG_TYPE & object )
	{
		const reflective::Type & test_type = get_type< TEST_TYPE >();
		return is_instance_of( object, test_type );
	}

	template <class TYPE> const Type & type_of( const TYPE & object );

	template <class TYPE> const Type & safe_type_of( const TYPE & object );

	// is_instance_of( object, test_type )
	template <class ARG_TYPE>
		inline bool is_instance_of( const ARG_TYPE & object, const reflective::Type & test_type )
	{
		const reflective::Type & arg_type = type_of( object );
		return arg_type.can_cast_to( test_type );
	}

	// is_strict_instance_of<TEST_TYPE>( object )
	template <class TEST_TYPE, class ARG_TYPE>
		inline bool is_strict_instance_of( const ARG_TYPE & object )
	{
		const reflective::Type & test_type = get_type< TEST_TYPE >();
		return is_strict_instance_of( object, test_type );
	}

	// is_strict_instance_of( object, test_type )
	template <class ARG_TYPE>
		inline bool is_strict_instance_of( const ARG_TYPE & object, const reflective::Type & test_type )
	{
		const reflective::Type & arg_type = type_of( object );
		return arg_type == test_type;
	}

	/* get_type_qualification<TYPE> - returns a TypeQualification of the TYPE, describing
		indirection levels of the type (see reflective::TypeQualification). */
	template <class TYPE>
		TypeQualification get_type_qualification();

	template <class TYPE>
		TypeQualification safe_get_type_qualification();

	// get_qualified_type
	template <class TYPE>
		QualifiedType get_qualified_type();

	// safe_get_qualified_type
	template <class TYPE>
		QualifiedType safe_get_qualified_type();

	// qualified_type_of
	template <class TYPE> QualifiedType qualified_type_of( const TYPE & object );

	// object_identifier_to_string
	bool object_identifier_to_string( StringOutputStream & dest_buffer, const void * object, const Type & type );
	bool object_identifier_to_string( StringOutputStream & dest_buffer, const void * object, const Class & class_obj );

	// to_string
	template <class TYPE>
		bool to_string( StringOutputStream & dest_buffer, const TYPE & source_object );

	// StringOutputStream << object
	template <class TYPE>
		inline StringOutputStream operator << ( StringOutputStream & dest_buffer, const TYPE & source_object )
	{
		to_string( dest_buffer, source_object );
		return dest_buffer;
	}

	// assign_from_string
	template <class TYPE>
		bool assign_from_string( TYPE & dest_object, FromStringBuffer & source_buffer );
	template <class TYPE>
		bool assign_from_string( TYPE & dest_object, FromStringBuffer & source_buffer, StringOutputStream error_buffer );

	/* count_inhertied_properties( type ) - count the number of properties of the type and
		all the base types. If the type is not a class, the return value is always zero. */
	size_t count_inhertied_properties( const Type & type );
	size_t count_inhertied_properties( const Class & type );
	
	/* EnumPropertiesCallback - callback to be used with enum_inhertied_properties. */
	typedef bool (*EnumPropertiesCallback)( 
		const Class & class_object, // the class owning the property
		const Property & property, // current property
		const void * object, /* the object passed to enum_inhertied_properties, after the 
			cast to class_object. This object can be safely used with class_object. */
		size_t index, // zero-based index of the property in the enumeration
		void * user_data ); // user param passed to enum_inhertied_properties

	/* enum_inhertied_properties - enumerates all the properties of the given type and all
		the base classes. The properties are enumerated starting from the most derived.

			type or class_obj - the most derived type whose properties must be enumerated. If
				it is not a class type, no property is enumerated.

			object - pointer to an object of the target type, which is cast to the correct base 
				type to be passed to the callback. Assuming that a pointer to a derived class
				can be used as pointer to a base class can cause memory corruption.

			callback - callback to be called for every property. If the callback return true
				the enumeration continues. If the callback returns false, the enumeration stops.

			user_data - user param passed to the callback, that has no meaning for enum_inhertied_properties

			return type - number of enumerated properties, or ~size_t( 0 ) if the callback has returned false. */
	size_t enum_inhertied_properties( const Type & type, const void * object,
			EnumPropertiesCallback callback, void * user_data );
	size_t enum_inhertied_properties( const Class & class_obj, const void * object,
		EnumPropertiesCallback callback, void * user_data );


	// PropertyAndClass
	struct PropertyAndClass
	{
		const Property * property;
		const Class * owning_class;
	};

	size_t get_inhertied_properties( const Type & type,
		PropertyAndClass * result, size_t result_max_size );

	size_t get_inhertied_properties( const Class & class_obj,
		PropertyAndClass * result, size_t result_max_size );

	size_t get_inhertied_properties_count( const Type & type );

	size_t get_inhertied_properties_count( const Class & class_obj );

	const Property * find_inhertied_property( const Class & class_obj, 
		SymbolName name, const Class * * out_owning_class,
		void * * inout_object = nullptr );

	const Action * find_inhertied_action( const Class & class_obj, 
		SymbolName name, const Class * * out_owning_class );


	// LifoBuffer
	class LifoBuffer
	{
	public:

		LifoBuffer( size_t initial_alignment, size_t initial_size );

		size_t curr_alignment() const;

		size_t curr_length() const;

		void * get();

		void * get( size_t needed_alignment, size_t needed_size );

		~LifoBuffer();

	private:
		LifoBuffer( const LifoBuffer & ); // unimplemented
		LifoBuffer & operator = ( const LifoBuffer & );  // unimplemented

	private: // data members
		void * _buffer;
		size_t _alignment, _length;
	};		



} // namespace reflective



namespace reflective_externals
{
	// reflection of reflective::PropertyAndClass
	void init_type( reflective::Type * volatile * o_result,
		reflective::PropertyAndClass * null_pointer_1,
		reflective::PropertyAndClass * null_pointer_2 );
	
} // namespace reflective_externals
