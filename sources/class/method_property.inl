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
	// new_property( name, &Class::getter, [attributes] )
	template <class ROOT_OBJECT, class OBJECT, class TYPE>
		inline Property * new_property( SymbolName name, 
			TYPE (OBJECT::*getter)() const,	ClassMember::Attributes attributes )
	{
		ROOT_OBJECT * just_a_type_check = static_cast<OBJECT*>( nullptr );
		REFLECTIVE_UNUSED( just_a_type_check );

		typedef TYPE (ROOT_OBJECT::*BaseGetter)() const;
		BaseGetter base_getter = static_cast<BaseGetter>( getter );

		void * allocation = reflective_externals::mem_lifo_alloc( 
				alignment_of( MethodProperty<ROOT_OBJECT, TYPE> ), 
				sizeof( MethodProperty<ROOT_OBJECT, TYPE> ),
				&default_destroyer<ClassMember> );

		safe_get_type<TYPE>(); // this make sure that the type is initialized		

		MethodProperty<ROOT_OBJECT, TYPE> * result = new( allocation ) 
			MethodProperty<ROOT_OBJECT, TYPE>( 
				name, base_getter, 0, attributes );

		return result;
	}

	// new_property( name, &Class::getter, &Class::setter, [attributes] )
	template <class ROOT_OBJECT, class OBJECT, class TYPE>
		Property * new_property( SymbolName name, 			
			TYPE (OBJECT::*getter)() const,
			void (OBJECT::*setter)( TYPE value ), 
			ClassMember::Attributes attributes )
	{
		ROOT_OBJECT * just_a_type_check = static_cast<OBJECT*>( nullptr );
		REFLECTIVE_UNUSED( just_a_type_check );

		typedef TYPE (ROOT_OBJECT::*BaseGetter)() const;
		typedef void (ROOT_OBJECT::*BaseSetter)( TYPE );
		BaseGetter base_getter = static_cast<BaseGetter>( getter );
		BaseSetter base_setter = static_cast<BaseSetter>( setter );

		void * allocation = reflective_externals::mem_lifo_alloc( 
				alignment_of( MethodProperty<ROOT_OBJECT, TYPE> ), 
				sizeof( MethodProperty<ROOT_OBJECT, TYPE> ),
				&default_destroyer<ClassMember> );

		safe_get_type<TYPE>(); // this make sure that the type is initialized

		MethodProperty<ROOT_OBJECT, TYPE> * result = new( allocation ) 
			MethodProperty<ROOT_OBJECT, TYPE>( 
				name, base_getter, base_setter, attributes );

		return result;
	}

	// new_property( name, &Class::getter, &Class::setter, [attributes] )
	template <class ROOT_OBJECT, class OBJECT, class TYPE>
		Property * new_property( SymbolName name, 			
			TYPE (OBJECT::*getter)() const,
			void (OBJECT::*setter)( const TYPE & value ), 
			ClassMember::Attributes attributes )
	{
		ROOT_OBJECT * just_a_type_check = static_cast<OBJECT*>( nullptr );
		REFLECTIVE_UNUSED( just_a_type_check );

		typedef TYPE (ROOT_OBJECT::*BaseGetter)() const;
		typedef void (ROOT_OBJECT::*BaseSetter)( const TYPE & );
		BaseGetter base_getter = static_cast<BaseGetter>( getter );
		BaseSetter base_setter = static_cast<BaseSetter>( setter );

		void * allocation = reflective_externals::mem_lifo_alloc( 
				alignment_of( MethodPropertyRefSetter<ROOT_OBJECT, TYPE> ), 
				sizeof( MethodPropertyRefSetter<ROOT_OBJECT, TYPE> ),
				&default_destroyer<ClassMember> );

		safe_get_type<TYPE>(); // this make sure that the type is initialized

		MethodPropertyRefSetter<ROOT_OBJECT, TYPE> * result = new( allocation ) 
			MethodPropertyRefSetter<ROOT_OBJECT, TYPE>( 
				name, base_getter, base_setter, attributes );

		return result;
	}


				/**** MethodProperty *****/

	// MethodProperty::constructor
	template <class ROOT_OBJECT, class TYPE>
		inline MethodProperty<ROOT_OBJECT,TYPE>::MethodProperty(
			SymbolName name, 
			TYPE (ROOT_OBJECT::*getter)() const, 
			void (ROOT_OBJECT::*setter)( TYPE value ),
			ClassMember::Attributes attributes )
				: Property( name, safe_get_type<TYPE>(), safe_get_type_qualification<TYPE>(), attributes ), 
					_getter( getter ), _setter( setter )
	{
	}

	// MethodProperty::on_get_value_inplace
	template <class ROOT_OBJECT, class TYPE>
		inline void * MethodProperty<ROOT_OBJECT,TYPE>::on_get_value_inplace( void * object ) const
	{
		REFLECTIVE_UNUSED( object );
		return nullptr; // not supported
	}

	// MethodProperty::on_set_value
	template <class ROOT_OBJECT, class TYPE>
		inline bool MethodProperty<ROOT_OBJECT,TYPE>::on_set_value( void * object, const void * value ) const
	{
		if( !_setter )
			return false;

		ROOT_OBJECT & obj = *static_cast<ROOT_OBJECT*>( object );

		#ifdef _MSC_VER
			#pragma warning( push )
			#pragma warning( disable: 4267 ) /* this warning (conversion from 'size_t' to 'type', possible loss 
				of data) may be generated compiling with /Wp64, bacause the compiler uses the same template instance 
				for size_t and unsigned int. */
		#endif

		//typedef typename Unreferencer<TYPE>::Type NONREF_TYPE;
		(obj.*_setter)( Unreferencer<TYPE>::void_indirection( value ) );

		#ifdef _MSC_VER
			#pragma warning( pop )
		#endif

		return true;
	}

	// MethodProperty::on_get_value
	template <class ROOT_OBJECT, class TYPE>
		inline bool MethodProperty<ROOT_OBJECT,TYPE>::on_get_value( const void * object, void * value ) const
	{
		if( !_getter )
			return false;
		const ROOT_OBJECT & obj = *static_cast<const ROOT_OBJECT*>( object );

		//typedef typename Unreferencer<TYPE>::Type NONREF_TYPE;	

		Unreferencer<TYPE>::copy_construct( value, (obj.*_getter)() );
		return true;
	}



				/**** MethodPropertyRefSetter *****/

	// MethodPropertyRefSetter::constructor
	template <class ROOT_OBJECT, class TYPE>
		inline MethodPropertyRefSetter<ROOT_OBJECT,TYPE>::MethodPropertyRefSetter(
			SymbolName name, 
			TYPE (ROOT_OBJECT::*getter)() const, 
			void (ROOT_OBJECT::*setter)( const TYPE & value ),
			ClassMember::Attributes attributes )
				: Property( name, safe_get_type<TYPE>(), safe_get_type_qualification<TYPE>(), attributes ), 
					_getter( getter ), _setter( setter )
	{
	}

	// MethodProperty::on_get_value_inplace
	template <class ROOT_OBJECT, class TYPE>
		inline void * MethodPropertyRefSetter<ROOT_OBJECT,TYPE>::on_get_value_inplace( void * object ) const
	{
		REFLECTIVE_UNUSED( object );
		return nullptr; // not supported
	}

	// MethodPropertyRefSetter::on_set_value
	template <class ROOT_OBJECT, class TYPE>
		inline bool MethodPropertyRefSetter<ROOT_OBJECT,TYPE>::on_set_value( void * object, const void * value ) const
	{
		if( !_setter )
			return false;

		ROOT_OBJECT & obj = *static_cast<ROOT_OBJECT*>( object );

		#ifdef _MSC_VER
			#pragma warning( push )
			#pragma warning( disable: 4267 ) /* this warning (conversion from 'size_t' to 'type', possible loss 
				of data) may be generated compiling with /Wp64, bacause the compiler uses the same template instance 
				for size_t and unsigned int. */
		#endif

		//typedef typename Unreferencer<TYPE>::Type NONREF_TYPE;
		(obj.*_setter)( Unreferencer<TYPE>::void_indirection( value ) );

		#ifdef _MSC_VER
			#pragma warning( pop )
		#endif

		return true;
	}

	// MethodPropertyRefSetter::on_get_value
	template <class ROOT_OBJECT, class TYPE>
		inline bool MethodPropertyRefSetter<ROOT_OBJECT,TYPE>::on_get_value( const void * object, void * value ) const
	{
		if( !_getter )
			return false;
		const ROOT_OBJECT & obj = *static_cast<const ROOT_OBJECT*>( object );

		//typedef typename Unreferencer<TYPE>::Type NONREF_TYPE;	

		Unreferencer<TYPE>::copy_construct( value, (obj.*_getter)() );
		return true;
	}


} //namespace reflective
