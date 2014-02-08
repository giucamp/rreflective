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
	// get_type<TYPE>()
	template <class TYPE> inline const Type & get_type()
	{
		return TypeContainer<TYPE>::get();
	}

	template <class TYPE> inline const Class * get_class()
	{
		const Type & type = TypeContainer<TYPE>::get();
		if( is_instance_of< Class >( type ) )
			return static_cast<const Class*>( &type );
		return nullptr;
	}

	// safe_get_type<TYPE>()
	template <class TYPE> inline const Type & safe_get_type()
	{
		TypeContainer<TYPE>::init_type();
		TypeContainer< typename CompileTimeTypeQualification<TYPE>::FinalType >::init_type();
		return TypeContainer<TYPE>::get();
	}

	// type_of( object )
	template <class TYPE> inline const Type & type_of( const TYPE & object )
	{
		const Type & base_type = get_type<TYPE>();
		if( base_type.check_capabilities( Type::eHasTypeResolver ) )
			return base_type.resolve_type( &object );
		else
			return base_type;
	}

	// safe_type_of( object )
	template <class TYPE> inline const Type & safe_type_of( const TYPE & object )
	{
		const Type & base_type = safe_get_type<TYPE>();
		if( base_type.check_capabilities( Type::eHasTypeResolver ) )
			return base_type.resolve_type( &object );
		else
			return base_type;
	}

	// qualified_type_of( object )
	template <class TYPE> 
		inline QualifiedType qualified_type_of( const TYPE & object )
	{
		return QualifiedType( type_of( object ), get_type_qualification<TYPE>() );
	}

	// get_type_qualification
	template <class TYPE>
		inline TypeQualification get_type_qualification()
	{
		const Type & final_type =
			get_type< typename CompileTimeTypeQualification<TYPE>::FinalType >();

		return TypeQualification(
			CompileTimeTypeQualification<TYPE>::indirection_levels,
			final_type,
			CompileTimeTypeQualification<TYPE>::constness_word );
	}

	// safe_get_type_qualification
	template <class TYPE>
		inline TypeQualification safe_get_type_qualification()
	{
		const Type & final_type =
			safe_get_type< typename CompileTimeTypeQualification<TYPE>::FinalType >();

		return TypeQualification(
			CompileTimeTypeQualification<TYPE>::indirection_levels,
			final_type,
			CompileTimeTypeQualification<TYPE>::constness_word );
	}

	// get_qualified_type
	template <class TYPE>
		inline QualifiedType get_qualified_type()
	{
		return QualifiedType( TypeContainer<TYPE>::get(), 
			TypeQualification(
				CompileTimeTypeQualification<TYPE>::indirection_levels,
				get_type< typename CompileTimeTypeQualification<TYPE>::FinalType >(),
				CompileTimeTypeQualification<TYPE>::constness_word ) );
	}

	// safe_get_qualified_type
	template <class TYPE>
		inline QualifiedType safe_get_qualified_type()
	{
		return QualifiedType( TypeContainer<TYPE>::get(), 
			TypeQualification(
				CompileTimeTypeQualification<TYPE>::indirection_levels,
				safe_get_type< typename CompileTimeTypeQualification<TYPE>::FinalType >(),
				CompileTimeTypeQualification<TYPE>::constness_word ) );
	}

	// to_string
	template <class TYPE>
		inline bool to_string( StringOutputStream & dest_buffer, const TYPE & source_object )
	{
		const Type & type = safe_type_of( source_object );
		if( !type.check_capabilities( Type::eHasToString ) )
			return false;

		type.to_string( dest_buffer, &source_object );
		return true;		
	}

	// assign_from_string
	template <class TYPE>
		inline bool assign_from_string( TYPE & dest_object, FromStringBuffer & source_buffer )
	{
		StringOutputStream error_buffer;
		const Type & type = type_of( dest_object );
		return type.assign_from_string( source_buffer, &dest_object, error_buffer );
	}

	// assign_from_string
	template <class TYPE>
		inline bool assign_from_string( TYPE & dest_object, FromStringBuffer & source_buffer, StringOutputStream error_buffer )
	{
		const Type & type = type_of( dest_object );
		return type.assign_from_string( source_buffer, &dest_object, error_buffer );
	}

	// count_inhertied_properties
	inline size_t count_inhertied_properties( const Type & type )
	{
		if( is_instance_of<Class>( type ) )
		{
			const Class & class_object = static_cast<const Class &>( type );
			return count_inhertied_properties( class_object );
		}
		return 0;
	}

	// _enum_inhertied_properties - internal function used by enum_inhertied_properties
	size_t _enum_inhertied_properties( const Class & class_object, const void * object,
		EnumPropertiesCallback callback, void * user_data, size_t * inout_index );

	// enum_inhertied_properties( type, object, callback, user_data )
	inline size_t enum_inhertied_properties( const Type & type, const void * object,
		EnumPropertiesCallback callback, void * user_data )
	{
		if( !is_instance_of<Class>( type ) )
			return 0;

		const Class & class_object = static_cast<const Class &>( type );

		size_t index = 0;
		return _enum_inhertied_properties( class_object, object, callback, user_data, &index );
	}

	// enum_inhertied_properties( class_object, object, callback, user_data )
	inline size_t enum_inhertied_properties( const Class & class_object, const void * object, EnumPropertiesCallback callback, void * user_data )
	{
		size_t index = 0;
		return _enum_inhertied_properties( class_object, object, callback, user_data, &index );
	}


	// get_inhertied_properties
	inline size_t get_inhertied_properties( const Type & type,
		PropertyAndClass * result, size_t result_max_size )
	{
		if( !is_instance_of<Class>( type ) )
			return 0;

		const Class & class_obj = static_cast<const Class &>( type );
		return get_inhertied_properties( class_obj, result, result_max_size );
	}

	// get_inhertied_properties_count
	inline size_t get_inhertied_properties_count( const Type & type )
	{
		if( !is_instance_of<Class>( type ) )
			return 0;

		const Class & class_obj = static_cast<const Class &>( type );
		return get_inhertied_properties_count( class_obj );
	}



					/**** LifoBuffer ****/

	// LifoBuffer::curr_alignment
	inline size_t LifoBuffer::curr_alignment() const
	{
		return _alignment;
	}

	// LifoBuffer::curr_length
	inline size_t LifoBuffer::curr_length() const
	{
		return _length;
	}

	// LifoBuffer::get
	inline void * LifoBuffer::get()
	{
		return _buffer;
	}


} // namespace reflective
