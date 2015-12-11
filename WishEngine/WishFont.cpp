
//STB TrueType
#define STB_TRUETYPE_IMPLEMENTATION 1
#include "stb_truetype.h"

// FreeType Headers
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

namespace Wish {

	///This function gets the first power of 2 >= the
	///int that we pass it.
	inline int next_p2(int a)
	{
		int rval = 1;
		while (rval < a) rval <<= 1;
		return rval;
	}

	u8 tmpBitmap[512 * 512 * 4];
	u8 tmpMonochrome[512 * 512];
	wish_texture ftex;
	wish_mesh mesh;

	wish_font::wish_font()
	{
		IsSTBCompiled = false;
		IsFTCompiled = false;
	}

	wish_font::~wish_font()
	{

	}

	b32 wish_font::FT_LoadFont(const char* fileName, i32 pixelHeight)
	{
		this->FontSize = (r32)pixelHeight;
		b32 result = true;
		FT_Library library;
		if (!FT_Init_FreeType(&library))
		{
			FT_Face face;
			if (!FT_New_Face(library, fileName, 0, &face))
			{

				// Quote: NeHe http://nehe.gamedev.net/tutorial/freetype_fonts_in_opengl/24001/
				// For Some Twisted Reason, FreeType Measures Font Size
				// In Terms Of 1/64ths Of Pixels.  Thus, To Make A Font
				// h Pixels High, We Need To Request A Size Of h*64.
				// (h << 6 Is Just A Prettier Way Of Writing h*64)
				FT_Set_Char_Size(face, pixelHeight << 6, pixelHeight << 6, 96, 96);


				u32 bWidth = 1024;
				u32 bHeight = 1024;
				u32 bBytesPerPixel = 4;
				u8* bitmapData = (u8*)malloc(bWidth * bHeight * bBytesPerPixel);
				//Clear the data
				for (u32 i = 0; i < (bWidth * bHeight * bBytesPerPixel); i++) 
				{
					bitmapData[i] = 255 << 24 | 255 << 16 | 255 << 8 | 255 << 0;
				}
				u32 bOffsetX = 0;
				u32 bOffsetY = 0;
				u32 highestGlyph = 0;

				u32 paddingX = 8;
				u32 paddingY = 8;

				//Add some kind of bounds check for tighter packing
				/*v2 glyphBounds[MAX_GLYPHS];
				//Initialize all glyphs to zero
				for (u32 i = 0; i < MAX_GLYPHS; i++) {
				glyphBounds[i].x = 0;
				glyphBounds[i].y = 0;
				}*/

				for (i32 i = 32; i < 96 + 32; i++)
				{
					FT_Load_Glyph(face, FT_Get_Char_Index(face, i), FT_LOAD_DEFAULT);

					FT_Glyph glyph;
					if (!FT_Get_Glyph(face->glyph, &glyph))
					{
						FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
						FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
						FT_Bitmap& bitmap = bitmap_glyph->bitmap;
						u32 wasHighestGlyph = highestGlyph;

						//u8* data = (u8*)Wish_Memory_Alloc(2 * width * height);
						//Check x bounds
						if (bOffsetX + bitmap.width > (i32)bWidth) {
							//We need to move down
							bOffsetY += highestGlyph + paddingX; //8 is offset
							bOffsetX = 0;
							highestGlyph = 0;
						}

						//Check y bounds
						if (bOffsetY + bitmap.rows > (i32)bHeight) {
							//Shit, we got issues
							WishException("Font won't fit in image.");
						}

						//Ref http://www.freetype.org/freetype2/docs/reference/ft2-basic_types.html#FT_Bitmap

						u32 index = 0, bufferIndex = 0, bufferVal = 0;
						u32 ftBytesPerPixel = 0;
						if (bitmap.pixel_mode == ft_pixel_mode_mono) {
							ftBytesPerPixel = 1; //Redundant
						}
						else if (bitmap.pixel_mode == ft_pixel_mode_grays) {
							//ft_pixel_mode_grays = FT_PIXEL_MODE_GRAY
							//
							//We should really get an exception handler
							//WishException("Unsupported FreeType pixel mode ft_pixel_mode_grays.");
							ftBytesPerPixel = 1;
						}
						else {
							WishException("Unsupported FreeType pixel mode: " + bitmap.pixel_mode);
						}
						for (u32 y = 0; y < (u32)bitmap.rows; y++)
						{
							for (u32 x = 0; x < (u32)bitmap.width; x++)
							{
								//Copy bitmap data
								index = ((bOffsetX + (bOffsetY * bWidth)) + (x + (y * bWidth)));
								if (index >(bWidth * bHeight * bBytesPerPixel)) {
									WishException("Index out of bounds.");
								}
								bufferIndex = (x + (bitmap.width * (bitmap.rows - y - 1))) * ftBytesPerPixel;
								if (bufferIndex < 0 || bufferIndex > (bitmap.width * bitmap.rows * ftBytesPerPixel)) {
									WishException("Index out of bounds.");
								}
								//We are inside the picture
								bufferVal = bitmap.buffer[bufferIndex];
								*((u32*)bitmapData + index) = bufferVal << 24 | bufferVal << 16 | bufferVal << 8 | bufferVal << 0;
							}
						}

						//Set bitmap values for this glyph
						Glyphs[i].BitmapX = bOffsetX;
						Glyphs[i].BitmapY = bOffsetY;

						//We have our position, now make sure that the next glyph knows that
						if (highestGlyph < (u32)bitmap.rows) {
							highestGlyph = bitmap.rows;
						}
						bOffsetX += bitmap.width + paddingY;

						Glyphs[i].Width = bitmap.width;
						Glyphs[i].Height = bitmap.rows;

						Glyphs[i].Left = bitmap_glyph->left;
						Glyphs[i].Top = bitmap_glyph->top;

						Glyphs[i].AdvanceX = face->glyph->advance.x;
						Glyphs[i].AdvanceY = face->glyph->advance.y;

						//Wish_Memory_Free(data);
					}
					else
					{
						result = false;
						//Break here?
						break;
					}
					FT_Done_Glyph(glyph);
				}

				if (result) {
					//Upload texture
					FontTexture = wish_texture::Create(FILTER_LINEAR, FILTER_LINEAR, bWidth, bHeight, RGBA, RGBA, PIXELTYPE_UNSIGNED_BYTE, bitmapData);
					IsFTCompiled = true;
				}

				//Free texture data
				//Wish_Memory_Free(bitmapData);
				free(bitmapData);

				//Free the face
				FT_Done_Face(face);
			}
			else
			{
				result = false;
			}
			//Free the library
			FT_Done_FreeType(library);
		}
		else
		{
			result = false;
		}
		return result;
	}

