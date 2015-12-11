//This one is temp
#include <SDL_image.h>

namespace Wish
{

	wish_texture wish_texture::Create(u32 width, u32 height, PixelFormat pixelFormatStore, PixelFormat pixelFormatIn, PixelType pixelType)
	{
		return wish_texture::Create(FILTER_NEAREST, FILTER_NEAREST, width, height, pixelFormatStore, pixelFormatIn, pixelType, NULL);
	}

	wish_texture wish_texture::Create(u32 width, u32 height, PixelFormat pixelFormatStore, PixelFormat pixelFormatIn, PixelType pixelType, void* data)
	{
		return wish_texture::Create(FILTER_NEAREST, FILTER_NEAREST, width, height, pixelFormatStore, pixelFormatIn, pixelType, data);
	}

	wish_texture wish_texture::Create(TextureFilter minFilter, TextureFilter magFilter, u32 width, u32 height, PixelFormat pixelFormatStore, PixelFormat pixelFormatIn, PixelType pixelType)
	{
		return wish_texture::Create(minFilter, magFilter, width, height, pixelFormatStore, pixelFormatIn, pixelType, NULL);
	}

	wish_texture wish_texture::Create(TextureFilter minFilter, TextureFilter magFilter, u32 width, u32 height, PixelFormat pixelFormatStore, PixelFormat pixelFormatIn, PixelType pixelType, void* data)
	{
		wish_texture tmp;
		Wish_Texture_Create(&tmp);
		glEnable(GL_TEXTURE_2D);
		tmp.Width = width;
		tmp.Height = height;
		glBindTexture(GL_TEXTURE_2D, tmp.GLHandle);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLfloat)minFilter);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLfloat)magFilter);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, pixelFormatStore, width, height, 0, pixelFormatIn, pixelType, data);
		return tmp;
	}

};

void Wish::Wish_Texture_Create(wish_texture* texture) {
	glGenTextures(1, &texture->GLHandle);
	ASSERT(texture->GLHandle != 0);
}
void Wish::Wish_Texture_Destroy(wish_texture* texture) {
	glDeleteTextures(1, &texture->GLHandle);
}
void Wish::Wish_Texture_Create(wish_texture* texture, GLuint width, GLuint height, PixelFormat pixelFormatStore, PixelFormat pixelFormatIn, GLenum type) {
	Wish_Texture_Create(texture, width, height, pixelFormatStore, pixelFormatIn, type, GL_NEAREST, GL_NEAREST);
}
void Wish::Wish_Texture_Create(wish_texture* texture, GLuint width, GLuint height, PixelFormat pixelFormatStore, PixelFormat pixelFormatIn, GLenum type, GLfloat minFilter, GLfloat magFilter) {
	Wish_Texture_Create(texture);
	glEnable(GL_TEXTURE_2D);
	texture->Width = width;
	texture->Height = height;
	glBindTexture(GL_TEXTURE_2D, texture->GLHandle);
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
		glBindTexture(GL_TEXTURE_2D, texture->GLHandle);
	}
}
void Wish::Wish_Texture_Enable() {
	glEnable(GL_TEXTURE_2D);
}
void Wish::Wish_Texture_Disable() {
	glDisable(GL_TEXTURE_2D);
}

bool Wish::Wish_Texture_DEBUGLoadTexture(wish_texture* texture, const char* fileName) {
	bool result = false;
	SDL_Surface *image = IMG_Load(fileName);
	//If we have a texture
	//Free it
	if (texture->GLHandle > 0) {
		glDeleteTextures(1, &texture->GLHandle);
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
			ASSERT((texture->GLHandle != 0));

			texture->Width = image->w;
			texture->Height = image->h;

			glBindTexture(GL_TEXTURE_2D, texture->GLHandle);
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