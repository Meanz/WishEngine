

namespace Wish
{

	wish_ui_window::wish_ui_window(i32 width, i32 height)
	{

		v4 grayBg(0.3, 0.3, 0.35, 1.0);
		v4 grayLight(0.4, 0.4, 0.45, 1.0);
		v4 grayHover(0.5, 0.5, 0.55, 1.0);

		//Add(&Background);
		Add(&TitleBar);
		//Add stuff to me
		CloseButton.Add(&CloseButtonText);
		CloseButtonText.Position.x = 16;
		CloseButtonText.Position.y = 16;
		CloseButtonText.SetText("X");
		CloseButtonText.FontSize = 24;
		CloseButtonText.Center = true;
		TitleBar.Add(&CloseButton);
		TitleText.Position.y = 8;
		TitleText.Position.x = 5; //Left padding
		TitleText.SetText("This is a window.");
		TitleBar.Add(&TitleText);

		CloseButton.Position.x = (r32)(width - 32);
		CloseButton.Dimensions.x = 32;
		CloseButton.Dimensions.y = 32;
		CloseButton.IsTransformDirty = true;
		CloseButton.Material.Color = grayLight;
		TitleBar.Material.Color = grayLight;
		this->Material.Color = grayBg;

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