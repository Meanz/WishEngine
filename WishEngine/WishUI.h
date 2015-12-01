#pragma once

#include "WishFont.h"
#include "WishSprite.h"
#include "WishUIComponent.h"
#include "WishUIPanel.h"
#include "WishUIText.h"
#include "WishUIButton.h"
#include "WishUIWindow.h"

namespace Wish
{

	class wish_ui
	{
	public:

		wish_ui_component Root;
		//
		wish_shader_program* UIProgram;
		//Hmm
		wish_shader_program* UIProgramColor;
		//
		wish_texture* TestTexture;
		//
		wish_texture* WhiteTexture;
		//
		wish_font DebugFont;

		// Input System
		wish_ui_component* HoverComponent;
		wish_ui_component* FocusComponent;

		//
		wish_ui();
		~wish_ui();


		
		void Init();
		void DebugString(const char* str, r32 x, r32 y);
		void Rect(i32 x, i32 y, i32 w, i32 h, u32 color);

		//
		wish_input* GetInput();

		void Draw();
	};

};