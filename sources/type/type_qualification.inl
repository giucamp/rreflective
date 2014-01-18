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
	// CompileTimeTypeQualification<TYPE&> - reference
	template <class TYPE> class CompileTimeTypeQualification<TYPE&>
	{
	public:
		typedef typename CompileTimeTypeQualification<TYPE>::FinalType FinalType;
		static const unsigned indirection_levels = CompileTimeTypeQualification<TYPE>::indirection_levels + 1;
		static const unsigned constness_word = 1 | (CompileTimeTypeQualification<TYPE>::constness_word << 1);
		static const unsigned is_reference = 1;
	};

	// CompileTimeTypeQualification<TYPE&> - pointer
	template <class TYPE> class CompileTimeTypeQualification<TYPE*>
	{
	public:
		typedef typename CompileTimeTypeQualification<TYPE>::FinalType FinalType;
		static const unsigned indirection_levels = CompileTimeTypeQualification<TYPE>::indirection_levels + 1;
		static const unsigned constness_word = CompileTimeTypeQualification<TYPE>::constness_word << 1;
		static const unsigned is_reference = CompileTimeTypeQualification<TYPE>::is_reference;
	};

	// CompileTimeTypeQualification<TYPE&> - const
	template <class TYPE> class CompileTimeTypeQualification<const TYPE>
	{
	public:
		typedef typename CompileTimeTypeQualification<TYPE>::FinalType FinalType;
		static const unsigned indirection_levels = CompileTimeTypeQualification<TYPE>::indirection_levels;
		static const unsigned constness_word = CompileTimeTypeQualification<TYPE>::constness_word | 1;
		static const unsigned is_reference = CompileTimeTypeQualification<TYPE>::is_reference;
	};

	// TypeQualification::default constructor
	inline TypeQualification::TypeQualification()
			  : _final_type( nullptr ), _indirection_word( 0 )
	{
	}

	// TypeQualification::constructor
	inline TypeQualification::TypeQualification( unsigned number_of_indirection_levels,
		const reflective::Type & final_type, unsigned constness_word )
			  : _final_type( &final_type ),
			    _indirection_word( number_of_indirection_levels )
	{
		REFLECTIVE_ASSERT( number_of_indirection_levels <= MAX_INDIRECTION_LEVELS );

		const unsigned shifted_constness_word = constness_word << INDIRECTION_CONSTNESS_OFFSET;
		REFLECTIVE_ASSERT( (shifted_constness_word >> INDIRECTION_CONSTNESS_OFFSET) 
			== constness_word ); /* If this fails some of the constness word is lost. The number
				of indirection levels is limited by MAX_INDIRECTION_LEVELS. */
		_indirection_word |= shifted_constness_word;

		REFLECTIVE_ASSERT( indirection_levels() == number_of_indirection_levels );
	}


	// TypeQualification::final_type
	inline const reflective::Type * TypeQualification::final_type() const
	{
		return _final_type;
	}

	// TypeQualification::indirection_levels
	inline unsigned TypeQualification::indirection_levels() const
	{
		return unsigned( _indirection_word & INDIRECTION_LEVELS_MASK );
	}

	// TypeQualification::indirection_levels_from_word
	inline unsigned TypeQualification::indirection_levels_from_word( IndirectionWordType word )
	{
		return unsigned( word & INDIRECTION_LEVELS_MASK );
	}

	// TypeQualification::full_indirection
	inline const void * TypeQualification::full_indirection( const void * object ) const
	{
		unsigned levels = indirection_levels();
		while( levels )
		{
			if( !object )
				return nullptr;
			object = *reinterpret_cast<const void * const *>( object );
			levels--;
		}
		return object;
	}

	// TypeQualification::full_indirection
	inline const void * TypeQualification::full_indirection( const void * object,
		const Type * * out_resulting_type ) const
	{
		*out_resulting_type = &pointer_type();
		unsigned levels = indirection_levels();
		if( levels ) for(;;) {
			if( !object )
				return nullptr;
			object = *reinterpret_cast<const void * const *>( object );
			levels--;
			if( !levels )
				break;
		}
		*out_resulting_type = _final_type;
		return object;
	}

	// TypeQualification::indirection
	inline const void * TypeQualification::indirection( const void * object, 
		const Type * * out_resulting_type ) const
	{
		*out_resulting_type = nullptr;

		const unsigned levels = indirection_levels();

		if( !object )
			return nullptr;

		if( levels > 0 )
		{
			if( levels > 1 )
				*out_resulting_type = &reflective_externals::pointer_type();
			else
				*out_resulting_type = _final_type;
			return *reinterpret_cast<const void * const *>( object );
		}

		return object;
	}

	// TypeQualification::indirection
	inline const void * TypeQualification::indirection( const void * object ) const
	{
		const unsigned levels = indirection_levels();

		if( !object )
			return nullptr;

		if( levels > 0 )
		{
			return *reinterpret_cast<const void * const *>( object );
		}

		return object;
	}

	// TypeQualification::is_const
	inline bool TypeQualification::is_const( unsigned indirection_level ) const
	{
		REFLECTIVE_ASSERT( indirection_level <= indirection_levels() );
		const uint32_t bit = 1 << ( indirection_level + INDIRECTION_CONSTNESS_OFFSET );
		return ( _indirection_word & bit ) != 0;
	}

	// TypeQualification::is_pointer
	inline bool TypeQualification::is_pointer() const
	{
		const unsigned levels = indirection_levels();
		return levels >= 1;
	}

	// TypeQualification::is_reference
	inline bool TypeQualification::is_reference() const
	{
		const unsigned levels = indirection_levels();
		return levels >= 1 && is_const( levels );
	}

	// TypeQualification::operator ==
	inline bool TypeQualification::operator == ( const TypeQualification & other ) const
	{
		return _indirection_word == other._indirection_word &&
			_final_type == other._final_type;
	}

	// TypeQualification::operator !=
	inline bool TypeQualification::operator != ( const TypeQualification & other ) const
	{
		return _indirection_word != other._indirection_word ||
			_final_type != other._final_type;
	}

	// TypeQualification::get_indirection_word
	inline TypeQualification::IndirectionWordType TypeQualification::get_indirection_word() const
	{
		return _indirection_word;
	}

	// TypeQualification::init
	inline void TypeQualification::init( TypeQualification::IndirectionWordType indirection_word,
		const reflective::Type * final_type )
	{
		_indirection_word = indirection_word;
		_final_type = final_type;
	}

} // namespace reflective

