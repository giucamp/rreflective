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

#ifndef INCLUDING_REFLECTIVE
	#error "cant't include this header directly, include reflective.h instead"
#endif

namespace reflective
{
	/** Retrieves (by value) a QualifiedTypePtr associated to the template argument.
		The resut  is never empty (is_empty() always return false). The template 
		argument may be void or any void pointer (with any cv-qualification). */
	template <typename TYPE>
		QualifiedTypePtr get_type();

	/** Scoped enum that stores a combination of cv-qualifiers. CV_Flags can be combined and subtracted with the overloaded bitwise operators | and &. */
	enum class CV_Flags
	{
		None = 0, /**< No flags */
		Const = 1 << 0, /**< Set for const types */
		Volatile = 1 << 1, /**< Set for volatile types */
	};

	inline CV_Flags operator | (CV_Flags i_first, CV_Flags i_seconds)
	{
		using underlying_type = std::underlying_type < CV_Flags >::type;
		return static_cast<CV_Flags>(static_cast<underlying_type>(i_first) | static_cast<underlying_type>(i_seconds));
	}

	inline CV_Flags operator & (CV_Flags i_first, CV_Flags i_seconds)
	{
		using underlying_type = std::underlying_type < CV_Flags >::type;
		return static_cast<CV_Flags>(static_cast<underlying_type>(i_first) & static_cast<underlying_type>(i_seconds));
	}


	/** Lightweight value-class holding a pointer to a reflective::Type, a number of indirection levels, and the cv-qualification
		(is it \c const? is it \c volatile?) for each indirection level. A QualifiedTypePtr can tell:
			- The **number of indirection levels**, that is is the nuber of '*' or '&' or '&&' appearing in the C++ declaration of
			   the type. A non-pointer types has zero indirection levels, while a pointer to a pointer has 2 indirection levels.
			   References are considered like const pointer (that is \c get_type<float&>() == get_type<float*const>() ).
			- The **primary type**, that is is the type of the first indirection level. For non-pointer types it is the same of the
			   final type. For pointer types is always equal to the result of \c get_naked_type<Pointer>(). If an object of has to be 
			   constructed, copied, or assigned, the primary type is what matters.
			- The **final type**, that is the type of the last indirection level. The final type is the type remaining after stripping away 
			  all the cv-qualification, pointer and reference parts from the C++ declaration. The final type can be thought as the type of 
			  the final object, that is the object found indirecting all the indirection levels.
			- **cv-quaification** for every insirection level, that is for every i >= 0 and <= indirection_levels.

		|type						|primary type	|final type		|# of indirection levels    |const levels		|volatile levels	 |
		|---------------------------|:-------------:|:-------------:|:-------------------------:|:-----------------:|:------------------:|
		|float						|float			|float			|0						    |					|					 |
		|volatile float				|float			|float			|0						    |					|0					 |
		|const float &				|void *			|float			|1						    |0, 1				|					 |
		|const void *				|void *			|void			|1						    |1					|					 |
		|void* const				|void *			|void			|1						    |0					|					 |
		|float*const*volatile**&	|void *			|float			|5						    |0, 4				|3					 |

		QualifiedTypePtr is copyable, assignable and movable.
		Use \c get_type<TYPE>() to get a \c QualifiedTypePtr from a compile-time type. 
		Note: <tt> <const int *> </tt> and <tt> <int const *> </tt> are the same C++ type. <br>
		
		Implementation note: currently QualifiedTypePtr has the same size of 2 pointers. Anyway, the user should not rely on this assumption. */
	class QualifiedTypePtr final
	{
	public:

					// constants

		/** Maximum indirection levels that this class can handle. This is 14 if \c uintptr_t is 32-bit wide or smaller, 28 otherwise.
			The global function \c get_type<TYPE>() checks this imit at compile-time (with a \c static_assert). */
		static const size_t s_max_indirection_levels = std::numeric_limits<uintptr_t>::digits <= 32 ? 14 : 28;
		
		// uintptr_t must be binary
		static_assert(std::numeric_limits<uintptr_t>::radix == 2, "uintptr_t is expected to be binary");


					// getters

		/** Returns the number of indirection levels of the type, that is is the number of '*' or '&' or '&&' appearing in the C++ 
			declaration. A non-pointer type has zero indirection levels, while a pointer to a pointer has 2 indirection levels.*/
		size_t indirection_levels() const			{ return m_indirection_levels; }

