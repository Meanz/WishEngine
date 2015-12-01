#define STB_TRUETYPE_IMPLEMENTATION 1
#include "stb_truetype.h"

namespace Wish {

	u8 tmpBitmap[512 * 512 * 4];
	u8 tmpMonochrome[512 * 512];

	wish_texture ftex;

	wish_mesh mesh;

	wish_font::wish_font()
	{

	}

	wish_font::~wish_font()
	{

	}

	b32 wish_font::LoadFont(const char* fileName, r32 pixelHeight)
	{
		this->FontSize = pixelHeight;
		i32 iWidth = 512;
		i32 iHeight = 512;
		u8* ttf_buffer = Wish_IO_ReadFile(fileName);
		if (true)
		{
			u8* tmpGreyscale = (u8*)Wish_Memory_Alloc(iWidth * iHeight);
			u8* tmpColorMultiplied = (u8*)Wish_Memory_Alloc(iWidth * iHeight * 4);
			stbtt_BakeFontBitmap(ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer, 0), pixelHeight, tmpGreyscale, iWidth, iHeight, 32, 96, (stbtt_bakedchar*)this->GlyphData);
			//Do texture thingymajig
			i32 idx = 0, idx2 = 0;
			u32 alpha;
			for (i32 x = 0; x < iWidth; x++)
			{
				for (i32 y = 0; y < iHeight; y++)
				{
					idx = x + ((iHeight - y) * iWidth);
					idx2 = x + (y * iWidth);
					alpha = tmpGreyscale[idx];
					*((u32*)&tmpColorMultiplied[idx2 * 4]) = (alpha << 24) | (alpha << 16) | (alpha << 8) | (alpha << 0);
				}
			}
			FontTexture = wish_texture::Create(FILTER_LINEAR, FILTER_LINEAR, iWidth, iHeight, RGBA, RGBA, PIXELTYPE_UNSIGNED_BYTE, tmpColorMultiplied);
			Wish_Memory_Free(tmpColorMultiplied);
			//Wish_Memory_Free(tmpGreyscale);
			return true;
		}

		b32 result = false;

		{

			//
			stbtt_fontinfo Font;
			stbtt_InitFont(&Font, (u8*)ttf_buffer, 0);

			//
			const u32 numChars = 78 + 5;
			u8 chars[] = {
				'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
				'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
				'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '[', ']', '{', '}', '(', ')', '/', '\\', '&', '%', '-', '+', '*', ',', '.', '_',
				':', ';', '\'', '<', '>'
			};

			//
			i32 xPos = 0;
			i32 yPos = 0;
			i32 yMax = 0;

			//
			i32 width, height, xOffset, yOffset, advanceX, leftSideBearing, ascent, descent, lineGap;
			u8 alpha;
			for (i32 i = 0; i < numChars; i++)
			{
				//Also fetch information about the character
				i32 codepoint = chars[i];
				if (codepoint > MAX_GLYPHS) {
					assert(false);
				}
				wish_font_glyph* glyph = &Glyphs[codepoint];

				stbtt_GetCodepointHMetrics(&Font, codepoint, &advanceX, &leftSideBearing);
				stbtt_GetFontVMetrics(&Font, &ascent, &descent, &lineGap);

				glyph->Kerning = (r32)advanceX * stbtt_ScaleForPixelHeight(&Font, pixelHeight);
				glyph->Ascent = (r32)ascent * stbtt_ScaleForPixelHeight(&Font, pixelHeight);
				glyph->Descent = (r32)descent * stbtt_ScaleForPixelHeight(&Font, pixelHeight);
				glyph->LineGap = (r32)lineGap * stbtt_ScaleForPixelHeight(&Font, pixelHeight);

				//Allocate
				u8* bitmap = stbtt_GetCodepointBitmap(&Font, 0, stbtt_ScaleForPixelHeight(&Font, pixelHeight), chars[i], &width, &height, &xOffset, &yOffset);

				//Blit it onto our texture thing
				if (height > yMax) {
					yMax = height;
				}

				//Check if the glyph fits
				if (xPos + width >= iWidth) {
					yPos += yMax;
					xPos = 0;
					yMax = 0;
				}

				//Check for bounds
				if (xPos + width >= iWidth || yPos + height >= iHeight) {
					assert(false);
				}

				//We have x and y pos, blit glyph
				u32 pos = 0;
				for (i32 x = 0; x < width; x++) {
					for (i32 y = 0; y < height; y++) {
						alpha = bitmap[x + (y * width)];
						pos = ((xPos + x) + ((yPos + y) * iWidth)) * 4;
						if (pos >= (512 * 512 * 4)) {
							assert(false);
						}
						*((u32*)&tmpBitmap[pos]) = (alpha << 24) | (alpha << 16) | (alpha << 8) | (alpha << 0);
					}
				}

				glyph->Width = (r32)width;
				glyph->Height = (r32)height;
				glyph->u0 = (r32)xPos / (r32)iWidth;
				glyph->v1 = ((r32)yPos) / (r32)iHeight;
				glyph->u1 = ((r32)xPos + (r32)width) / (r32)iWidth;
				glyph->v0 = ((r32)yPos + (r32)height) / (r32)iHeight;

				//Advance xpos
				xPos += width;

				//Free
				stbtt_FreeBitmap(bitmap, 0);

			}

			FontTexture = wish_texture::Create(FILTER_NEAREST, FILTER_NEAREST, iWidth, iHeight, RGBA, RGBA, PIXELTYPE_UNSIGNED_BYTE, tmpBitmap);
			//Wish_Primitive_Rect_VT(&mesh, 0, 0, (r32)iWidth, (r32)iHeight);

			//Free
			if (ttf_buffer)
			{
				//Wish_Memory_Free(ttf_buffer);
			}
		}

