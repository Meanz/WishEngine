namespace Wish
{

	wish_ui_text::wish_ui_text()
	{
		Geometry.MeshType = wish_mesh_type::WISH_VERTEX_VT;
		Geometry.DrawMode = wish_mesh_drawmodes::WISH_DRAW_DYNAMIC;
		Material.Color = v4(1.0, 1.0, 1.0, 1.0);
		IsGeometryDirty = true;
		IsTransformDirty = true;
		Text = "Null";
		Center = false;
		//Default to a font size of 16?
		FontSize = 16.0f;

		//Dis brewk
		Font = 0;
	}

	wish_ui_text::~wish_ui_text()
	{

	}

	void wish_ui_text::SetFont(wish_font* font) {
		this->Font = font;
		Material.Albedo = &Font->FontTexture;
	}

	void wish_ui_text::SetText(const char* txt)
	{
		//What font are we using, query asset system using hash?
		//Copy string
		Text = txt;
		IsTransformDirty = true;
		IsGeometryDirty = true;
	}

	void wish_ui_text::OnDraw(wish_ui* ui)
	{
		if (!Font) {
			SetFont(&Wish_Get_UI()->DebugFont);
		}
		if (Font) {
			//Do parent operations
			wish_ui_component::OnDraw(ui);

			//ui->Rect(0, 0, 32, 32, 0x00ff00);
			if (IsGeometryDirty) {
				Font->MakeTextMesh(&this->Geometry, 0, 0, Wish_CString(Text));
				IsGeometryDirty = false;
			}

			if (IsTransformDirty) {
				//Recalculate our internal transformation
				r32 offX = 0;// -(Geometry.Bounds.min.x) - (Center ? (Geometry.Bounds.Width() / 2) : 0);
				r32 offY = -Font->FontSize;// -(Geometry.Bounds.max.y) + (Center ? (Geometry.Bounds.Height() / 2) : 0);
				if (Center) {
					//Do some magic
					offY /= 2;
					offX = -Geometry.Bounds.Width() / 2;
				}
				r32 scale = 1.0;//FontSize / Font->FontSize;
				TextTransformationMatrix = glm::scale(glm::translate(TransformationMatrix, v3(offX * scale, offY * scale, 0.0)), v3(scale));
			} 

			//Apply texture
			Wish_Renderer_BindTexture(0, &Font->FontTexture);

			//Apply transformation matrix for this text
			Wish_Renderer_SetWorldMatrix(TextTransformationMatrix);

			//
			Wish_Renderer_ApplyUniforms(Wish_Renderer_GetShaderProgram(), &Material, NULL);

			//Draw the panel
			Font->Render(&Geometry);

			//
			//Font->Print(Wish_CString(this->Text), this->GlobalPosition.x, this->GlobalPosition.y);
		}
		else
		{
			assert(false);
		}
	}
}