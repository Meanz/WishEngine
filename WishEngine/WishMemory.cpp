#include "stdafx.h"
#include "Wish.hpp"
#include "WishMemory.hpp"

void Wish::Wish_Memory_Zero(void* what)
{
	wish_memory_block* block = ((wish_memory_block*)what - 1);
	for (u32 ptr = 0; ptr < block->Size; ptr++) 
	{
		((u8*)what)[ptr] = 0;
	}
}

void Wish::Wish_Memory_Zero(void* what, u32 size)
{
	//If you have trust issues, then walk away now.
	for (u32 ptr = 0; ptr < size; ptr++)
	{
		((u8*)what)[ptr] = 0;
	}
}

void Wish::Wish_Memory_Init(wish_game_memory* gameMemory)
{
	gameMemory->FirstBlock = (wish_memory_block*)gameMemory->PermMemory;
	gameMemory->FirstBlock->Flags = 0;
	gameMemory->FirstBlock->Size = 0;
	gameMemory->FirstBlock->Next = gameMemory->FirstBlock;
	gameMemory->FirstBlock->Prev = gameMemory->FirstBlock;

	//Insert the memory block
	Wish_Memory_B_InsertBlock(
		gameMemory->FirstBlock,
		gameMemory->PermMemSize - sizeof(wish_memory_block),
		(void*)((u8*)gameMemory->PermMemory + sizeof(wish_memory_block)));
}

void* Wish::Wish_Memory_Alloc(wish_game_memory* gameMemory, u32 size) {
	
	void* mem = Wish_Memory_B_Alloc(gameMemory->FirstBlock, (u64)size);
	return mem;
}

void* Wish::Wish_Memory_Alloc(u32 size) {
	wish_game_memory* gameMemory = Wish_Engine_GetContext()->Memory;
	return Wish_Memory_Alloc(gameMemory, size);
}

void* Wish::Wish_Memory_Alloc(MemoryType type, u32 size)
{
	return Wish_Memory_Alloc(size);
}

void Wish::Wish_Memory_Free(void* memory) {
	wish_game_memory* gameMemory = Wish_Engine_GetContext()->Memory;
	Wish_Memory_B_Free(gameMemory->FirstBlock, memory);
}

wish_memory_block* Wish::Wish_Memory_B_InsertBlock(wish_memory_block* prev, u64 size, void* memory)
{
	ASSERT((size > sizeof(wish_memory_block)));
	wish_memory_block* block = (wish_memory_block*)memory;
	block->Flags = 0;
	block->Size = size - sizeof(wish_memory_block);
	block->Prev = prev;
	block->Next = prev->Next;
	block->Prev->Next = block;
	block->Next->Prev = block;
	return block;
}
wish_memory_block* Wish::Wish_Memory_B_FindBlockForSize(wish_memory_block* sentinel, u64 size)
{
	wish_memory_block* result = 0;
	// TODO(meanzie): find best match block!
	for (wish_memory_block* block = sentinel->Next; block != sentinel; block = block->Next)
	{
		if (!(block->Flags & Memory_Used))
		{
			if (block->Size >= size)
			{
				result = block;
				break;
			}
		}
	}
	return(result);
}

Wish::i32 Wish::Wish_Memory_B_MergeIfPossible(wish_memory_block* sentinel, wish_memory_block* blockA, wish_memory_block* blockB)
{
	if (blockA == sentinel ||
		blockB == sentinel ||
		blockA->Flags & Memory_Used ||
		blockB->Flags & Memory_Used) {
		return (0);
	}
	uint8* expectedSecond = (uint8*)(blockA + sizeof(wish_memory_block) + blockA->Size);
	if ((uint8*)blockB != expectedSecond)
	{
		return(0);
	}
	//Merge into a
	blockA->Size += (blockB->Size + sizeof(wish_memory_block));
	blockA->Next = blockB->Next;
	blockB->Next->Prev = blockA;
	if (!blockB->Next)
	{
		ASSERT(false);
	}
	//Delete block, null the memory
	blockB->Flags = 0;
	blockB->Next = 0;
	blockB->Prev = 0;
	blockB->Size = 0;
	return(1);
}

void Wish::Wish_Memory_B_Free(wish_memory_block* sentinel, void* memory)
{
	wish_memory_block* block = ((wish_memory_block*)memory - 1);
	block->Flags &= ~Memory_Used;

	//Merge
	if (Wish_Memory_B_MergeIfPossible(sentinel, block->Prev, block))
	{
		block = block->Prev;
	}
	Wish_Memory_B_MergeIfPossible(sentinel, block, block->Next);
}

void* Wish::Wish_Memory_B_Alloc(wish_memory_block* sentinel, u64 size)
{
	size += sizeof(wish_memory_block);
	void* result = 0;
	for (;;) {
		wish_memory_block* block = Wish_Memory_B_FindBlockForSize(sentinel, size);
		if (block && (size <= block->Size))
		{
			block->Flags |= Memory_Used;
			result = (void*)((uint8*)block + sizeof(wish_memory_block));
			uint64 remainingSize = block->Size - size;
			uint64 blockSplitThreshold = 4096; //1 page
			if (remainingSize > blockSplitThreshold)
			{
				block->Size -= remainingSize;
				Wish_Memory_B_InsertBlock(block, remainingSize, (void*)((uint8*)result + size));
			}
			break;
		}
		else
		{
			//Evict assets
			break;
		}
	}
	return result;
}