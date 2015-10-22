#pragma once

namespace Wish
{

#define WISH_TRANSFORM_NAME_SIZE 50

	struct wish_aabb
	{
		v3 Min;
		v3 Max;
	};

	b32 Wish_Physics_AABB_AABB(wish_aabb* a1, wish_aabb* a2);

	enum wish_transform_type
	{
		WISH_TRANSFORM_TYPE_TRANSFORM,
		WISH_TRANSFORM_TYPE_GEOMETRY,
		WISH_TRANSFORM_TYPE_GAME_OBJECT,
		WISH_TRANSFORM_TYPE_LIGHT,
	};

	/* Example C++ Impl */
	class WishTransform
	{
	public:
		char Name[WISH_TRANSFORM_NAME_SIZE];
		vec3 Position;
		quat Rotation;
		vec3 Scale;

		mat4 Local;
		mat4 Global;

		wish_transform_type TransformType;

		//Circular Hierarchy
		WishTransform* Child;  //32
		WishTransform* Parent; //32
		WishTransform* Prev;	//32
		WishTransform* Next;	//PTR, 32?


		WishTransform();
		WishTransform(const char* name);
		__Wish_Export ~WishTransform();

		void SetName(const char* name);

		__Wish_Export b32 HasChild(WishTransform* child);
		__Wish_Export WishTransform* GetLastChild();

		__Wish_Export void Attach(WishTransform* other);
		__Wish_Export void Detach(WishTransform* other);

		__Wish_Export mat4& CalculateLocal();
		__Wish_Export mat4& CalculateGlobal(mat4& parent);

		__Wish_Export void LookAt(v3 eye, v3 center, v3 up);
	};

}