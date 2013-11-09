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
	/*
		Reflection definition kinds:

		- implicit definition:
				No code

		- non instrusive definition:

				namespace reflective_externals
				{
					// reflection of CLASS
					reflective::Class * init_type( CLASS * null_pointer_1, CLASS * null_pointer_2 );
				}

		- intrusive definition:

				class CLASS
				{
					public: // intrusive reflection
						static reflective::Class * create_class_object( CLASS * * );
				};

	
		- intrusive definition:

				class CLASS
				{
				public:
					typedef CLASS ThisClass;
					typedef BASE_CLASS BaseClass;
					static void init_class( reflective::Class & class_obj );
				};			

		
		Reflection explicit indications:

				class CLASS : public BASE_CLASS
				{
				public:
					typedef BASE_CLASS BaseClass;

					struct reflective_can_construct { static const int value = 1; };
					struct reflective_can_copy_assign { static const int value = 1; };
					struct reflective_can_destroy { static const int value = 1; };
				};

		reflctive can detect whether a class has a default constructor and whether is abstract.
		If it is not abstract and does have a constructor, reflective assumes that it has:
			- a public copy constructor 
			- a public assignment operator
			- a public destructor 
		If this is not true, explicit indications are necessary.
	*/

	// is_abstract< TYPE >::value - non-zero if the type is abstract
	// implementation - msc intrisic
	template <class TYPE>
		struct is_abstract
		{
			static const int value = __is_abstract(TYPE);
		};

	// has_explicit_constructor_indication< TYPE >::value - non-zero if the type has an inner type named reflective_can_construct
	// implementation - SFINAE - see http://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error
	struct ___has_explicit_constructor_indication
	{
		struct ___true { static const int val = 1; };
		struct ___false { static const int val = 0; };
		
		template <class TYPE>
			static ___true func( int, typename TYPE::reflective_can_construct * par = 0 );

		template <class TYPE>
			static ___false func( ... );
	};			
	template <class TYPE>
		struct has_explicit_constructor_indication : ___has_explicit_constructor_indication
	{
		typedef decltype( func<TYPE>( 0 ) ) B;
		static const int value = is_abstract<TYPE>::value ? 0 : B::val;
	};

	// has_explicit_copy_assign_indication< TYPE >::value - non-zero if the type has an inner type named reflective_can_copy_assign
	// implementation - SFINAE - see http://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error
	struct ___has_explicit_copy_assign_indication
	{
		struct ___true { static const int val = 1; };
		struct ___false { static const int val = 0; };
		
		template <class TYPE>
			static ___true func( int, typename TYPE::reflective_can_copy_assign * par = 0 );

		template <class TYPE>
			static ___false func( ... );
	};			
	template <class TYPE>
		struct has_explicit_copy_assign_indication : ___has_explicit_copy_assign_indication
	{
		typedef decltype( func<TYPE>( 0 ) ) B;
		static const int value = is_abstract<TYPE>::value ? 0 : B::val;
	};

	// has_explicit_destroy_indication< TYPE >::value - non-zero if the type has an inner type named reflective_can_destroy
	// implementation - SFINAE - see http://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error
	struct ___has_explicit_destroy_indication
	{
		struct ___true { static const int val = 1; };
		struct ___false { static const int val = 0; };
		
		template <class TYPE>
			static ___true func( int, typename TYPE::reflective_can_destroy * par = 0 );

		template <class TYPE>
			static ___false func( ... );
	};			
	template <class TYPE>
		struct has_explicit_destroy_indication : ___has_explicit_destroy_indication
	{
		typedef decltype( func<TYPE>( 0 ) ) B;
		static const int value = is_abstract<TYPE>::value ? 0 : B::val;
	};

	// has_base_class_indication< TYPE >::value - non-zero if the type has an inner type named BaseClass
	// implementation - SFINAE - see http://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error
	struct ___has_base_class_indication
	{
		struct ___true { static const int val = 1; };
		struct ___false { static const int val = 0; };
		
		template <class TYPE>
			static ___true func( int, typename TYPE::BaseClass * par = 0 );

		template <class TYPE>
			static ___false func( ... );
	};			
	template <class TYPE>
		struct has_base_class_indication : ___has_base_class_indication
	{
		typedef decltype( func<TYPE>( 0 ) ) B;
		static const int value = B::val;
	};

	// has_default_constructor< TYPE >::value - non-zero if the type has a public default constructor
	// implementation - SFINAE - see http://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error
	struct ___has_default_constructor
	{
		struct ___true { static const int val = 1; };
		struct ___false { static const int val = 0; };
		
		template <class TYPE>
			static ___true func( int, decltype( TYPE() ) * par = 0 );

		template <class TYPE>
			static ___false func( ... );
	};
	template <class TYPE, bool HAS_INDICATION> struct _has_default_constructor;
	template <class TYPE>
		struct _has_default_constructor<TYPE,false> : ___has_default_constructor
		{
			typedef decltype( func<TYPE>( 0 ) ) B;
			static const int value = is_abstract<TYPE>::value ? 0 : B::val;
		};
	template <class TYPE>
		struct _has_default_constructor<TYPE,true>
		{
			static const int value = TYPE::reflective_can_construct::value;
		};
	template <class TYPE>
		struct has_default_constructor : public _has_default_constructor<TYPE, has_explicit_constructor_indication<TYPE>::value != 0 >
	{
	};

	// has_copy_constructor< TYPE >::value - non-zero if the type has a public copy constructor
	/* implementation - has_default_constructor is used for non-abstract types. A struct\class may
		define an inner struct\class named reflective_can_copy_assign to override this value:
			struct reflective_can_copy_assign { static const int value = 0; };
			struct reflective_can_copy_assign { static const int value = 1; }; */
	template <class TYPE, bool HAS_EXPLICIT_COPY_ASSIGN_INDICATION> struct ___has_copy_constructor;
	template <class TYPE> struct ___has_copy_constructor<TYPE,true>
	{
		typedef typename TYPE::reflective_can_copy_assign INNER_STRUCT;
		static const int val = INNER_STRUCT::value;
	};
	template <class TYPE> struct ___has_copy_constructor<TYPE,false>
	{
		static const int val = is_abstract<TYPE>::value ? 0 : has_default_constructor<TYPE>::value;
	};		
	template <class TYPE>
		struct has_copy_constructor : ___has_copy_constructor< TYPE, has_explicit_copy_assign_indication<TYPE>::value != 0 >
		{
			static const int value = ___has_copy_constructor::val;
		};

	// has_assignment< TYPE >::value - non-zero if the type has a public assignment operator
	/* implementation - has_default_constructor is used for non-abstract types. A struct\class may
		define an inner struct\class named reflective_can_copy_assign to override this value:
			struct reflective_can_copy_assign { static const int value = 0; };
			struct reflective_can_copy_assign { static const int value = 1; }; */
	template <class TYPE, bool HAS_EXPLICIT_COPY_ASSIGN_INDICATION> struct ___has_has_assignment;
	template <class TYPE> struct ___has_has_assignment<TYPE,true>
	{
		typedef typename TYPE::reflective_can_copy_assign INNER_STRUCT;
		static const int val = INNER_STRUCT::value;
	};
	template <class TYPE> struct ___has_has_assignment<TYPE,false>
	{
		static const int val = is_abstract<TYPE>::value ? 0 : has_default_constructor<TYPE>::value;
	};		
	template <class TYPE>
		struct has_assignment : ___has_has_assignment< TYPE, has_explicit_copy_assign_indication<TYPE>::value != 0 >
		{
			static const int value = ___has_has_assignment::val;
		};

	// has_destructor< TYPE >::value - non-zero if the type has a public destructor
	/* implementation - has_default_constructor is used for non-abstract types. A struct\class may
		define an inner struct\class named reflective_can_copy_assign to override this value:
			struct reflective_can_destroy { static const int value = 0; };
			struct reflective_can_destroy { static const int value = 1; }; */
	template <class TYPE, bool HAS_EXPLICIT_COPY_ASSIGN_INDICATION> struct ___has_has_destructor;
	template <class TYPE> struct ___has_has_destructor<TYPE,true>
	{
		typedef typename TYPE::reflective_can_destroy INNER_STRUCT;
		static const int val = INNER_STRUCT::value;
	};
	template <class TYPE> struct ___has_has_destructor<TYPE,false>
	{
		static const int val = is_abstract<TYPE>::value ? 0 : has_default_constructor<TYPE>::value;
	};		
	template <class TYPE>
		struct has_destructor : ___has_has_destructor< TYPE, has_explicit_destroy_indication<TYPE>::value != 0 >
		{
			static const int value = ___has_has_destructor::val;
		};

	/* has_init_class_static_func< TYPE >::value - non-zero if the type has a static function with the signature:
			static void init_class( reflective::Class & class_obj );
		implementation - SFINAE - see http://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error */
	struct ___has_init_class_static_func
	{
		struct ___true { static const int val = 1; };
		struct ___false { static const int val = 0; };
		
		template <class TYPE>
			static ___true func( int, decltype( TYPE::init_class( *(Class*)0 ) ) par = 0 );

		template <class TYPE>
			static ___false func( ... );
	};			
	template <class TYPE>
		struct has_init_class_static_func : ___has_init_class_static_func
	{
		typedef decltype( func<TYPE>( 0 ) ) B;
		static const int value = B::val;
	};

	/* has_create_class_static_func< TYPE >::value - non-zero if the type has a static function with the signature:
			static Class * create_class();
		implementation - SFINAE - see http://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error */
	struct ___has_create_class_static_func
	{
		struct ___true { static const int val = 1; };
		struct ___false { static const int val = 0; };
		
		template <class TYPE>
			static ___true func( int, decltype( TYPE::create_class_object( (TYPE**)0 ) ) par = 0 );

		template <class TYPE>
			static ___false func( ... );
	};			
	template <class TYPE>
		struct has_create_class_static_func : ___has_create_class_static_func
	{
		typedef decltype( func<TYPE>( 0 ) ) B;
		static const int value = B::val;
	};

	// DefaultTypeTraits<TYPE>
	template <class TYPE>
		class DefaultTypeTraits
	{
	public:
		static const size_t size = sizeof( TYPE );
		static const size_t alignment = alignment_of( TYPE );				
	};

	// TypeTraits<TYPE>
	template <class TYPE>
		class TypeTraits : public DefaultTypeTraits<TYPE>
	{
	public:
	};

	// NumericTraits<TYPE>
	template <class TYPE>
		class NumericTraits : public DefaultTypeTraits<TYPE>
	{
	public:
		typedef bool CompareResult;
		static const unsigned bit_count = sizeof( TYPE ) * 8;
		static const TYPE zero;
		static const TYPE min;
		static const TYPE max;
		static const bool is_integer;
		static const bool is_signed;
	};

	// FloatTraits
	template <class TYPE>
		class FloatTraits : public NumericTraits<TYPE>
	{
	public:
		static const TYPE one;
		static const TYPE infinity;
		static const TYPE minus_infinity;
		static const TYPE indefinite;
		static const TYPE qnan;
		static const TYPE snan;
		static const TYPE pi;
	};


	// IntegerTraits
	template <class TYPE>
		class IntegerTraits : public NumericTraits<TYPE>
	{
	public:

	};


	// TypeContainer
	template <class TYPE>
		class TypeContainer
	{
	public:		

		static void init_type();

		static bool is_initialized();

		static const Type & get();
		static Type & non_const_get();

	private: // data members
		static Type * _type;
	};

	template <class TYPE>
		class TypeContainer<TYPE*>
	{
	public:
		static void init_type()
		{
			reflective::pointer_type();
		}

		static const Type & get()
		{
			return reflective::pointer_type();
		}
	};

	template <class TYPE>
		class TypeContainer<TYPE&>
	{
	public:
		static void init_type()
		{
			reflective::pointer_type();
		}

		static const Type & get()
		{
			return reflective::pointer_type();
		}
	};

	template <class TYPE>
		class TypeContainer<const TYPE>
	{
	public:
		static void init_type()
		{
			TypeContainer<TYPE>::init_type();
		}

		static const Type & get()
		{
			return TypeContainer<TYPE>::get();
		}
	};

	template <>
		class TypeContainer<void>
	{
	public:
		static void init_type()
		{
			reflective::void_type();
		}

		static const Type & get()
		{
			return reflective::void_type();
		}
	};


} // namespace reflective
