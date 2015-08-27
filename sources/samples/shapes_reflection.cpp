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

#include "sample_common.h"
#include "Shapes.h"

namespace reflective_externals
{
	// reflection of Shapes::Shape
	reflective::Class * init_type(
		Shapes::Shape * null_pointer_1,
		Shapes::Shape * null_pointer_2 )
	{
		using namespace ::reflective;
		typedef Shapes::Shape ThisClass;
		
		// class object
		Class * class_object = new_class<ThisClass>();
		class_object->set_no_base_type();
		
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "color", &ThisClass::get_color, &ThisClass::set_color ),
			new_property<ThisClass>( "perimeter", &ThisClass::perimeter ),
			new_property<ThisClass>( "area", &ThisClass::area ),
		};
		
		// actions
		const Action * actions[] = 
		{
			new_action<ThisClass>( "move_by", &ThisClass::move_by, "offset"),
		};
		
		// assign members
		class_object->assign_properties( properties );
		class_object->assign_actions( actions );
		
		// return type
		return class_object;
	}
	
	// reflection of Shapes::Circle
	reflective::Class * init_type(
		Shapes::Circle * null_pointer_1,
		Shapes::Circle * null_pointer_2 )
	{
		using namespace ::reflective;
		typedef Shapes::Circle ThisClass;
		typedef Shapes::Shape BaseClass;
	
		// class object
		Class * class_object = new_class<ThisClass>();
		class_object->set_base_type( BaseType::from_types<ThisClass,BaseClass>() );
		
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "center", &ThisClass::get_center, &ThisClass::set_center ),
			new_property<ThisClass>( "radius", &ThisClass::get_radius, &ThisClass::set_radius ),
			new_property<ThisClass>( "perimeter", &ThisClass::perimeter ),
			new_property<ThisClass>( "area", &ThisClass::area ),
		};
		
		// actions
		const Action * actions[] = 
		{
			new_action<ThisClass>( "move_by", &ThisClass::move_by, "offset"),
			new_action<ThisClass>( "scale_radius", &ThisClass::scale_radius, "factor"),
		};
		
		// assign members
		class_object->assign_properties( properties );
		class_object->assign_actions( actions );
		
		// return type
		return class_object;
	}

	// reflection of Shapes::Color
	reflective::Enum * init_type(
		Shapes::Color * null_pointer_1,
		Shapes::Color * null_pointer_2 )
	{
		using namespace ::reflective;	
	
		const Enum::Member * members[] = 
		{
			new_enum_member( "White", Shapes::eColorWhite ),
			new_enum_member( "Red", Shapes::eColorRed ),
			new_enum_member( "Yellow", Shapes::eColorYellow ),
			new_enum_member( "Magenta", Shapes::eColorMagenta ),
		};
		
		return new_enum( "Shapes", "Color", members );
	}
	
} // namespace reflective_externals
