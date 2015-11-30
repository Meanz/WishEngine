

namespace Wish
{

	wish_ui_panel::wish_ui_panel()
	{
		Geometry.MeshType = wish_mesh_type::WISH_VERTEX_VT;
		Material.Color = v4(1.0, 0.0, 0.0, 1.0);
		IsGeometryDirty = true;
	}

	wish_ui_panel::~wish_ui_panel()
	{

	}

	void wish_ui_panel::OnInput(wish_ui* ui)
	{
		if ((ui->HoverComponent == this && ui->FocusComponent == NULL) || ui->FocusComponent == this)
		{
			if (Wish_Get_Input()->IsMouseDown(1))
			{
				if (Wish_Get_Input()->IsMousePressed(1))
				{
					this->ClickX = (i32)(Wish_Get_Input()->GetMouseX() - GlobalPosition.x);
					this->ClickY = (i32)(Wish_Get_Input()->GetMouseY() - GlobalPosition.y);
					ui->FocusComponent = this;
					Material.Color += v4(0.5, 0.5, 0.5, 1.0);
				}
				else
				{
					//This is kinda dumb :D
					this->Position.x = (r32)(Wish_Get_Input()->GetMouseX() - this->ClickX - (this->Parent == NULL ? 0 : this->Parent->GlobalPosition.x));
					this->Position.y = (r32)(Wish_Get_Input()->GetMouseY() - this->ClickY - (this->Parent == NULL ? 0 : this->Parent->GlobalPosition.y));
					this->IsTransformDirty = true;
				}
			}
			if (Wish_Get_Input()->IsMouseReleased(1) && ui->FocusComponent == this)
			{
				ui->FocusComponent = NULL;
				Material.Color -= v4(0.5, 0.5, 0.5, 1.0);
			}
		}
	}

	void wish_ui_panel::OnDraw(wish_ui* ui)
	{
		//Do parent operations
		wish_ui_component::OnDraw(ui);

		//ui->Rect(0, 0, 32, 32, 0x00ff00);
		if (IsGeometryDirty) {
			Wish_Primitive_Rect_VT(&Geometry, 0, 0, Dimensions.x, Dimensions.y);
			IsGeometryDirty = false;
		}

		//Apply white texture
		Wish_Renderer_BindTexture(0, ui->WhiteTexture);

		//Apply transformation matrix for this panel
		Wish_Renderer_SetWorldMatrix(TransformationMatrix);

		//Upload the uniforms
		Wish_Renderer_ApplyUniforms(Wish_Renderer_GetShaderProgram(), &Material, NULL);

		//Draw the panel
		Geometry.Draw();
	}

}