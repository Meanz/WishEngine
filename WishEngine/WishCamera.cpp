#include "stdafx.h"
#include "WishCore.h"
#include "WishTransform.hpp"
#include "WishCamera.hpp"

void Wish::Wish_Camera_Perspective(wish_camera* camera, float fovY, float aspect, float zNear, float zFar)
{
	camera->Projection = glm::perspective(fovY, aspect, zNear, zFar);
}
void Wish::Wish_Camera_Ortho2D(wish_camera* camera, float left, float right, float top, float bottom, float zNear, float zFar)
{
	camera->Projection = glm::ortho(left, right, bottom, top, zNear, zFar);
}
void Wish::Wish_Camera_CalculateViewMatrix(wish_camera* camera) 
{
	camera->View = *Wish_Transform_CalculateGlobal(&camera->Transform, mat4(1.0f));
}
void Wish::Wish_Camera_ApplyImmediate(wish_camera* camera)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf(value_ptr(camera->Projection));
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf(value_ptr(camera->View));
}

void Wish::Wish_Camera_LookAt(wish_camera* camera, vec3 eye, vec3 center) 
{
	Wish_Transform_LookAt(&camera->Transform, eye, center, vec3(0.0f, 1.0f, 0.0f));
}