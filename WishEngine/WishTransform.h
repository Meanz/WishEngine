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
	class wish_transform
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
		wish_transform* Child;  //32
		wish_transform* Parent; //32
		wish_transform* Prev;	//32
		wish_transform* Next;	//PTR, 32?


		wish_transform();
		__Wish_Export wish_transform(const char* name);
		__Wish_Export ~wish_transform();

		void SetName(const char* name);

		__Wish_Export b32 HasChild(wish_transform* child);
		__Wish_Export wish_transform* GetLastChild();

		__Wish_Export void Attach(wish_transform* other);
		__Wish_Export void Detach(wish_transform* other);

		__Wish_Export mat4& CalculateLocal();
		__Wish_Export mat4& CalculateGlobal(mat4& parent);

		__Wish_Export void LookAt(v3 eye, v3 center, v3 up);
	};

}