		return result;
	}


	void wish_font::MakeTextMesh(wish_mesh* mesh, r32 x, r32 y, const char* str)
	{
		i32 cnt = strlen(str);

		Vertex_VT* vertices = (Vertex_VT*)Wish_Memory_Alloc(cnt * 4 * sizeof(Vertex_VT));
		u32* indices = (u32*)Wish_Memory_Alloc(cnt * 6 * sizeof(u32));

		u32 vtOff = 0;
		u32 iOff = 0;
		u32 cvtOff = 0;

		r32 xOff = x;
		r32 yOff = y;
		for (i32 i = 0; i < cnt; i++)
		{
			if (str[i] < MAX_GLYPHS) {

				stbtt_aligned_quad q;
				stbtt_GetBakedQuad((stbtt_bakedchar*)this->GlyphData, 512, 512, (i32)str[i] - 32, &xOff, &yOff, &q, 1);
				//3----2
				//|    |
				//|    |
				//0----1

				r32 w = q.x1 - q.x0;
				r32 h = q.y1 - q.y0;
			

				r32 u0 = q.s0;
				r32 u1 = q.s1;
				r32 v0 = q.t0;
				r32 v1 = q.t1;

				r32 xMin = q.x0;
				r32 xMax = xMin + w;
				r32 yMin = q.y0;
				r32 yMax = q.y1;

				cvtOff = vtOff;
				vertices[vtOff++] = Vertex_VT{ v3(xMin, -yMin, 0.0), v2(u0, 1 - v0) };
				vertices[vtOff++] = Vertex_VT{ v3(xMax, -yMin, 0.0), v2(u1, 1 - v0) };
				vertices[vtOff++] = Vertex_VT{ v3(xMax, -yMax, 0.0), v2(u1, 1 - v1) };
				vertices[vtOff++] = Vertex_VT{ v3(xMin, -yMax, 0.0), v2(u0, 1 - v1) };
				indices[iOff++] = cvtOff + 0;
				indices[iOff++] = cvtOff + 1;
				indices[iOff++] = cvtOff + 2;
				indices[iOff++] = cvtOff + 2;
				indices[iOff++] = cvtOff + 3;
				indices[iOff++] = cvtOff + 0;

				//xOff += glyph->Width - (glyph->Kerning / FontTexture.width);

			}
			else {
				vtOff += 4;
				iOff += 6;
			}
		}
		mesh->MeshType = WISH_VERTEX_VT;
		mesh->SetVertices((r32*)vertices, cnt * 4);
		mesh->SetIndices(indices, cnt * 6);
		//Or, deferr compilation
		mesh->Compile();
		//Delete data
		mesh->Vertices = 0;
		mesh->Indices = 0;
		Wish_Memory_Free(vertices);
		Wish_Memory_Free(indices);
	}

	void wish_font::Render(wish_mesh* mesh)
	{
		//Draw mesh
		//glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		mesh->Draw();
		glDisable(GL_BLEND);
	}

	void wish_font::Print(const char* what, r32 x, r32 y)
	{
		//Do stuff

		//Set our UI shader
		Wish_Renderer_SetShaderProgram(Wish_Get_UI()->UIProgram);

		//Apply values too
		wish_material test;
		ZeroMemory(&test, sizeof(wish_material));
		test.Color = v4(1.0, 1.0, 1.0, 1.0);
		test.Albedo = &FontTexture;

		//
		Wish_Renderer_BindTexture(0, &FontTexture);

		//Apply transformation matrix for this panel
		v2 GlobalPosition(x, y);
		Wish_Renderer_SetWorldMatrix(glm::translate(m4(1.0), v3(GlobalPosition.x, Wish_Window_GetHeight() - GlobalPosition.y, 0)));

		//
		Wish_Renderer_ApplyUniforms(Wish_Renderer_GetShaderProgram(), &test, NULL);

		//Update mesh per frame
		MakeTextMesh(&mesh, 0, 0, what);
		 
		//Draw mesh
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		mesh.Draw();
		glDisable(GL_BLEND);
	}


	///This function gets the first power of 2 >= the
	///int that we pass it.
	inline int next_p2(int a)
	{
		int rval = 1;
		while (rval < a) rval <<= 1;
		return rval;
	}
};
