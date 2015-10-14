#include "stdafx.h"
#include "Wish.hpp"
#include "WishIO.hpp"

namespace Wish
{
	const char* WishIOException::what() const throw()
	{
		return exception::what();
	}

	wish_string Wish_IO_ReadFileToString(const char* fileName)
	{
		wish_string str;
	
		//So here is the deal, I became sad, then I did this

		std::string rfts = ReadFileToString(fileName);

		//Then I was a it ashamed.
		Wish_String_Alloc(&str, (rfts.size() * sizeof(u8))); //The entire file, + one byte for the null terminator

		//But I looked the other way
		str = rfts.c_str();

		//And you know, I ended up getting there
		return str;
	}

	std::string ReadFileToString(const char* fileName) {
		std::ifstream file(fileName);
		if (!file.is_open())
		{
			throw WishIOException("Could not open file");
		}
		std::string str;
		std::string file_contents;
		while (std::getline(file, str))
		{
			file_contents += str;
			file_contents.push_back('\n');
		}
		file.close();
		return file_contents;
	}

	u8* Wish_IO_ReadFile(const char* fileName)
	{
		FILE* file;
		u32 fileLen;
		u8* result = NULL;
		//Open file
		fopen_s(&file, fileName, "rb");
		if (file)
		{
			//Fetch file length
			fseek(file, 0, SEEK_END);
			fileLen = ftell(file);
			fseek(file, 0, SEEK_SET);

			//Allocate memory
			result = (u8*)Wish_Memory_Alloc(fileLen * sizeof(u8));
			if (result)
			{
				fread(result, fileLen, 1, file);
			}
			else
			{
				printf("Unable to allocate memory for file %s.\n", fileName);
			}

			fclose(file);
		}
		else
		{
			printf("Unable to read %s.\n", fileName);
		}
		return result;
	}

	b32 Wish_IO_WriteFile(const char* fileName, u8* data, u32 size)
	{
		FILE* file;
		b32 result = 0;
		//Open file
		fopen_s(&file, fileName, "wb");
		if (file)
		{
			if (fwrite(data, size, 1, file))
			{
				result = 1;
			}
			fclose(file);
		}
		else
		{
			printf("Unable to open %s for writing.\n", fileName);
		}
		return result;
	}
}
