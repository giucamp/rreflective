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


#include "Shapes.h"

namespace Shapes
{
	// Shape::constructor
	Shape::Shape()
		: _color( eColorWhite )
	{
	}

	// Shape::get_color
	Color Shape::get_color() const
	{
		return _color;
	}

	// Shape::set_color
	void Shape::set_color( Color color )
	{
		_color = color;
	}

	// Circle::default constructor
	Circle::Circle()
		: _radius( 0.f )
	{
	}

	// Circle::constructor
	Circle::Circle( Vector center, float radius )
		: _center( center ), _radius( radius )
	{
	}

	// Circle::get_center
	const Vector & Circle::get_center() const
	{
		return _center;
	}

	// Circle::set_center
	void Circle::set_center( const Vector & new_center )
	{
		_center = new_center;
	}

	// Circle::get_radius
	float Circle::get_radius() const
	{
		return _radius;
	}

	// Circle::set_radius
	void Circle::set_radius( float new_radius )
	{
		_radius = new_radius;
	}

	// Circle::perimeter
	float Circle::perimeter() const
	{
		return _radius * 6.28f;
	}

	// Circle::area
	float Circle::area() const
	{
		return _radius * _radius * 3.14f;
	}

	// Circle::move_by
	void Circle::move_by( const Vector & offset )
	{
		_center.x += offset.x;
		_center.y += offset.y;
	}

	// Circle::scale_radius
	void Circle::scale_radius( float factor )
	{
		_radius *= factor;
	}
}