	//TODO(Meanzie): Revise whether to make geometry or not for char 32 ( space )
	void wish_font::FT_MakeTextMesh(wish_mesh* mesh, r32 x, r32 y, const char* str)
	{
		i32 cnt = strlen(str);

		Vertex_VT* vertices = (Vertex_VT*)Wish_Memory_Alloc(cnt * 4 * sizeof(Vertex_VT));
		u32* indices = (u32*)Wish_Memory_Alloc(cnt * 6 * sizeof(u32));

		u32 vtOff = 0;
		u32 iOff = 0;
		u32 cvtOff = 0;

		r64 xOff = x;
		r64 yOff = y;

		u32 imgW = this->FontTexture.Width;
		u32 imgH = this->FontTexture.Height;

		r64 UnitX = 1.0f / (r64)imgW;
		r64 UnitY = 1.0f / (r64)imgH;

		for (i32 i = 0; i < cnt; i++)
		{
			if (str[i] < MAX_GLYPHS) {

				wish_font_ft_glyph glyph = Glyphs[str[i]];

				//Reset Y
				yOff = 0;
				yOff += (r64)glyph.Top - (r64)glyph.Height;

				r64 xMin = xOff + (r64)glyph.Left;
				r64 xMax = xMin + (r64)glyph.Width;
				r64 yMin = yOff;
				r64 yMax = yOff + (r64)glyph.Height;

				r64 u0 = ((r64)glyph.BitmapX) * UnitX;
				r64 u1 = ((r64)glyph.BitmapX + (r64)glyph.Width) * UnitX;

				r64 v0 = (r64)((r64)glyph.BitmapY) * UnitY;
				r64 v1 = (r64)((r64)glyph.BitmapY + (r64)glyph.Height) * UnitY;

				cvtOff = vtOff;

				vertices[vtOff++] = Vertex_VT{ v3((r32)xMin, (r32)yMin, 0.0), v2((r32)u0, (r32)v0) };
				vertices[vtOff++] = Vertex_VT{ v3((r32)xMax, (r32)yMin, 0.0), v2((r32)u1, (r32)v0) };
				vertices[vtOff++] = Vertex_VT{ v3((r32)xMax, (r32)yMax, 0.0), v2((r32)u1, (r32)v1) };
				vertices[vtOff++] = Vertex_VT{ v3((r32)xMin, (r32)yMax, 0.0), v2((r32)u0, (r32)v1) };

				indices[iOff++] = cvtOff + 0;
				indices[iOff++] = cvtOff + 1;
				indices[iOff++] = cvtOff + 2;
				indices[iOff++] = cvtOff + 2;
				indices[iOff++] = cvtOff + 3;
				indices[iOff++] = cvtOff + 0;

				xOff += (r64) (glyph.AdvanceX >> 6);
				//yOff += glyph.AdvanceY;
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

	b32 wish_font::STB_LoadFont(const char* fileName, r32 pixelHeight)
	{
		b32 result = true;
		this->FontSize = pixelHeight;
		i32 iWidth = 512;
		i32 iHeight = 512;
		u8* ttf_buffer = Wish_IO_ReadFile(fileName);

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

		return result;
	}

	//Expensive hack
	//TODO(Meanzie): Optimize
	r32 wish_font::GetTextWidth(const char* str)
	{
		i32 cnt = strlen(str);
		r32 xOff = 0;
		r32 yOff = 0;
		r32 xMax = 0;
		for (i32 i = 0; i < cnt; i++)
		{
			if (str[i] < MAX_GLYPHS)
			{
				stbtt_aligned_quad q;
				stbtt_GetBakedQuad((stbtt_bakedchar*)this->GlyphData, 512, 512, (i32)str[i] - 32, &xOff, &yOff, &q, 1);
				r32 w = q.x1 - q.x0;
				xMax = q.x0 + w;
			}
		}
		return xMax;
	}

	void wish_font::STB_MakeTextMesh(wish_mesh* mesh, r32 x, r32 y, const char* str)
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

	void wish_font::MakeTextMesh(wish_mesh* mesh, r32 x, r32 y, const char* what)
	{
		if (this->IsFTCompiled) {
			this->FT_MakeTextMesh(mesh, x, y, what);
		}
		else if (this->IsSTBCompiled) {
			this->STB_MakeTextMesh(mesh, x, y, what);
		}
		else {
			WishException("Tried to make font mesh without any loaded fonts.");
		}
	}

	void wish_font::Render(wish_mesh* mesh)
	{
		//Draw mesh
		glEnable(GL_BLEND);
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

};
