#pragma once

#include "WishTexture.hpp"

namespace Wish
{

	enum FramebufferAttachment
	{

		DEPTH = GL_DEPTH_ATTACHMENT,
		COLOR0 = GL_COLOR_ATTACHMENT0,
		COLOR1 = GL_COLOR_ATTACHMENT1,
		COLOR2 = GL_COLOR_ATTACHMENT2,
		COLOR3 = GL_COLOR_ATTACHMENT3,
		//COLOR4 = GL_COLOR_ATTAHCMENT4,
		//COLOR5 = GL_COLOR_ATTAHCMENT5,
		COLOR6 = GL_COLOR_ATTACHMENT6,
		//COLOR7 = GL_COLOR_ATTAHCMENT7,
		//COLOR8 = GL_COLOR_ATTAHCMENT8,
	};

	struct wish_renderbuffer
	{
		GLuint glHandle;
		GLuint m_Width;
		GLuint m_Height;
		bool isValid;
	};

	void Wish_Renderbuffer_Create(wish_renderbuffer* renderbuffer, GLenum internalformat, GLuint width, GLuint height);
	void Wish_Renderbuffer_Destroy(wish_renderbuffer* renderbuffer);
	void Wish_Renderbuffer_Bind(wish_renderbuffer* renderbuffer);

	struct wish_framebuffer
	{
		GLuint glHandle;
		bool isValid;
	};

	void Wish_Framebuffer_Create(wish_framebuffer* framebuffer);
	void Wish_Framebuffer_Destroy(wish_framebuffer* framebuffer);

	void Wish_Framebuffer_AttachRenderbuffer(wish_framebuffer* framebuffer, wish_renderbuffer* renderbuffer, FramebufferAttachment attachment);
	void Wish_Framebuffer_AttachTexture(wish_framebuffer* framebuffer, wish_texture* pTexture, FramebufferAttachment attachment);

	bool Wish_Framebuffer_Validate(wish_framebuffer* framebuffer);
	bool Wish_Framebuffer_PackFramebuffer(wish_framebuffer* framebuffer);

	void Wish_Framebuffer_Bind(wish_framebuffer* framebuffer);
	void Wish_Framebuffer_BindRead(wish_framebuffer* framebuffer);
	void Wish_Framebuffer_BindWrite(wish_framebuffer* framebuffer);

	void Wish_Framebuffer_SetReadBuffer(FramebufferAttachment fa);

	struct wish_render_texture
	{
		wish_framebuffer Framebuffer;
		wish_texture Texture;
	};

	void Wish_RenderTexture_Create(wish_render_texture* renderTexture, GLuint width, GLuint height);
	void Wish_RenderTexture_Prepare(wish_render_texture* renderTexture);

}