// TestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WishOBJLoader.hpp>
#include "Wish_FPS_Camera.h"
#include <WishPrimitives.h>

using namespace Wish;


wish_geometry* m_JeepGo;
wish_mesh* m_SphereMesh;
wish_fps_camera fpsCamera;

void AddLight(vec3 lightPos) {
	if (m_SphereMesh == 0)
		m_SphereMesh = WishOBJLoader_Load("./data/models/sphere.obj");
	wish_geometry* m_Go = Wish_Scene_NewGeometry("PointLightThing");

	m_Go->Transform.Position = lightPos;
	m_Go->Transform.Scale = vec3(0.3f, 0.3f, 0.3f);
	Wish_Transform_CalculateGlobal(&m_Go->Transform, mat4(1.0f));

	m_Go->Meshes[0] = (m_SphereMesh);
	m_Go->Material.Albedo = Wish_Asset_GetTexture("jeep");

	Wish_Transform_Attach(Wish_Scene_GetRoot(), &m_Go->Transform);

	//
	wish_point_light* light = Wish_Scene_NewPointLight("ActualLight");

	Wish_Transform_Attach(&m_Go->Transform, &light->base.Transform);

}

//Let's do some cool stuff here
void OnInit()
{
	//Create the jeep
	wish_geometry* geom = Wish_Scene_NewGeometry("jeep_model");
	wish_game_object* jeep = Wish_Scene_NewGameObject("jeep_object");
	Wish_Transform_Attach(&jeep->Transform, &geom->Transform);
	geom->Meshes[0] = (WishOBJLoader_Load("./data/models/jeep.obj"));
	geom->Material.Albedo = Wish_Asset_LoadTexture("./data/models/jeep_army.png");
	geom->Transform.Scale = vec3(0.01f, 0.01f, 0.01f);
	Wish_Transform_CalculateGlobal(&geom->Transform, mat4(1.0f));
	Wish_Transform_Attach(Wish_Scene_GetRoot(), &jeep->Transform);


	//Create plane
	wish_geometry* plane = Wish_Scene_NewGeometry("plane");
	plane->Material.Albedo = Wish_Asset_LoadTexture("./data/textures/wald.png");
	Wish_Geometry_AddMesh(plane, Wish_Primitive_Plane(256, 256, 10, 10));
	
	Wish_Transform_Attach(Wish_Scene_GetRoot(), &plane->Transform);

	//Set the camera position
	fpsCamera.Position = v3(-100, 100, -100);

	//Add a light!
	AddLight(v3(-100, 10, -100));

	//We need to add a directional light
	wish_light* dirLight = Wish_Scene_NewDirectionalLight("Sun");
	dirLight->Position = v3(0.3, -0.8, 0.3);//Should deprecate this and use transform instead
	dirLight->m_Diffuse = v3(1.0f, 1.0f, 1.0f);
	dirLight->m_DiffuseIntensity = 0.6f;
	dirLight->m_Specular = v3(1.0f, 1.0f, 1.0f);
	dirLight->m_SpecularIntensity = 0.0f;

	//Fake position
	vec3 lightPos = vec3(-0.4f, -0.6f, -0.4f);

	Wish_Transform_Attach(Wish_Scene_GetRoot(), &dirLight->Transform);
}

void OnUpdate() {

}

void OnFrame() {

	fpsCamera.Camera = Wish_Scene_GetCamera();

	//Do some legacy drawing
	//Do camera stuff
	//m_Camera.Ortho2D(0.0f, 800.0f, 0.0f, 640.0f, 0.0, 1.0f);
	Wish_Camera_Perspective(Wish_Scene_GetCamera(), glm::radians(60.0f), (GLfloat)(Wish_Window_GetAspect()), 1.0f, 1000.0f);
	//m_Camera.GetTransform()->Translate(-5.0f, 5.0f, -5.0f);
	//m_Camera.GetTransform()->Rotate(180.0f, vec3(0.0f, 1.0f, 0.0f));
	//m_Camera.GetTransform()->Rotate(-1.0f, vec3(1.0f, 0.0f, 0.0f));

	//Update camera rotation n shizz
	int mx = Wish_Input_GetMouseX();
	int my = Wish_Input_GetMouseY();
	int dx = Wish_Input_GetMouseDX();
	int dy = Wish_Input_GetMouseDY();
	//m_Camera.GetTransform()->Rotate((my * 0.1f), vec3(1.0f, 0.0f, 0.0f));
	//m_Camera.GetTransform()->Rotate((mx * 0.1f), vec3(0.0f, 1.0f, 0.0f));
	float sens = 0.1f;
	fpsCamera.Yaw += dx * sens;
	fpsCamera.Pitch += -dy * sens;

	if (fpsCamera.Pitch > 89.0f)
		fpsCamera.Pitch = 89.0f;
	if (fpsCamera.Pitch < -89.0f)
		fpsCamera.Pitch = -89.0f;
	if (fpsCamera.Yaw > 360.0f) 
		fpsCamera.Yaw = 0.0f;
	if (fpsCamera.Yaw < 0.0f) 
		fpsCamera.Yaw = 360.0f;
	

	//yaw = (10.0f * glm::cos(glm::radians(someVal * 0.5f))) + 45.0f;

	///printf("Yaw: %f\n", yaw);

	//Calculate the direction vector

	if (Wish_Input_IsKeyDown(WISH_SCANCODE_W)) {
		Wish_FPSCamera_MoveForward(&fpsCamera, 1.0f);
	}
	if (Wish_Input_IsKeyDown(WISH_SCANCODE_S)) {
		Wish_FPSCamera_MoveForward(&fpsCamera, -1.0f);
	}
	if (Wish_Input_IsKeyDown(WISH_SCANCODE_A)) {
		Wish_FPSCamera_Strafe(&fpsCamera, 1.0f);
	}
	if (Wish_Input_IsKeyDown(WISH_SCANCODE_D)) {
		Wish_FPSCamera_Strafe(&fpsCamera, -1.0f);
	}
	if (Wish_Input_IsKeyPressed(WISH_SCANCODE_1)) {
		Wish_Engine_GetContext()->Renderer.ShowSSAO = !Wish_Engine_GetContext()->Renderer.ShowSSAO;
	}
	if (Wish_Input_IsKeyPressed(WISH_SCANCODE_T)) {
		Wish_Engine_GetContext()->Renderer.ShowTextures = !Wish_Engine_GetContext()->Renderer.ShowTextures;
	}
	if (Wish_Input_IsKeyPressed(WISH_SCANCODE_SPACE)) {
		if (Wish_Engine_GetPlatform()->IsMouseLocked())
		{
			printf("TestApp::FreeMouse\n");
			Wish_Engine_GetPlatform()->FreeMouse();
		}
		else
		{
			printf("TestApp::LockMouse\n");
			Wish_Engine_GetPlatform()->LockMouse();
		}
	
	}
	if (Wish_Input_IsKeyPressed(WISH_SCANCODE_R)) {
		printf("Set Deferred rendering to %d\n", (!Wish_Engine_GetContext()->Renderer.m_DeferredRendering));
		Wish_Engine_GetContext()->Renderer.m_DeferredRendering = !Wish_Engine_GetContext()->Renderer.m_DeferredRendering;
	}
	Wish_FPSCamera_Update(&fpsCamera);
}

Wish_GameEntry()
{

	wish_engine_callback wec;
	wec.OnInit = &OnInit;
	wec.OnUpdate = &OnUpdate;
	wec.OnFrame = &OnFrame;

	Wish_Engine_SetApp(wec);
	//The engine will now call us.
	printf("We did it :D\n");
}