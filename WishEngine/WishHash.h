#pragma once

namespace Wish
{

	extern "C"
	{
		struct wish_murmur3_hash
		{
			u32 hash[4];
			bool operator ==(const wish_murmur3_hash& o);
		};

		wish_murmur3_hash Wish_Hash(const char* str);
	}
}