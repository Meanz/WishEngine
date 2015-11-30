
#include "stdafx.h"
#include "Wish.h"
#include "WishFontRenderer.hpp"


#include "WishUIComponent.cpp"
#include "WishUIPanel.cpp"
#include "WishUIButton.cpp"
#include "WishUIWindow.cpp"


namespace Wish
{

	wish_font DebugFont;

	wish_ui::wish_ui() {
		HoverComponent = NULL;
		FocusComponent = NULL;
	}

	wish_ui::~wish_ui() {

	}

	void wish_ui::Init()
	{
		wish_ui_component::wish_ui_component(Root);
		DebugFont.LoadFont("./data/fonts/arial.ttf", 18);

		UIProgram = Wish_Asset_LoadShader("_ui", "./data/shaders/ui.glsl");
		UIProgramColor = Wish_Asset_LoadShader("_ui_color", "./data/shaders/ui_color.glsl");
		TestTexture = Wish_Asset_LoadTexture("_ui_test_texture", "./data/textures/test2.png");
		WhiteTexture = Wish_Asset_LoadTexture("_ui_white_texture", "./data/textures/white.png");
	}

	void wish_ui::DebugString(const char* str, r32 x, r32 y) 
	{
		DebugFont.Print(str, x, y);
	}
	
	void wish_ui::Rect(i32 x, i32 y, i32 w, i32 h, u32 color)
	{
		//Immediate atm

		Wish_Texture_Disable();

		//extract rgb
		r32 r = (r32)(color & 0xff000000);
		r32 g = (r32)(color & 0x00ff0000);
		r32 b = (r32)(color & 0x0000ff00);
		r32 a = (r32)(color & 0x000000ff);

		glColor4f(r, g, b, a);
		glBegin(GL_QUADS);
		{
			glVertex2f((r32)x, (r32)y + (r32)h);
			glVertex2f((r32)x + (r32)w, (r32)y + (r32)h);
			glVertex2f((r32)x + (r32)w, (r32)y);
			glVertex2f((r32)x, (r32)y);
		}
		glEnd();
	}

	wish_input* GetInput() 
	{
		return Wish_Get_Input();
	}

	void wish_ui::Draw()
	{
		//Start processing UI Events
		//Find which component we are over
		HoverComponent = Root.RayCast(Wish_Get_Input()->GetMouseX(),
			Wish_Get_Input()->GetMouseY());

		//Process UI actions
		Root.DoInput(this);

		//Set our UI shader
		Wish_Renderer_SetShaderProgram(UIProgram);

		//Draw the UI
		Root.Draw(this);
	}
}