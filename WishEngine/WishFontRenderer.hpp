#pragma once

#include "stdafx.h"

namespace Wish
{

	//Inside of this namespace, give ourselves the ability
	//to write just "vector" instead of "std::vector"
	using std::vector;

	//Ditto for string.
	using std::string;

	//This holds all of the information related to any
	//freetype font that we want to create.  
	struct font_data {
		float h;			///< Holds the height of the font.
		GLuint * textures;	///< Holds the texture id's 
		GLuint list_base;	///< Holds the first display list id

		//The init function will create a font of
		//of the height h from the file fname.
		void init(const char * fname, unsigned int h);

		//Free all the resources assosiated with the font.
		void clean();
	};

	//The flagship function of the library - this thing will print
	//out text at window coordinates x,y, using the font ft_font.
	//The current modelview matrix will also be applied to the text. 
	void print(const font_data &ft_font, float x, float y, const char *fmt, ...);


	class FreetypeFont
	{
	private:
		float m_Height; //Holds the height of the font
		GLuint* m_Textures; //Holds texture ids
		GLuint m_ListBase; //Holds the first display list id

	public:

		FreetypeFont();
		~FreetypeFont();

		bool LoadFont(std::string fontPath, unsigned int height);
		void DeleteFont();
		void GLDraw(std::string str, float x, float y);

	};
};