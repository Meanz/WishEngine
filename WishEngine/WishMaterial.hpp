#pragma once

#include "WishTexture.hpp"

namespace Wish
{
	struct wish_material
	{
		//
		wish_texture* Albedo;

		//
		wish_texture* NormalMap;

		//Lambert
		v3 Diffuse;
		v3 Specular;
		r32 SpecularIntensity;
	};

}