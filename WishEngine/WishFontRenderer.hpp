#pragma once

#include "stdafx.h"

namespace Wish
{
#define MAX_GLYPHS 256

	struct wish_font_glyph
	{
		u32 Codepoint;
		r32 Width, Height;
		r32 u0, v0, u1, v1;
		r32 Kerning;
		r32 Ascent;
		r32 Descent;
		r32 LineGap;
	};

	struct wish_font_glyph2
	{
		unsigned short x0, y0, x1, y1; // coordinates of bbox in bitmap
		float xoff, yoff, xadvance;
	};

	class wish_font
	{

	public:
		wish_texture FontTexture;
		wish_font_glyph2 GlyphData[MAX_GLYPHS];
		wish_font_glyph Glyphs[MAX_GLYPHS];
		r32 FontSize;

		wish_font();
		~wish_font();

		void MakeTextMesh(wish_mesh* mesh, r32 x, r32 y, const char* what);
		b32 LoadFont(const char* location, r32 pixelHeight);
		void Print(const char* what, r32 x, r32 y);

	};
};