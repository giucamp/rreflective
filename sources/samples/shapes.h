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

namespace Shapes
{
	struct Vector
	{	
		float x, y;

		Vector() : x( 0.f ), y( 0.f ) { }
		Vector( float x_, float y_ ) : x( x_ ), y( y_ ) { }
	};

	enum Color
	{
		eColorWhite,
		eColorRed,
		eColorYellow,
		eColorMagenta
	};

	// Shape
	class Shape
	{
	protected:
		Shape();

	public:

		Color get_color() const;
		void set_color( Color color );

		virtual float perimeter() const = 0;
		virtual float area() const = 0;
		virtual void move_by( const Vector & offset ) = 0;

		

	private: // data members
		Color _color;
	};

	// Circle
	class Circle : public Shape
	{
	public:
		Circle();
		Circle( Vector center, float radius );

		// center
		const Vector & get_center() const;
		void set_center( const Vector & new_center );

		// radius
		float get_radius() const;
		void set_radius( float new_radius );

		// perimeter, area
		float perimeter() const;
		float area() const;

		void move_by( const Vector & offset );
		void scale_radius( float factor );

	private: // data members
		Vector _center;
		float _radius;
	};

}

namespace reflective_externals
{
	// reflection of Shapes::Shape
	reflective::Class * init_type(
		Shapes::Shape * null_pointer_1,
		Shapes::Shape * null_pointer_2 );
	
	// reflection of Shapes::Circle
	reflective::Class * init_type(
		Shapes::Circle * null_pointer_1,
		Shapes::Circle * null_pointer_2 );
	
	// reflection of Shapes::Color
	reflective::Enum * init_type(
		Shapes::Color * null_pointer_1,
		Shapes::Color * null_pointer_2 );
		
} // namespace reflective_externals

