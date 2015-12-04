#pragma once

namespace Wish
{

	class wish_ui_text : public wish_ui_component
	{

	public:

		wish_mesh Geometry;
		wish_material Material;
		wish_font* Font;
		r32 FontSize;
		wish_string Text;
		m4 TextTransformationMatrix;
		b32 IsGeometryDirty;
		//Whether the text should be centered at the transforms position or 
		//if not flagged the text will be place as if the transforms position was the TL corner of the text
		b32 Center;

		wish_ui_text();
		~wish_ui_text();

		/* Unreliable when geometry hasn't been updated. */
		r32 GetTextWidth();
		r32 GetTextHeight();

		void SetFont(wish_font* font);
		void SetText(const char* txt);

		virtual void OnDraw(wish_ui* ui);

	};
}