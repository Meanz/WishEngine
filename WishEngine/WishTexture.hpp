#pragma once

namespace Wish
{
	enum MinMagFilter
	{
#if WISH_RENDERER == WISH_OPENGL
		NEAREST = GL_NEAREST,
		NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
		NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
		LINEAR = GL_LINEAR,
		LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
		LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST
#endif
	};

	enum MagFilter
	{

	};

	//Oh dear!
	enum PixelFormat
	{
#if WISH_RENDERER == WISH_OPENGL
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		RGBA16F = GL_RGBA16F,
		RGBA32F = GL_RGBA32F,
		DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
		DEPTH_COMPONENT32 = GL_DEPTH_COMPONENT32,
		DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,
		DEPTH_COMPONENT = GL_DEPTH_COMPONENT
#endif
	};

	struct wish_texture
	{
		GLuint glHandle;
		GLuint width, height;
	};

	void Wish_Texture_Create(wish_texture* texture);
	void Wish_Texture_Destroy(wish_texture* texture);
	void Wish_Texture_Create(wish_texture* texture, GLuint width, GLuint height, PixelFormat pixelFormatStore, PixelFormat pixelFormatIn, GLenum type);
	void Wish_Texture_Create(wish_texture* texture, GLuint width, GLuint height, PixelFormat pixelFormatStore, PixelFormat pixelFormatIn, GLenum type, GLfloat minFilter, GLfloat magFilter);

	void Wish_Texture_Bind(wish_texture* texture, GLenum slot);
	void Wish_Texture_Enable();
	void Wish_Texture_Disable();

	bool Wish_Texture_DEBUGLoadTexture(wish_texture* texture, String fileName);
}