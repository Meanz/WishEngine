#include "stdafx.h"
#include "Wish.h"
#include "WishModel.h"

namespace Wish
{

	b32 Wish_Model_Save(wish_model* model, const char* fileName)
	{
		b32 result = false;
		//Fill the model header
		
		return 0;
	}

	wish_model* Wish_Model_Load(const char* fileName)
	{
		wish_model* result = NULL;
		//Read all data from file
		u8* data = Wish_IO_ReadFile(fileName);
		if (data)
		{
			//Put the file thing into one of our structs
			result = (wish_model*)Wish_Memory_Alloc(sizeof(wish_model));
			

			result->header = *(wish_model_header*)data;

			for (u32 i = 0; i < result->header.NumMeshes; i++)
			{
			}

			//Free the data
			Wish_Memory_Free(data);
		}
		else
		{
			printf("Unable to load model %s.\n", fileName);
		}
		return result;
	}

}