namespace Wish
{

	wish_ui_text::wish_ui_text()
	{
		Geometry.MeshType = wish_mesh_type::WISH_VERTEX_VT;
		Material.Color = v4(1.0, 0.0, 0.0, 1.0);
		IsGeometryDirty = true;
		Text = "Null";
		Font = &Wish_Get_UI()->DebugFont;
	}

	wish_ui_text::~wish_ui_text()
	{

	}

	void wish_ui_text::SetFont(wish_font* font) {
		this->Font = font;
	}

	void wish_ui_text::SetText(const char* txt)
	{
		//What font are we using, query asset system using hash?
		//Copy string
		Text = txt;
		IsGeometryDirty = true;
	}

	void wish_ui_text::OnDraw(wish_ui* ui)
	{
		r32 fontSize = 16;
		if (Font) {
			//Do parent operations
			wish_ui_component::OnDraw(ui);

			//ui->Rect(0, 0, 32, 32, 0x00ff00);
			if (IsGeometryDirty) {
				Font->MakeTextMesh(&this->Geometry, 0, 0, Wish_CString(Text));
				IsGeometryDirty = false;
			}

			//Do stuff

			//Set our UI shader
			Wish_Renderer_SetShaderProgram(Wish_Get_UI()->UIProgram);

			//Apply values too
			Material.Color = v4(1.0, 1.0, 1.0, 1.0);
			Material.Albedo = &Font->FontTexture;

			//Apply texture
			Wish_Renderer_BindTexture(0, &Font->FontTexture);

			//Apply transformation matrix for this panel
			//TODO(Meanzie): Do this calculation when needed, not per frame
			r32 offX = -(Geometry.Bounds.min.x);
			r32 offY = -(Geometry.Bounds.max.y);
			r32 scale = fontSize / Font->FontSize;

			Wish_Renderer_SetWorldMatrix(glm::scale(glm::translate(TransformationMatrix, v3(offX * scale, offY * scale, 0.0)), v3(scale)));

			//
			Wish_Renderer_ApplyUniforms(Wish_Renderer_GetShaderProgram(), &Material, NULL);

			//Draw the panel
			Font->Render(&Geometry);
		}
		else {
			assert(false);
		}
	}
}