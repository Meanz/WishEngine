#pragma once

/*

	-- TODO List, planned features, whatever you want to call it

	* Formalize Mesh/Geometry/Material/Model
		Mesh, SkinnedMesh
		Material

		(Scene Object) Geometry -> Mesh + Material

	* Optimize mesh compilation
		* Heuristic for dyanmic mesh compilation
		* Mesh optimizing algorithm ( sort data )
		* Functions for cloning and instancing
	
	* Multithreading support
		* Implement own multithreading support ( this one is going to be fun )

	* Revise memory system, call everything through our own memory functions, but use malloc instead of our own shit.
		* Use handles for objects? This allows memory to be moved

	* Debug system
		* After UI and memorys system is in place, we need a proper debugging system.
		* Graphs, because they are cool
		* Windows to look through all the textures in the OpenGL system

	* LUA Integration of Scene stuff

	* UI
		* Finish Windows
		* Finish an Event System
		* Fix input lagg ( Mouse coordinates not updating fast enough, SDL halp? )

	* Materials/Shading
		* Add static data, such as the data required for the internal lighting system
		* Add dynamic data, so that the material system can be extended
		* Revise how shaders are handled internally
		* Add functions for post processing effects and hooks for post processing effects
	
	* Renderer
		* Finish up basic lighting system
		* Add shadow mapping
		* Revise current SSAO implementation

	* Scene
		* Extend multithreading functionality for updating scene nodes ( Physics step... )
		
	* Skeletal Animation
		* Each bone is a transformation ( scene object? very expensive update loops? 
			or booleans that stop them from being processing while moving down the hierarchy?

	* Asset System
		* Hashing resource names for fast Asset.Find();

	* OpenAL Sound implementation, but seriously, who likes sounds.

	* Exception handling etc etc..
*/


//## BEGIN CORE ##
//Math library, it's header only, relax
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//FWD's
namespace Wish {};
using namespace Wish;
using glm::ivec2;
using glm::vec2;
using glm::ivec3;
using glm::vec3;
using glm::ivec4;
using glm::vec4;
using glm::quat;
using glm::mat3;
using glm::mat4;

typedef vec2 v2;
typedef vec3 v3;
typedef vec4 v4;
typedef mat3 m3;
typedef mat4 m4;

//Configs

#define WISH_OPENGL 1
#define WISH_RENDERER WISH_OPENGL

//Third party
#if WISH_RENDERER == WISH_OPENGL
#include <GL\glew.h>
#include <GL\GL.h>
#endif

//Types
#include "WishTypes.h"

//Not sure about this one yet
//This allocates a string for you
#include "WishString.h"

//
#include "WishHash.h"

//
#include "WishMemory.h"
//## END CORE ##

//Pre engine cut
//Debug
#include "WishDebug.h"

//
#include "WishIO.h"

//
#include "WishEngine.h"

//
namespace Wish {
	__Wish_Export wish_game_memory* Wish_Get_GameMemory();
	__Wish_Export wish_lua_script* Wish_Get_Lua();
	__Wish_Export wish_scene* Wish_Get_Scene();
	__Wish_Export wish_renderer* Wish_Get_Renderer();
	__Wish_Export wish_ui* Wish_Get_UI();
	__Wish_Export wish_input* Wish_Get_Input();
}

//Exception handling basics
#define WishException(what) printf("Exception in function %s at line %s \n Message: %s\n", __FUNCTION__, std::to_string(__LINE__).c_str(), what); DebugBreak();