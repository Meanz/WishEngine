#pragma once

namespace Wish
{
#define __Wish_Export __declspec(dllexport)
#define __Wish_Import __declspec(dllimport)

#include <stdint.h>

#define ASSERT(expr) if(!expr) { \
	((int*)0)[0] = 0; \
				}

#define Kilobytes(Value) (Value * 1024LL)
#define Megabytes(Value) (Kilobytes(Value) * 1024LL)
#define Gigabytes(Value) (Megabytes(Value) * 1024LL)
#define Terabytes(Value) (Gigabytes(Value) * 1024LL)

	//Casey Muratori doesn't like the keyword static because it has different meanings in different scopes, 
	//and we are adopting his views just as a good student should!
#define local_persist static
#define global_variable static

#define Pi32 3.14159265359f

	//Primitive renaming
	typedef int8_t int8;
	typedef int16_t int16;
	typedef int32_t int32;
	typedef int64_t int64;
	typedef int32 bool32;

	typedef uint8_t uint8;
	typedef uint16_t uint16;
	typedef uint32_t uint32;
	typedef uint64_t uint64;

	typedef float real32;
	typedef double real64;

	//Primitive shorthands
	typedef int8 i8;
	typedef int16 i16;
	typedef int32 i32;
	typedef int64 i64;
	typedef bool32 b32;

	typedef uint8 u8;
	typedef uint16 u16;
	typedef uint32 u32;
	typedef uint64 u64;

	typedef real32 r32;
	typedef real64 r64;

	typedef b32(*Wish_Bool_Func)(void);
	typedef void(*Wish_Void_Func)(void);
	typedef void(*Wish_Class_Func)(void*);

}

