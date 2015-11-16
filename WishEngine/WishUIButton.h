#pragma once

namespace Wish
{

	class wish_ui_button : public wish_ui_component
	{


	public:

		void* OnPress;

		wish_ui_button();
		~wish_ui_button();
	};

}