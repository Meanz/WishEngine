#pragma once

//This one is kind of unique
#include "FileWatcher.h"

#define DATA_DIRECTORY "./data/"

namespace Wish
{

#define WISH_ASSET_NAME_SIZE 64
#define WISH_ASSET_FILENAME_SIZE 128
#define WISH_ASSET_FOLDER_SIZE 512

	struct wish_asset_folder
	{
		//Id used for fast indexing
		u32 Id;
		u32 HashId;
		//
		char Path[WISH_ASSET_FOLDER_SIZE];
	};

	struct wish_asset_node
	{
		//
		u32 Id;
		wish_murmur3_hash HashId;
		//Header
		wish_asset_node* Next;
		wish_asset_node* Prev;
		//Asset Folder
		wish_asset_folder* Folder;
		//Asset FileName (AbsPath: Folder + FileName)
		char FileName[WISH_ASSET_FILENAME_SIZE];
		//The internal name for the Asset
		char Name[WISH_ASSET_NAME_SIZE];
	};

	struct wish_asset_texture
	{
		wish_asset_node Base;
		wish_texture Texture;
	};

	struct wish_asset_shaderprogram
	{
		wish_asset_node Base;
		wish_shader_program* ShaderProgram;
	};

	struct wish_assetmanager_context
	{
		u32 m_TexCounter;
		wish_asset_node RootTexture;
		wish_asset_node RootShaderProgram;
	};

	__Wish_Export b32 Wish_Asset_CreateTexture(const char* id, u32 width, u32 height, PixelFormat pixelFormat);
	__Wish_Export wish_texture* Wish_Asset_LoadTexture(const char* file);
	__Wish_Export wish_texture* Wish_Asset_LoadTexture(const char* id, const char* file);
	__Wish_Export wish_texture* Wish_Asset_GetTexture(const char* id);

	__Wish_Export wish_shader_program* Wish_Asset_LoadShader(const char* id, const char* file);
	__Wish_Export wish_shader_program* Wish_Asset_GetShader(const char* id);

	void Wish_AssetManager_Init();
	void Wish_AssetManager_FileUpdate(const char* dir, const char* fileName);
	void Wish_AssetManager_Update();
}