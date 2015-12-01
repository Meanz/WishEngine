

namespace Wish
{

	wish_ui_window::wish_ui_window(i32 width, i32 height)
	{
		//Add(&Background);
		Add(&TitleBar);
		//Add stuff to me
		CloseButton.Add(&CloseButtonText);
		CloseButtonText.SetText("X");
		TitleBar.Add(&CloseButton);
		TitleText.Position.y = 32;
		TitleText.IsTransformDirty = true;
		TitleBar.Add(&TitleText);

		CloseButton.Position.x = (r32)(width - 32);
		CloseButton.Dimensions.x = 32;
		CloseButton.Dimensions.y = 32;
		CloseButton.IsTransformDirty = true;
		CloseButton.Material.Color = v4(0.0, 0.0, 1.0, 1.0);
	

		TitleBar.Material.Color = v4(0.0, 1.0, 0.0, 1.0);

		Position.x = 100;
		Position.y = 100;

		SetSize(width, height);
		Background.IsTransformDirty = true;
		TitleBar.IsTransformDirty = true;
		IsTransformDirty = true;
	}

	wish_ui_window::~wish_ui_window()
	{

	}

	void wish_ui_window::SetSize(i32 width, i32 height)
	{
		Dimensions.x = (r32)width;
		Dimensions.y = (r32)height;
		Background.Dimensions.x = (r32)width;
		Background.Dimensions.y = (r32)height;


		TitleBar.Dimensions.x = (r32)width;
		TitleBar.Dimensions.y = 32.0;
	}

	//Draw the window hmm
	void wish_ui_window::OnDraw(wish_ui* ui)
	{
		//Draw window background
		wish_ui_panel::OnDraw(ui);
	}
}