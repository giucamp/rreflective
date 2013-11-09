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
	// UpDownCaster<UpDownCasterOffsetting_Diagnostic>::from_types
	template < class BASE_CLASS, class THIS_CLASS >
		inline UpDownCaster<UpDownCasterOffsetting_Diagnostic>
			UpDownCaster<UpDownCasterOffsetting_Diagnostic>::from_types()
	{
		UpDownCaster<UpDownCasterOffsetting_Diagnostic> result;
		result._unsafe_caster = UpDownCaster<UpDownCasterOffsetting>::from_types< BASE_CLASS, THIS_CLASS >();
		result._safe_caster = UpDownCaster<UpDownCasterFullCompliance>::from_types< BASE_CLASS, THIS_CLASS >();
		return result;
	}

	// UpDownCaster<UpDownCasterOffsetting_Diagnostic>::from_types_only_to_base
	template < class BASE_CLASS, class THIS_CLASS >
		inline UpDownCaster<UpDownCasterOffsetting_Diagnostic>
			UpDownCaster<UpDownCasterOffsetting_Diagnostic>::from_types_only_to_base()
	{
		UpDownCaster<UpDownCasterOffsetting_Diagnostic> result;
		result._unsafe_caster = UpDownCaster<UpDownCasterOffsetting>::from_types_only_to_base< BASE_CLASS, THIS_CLASS >();
		result._safe_caster = UpDownCaster<UpDownCasterFullCompliance>::from_types_only_to_base< BASE_CLASS, THIS_CLASS >();
		return result;
	}

	// UpDownCaster<UpDownCasterOffsetting_Diagnostic>::pointer_to_base_type
	inline void * UpDownCaster<UpDownCasterOffsetting_Diagnostic>::pointer_to_base_type( void * pointer ) const
	{
		void * unsafe_result = _unsafe_caster.pointer_to_base_type( pointer );
		#if REFLECTIVE_ENABLE_ASSERT
			void * safe_result = _safe_caster.pointer_to_base_type( pointer );
			REFLECTIVE_ASSERT( unsafe_result == safe_result );
		#endif
		return unsafe_result;
	}

	// UpDownCaster<UpDownCasterOffsetting_Diagnostic>::pointer_from_base_type
	inline void * UpDownCaster<UpDownCasterOffsetting_Diagnostic>::pointer_from_base_type( void * pointer ) const
	{
		void * unsafe_result = _unsafe_caster.pointer_from_base_type( pointer );
		#if REFLECTIVE_ENABLE_ASSERT
			void * safe_result = _safe_caster.pointer_from_base_type( pointer );
			REFLECTIVE_ASSERT( unsafe_result == safe_result );
		#endif
		return unsafe_result;
	}

	// UpDownCaster<UpDownCasterOffsetting_Diagnostic>::pointer_to_base_type
	inline const void * UpDownCaster<UpDownCasterOffsetting_Diagnostic>::pointer_to_base_type( const void * pointer ) const
	{
		const void * unsafe_result = _unsafe_caster.pointer_to_base_type( pointer );
		#if REFLECTIVE_ENABLE_ASSERT
			const void * safe_result = _safe_caster.pointer_to_base_type( pointer );
			REFLECTIVE_ASSERT( unsafe_result == safe_result );
		#endif
		return unsafe_result;
	}

	// UpDownCaster<UpDownCasterOffsetting_Diagnostic>::pointer_from_base_type
	inline const void * UpDownCaster<UpDownCasterOffsetting_Diagnostic>::pointer_from_base_type( const void * pointer ) const
	{
		const void * unsafe_result = _unsafe_caster.pointer_from_base_type( pointer );
		#if REFLECTIVE_ENABLE_ASSERT
			const void * safe_result = _safe_caster.pointer_from_base_type( pointer );
			REFLECTIVE_ASSERT( unsafe_result == safe_result );
		#endif
		return unsafe_result;
	}

} // namespace reflective
