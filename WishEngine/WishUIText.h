#pragma once

namespace Wish
{

	class wish_ui_text : public wish_ui_component
	{

	public:

		wish_mesh Geometry;
		wish_material Material;
		wish_font* Font;
		wish_string Text;
		b32 IsGeometryDirty;

		wish_ui_text();
		~wish_ui_text();

		void SetFont(wish_font* font);
		void SetText(const char* txt);

		virtual void OnDraw(wish_ui* ui);

	};
}