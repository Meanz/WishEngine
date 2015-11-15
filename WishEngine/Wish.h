#pragma once


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
#include "WishIO.hpp"

//
#include "WishEngine.h"
