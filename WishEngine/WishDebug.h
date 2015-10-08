#pragma once

#if !defined(WISH_DEBUG_H)

//This only works per compilation unit, because of counter
#define TIMED_BLOCK__(Number, ...) timed_block TimedBlock_##Number(__COUNTER__, __FILE__, __LINE__, __FUNCTION__, ## __VA_ARGS__);
#define TIMED_BLOCK_(Number, ...) TIMED_BLOCK__(Number, ## __VA_ARGS__)
#define TIMED_BLOCK(...) TIMED_BLOCK_(__LINE__, ## __VA_ARGS__)

struct wish_debug_record
{
	u64 Timestamp;
	char* FileName;
	char* FunctionName;
	u32 LineNumber;
	u64 Clocks;
	u32 HitCount;
};

//FWD Declaration
wish_debug_record DebugRecordArray[];

struct wish_timed_block
{
	wish_debug_record* Record;
	uint64 StartCycleCount;

	wish_timed_block(i32 counter, char* fileName, i32 lineNumber, char* functionName, u32 HitCount = 1)
	{
		Record = DebugRecordArray + counter;
		Record->FileName = fileName;
		Record->LineNumber = lineNumber;
		Record->FunctionName = functionName;
		//Record->Clocks = __rdtsc();
		Record->HitCount += HitCount;
	}

	~wish_timed_block()
	{
		//Record->Clocks += __rdtsc();
	}
};

#define WISH_DEBUG_H
#endif