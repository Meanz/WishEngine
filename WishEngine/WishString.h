#pragma once

//A structure used to define a string that is located in our own memory pool
namespace Wish
{

	//Okay so here's the dealio, 
	//We want to be able to move all the memory here
	//One approach is to create a string table thing
	//struct string_table { u32[4096] Handles; }
	//So if we for some reason have to reorganize data
	//For example by going through some kind of fragmentation process
	//We can update all the pointers
	//The drawback here is increased allocation time and extra memory overhead in form of a handle table
	//
	struct wish_string
	{
		u32 Handle;
		wish_string operator=(const wish_string& other);
		wish_string operator=(const char* other);
		wish_string operator+(const char* other);
		/*
		These will all be slow major copies, but oh well :p
		Rather use Wish_Stringf
		wish_string operator+(const u8 other);
		wish_string operator+(const u16 other);
		wish_string operator+(const u32 other);
		wish_string operator+(const u64 other);
		wish_string operator+(const r32 other);
		wish_string operator+(const r64 other);
		*/
	};

	//Hmm
	__Wish_Export char* Wish_CString(wish_string str);
	void Wish_String_Alloc(wish_string* str, u32 strlen);
	__Wish_Export b32 Wish_String_Free(wish_string str);
	__Wish_Export b32 Wish_String_IsValid(wish_string str);
	__Wish_Export wish_string Wish_String(const char* data);
	__Wish_Export wish_string Wish_Stringf(const char* format, ...);
}
