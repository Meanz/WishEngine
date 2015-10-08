#pragma once

//I don't like these
#include <string>
#include <fstream>
#include <cassert>
#include <exception>

namespace Wish
{

	class WishIOException : public std::exception
	{
	public:
		WishIOException(const char * myWhat) : exception(myWhat) {};
		virtual const char* what() const throw();
	};


	//THIS IS A SUCKY FUNCTION
	//BUT IS WORKS
	__Wish_Export std::string ReadFileToString(const char* fileName);
	__Wish_Export u8* Wish_IO_ReadFile(const char* fileName);
	__Wish_Export b32 Wish_IO_WriteFile(const char* fileName, u8* data, u32 size);

}