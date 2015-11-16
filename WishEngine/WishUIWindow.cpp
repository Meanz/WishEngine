

namespace Wish
{

	wish_ui_window::wish_ui_window()
	{

		//Add stuff to me
		Add(&CloseButton);
	}

	wish_ui_window::~wish_ui_window()
	{

	}

	//Draw the window hmm
	void wish_ui_window::OnDraw(wish_ui* ui)
	{
		//Do input checking here, or in fixed update?



		//Draw top title bar

		//Draw window background

		//Draw my children too
		wish_ui_component::Draw(ui);
	}
}