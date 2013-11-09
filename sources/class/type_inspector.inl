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
	// operator | ( InspectionFlags )
	inline TypeInspector::InspectionFlags operator | ( TypeInspector::InspectionFlags op1,
		TypeInspector::InspectionFlags op2 )
	{
		const int int_op1 = op1;
		const int int_op2 = op2;
		return static_cast< TypeInspector::InspectionFlags >( int_op1 | int_op2 );
	}

	// TypeInspector::destructor
	inline TypeInspector::~TypeInspector()
	{
		uninspect();
	}
	
	// TypeInspector::get_property
	inline const TypeInspector::PropertyAndValue & TypeInspector::get_property( size_t index )
	{
		REFLECTIVE_ASSERT( index < _property_count );
		return _properties[ index ];
	}

	// TypeInspector::get_property_count
	inline size_t TypeInspector::get_property_count() const
	{
		return _property_count;
	}

	// TypeInspector::get_classes
	inline const TypeInspector::ClassAndObject & TypeInspector::get_classes( size_t index )
	{
		REFLECTIVE_ASSERT( index < _classes_count );
		return _clasess_and_objects[ index ];
	}

	// TypeInspector::get_classes_count
	inline size_t TypeInspector::get_classes_count() const
	{
		return _classes_count;
	}

} // namespace reflective

