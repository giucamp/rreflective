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
	namespace details
	{
		class DerivedTypesList // this class is not supposed to be referenced outside the library
		{
		public:

			DerivedTypesList();

			void add(Type & i_type);
			void remove(Type & i_type);

			class ConstIterator; // defined later

			ConstIterator begin() const;
			ConstIterator end() const;

			ConstIterator cbegin() const;
			ConstIterator cend() const;

		private:
			Type * m_first;
		};
	}
	
	class Type : public NamespaceMember
	{
	public:

		static const size_t s_max_size = (1 << 24) - 1;
		static const size_t s_max_alignment = (1 << 8) - 11;

		/** Constructs a type, assigning name, size and alignment (which are all immutable). 
			@param i_name name of the type
			@param i_name i_size of the type
		*/
		Type(SymbolName i_name, size_t i_size, size_t i_alignment );

		virtual ~Type() {}

		/** Returns the size of the type in bytes. Usually the size*/
		size_t size() const							{ return m_size; }

		size_t alignment() const					{ return m_alignment; }
								
		void full_name_to_string(OutBufferTextStream & i_dest) const;
		
					// flags
		
		void set_implicit_reflection(bool i_value)
		{
			m_implicit_reflection = i_value;
		}

		bool is_implicit_reflection() const
		{
			return m_implicit_reflection;
		}

					// special_functions

		const SpecialFunctions & special_functions() const { return m_special_functions; }

		void set_special_functions(const SpecialFunctions & i_special_functions) { m_special_functions = i_special_functions; }


					//

		using MostDerivedFunc = const Type & (*)(const void * i_object);

		void set_most_derived_type_func(MostDerivedFunc i_function)			{ m_most_derived_func = i_function; }

		MostDerivedFunc most_derived_type_func() const						{ return m_most_derived_func; }


					// inheritance	

		void set_base_types(const ArrayView<const BaseType> & i_base_types);

		const details::DerivedTypesList & derived_types() const { return m_derived_types; }

		bool can_upcast_to(const Type & i_base_type) const;

		void * upcast(void * i_object, const Type & i_dest_base_type) const;
		
		const Type * most_derived(const void * i_object) const;

		void * try_dynamic_cast(void * i_source_object, const Type & i_dest_type) const;

		class BaseTypeContainer;

		/** Returns a container of all the base types of this type.*/
		const BaseTypeContainer base_types() const;
		

					// unit testing

		#if REFLECTIVE_ENABLE_TESTING
			
			/** Runs an unit test for this class */
			static void unit_test();

		#endif

	private:
		
		void add_derived( Type & i_derived_type );
		void remove_derived( Type & i_derived_type );

		#if REFLECTIVE_ENABLE_MULTIPLE_INHERITANCE	
			size_t base_type_multeplicity(const Type & i_base_type) const;
		#endif

		static bool internal_find_path_to_type(const Type & i_source_type, const Type & i_target_type, std::vector<BaseType> & io_base_types);

	private:

		SpecialFunctions m_special_functions;
		const size_t m_size;
		const size_t m_alignment;

		bool m_implicit_reflection; // temp

		// inheritance data
		BaseType m_single_base;
		#if REFLECTIVE_ENABLE_MULTIPLE_INHERITANCE
			std::vector<BaseType> m_other_base_types;
		#endif
		MostDerivedFunc m_most_derived_func;
		Type * m_next_derived;
		details::DerivedTypesList m_derived_types;

		// misc
		StringFunctions m_string_functions;

		friend class details::DerivedTypesList;
	};
}
