#include "stdafx.h"
#include "WishCore.h"
#include "WishFramebuffer.hpp"

//###################
//## RENDER BUFFER ##
//###################

void Wish::Wish_Renderbuffer_Create(wish_renderbuffer* renderbuffer, GLenum internalformat, GLuint width, GLuint height)
{
	glGenRenderbuffers(1, &renderbuffer->glHandle);
	renderbuffer->m_Width = width;
	renderbuffer->m_Height = height;
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer->glHandle);
	glRenderbufferStorageEXT(GL_RENDERBUFFER, internalformat, width, height);
}

void Wish::Wish_Renderbuffer_Destroy(wish_renderbuffer* renderbuffer)
{
	glDeleteRenderbuffers(1, &renderbuffer->glHandle);
}

void Wish::Wish_Renderbuffer_Bind(wish_renderbuffer* renderbuffer)
{
	if (renderbuffer == NULL) {
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
	else {
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer->glHandle);
	}
}


//##################
//## FRAME BUFFER ##
//##################

void Wish::Wish_Framebuffer_Create(wish_framebuffer* framebuffer) {
	glGenFramebuffers(1, &framebuffer->glHandle);
	framebuffer->isValid = true;
	ASSERT((framebuffer->glHandle != 0));
}

void Wish::Wish_Framebuffer_Destroy(wish_framebuffer* framebuffer) {
	glDeleteFramebuffers(1, &framebuffer->glHandle);
}

void Wish::Wish_Framebuffer_AttachRenderbuffer(wish_framebuffer* framebuffer, wish_renderbuffer* renderbuffer, FramebufferAttachment attachment) {
	Wish_Framebuffer_Bind(framebuffer);
	Wish_Renderbuffer_Bind(renderbuffer);
	//Attach renderbuffer to FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderbuffer->glHandle);
}

void Wish::Wish_Framebuffer_AttachTexture(wish_framebuffer* framebuffer, wish_texture* pTexture, FramebufferAttachment attachment) {
	//isBound?
	Wish_Framebuffer_Bind(framebuffer);
	//slot == GL_COLOR_ATTACHMENT0
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, pTexture->glHandle, 0);
}

bool Wish::Wish_Framebuffer_Validate(wish_framebuffer* framebuffer) {
	GLenum status;
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	ASSERT((status == GL_FRAMEBUFFER_COMPLETE));

	switch (status)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		printf("glCheckFramebufferStatus() == OK!\n");
		return true;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		printf("glCheckFramebufferStatus() => Not all framebuffer attachment points are framebuffer attachment complete.\n This means that at least one attachment point with a renderbuffer or texture attached has its attached object no longer in existence or has an attached image with a width or height of zero, or the color attachment point has a non-color-renderable image attached, or the depth attachment point has a non-depth-renderable image attached, or the stencil attachment point has a non-stencil-renderable image attached.");
		return false;
	default:
		printf("glCheckFramebufferStatus() => %d\n", status);
	}

	return false;
}

void Wish::Wish_Framebuffer_Bind(wish_framebuffer* framebuffer) {
	if (framebuffer == NULL) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->glHandle);
	}
}

void Wish::Wish_Framebuffer_BindRead(wish_framebuffer* framebuffer) {
	if (framebuffer == NULL)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	}
	else
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer->glHandle);
	}
}

void Wish::Wish_Framebuffer_BindWrite(wish_framebuffer* framebuffer) {
	if (framebuffer == NULL)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}
	else
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer->glHandle);
	}
	
}

void Wish_Framebuffer_SetReadBuffer(FramebufferAttachment fa) {
	glReadBuffer(fa);
}

//###################
//## RenderTexture ##
//###################

void Wish::Wish_RenderTexture_Create(wish_render_texture* renderTexture, GLuint width, GLuint height)
{
	Wish_Texture_Create(&renderTexture->Texture, width, height, PixelFormat::RGBA32F, PixelFormat::RGBA, GL_FLOAT, GL_NEAREST, GL_NEAREST);
	Wish_Framebuffer_Create(&renderTexture->Framebuffer);
	Wish_Framebuffer_AttachTexture(&renderTexture->Framebuffer, &renderTexture->Texture, FramebufferAttachment::COLOR0);
	Wish_Framebuffer_Validate(&renderTexture->Framebuffer);
	Wish_Framebuffer_Bind(NULL);
}

void Wish::Wish_RenderTexture_Prepare(wish_render_texture* renderTexture)
{
	Wish_Framebuffer_BindWrite(&renderTexture->Framebuffer);
	//GLenum buffers[] = { FramebufferAttachment::COLOR0 };
	//glDrawBuffer(FramebufferAttachment::COLOR0);
	glClear(GL_COLOR_BUFFER_BIT);
}