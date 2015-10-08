#include "stdafx.h"
#include "WishCore.h"
#include "WishTexture.hpp"

//This one is temp
#include <SDL_image.h>

void Wish::Wish_Texture_Create(wish_texture* texture) {
	glGenTextures(1, &texture->glHandle);
	ASSERT(texture->glHandle != 0);
}
void Wish::Wish_Texture_Destroy(wish_texture* texture) {
	glDeleteTextures(1, &texture->glHandle);
}
void Wish::Wish_Texture_Create(wish_texture* texture, GLuint width, GLuint height, PixelFormat pixelFormatStore, PixelFormat pixelFormatIn, GLenum type) {
	Wish_Texture_Create(texture, width, height, pixelFormatStore, pixelFormatIn, type, GL_NEAREST, GL_NEAREST);
}
void Wish::Wish_Texture_Create(wish_texture* texture, GLuint width, GLuint height, PixelFormat pixelFormatStore, PixelFormat pixelFormatIn, GLenum type, GLfloat minFilter, GLfloat magFilter) {
	Wish_Texture_Create(texture);
	glEnable(GL_TEXTURE_2D);
	texture->width = width;
	texture->height = height;
	glBindTexture(GL_TEXTURE_2D, texture->glHandle);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, pixelFormatStore, width, height, 0, pixelFormatIn, type, NULL);
	//glTexStorage2D(GL_TEXTURE_2D, 0, pixelFormatStore, width, height);
}

void Wish::Wish_Texture_Bind(wish_texture* texture, GLenum slot) {
	glActiveTexture(slot);
	if (texture == NULL) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, texture->glHandle);
	}
}
void Wish::Wish_Texture_Enable() {
	glEnable(GL_TEXTURE_2D);
}
void Wish::Wish_Texture_Disable() {
	glDisable(GL_TEXTURE_2D);
}

bool Wish::Wish_Texture_DEBUGLoadTexture(wish_texture* texture, String fileName) {
	bool result = false;
	SDL_Surface *image = IMG_Load(fileName.c_str());
	//If we have a texture
	//Free it
	if (texture->glHandle > 0) {
		glDeleteTextures(1, &texture->glHandle);
	}
	if (image != NULL) {

		GLuint srcFormat = 0;

		bool foundFormat = false;
		if (image->format->format == SDL_PIXELFORMAT_RGB24
			|| image->format->format == SDL_PIXELFORMAT_RGB332
			|| image->format->format == SDL_PIXELFORMAT_RGB444
			|| image->format->format == SDL_PIXELFORMAT_RGB555
			|| image->format->format == SDL_PIXELFORMAT_RGB565
			|| image->format->format == SDL_PIXELFORMAT_RGB888)
		{
			srcFormat = GL_RGB;
		}
		else if (image->format->format == SDL_PIXELFORMAT_RGBA4444
			|| image->format->format == SDL_PIXELFORMAT_RGBA5551
			|| image->format->format == SDL_PIXELFORMAT_RGBA8888
			|| image->format->format == SDL_PIXELFORMAT_ABGR8888)
		{
			srcFormat = GL_RGBA;
		}

		if (srcFormat > 0)
		{
			glEnable(GL_TEXTURE_2D);
			Wish_Texture_Create(texture);
			ASSERT((texture->glHandle != 0));

			texture->width = image->w;
			texture->height = image->h;

			glBindTexture(GL_TEXTURE_2D, texture->glHandle);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, srcFormat, GL_UNSIGNED_BYTE, image->pixels);
			SDL_FreeSurface(image);
			result = true;
		}
		else
		{
			printf("The fuck, pixel format is not recognized?\n");
			printf("Format: %i\n", image->format->format);
			SDL_FreeSurface(image);
		}
	}
	return result;
}