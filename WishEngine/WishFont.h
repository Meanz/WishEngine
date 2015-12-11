#pragma once

#include "stdafx.h"

namespace Wish
{
#define MAX_GLYPHS 256

	struct wish_font_ft_glyph
	{
		//Deprecated by now?
		u32 Codepoint;
		//
		u32 BitmapX, BitmapY;
		//unsigned int
		u32 Width, Height;
		//ft_int -> signed int
		i32 Left, Top;
		//ft_pos -> signed long
		i32 AdvanceX, AdvanceY;
	};

	struct wish_font_stb_glyph
	{
		unsigned short x0, y0, x1, y1; // coordinates of bbox in bitmap
		float xoff, yoff, xadvance;
	};

	class wish_font
	{

	public:
		wish_texture FontTexture;
		wish_font_stb_glyph GlyphData[MAX_GLYPHS];
		wish_font_ft_glyph Glyphs[MAX_GLYPHS];
		r32 FontSize;
		b32 IsFTCompiled, IsSTBCompiled;

		wish_font();
		~wish_font();

		r32 GetTextWidth(const char* text);

		void FT_MakeTextMesh(wish_mesh* mesh, r32 x, r32 y, const char* what);
		b32 FT_LoadFont(const char* fileName, i32 pixelHeight);
		
		void STB_MakeTextMesh(wish_mesh* mesh, r32 x, r32 y, const char* what);
		b32 STB_LoadFont(const char* location, r32 pixelHeight);

		void MakeTextMesh(wish_mesh* mesh, r32 x, r32 y, const char* what);

		void Render(wish_mesh* mesh);
		void Print(const char* what, r32 x, r32 y);

	};
};