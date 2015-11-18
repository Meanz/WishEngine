

namespace Wish
{

	void MakeRect(wish_mesh* mesh, i32 x, i32 y, i32 w, i32 h)
	{
		Vertex_VT vertices[4];
		u32 indices[6];
		//invert y
		i32 vh = 0;//Wish_Window_GetHeight();
		vertices[0] = Vertex_VT{ v3(x, vh - (y + h), 0.0), v2(0.0, 1.0) };
		vertices[1] = Vertex_VT{ v3(x + w, vh - (y + h), 0.0), v2(1.0, 1.0) };
		vertices[2] = Vertex_VT{ v3(x + w, vh - (y), 0.0), v2(1.0, 0.0) };
		vertices[3] = Vertex_VT{ v3(x, vh - (y), 0.0), v2(0.0, 0.0) };
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 2;
		indices[4] = 3;
		indices[5] = 0;
		mesh->SetVertices((r32*)vertices, 4);
		mesh->SetIndices(indices, 6);
		//Or, deferr compilation
		mesh->Compile();
	}

	wish_ui_panel::wish_ui_panel()
	{
		Geometry.MeshType = wish_mesh_type::WISH_VERTEX_VT;
		Material.Color = v4(1.0, 0.0, 0.0, 0.0);
		IsGeometryDirty = true;
		IsMouseInside = false;
	}

	wish_ui_panel::~wish_ui_panel()
	{

	}

	void wish_ui_panel::OnDraw(wish_ui* ui) {

		//Call parent on draw
		wish_ui_component::OnDraw(ui);

		//ui->Rect(0, 0, 32, 32, 0x00ff00);
		if (IsGeometryDirty) {
			MakeRect(&Geometry, (i32)0, (i32)0, (i32)Dimensions.x, (i32)Dimensions.y);
			IsGeometryDirty = false;
		}

		if (Wish_Get_Input()->GetMouseX() > GlobalPosition.x && Wish_Get_Input()->GetMouseX() < GlobalPosition.x + Dimensions.x
			&& Wish_Get_Input()->GetMouseY() > GlobalPosition.y && Wish_Get_Input()->GetMouseY() < GlobalPosition.y + Dimensions.y) {
			if (!IsMouseInside) {
				SaveColor = Material.Color;
			}
			IsMouseInside = true;
			
			ui->HoverComponent = this;
			Material.Color = v4(1.0, 1.0, 1.0, 1.0);
		}
		else {
			if (IsMouseInside) {
				Material.Color = SaveColor;
			}
			IsMouseInside = false;
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