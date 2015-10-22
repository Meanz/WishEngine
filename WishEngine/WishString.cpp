#include "stdafx.h"
#include "WishCore.h"
#include "WishString.h"

#include <stdarg.h> 

namespace Wish
{

	wish_string wish_string::operator = (const wish_string& other)
	{
		//No problemo!
		if (other.Handle == this->Handle) {
			return *this; //Nothing to do really :p
		}
		//We have to do copy :(
		ASSERT(false);
		*this = Wish_CString(other);
		return *this;
	}

	wish_string wish_string::operator=(const char* other)
	{
		//check if we already have an allocated buffer
		if (this->Handle && other)
		{
			Wish_String_Free(*this);
		}
		if (other)
		{
			u32 len = strlen(other); //excludes the \0
			//allocate space for this thing
			Wish_String_Alloc(this, len + 1);
			strcpy_s(Wish_CString(*this), len + 1, other);
		}
		else
		{
			ASSERT(false);
		}
		return *this;
	}

	wish_string wish_string::operator+(const char* other)
	{
		//hehe

		return *this;
	}

	void Wish_String_Alloc(wish_string* str, u32 length) {
		char* memory = (char*)malloc(length);
		if (memory)
		{
			memory[0] = '\0'; //Add the null terminator in the front
		}
		str->Handle = (u32)memory;
	}

	b32 Wish_String_Free(wish_string str) {
		//The handle points to a data structure
		b32 result = false;
		if (str.Handle)
		{
			free((char*)str.Handle);
			str.Handle = 0; //Zero the handle
			result = true;
		}
		return result;
	}

	b32 Wish_String_IsValid(wish_string str)
	{
		return (str.Handle > 0) ? 1 : 0;
	}

	char* Wish_CString(wish_string str) {
		//Here is where we would look after the string in a handle map
		//For now, we have a funny malloc implementation
		//However we only have to change the alloc function and this implementation to change that
		//Seing as all accessing is going to run through this function

		return str.Handle ? (char*)str.Handle : NULL;
	}

	wish_string Wish_String(const char* data)
	{
		wish_string str;
		str = data; //Mehehehe
		return str;
	}

	//Now the way this works is that incoming string
	//is a string allocated on the stack
	//The string is then run through snprintf to determine the final output length
	//We then allocate enough data in our own memory pool and run sprintf on our data there
	//then we return a handle to our data
	wish_string Wish_Stringf(const char* format, ...)
	{
		va_list args;
		wish_string str = { 0 };
		//This is a little yanky
		va_start(args, format);
		int size = _vsnprintf_s(NULL, 0, 0, format, args);
		va_end(args);
		if (size > 0)
		{
			Wish_String_Alloc(&str, size + 1);
			//Copy string data
			va_start(args, format);
			_vsprintf_p(Wish_CString(str), size, format, args);
			va_end(args);
		}
		return str;
	}
}