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
	// CompileTimeTypeQualification
	template <class TYPE> class CompileTimeTypeQualification
	{
	public:
		typedef TYPE FinalType; /* it's the final type, that is the type with maximum 
			indirection level. For example the following types have all 'int' as final 
			type: 'int', 'int &', 'int * * * const * const &'. 
			C++ language remainder: a reference is like a const pointer, that is 'int &' is 
			the same as 'int * const', except that the first exposes directly its last 
			level of indirection. */
		static const unsigned indirection_levels = 0; /* the number ofindirection levels,
			which is zero for non-pointers, 1 for pointers, 2 for pointers to pointers, 
			and so on. */
		static const unsigned constness_word = 0; /* specifies, for each indirection 
				level,  the constness of the type. 
			- 'int', 'int**', "int&": no const is present, constness_word is zero.
			- 'int * * const': the first indirection level is const (that is, the
				pointer cannot be assigned after initialization), so the 
				constness_word is 1 (only bit 0 set).
			- 'const int ********': only the last indirection level (8) is const,
				so the 8th bit of the constness_word is 1 (constness_word=256).
			C++ language remainder: the const keyword can be prefix or postfix to 
				the type it applies to: 'const int *' and 'int const *' are the 
				same declarations. */

		static const unsigned is_reference = 0;
	};
	template <class TYPE> class CompileTimeTypeQualification<TYPE&>;
	template <class TYPE> class CompileTimeTypeQualification<TYPE*>;
	template <class TYPE> class CompileTimeTypeQualification<const TYPE>;


	/* TypeQualification. Gives additional info about a type:
		- the number of indirection levels. For non-pointer type this is zero, for simple
			pointer is 1, for pointer to pointer is 2, and so on.
		- a boolean value for each indirection level, which specifies if the level is const.
		- an optional final type, which is the pointed type. If the number of indirection
			levels is zero, the final type is nullptr, otherwise it is != nullptr.
	   The TypeQualification should be used in couple with a reference to a Type. It is suitable 
		to encode the type of properies, template parameters, action parameters and return types. */
	class TypeQualification
	{
	public:

		TypeQualification();		
		TypeQualification( unsigned number_of_indirection_levels,
			const reflective::Type & final_type, unsigned constness_word = 0 );

		const reflective::Type * final_type() const;

		unsigned indirection_levels() const;
		bool is_const( unsigned indirection_level ) const;

		const void * indirection( const void * object ) const;
		const void * indirection( const void * object, const Type * * out_resulting_type ) const;

		const void * full_indirection( const void * object ) const;
		const void * full_indirection( const void * object, const Type * * out_resulting_type ) const;

		bool is_pointer() const;

		bool is_reference() const;

		static const unsigned MAX_INDIRECTION_LEVELS = 15; 

		// equality
		bool operator == ( const TypeQualification & other ) const;
		bool operator != ( const TypeQualification & other ) const;

		typedef uint32_t IndirectionWordType;

		IndirectionWordType get_indirection_word() const;

		void init( IndirectionWordType indirection_word, const reflective::Type * final_type );

		static unsigned indirection_levels_from_word( IndirectionWordType word );

	private: // data members
		const reflective::Type * _final_type;		
		uint32_t _indirection_word; /* this word encodes the number of indiretion levels 
			and the constness of each of them. */
		static const uint32_t INDIRECTION_LEVELS_MASK = (1 << 4) - 1;
		static const uint32_t INDIRECTION_CONSTNESS_OFFSET = 4;
	};

} // namespace reflective

namespace reflective_externals
{
	// reflection of reflective::TypeQualification
	reflective::Class * init_type(
		reflective::TypeQualification * null_pointer_1,
		reflective::TypeQualification * null_pointer_2 );

} // namespace reflective_externals