		/** Retrieves the primary type, that is the type at the 0-th indirection level. 
			If the type is empty (= default constructed) the primary type is nullptr. Otherwise is != \c nullptr. */
		const Type * primary_type() const;

		/** Retrieves the final type, that is the type at the last indirection level.
			If the type is empty (= default constructed) the final type is nullptr. Otherwise is != \c nullptr. */
		const Type * final_type() const				{ return m_final_type; }

		/** Retrieves whether a given indirection level has a const qualifier.
			@param i_indirection_level indirection level (must be <= \c indirection_levels()) for which the consteness is 
				queried. In the type: <tt>float const**</tt>, <tt>is_const(0)</tt> and <tt>is_const(1)</tt> return \c false,
				while <tt>is_const(2)</tt> returns \c true. */
		bool is_const(size_t i_indirection_level) const;

		/** Retrieves whether a given indirection level has a volatile qualifier.
			@param i_indirection_level indirection level (must be <= \c indirection_levels()) for which the volatileness is 
				queried. In the type: <tt>float volatile**</tt>, <tt>is_volatile(0)</tt> and <tt>is_volatile(1)</tt> return
				\c false, while \c <tt>is_volatile(2)</tt> returns \c true. */
		bool is_volatile(size_t i_indirection_level) const;

		/** Returns whether is empty (that is default constructed). */
		bool is_empty() const						{ return final_type() == nullptr; }

		/** Retrieves whether the type rapresent a pointer, that is indirection_levels() > 0 */
		bool is_pointer() const						{ return indirection_levels() > 0; }


				// derived getters

		/** Retrieves a \c CV_Flags that specifies the cv-qualification for the specified indirection level.
			Given the type <tt>get_type<float volatile*const volatile*const*>()</tt>:
				- \c cv_flags(0) returns <tt> CV_Flags::None </tt>
				- \c cv_flags(1) returns <tt> CV_Flags::Const </tt>
				- \c cv_flags(2) returns <tt> CV_Flags::Const | CV_Flags::Volatile </tt>
				- \c cv_flags(3) returns <tt> CV_Flags::Volatile </tt>
			
			Implementation note: \c cv_flags() is impemented using \c is_const() and \c is_volatile().
			@param i_indirection_level indirection level for which the qualification is queried. It must be <= \c indirection_levels() */
		CV_Flags cv_flags(size_t i_indirection_level) const
			{ return (is_const(i_indirection_level) ? CV_Flags::Const : CV_Flags::None) |
				(is_volatile(i_indirection_level) ? CV_Flags::Volatile : CV_Flags::None); }


				// special functions
	
		/** Constructs an empty QualifiedTypePtr (is_empty() will return true). The object may be later the destination of an assignment, changing its state. */
		QualifiedTypePtr();

		/** Constructs a non-empty QualifiedTypePtr from a final type and an array of CV_Flags's that specifies the cv-qualifiers of the indirection levels.
			The size of the array of CV_Flags's determines the number of indirection levels.
			In the the following code <tt>q_type_ptr_1 == q_type_ptr_2</tt>:<br>
			<tt>QualifiedTypePtr q_type_ptr_1(get_naked_type<void>(), { CV_Flags::Const | CV_Flags::Volatile, CV_Flags::None, CV_Flags::Volatile });<br>
			QualifiedTypePtr q_type_ptr_2 = get_type<void volatile * * volatile const >();</tt><br>
			@param i_final_type final type. May be get_naked_type<void>().
			@param i_cv_flags cv-qualification for each indirection level. The n-th element of this array specifies a combination of cv flags for the n-th indirection 
				level. The number of indirection levels of the type is the size of this array, minus 1. So, to construct a pointer to a pointer, specify an array
				of 3 elements. If the array is empty, the number of indirection levels is zero. */
		QualifiedTypePtr(const Type & i_final_type, ArrayView<const CV_Flags> i_cv_flags);

		/** Copies from the source QualifiedTypePtr */
		QualifiedTypePtr(const QualifiedTypePtr & i_source) = default;

		/** Assigns from the source QualifiedTypePtr */
		QualifiedTypePtr & operator = (const QualifiedTypePtr & i_source) = default;
		

				// comparison

