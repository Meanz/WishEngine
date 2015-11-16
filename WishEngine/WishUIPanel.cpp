

namespace Wish
{

	void MakeRect(wish_mesh* mesh, i32 x, i32 y, i32 w, i32 h)
	{
		Vertex_VT vertices[4];
		u32 indices[6];
		//invert y
		i32 vh = Wish_Window_GetHeight();
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

	}

	wish_ui_panel::~wish_ui_panel()
	{

	}

	void wish_ui_panel::OnDraw(wish_ui* ui) {

		//ui->Rect(0, 0, 32, 32, 0x00ff00);
		if (IsDirty) {
			MakeRect(&Geometry, Position.x, Position.y, Dimensions.x, Dimensions.y);
			IsDirty = true;
		}

		//glDisable(GL_CULL_FACE);

		Wish_Renderer_SetShaderProgram(ui->UIProgramColor);

		//mat4 scale = glm::scale(mat4(1.0), v3(1.0, -1.0, 1.0));
		//Wish_Renderer_SetWorldMatrix(scale);


		Material.Color = v4(1.0, 0.0, 0.0, 0.0);

		Wish_Renderer_ApplyUniforms(Wish_Renderer_GetShaderProgram(), &Material, NULL);

		Geometry.Draw();

	}

}