#pragma once

#include "WishCore.h"

// TODO: reference additional headers your program requires here
#include "WishScancode.h"
#include "WishMemory.h"
#include "WishHash.hpp"

//Pre engine cut
//Debug
#include "WishDebug.h"

//Platform layer
#include "WishPlatform.h"

//Scripting System
#include "WishLua.h"

//Define things that the scene might use
#include "WishMesh.hpp"
#include "WishMaterial.hpp"

//Scenegraph System
#include "WishScene.h"

//For now, I guess
#include "WishFontRenderer.hpp"
#include "WishFramebuffer.hpp"

//Asset System
#include "WishAssetManager.hpp"

//Rendering System
#if WISH_RENDERER == WISH_OPENGL
#include "WishRenderer.hpp"
#else
#include "WishGLRenderer.hpp"
#endif
//Post engine cut

#include "WishEngine.h"



