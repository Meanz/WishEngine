#pragma once

namespace Wish
{

	__Wish_Export wish_mesh* Wish_Primitive_Quad(i32 xMin, i32 xMax, i32 yMin, i32 yMax, i32 u, i32 v);
	__Wish_Export wish_mesh* Wish_Primitive_Plane(i32 width, i32 length);
	__Wish_Export wish_mesh* Wish_Primitive_Plane(i32 width, i32 length, i32 u, i32 v);
	__Wish_Export wish_mesh* Wish_Primitive_Cube(i32 width, i32 height, i32 length);
	__Wish_Export wish_mesh* Wish_Primitive_Cylinder(i32 radius, i32 height, u32 resolution);
	__Wish_Export wish_mesh* Wish_Primitive_Sphere(i32 radius, u32 resolution);

}