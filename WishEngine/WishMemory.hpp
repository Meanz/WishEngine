#pragma once

namespace Wish {

	enum MemoryType
	{
		LONG_LIFE = 0, //Use block allocator
		SHORT_LIFE = 0 //Use bump pointer
	};

	//
	//Flags for the memory block
	enum
	{
		Memory_Used = 0x1, //2^0 - bit 0
		Memory_Life_Long = 0x2 //2^1 - bit 1
	};

	struct wish_memory_block
	{
		wish_memory_block* Prev;
		wish_memory_block* Next;
		u64 Flags;
		u64 Size;
	};

	struct wish_game_memory
	{
		u64 PermMemSize;
		void* PermMemory;

		u64 TransMemSize;
		void* TransMemory;

		wish_memory_block* FirstBlock;
	};

	void Wish_Memory_Zero(void* what);
	void Wish_Memory_Zero(void* what, u32 size);

	void Wish_Memory_Init(wish_game_memory* gameMemory);
	void* Wish_Memory_Alloc(wish_game_memory* gameMemory, u32 size);

	__Wish_Export void* Wish_Memory_Alloc(u32 size);
	__Wish_Export void* Wish_Memory_Alloc(MemoryType type, u32 size);
	__Wish_Export void Wish_Memory_Free(void* memory);

	wish_memory_block* Wish_Memory_B_InsertBlock(wish_memory_block* prev, u64 size, void* memory);
	wish_memory_block* Wish_Memory_B_FindBlockForSize(wish_memory_block* sentinel, u64 size);

	i32 Wish_Memory_B_MergeIfPossible(wish_memory_block* sentinel, wish_memory_block* blockA, wish_memory_block* blockB);
	void Wish_Memory_B_Free(wish_memory_block* sentinel, void* memory);
	void* Wish_Memory_B_Alloc(wish_memory_block* sentinel, u64 size);

}