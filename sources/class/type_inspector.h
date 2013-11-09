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
	// TypeInspector
	class TypeInspector
	{
	public:
		
		enum InspectionFlags { eFlagsNone = 0, eIncludeNonPublicMembers = 2 };

		TypeInspector( 
			InspectionFlags inspection_flags = eFlagsNone );

		~TypeInspector();

		void inspect( const Type & type, const void * object,
			size_t max_property_count = ~size_t(0) );

		void uninspect();


			/**** properties ****/

		struct PropertyAndValue
		{
			const Property * property;
			void * value;
		};

		const PropertyAndValue & get_property( size_t index );

		size_t get_property_count() const;


			/**** classess ****/

		struct ClassAndObject
		{
			const Class * class_obj;
			const void * object_part;
		};		

		const ClassAndObject & get_classes( size_t index );

		size_t get_classes_count() const;

	private:

		struct Counts
		{
			size_t class_count;
			size_t property_count;
			size_t action_count;
			size_t event_count;
		};

		void _compute_counts( const Class & class_obj, Counts & inout_counts );

		TypeInspector::ClassAndObject * _gather_classes_and_objects(
			const Class & class_obj, const void * object,
			ClassAndObject * class_and_objects );

	private: // data members		
		InspectionFlags _inspection_flags;
		const Type * _type;

		ClassAndObject * _clasess_and_objects;
		size_t _classes_count;

		PropertyAndValue * _properties;
		size_t _property_count;
		void * _property_values_buffer;
	};


	TypeInspector::InspectionFlags operator | ( TypeInspector::InspectionFlags op1,
		TypeInspector::InspectionFlags op2 );

} // namespace reflective

namespace reflective_externals
{
	// reflection of reflective::TypeInspector
	reflective::Class * init_type(
		reflective::TypeInspector * null_pointer_1,
		reflective::TypeInspector * null_pointer_2 );
}
