
namespace Wish
{

	wish_camera::wish_camera() : wish_transform("Camera")
	{

	}

	wish_camera::~wish_camera()
	{

	}

	void wish_camera::Perspective(r32 fovY, r32 aspect, r32 zNear, r32 zFar)
	{
		Projection = glm::perspective(fovY, aspect, zNear, zFar);
	}

	void wish_camera::Ortho(r32 left, r32 right, r32 top, r32 bottom, r32 zNear, r32 zFar)
	{
		Projection = glm::ortho(left, right, bottom, top, zNear, zFar);
	}

	void wish_camera::CalculateViewMatrix()
	{
		View = CalculateGlobal(mat4(1.0f));
	}

	void wish_camera::ApplyImmediate()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glLoadMatrixf(value_ptr(Projection));
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glLoadMatrixf(value_ptr(View));
	}

}