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

	struct wish_transform
	{
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
	};

	__Wish_Export void Wish_Transform_Init(wish_transform* transform);

	__Wish_Export void Wish_Transform_SetName(wish_transform* transform, const char* name);

	__Wish_Export void Wish_Transform_Attach(wish_transform* transform, wish_transform* child);
	__Wish_Export void Wish_Transform_Detatch(wish_transform* transform, wish_transform* child);

	__Wish_Export mat4* Wish_Transform_CalculateLocal(wish_transform* transform);
	__Wish_Export mat4* Wish_Transform_CalculateGlobal(wish_transform* transform, mat4& parent);

	__Wish_Export void Wish_Transform_LookAt(wish_transform* transform, vec3 eye, vec3 center, vec3 up);

}