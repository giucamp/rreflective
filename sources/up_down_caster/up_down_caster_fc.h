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
	// UpDownCaster<UpDownCasterFullCompliance>
	template <>
		class UpDownCaster<UpDownCasterFullCompliance>
	{
	public:

		// constructor
		UpDownCaster();

		// from_types< BASE_CLASS, THIS_CLASS >
		template < class BASE_CLASS, class THIS_CLASS >
			static UpDownCaster from_types();

		// from_types_only_to_base< BASE_CLASS, THIS_CLASS >
		template < class BASE_CLASS, class THIS_CLASS >
			static UpDownCaster from_types_only_to_base();

		// pointer_to_base_type \ pointer_from_base_type
		void * pointer_to_base_type( void * pointer ) const;
		void * pointer_from_base_type( void * pointer ) const;
		const void * pointer_to_base_type( const void * pointer ) const;
		const void * pointer_from_base_type( const void * pointer ) const;

	private: // internal services

		typedef void * (*ToBaseCaster)( void * pointer );
		typedef void * (*FromBaseCaster)( void * pointer );
		typedef const void * (*ToConstBaseCaster)( const void * pointer );
		typedef const void * (*FromConstBaseCaster)( const void * pointer );

		UpDownCaster(
			ToBaseCaster to_base_caster,
			FromBaseCaster from_base_caster,
			ToConstBaseCaster to_const_base_caster,
			FromConstBaseCaster from_const_base_caster );

		template < class BASE_CLASS, class THIS_CLASS >
			static void * _typed_to_base_caster( void * pointer );
		template < class BASE_CLASS, class THIS_CLASS >
			static void * _typed_from_base_caster( void * pointer );
		template < class BASE_CLASS, class THIS_CLASS >
			static const void * _typed_to_const_base_caster( const void * pointer );
		template < class BASE_CLASS, class THIS_CLASS >
			static const void * _typed_from_const_base_caster( const void * pointer );

	private: // data members
		ToBaseCaster _to_base_caster;
		FromBaseCaster _from_base_caster;
		ToConstBaseCaster _to_const_base_caster;
		FromConstBaseCaster _from_const_base_caster;
	};

} // namespace reflective
