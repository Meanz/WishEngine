#include "stdafx.h"

__declspec(dllexport) void* operator new (size_t size, const char* filename, int line){
	void* ptr = new char[size];
	printf("NEW size = %i filename = %s line = %i \n", size, filename, line);
	return ptr;
}

__declspec(dllexport) void operator delete (void* data, size_t size, const char* filename, int line){
	delete data;
	printf("DELETE size = %i filename = %s line = %i \n", size, filename, line);
}