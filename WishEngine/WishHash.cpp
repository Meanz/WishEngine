#include "stdafx.h"
#include "Wish.h"
#include "WishHash.h"
#include "Murmur3.h"

namespace Wish
{

	bool wish_murmur3_hash::operator==(const wish_murmur3_hash& o) {
		if (o.hash[0] == hash[0] &&
			o.hash[1] == hash[1] &&
			o.hash[2] == hash[2] &&
			o.hash[3] == hash[3]) {
			return true;
		}
		else {
			return false;
		}
	}

#define MURMUR3_SEED 1337
#define MURMUR3(key, len, out) MurmurHash3_x86_128(key, len, MURMUR3_SEED, out)

	wish_murmur3_hash Wish_Hash(const char* str) {
		//We have 3 funcs
		wish_murmur3_hash out;
		MURMUR3(str, strlen(str), out.hash);
		return out;
	}

}