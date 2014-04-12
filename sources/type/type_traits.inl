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
	template< class TYPE, class LIFE_EFUNC_ENUM, bool IS_CLASS, bool IS_ENUM >
		class DefaultTypeFactory;

	/*template< class TYPE, bool HAS_CONSTRUCTOR, bool HAS_COPY_ASSIGNMENT, bool IS_UNGRABBED, bool IS_CLASS, bool IS_ENUM >
		class DefaultTypeFactory
	{
	public:
		static reflective::Type * create_type();
	};*/

	// _CreateClass
	template < class TYPE, bool HAS_CREATE_CLASS > struct _CreateClass;
	template < class TYPE > struct _CreateClass< TYPE, true >
	{
		static Class * _create()
			{ return TYPE::create_class_object( static_cast<TYPE**>( 0 ) ); }
	};
	template < class TYPE > struct _CreateClass< TYPE, false >
	{
		static Class * _create()
		{
			static Class * result = new_class<TYPE>();
			return result;
		}
	};

	// DefaultTypeFactory for classes
	template< class TYPE, class LIFE_EFUNC_ENUM >
		class DefaultTypeFactory< TYPE, LIFE_EFUNC_ENUM, true, false >
	{
	private:

		// _SetBaseClass
		template <bool HAS_BASE_CLASS_INDICATION > struct _SetBaseClass;
		template <> struct _SetBaseClass< true >
		{
			static void _set( Class & new_class )
			{
				new_class.set_base_type( BaseType::from_types< TYPE, typename TYPE::BaseClass >() );
			}
		};
		template <> struct _SetBaseClass< false >
		{
			static void _set( Class & new_class )
			{
				REFLECTIVE_UNUSED( new_class );
			}
		};

		// _InitClass
		template <bool HAS_CREATE_CLASS > struct _InitClass;
		template <> struct _InitClass< true >
		{
			static void _init( Class & class_obj )
				{ return TYPE::init_class( class_obj ); }
		};
		template <> struct _InitClass< false >
		{
			static void _init( Class & )
				{ }
		};

	public:
		static Type * create_type()
		{
			Class * new_class = _CreateClass< TYPE, has_create_class_static_func<TYPE>::value != 0 >::_create();

			REFLECTIVE_ASSERT( new_class->size() == sizeof( TYPE ) );
			REFLECTIVE_ASSERT( new_class->alignment() == alignment_of( TYPE ) );

			const LIFE_EFUNC_ENUM enum_value = static_cast<LIFE_EFUNC_ENUM>( 0 );

			new_class->set_life_functions( LifeFunctions::from_type< TYPE >( enum_value ) );

			if( new_class->base_types_count() == 0 )
				_SetBaseClass< has_base_class_indication<TYPE>::value != 0 >::_set( *new_class );

			_InitClass< has_init_class_static_func<TYPE>::value != 0 >::_init( *new_class );

			return new_class;
		}
	};

	// DefaultTypeFactory for enums
	template< class TYPE, class LIFE_EFUNC_ENUM >
		class DefaultTypeFactory< TYPE, LIFE_EFUNC_ENUM, false, true >
	{
	public:
		static Type * create_type()
		{
			StaticConstString parent_namespace_path, name;
			const char * full_type_name = typeid( TYPE ).name();			
			separate_symbol_name( full_type_name, &parent_namespace_path, &name, false );

			return new_enum( parent_namespace_path, name );
		}
	};

}

namespace reflective_externals
{
	template< class TYPE >
		inline void init_type( reflective::Type * volatile * o_result, TYPE * null_pointer_1, TYPE * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );		

		if( *o_result == nullptr )
		{		
			#define __REFLECTIVE_HAS_CONSTRUCTOR		( reflective::has_default_constructor< TYPE >::value && reflective::has_destructor< TYPE >::value )
			#define __REFLECTIVE_IS_ABSTRACT			reflective::is_abstract< TYPE >::value
			#define __REFLECTIVE_HAS_COPY				reflective::has_copy_constructor< TYPE >::value
			#define __REFLECTIVE_HAS_ASSIGN				reflective::has_assignment< TYPE >::value

			on_implicit_type_instance( typeid(TYPE).name(), 
				reflective::has_default_constructor< TYPE >::value != 0,
				reflective::has_copy_constructor< TYPE >::value != 0,
				reflective::has_assignment< TYPE >::value != 0,
				reflective::has_destructor< TYPE >::value != 0 );
			typedef std::conditional< __REFLECTIVE_HAS_CONSTRUCTOR && !__REFLECTIVE_IS_ABSTRACT,

				// has constructor
				std::conditional< __REFLECTIVE_HAS_COPY && __REFLECTIVE_HAS_ASSIGN, ConstructorCopyAssignmentDestructor, ConstructorDestructor >::type,


				// no constructor
				std::conditional< __REFLECTIVE_HAS_COPY && __REFLECTIVE_HAS_ASSIGN && !__REFLECTIVE_IS_ABSTRACT,
					CopyAssignmentDestructor, NoLifeFunctions >::type
		
			>::type LifeFuncEnum;

			*o_result = reflective::DefaultTypeFactory< TYPE, LifeFuncEnum,
				std::is_class< TYPE >::value,
				std::is_enum< TYPE >::value 
					>::create_type();

			#undef __REFLECTIVE_HAS_CONSTRUCTOR
			#undef __REFLECTIVE_IS_ABSTRACT
			#undef __REFLECTIVE_HAS_COPY
			#undef __REFLECTIVE_HAS_ASSIGN
		}
	}
}

namespace reflective
{	
	// TypeContainer<TYPE>::_type
	template <class TYPE>
		Type * TypeContainer<TYPE>::_type = TypeContainer<TYPE>::_create_type();

	// * TypeContainer<TYPE>::_create_type
	template <class TYPE>
		Type * TypeContainer<TYPE>::_create_type()
	{
		init_type();
		REFLECTIVE_ASSERT( _type != nullptr );
		return _type;
	}

	// TypeContainer<TYPE>::init_type
	template <class TYPE>
		void TypeContainer<TYPE>::init_type()
	{
		if( !_type )
		{
			reflective_externals::init_type( &_type,
				static_cast<TYPE *>( nullptr ), static_cast<TYPE *>( nullptr ) );
			REFLECTIVE_ASSERT( _type != nullptr );
		}
	}

	// TypeContainer<TYPE>::is_initialized
	template <class TYPE>
		bool TypeContainer<TYPE>::is_initialized()
	{
		return _type != nullptr;
	}

	// TypeContainer<TYPE>::get
	template <class TYPE>
		const Type & TypeContainer<TYPE>::get()
	{
		REFLECTIVE_ASSERT( _type != nullptr ); /* the type has not been created - safe_get_type<TYPE>() 
			instead of get_type<TYPE>() can be used during the initialization of the globals
			to ensure that the type is instantiated. */
		return *_type;
	}

} // namespace reflective