		/** Returns true whether two QualifiedTypePtrs are indistinguishable */
		bool operator == (const QualifiedTypePtr & i_source) const;

		/** Returns false whether two QualifiedTypePtrs are indistinguishable */
		bool operator != (const QualifiedTypePtr & i_source) const		{ return !operator == (i_source); }
		
	private:
		
		QualifiedTypePtr(const Type * i_final_type, size_t i_indirection_levels, size_t i_constness_word, size_t i_volatileness_word);
		
		template <typename TYPE>
			friend QualifiedTypePtr get_type();

		template <typename UNDERLYING_STREAM>
			friend InTxtStreamAdapt<UNDERLYING_STREAM, char> & operator >> (InTxtStreamAdapt<UNDERLYING_STREAM, char> & i_source, QualifiedTypePtr & o_dest_qualified_type);
			
	private: // data members (currently a QualifiedTypePtr is big as two pointers)
		const Type * m_final_type;
		uintptr_t m_indirection_levels : (std::numeric_limits<uintptr_t>::digits - s_max_indirection_levels * 2);
		uintptr_t m_constness_word : s_max_indirection_levels;
		uintptr_t m_volatileness_word : s_max_indirection_levels;
	};

	#if REFLECTIVE_ENABLE_TESTING
		void unit_test(QualifiedTypePtr**, CorrectnessTestContext & i_context);
	#endif
	
	template <typename OUT_STREAM>
		OUT_STREAM & operator << (OUT_STREAM & i_dest, const QualifiedTypePtr & i_qt)
	{
		if (!i_qt.is_empty())
		{
			i_dest << i_qt.final_type()->name();

			uintptr_t level = 0;
			const uintptr_t ind_levels = i_qt.indirection_levels();
			do {

				if (i_qt.is_const(level))
				{
					i_dest << " const";
				}

				if (i_qt.is_volatile(level))
				{
					i_dest << " volatile";
				}

				if (level < ind_levels)
				{
					i_dest << " *";
				}

				level++;

			} while (level <= ind_levels);
		}
		return i_dest;
	}

	template <typename UNDERLYING_STREAM>
		InTxtStreamAdapt<UNDERLYING_STREAM, char> & operator >> (InTxtStreamAdapt<UNDERLYING_STREAM, char> & i_source, QualifiedTypePtr & o_dest_qualified_type)
	{
		size_t constness_word = 0, volatileness_word = 0;
		const Type * final_type = nullptr;
		size_t indirection_levels = 0; // this variable is not the index of the current i.l., but the number of i.l.'s so far
		
		for (;;)
		{
			i_source.accept_whitespaces();

			if (i_source.accept_literal("const")) // accept "const"
			{
				constness_word |= 1;
			}
			else if (i_source.accept_literal("volatile")) // accept "volatile"
			{
				volatileness_word |= 1;
			}
			else if (i_source.accept_char('*'))
			{
				constness_word <<= 1;
				volatileness_word <<= 1;
				indirection_levels++;
				if (indirection_levels > QualifiedTypePtr::s_max_indirection_levels)
				{
					break;
				}
			}
			else if (i_source.accept_char('&') || i_source.accept_literal("&&"))
			{
				constness_word <<= 1;
				constness_word |= 1;
				volatileness_word <<= 1;
				indirection_levels++;
				break;
			}
			else if (indirection_levels == 0 && final_type == nullptr) // only in the last indirection level (that is before any *, & or &&
			{
				// accept the final type
				std::string type_name;
				parse_type_name(i_source, type_name);
				final_type = GlobalRegistry::instance().find_type_by_full_name(type_name);
				if (final_type == nullptr)
				{
					i_source.error_stream() << "Unknwon type: '" << type_name << "'" << std::endl;
					break;
				}
			}
			else
			{
				break;
			}
		}

		if (indirection_levels > QualifiedTypePtr::s_max_indirection_levels)
		{
			i_source.error_stream() << "Exceeded the maximum number of indirection levels (" << QualifiedTypePtr::s_max_indirection_levels << ")" << std::endl;
		}
		else if (final_type == nullptr)
		{
			i_source.error_stream() << "Missing final type" << std::endl;
		}
		else
		{
			o_dest_qualified_type = QualifiedTypePtr(final_type, indirection_levels, constness_word, volatileness_word);
		}

		return i_source;
	}
}
