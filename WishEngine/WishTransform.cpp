#include "stdafx.h"
#include "WishCore.h"
#include "WishTransform.hpp"


b32 Wish::Wish_Physics_AABB_AABB(wish_aabb* a1, wish_aabb* a2) {
	return
		a1->Min.x < a2->Max.x &&
		a1->Max.x > a2->Min.x &&
		a1->Min.y < a2->Max.y &&
		a1->Max.y > a2->Min.y &&
		a1->Min.z < a2->Max.z &&
		a1->Max.z > a2->Min.z;
}

void Wish::Wish_Transform_Init(wish_transform* transform)
{
	//ZeroMemory(transform, sizeof(wish_transform));
	transform->Scale = vec3(1.0f);
	transform->Rotation = quat(1.0f, 0.0f, 0.0f, 0.0f);
	transform->Local = mat4(1.0f);
	transform->Global = mat4(1.0f);
	transform->Child = NULL;
	transform->Next = NULL;
	transform->Parent = NULL;
	Wish_Transform_SetName(transform, "Null");
}

void Wish::Wish_Transform_SetName(wish_transform* transform, const char* name)
{
	if (strlen(name) < WISH_TRANSFORM_NAME_SIZE) {
		strcpy_s(transform->Name, name);
	}
}

wish_transform* GetLastChild(wish_transform* transform)
{
	if (transform->Child)
	{
		wish_transform* it = transform->Child;
		while (it->Next != NULL)
		{
			it = it->Next;
		}
		return it;
	}
	return NULL;
}

void Wish::Wish_Transform_Attach(wish_transform* transform, wish_transform* child) {

	//Attach us to the last child

	if (!transform->Child)
	{
		//We are inserting the first transform
		child->Parent = transform;
		child->Prev = NULL;
		child->Next = NULL;
		transform->Child = child;
	}
	else
	{
		wish_transform* lastChild = GetLastChild(transform);
		if (lastChild) {
			lastChild->Next = child;
			child->Parent = lastChild->Parent;
			child->Next = NULL;
			child->Prev = lastChild;
		}
		else {
			//Something weeeeird is happening
			throw runtime_error("Could not retrieve last child, broken hierarchy?");
		}
	}
}


//Note, this is not recursive, it doesn't check sub nodes
bool Wish_Transform_HasChild(wish_transform* transform, wish_transform* child)
{
	bool result = false;
	wish_transform* firstChild = transform->Child;
	if (firstChild)
	{
		while (firstChild != transform)
		{
			if (firstChild == child) {
				result = true;
				break;
			}
			firstChild = firstChild->Next;
		}
	}
	return result;
}

void Wish::Wish_Transform_Detatch(wish_transform* transform, wish_transform* child) {
	size_t nodePos = std::string::npos; //unsigned thing bad for bisniz

	//
	// 
	bool result = false;

	if (Wish_Transform_HasChild(transform, child))
	{
		if (child->Next != transform)
		{
			child->Next->Prev = child->Prev;
		}
		if (child->Prev != transform)
		{
			child->Prev->Next = child->Next;
		}
		child->Prev = child;
		child->Next = child;
		if (transform->Child == child)
		{
			transform->Child = NULL;
		}
	}
}

mat4* Wish::Wish_Transform_CalculateLocal(wish_transform* transform) {
	transform->Local = mat4_cast(transform->Rotation) * glm::translate(mat4(1.0f), transform->Position) * glm::scale(mat4(1.0f), transform->Scale);
	return &transform->Local;
}

mat4* Wish::Wish_Transform_CalculateGlobal(wish_transform* transform, mat4& parent) {
	Wish_Transform_CalculateLocal(transform);
	transform->Global = parent * transform->Local;
	return &transform->Global;
}

void Wish::Wish_Transform_LookAt(wish_transform* transform, vec3 eye, vec3 center, vec3 up)
{
	mat4 lookat_matrix = glm::lookAt(eye, center, up);
	transform->Rotation = quat(lookat_matrix);
	transform->Position = -eye;
}

/*
void Transform::Rotate(float angle, vec3 axis) {
quat angAx = glm::angleAxis(glm::radians(angle), axis);
m_Rotation = m_Rotation * angAx;
}

void Transform::Translate(vec3 other) {
m_Position += other;
}

void Transform::Translate(float x, float y, float z) {
m_Position.x += x;
m_Position.y += y;
m_Position.z += z;
}

void Transform::LookAt(vec3 eye, vec3 center) {
glm::mat4 lookat_matrix = glm::lookAt(eye, center, vec3(0.0f, 1.0f, 0.0f));
m_Rotation = quat(lookat_matrix);
m_Position = -eye;
}*/