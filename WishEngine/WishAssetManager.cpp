#include "stdafx.h"

//Mandatory include?
#include "Wish.h"
#include "WishAssetManager.hpp"

class UpdateListener : public FW::FileWatchListener
{
private:

public:
	void handleFileAction(FW::WatchID watchid, const std::string& dir, const std::string& filename,
		FW::Action action)
	{
		switch (action)
		{
		case FW::Actions::Add:
			//std::cout << "File (" << dir + "\\" + filename << ") Added! " << std::endl;
			break;
		case FW::Actions::Delete:
			//std::cout << "File (" << dir + "\\" + filename << ") Deleted! " << std::endl;
			break;
		case FW::Actions::Modified:
			//std::cout << "File (" << dir + "\\" + filename << ") Modified! " << std::endl;
			Wish_AssetManager_FileUpdate(dir.c_str(), filename.c_str());
			break;
		default:
			printf("Should never happen!\n");
		}
	};
};

void Wish_Asset_SetName(wish_asset_node* node, char* name)
{
	//
	if (strlen(name) < WISH_ASSET_NAME_SIZE)
	{
		strcpy_s(node->Name, name);
	}
	else
	{
		throw runtime_error("Asset name too long");
	}
}

void Wish_Asset_Insert(wish_asset_node* root, wish_asset_node* node)
{
	//Generate UID based on name using a hashing function
	u32 seed = 1337;

	//Assume the possibility of a collision is close to zero!
	node->HashId = Wish_Hash(node->Name);
	//Insert into the back of the list
	wish_asset_node* prev = root->Prev;
	//Forward
	node->Next = prev->Next;
	prev->Next->Prev = node;
	prev->Next = node;	
	node->Prev = prev;
	//This is good insert!
}

wish_asset_node* Wish_Asset_Find(wish_asset_node* root, const char* id)
{
	wish_asset_node* ret = NULL;
	//iterate through everything and do cmps!
	wish_asset_node* curr = root->Next;
	//The list is circular, so if we find ourselves then we did a whole loop
	while (curr != NULL && curr != root)
	{
		if (strcmp(id, curr->Name) == 0) {
			ret = curr;
			break;
		}
		curr = curr->Next;
	}
	return ret;
}

wish_asset_node* Wish_Asset_Find(wish_asset_node* root, wish_murmur3_hash hash)
{
	wish_asset_node* ret = NULL;
	//iterate through everything and do cmps!
	wish_asset_node* curr = root->Next;
	//The list is circular, so if we find ourselves then we did a whole loop
	while (curr != NULL && curr != root)
	{
		if (curr->HashId == hash) {
			ret = curr;
			break;
		}
		curr = curr->Next;
	}
	return ret;
}


b32 Wish::Wish_Asset_CreateTexture(const char* id, u32 width, u32 height, PixelFormat pixelFormat)
{
	wish_assetmanager_context& assetManager = Wish_Engine_GetContext()->AssetManager;
	if (!Wish_Asset_GetTexture(id)) {
		//Alloc a new node
		wish_asset_texture* asset = (wish_asset_texture*)Wish_Memory_Alloc(MemoryType::LONG_LIFE, sizeof(wish_asset_texture));
		ZeroMemory(asset, sizeof(wish_asset_texture));

		Wish_Texture_Create(&asset->Texture, width, height, pixelFormat, pixelFormat, GL_UNSIGNED_BYTE);

		strcpy_s(asset->Base.Name, id);
	}
	return true;
}

wish_texture* Wish::Wish_Asset_LoadTexture(const char* file)
{
	wish_assetmanager_context& assetManager = Wish_Engine_GetContext()->AssetManager;
	//Gen random name
	return Wish_Asset_LoadTexture("tex_", file);
}

wish_texture* Wish::Wish_Asset_LoadTexture(const char* id, const char* file)
{
	wish_assetmanager_context& assetManager = Wish_Engine_GetContext()->AssetManager;
	//Do we have any assets with this name already?
	//Hash our id
	//Murmur3Hash hash = Wish_Hash(id.c_str());
	wish_murmur3_hash hash = Wish_Hash(id);
	wish_asset_texture* asset = (wish_asset_texture*)Wish_Asset_Find(&assetManager.RootTexture, hash);
	if (asset == NULL)
	{
		//Alloc a new node
		asset = (wish_asset_texture*)Wish_Memory_Alloc(MemoryType::LONG_LIFE, sizeof(wish_asset_texture));
		ZeroMemory(asset, sizeof(wish_asset_texture));

		if (Wish_Texture_DEBUGLoadTexture(&asset->Texture, file)) {
			//I don't like these things, they feel like dyn alloc things
			//OH WELL
			
			//Wrap in std string
			std::string sfile(file);
			size_t _last_slash = sfile.find_last_of("/\\");
			ASSERT((_last_slash != std::string::npos));
			std::string _file = sfile.substr(_last_slash + 1);
			printf("LoadTexture \"%s\"\n", _file.c_str());

			//asset->Base.Folder = folder;
			strcpy_s(asset->Base.FileName, _file.c_str());
			strcpy_s(asset->Base.Name, id);

			Wish_Asset_Insert(&assetManager.RootTexture, &asset->Base);

			//Hmm
			assetManager.m_TexCounter++;
		}
		else
		{
			//Just free the memory we allocated, this texture is not usable
			Wish_Memory_Free((void*)asset);
			asset = NULL;
			printf("Unable to load texture: %s\n", file);
		}
	}
	else
	{
		throw runtime_error("Dupl Asset");
	}
	return asset == NULL ? NULL : &asset->Texture;
}

wish_texture* Wish::Wish_Asset_GetTexture(const char* id)
{
	wish_assetmanager_context& assetManager = Wish_Engine_GetContext()->AssetManager;
	wish_murmur3_hash hash = Wish_Hash(id);
	wish_asset_texture* asset = (wish_asset_texture*)Wish_Asset_Find(&assetManager.RootTexture, hash);
	return asset == NULL ? NULL : &asset->Texture;
}

wish_shader_program* Wish::Wish_Asset_LoadShader(const char* id, const char* file)
{
	wish_assetmanager_context& assetManager = Wish_Engine_GetContext()->AssetManager;
	wish_murmur3_hash hash = Wish_Hash(id);
	wish_asset_shaderprogram* asset = (wish_asset_shaderprogram*)Wish_Asset_Find(&assetManager.RootShaderProgram, hash);
	if (asset == NULL)
	{
		//Alloc a new node
		asset = (wish_asset_shaderprogram*)Wish_Memory_Alloc(MemoryType::LONG_LIFE, sizeof(wish_asset_shaderprogram));
		ZeroMemory(asset, sizeof(wish_asset_shaderprogram));

		
		//Wrap in std string
		std::string sfile(file);
		size_t _last_slash = sfile.find_last_of("/\\");
		ASSERT((_last_slash != std::string::npos));
		std::string _file = sfile.substr(_last_slash + 1);
		printf("LoadShader \"%s\"\n", _file.c_str());

		//asset->Base.Folder = folder;
		strcpy_s(asset->Base.FileName, _file.c_str());
		strcpy_s(asset->Base.Name, id);

		//TODO: ALLOC DIFFERENTLY
		wish_shader_program* pProgram = (wish_shader_program*)Wish_Memory_Alloc(sizeof(wish_shader_program));
		//->
		asset->ShaderProgram = pProgram;
		Wish_ShaderProgram_Parse(pProgram, file);

		Wish_Asset_Insert(&assetManager.RootShaderProgram, &asset->Base);
	}
	return asset == NULL ? NULL : asset->ShaderProgram;
}

wish_shader_program* Wish::Wish_Asset_GetShader(const char* id)
{
	wish_assetmanager_context& assetManager = Wish_Engine_GetContext()->AssetManager;
	wish_murmur3_hash hash = Wish_Hash(id);
	wish_asset_shaderprogram* asset = (wish_asset_shaderprogram*)Wish_Asset_Find(&assetManager.RootShaderProgram, hash);
	return asset == NULL ? NULL : asset->ShaderProgram;
}

void Wish::Wish_AssetManager_Init()
{
	wish_assetmanager_context& assetManager = Wish_Engine_GetContext()->AssetManager;
	// Create the object
	//assetManager.m_pFileWatcher = new FW::FileWatcher();

	//Introducing MemLeak 2k15
	//UpdateListener* ul = new UpdateListener();

	// add a directory watch
	//FW::WatchID watchid = assetManager.m_pFileWatcher->addWatch("./data/shaders/", ul);
	//FW::WatchID watchid2 = assetManager.m_pFileWatcher->addWatch("./data/textures/", ul);

	//Wrap the root nodes around themselves
	ZeroMemory(&assetManager.RootTexture, sizeof(wish_asset_node));
	assetManager.RootTexture.Next = &assetManager.RootTexture;
	assetManager.RootTexture.Prev = &assetManager.RootTexture;
	ZeroMemory(&assetManager.RootShaderProgram, sizeof(wish_asset_node));
	assetManager.RootShaderProgram.Next = &assetManager.RootShaderProgram;
	assetManager.RootShaderProgram.Prev = &assetManager.RootShaderProgram;
}


void Wish::Wish_AssetManager_FileUpdate(const char* dir, const char* file) {
	/*wish_assetmanager_context& assetManager = Wish_Engine_GetContext()->AssetManager;
	//Find file
	auto iter = assetManager.m_mTrackedTextures.find(file);
	if (iter != assetManager.m_mTrackedTextures.end())
	{
		//Supposedly reload this texture! but for now ignore :D
		//printf("Found file update for %s\n", file.c_str());

		auto iterTexture = assetManager.m_mTextures.find(iter->second);

		if (iterTexture != assetManager.m_mTextures.end()) {
			printf("FileUpdate: %s\n", (dir + file).c_str());
			Wish_Texture_DEBUGLoadTexture(iterTexture->second, dir + file);
		}
		else {
			throw new runtime_error("Unexpected event.");
		}
	}
	else {

		//Try to find a shader update
		auto iter = assetManager.m_mTrackedShaders.find(file);
		if (iter != assetManager.m_mTrackedShaders.end()) {

			auto iterShader = assetManager.m_mShaders.find(iter->second);
			if (iterShader != assetManager.m_mShaders.end()) {
				printf("ShaderUpdate: %s\n", (dir + file).c_str());

				//Raw load this shader
				wish_shader_program* program = new wish_shader_program();

				Wish_ShaderProgram_Parse(program, dir + file);

				if (program->glHandle > 0)
				{
					//W0h00

					//Delete current program
					Wish_ShaderProgram_Delete(iterShader->second);

					//Copy over relevant data
					iterShader->second->glHandle = program->glHandle;
					iterShader->second->m_UniformData = program->m_UniformData;

					//We don't want to delete the new program, which will be done after
					//delete program;
					program->glHandle = -1;
				}

				//Delete the program
				delete program;
			}
		}
	}*/
}

void Wish::Wish_AssetManager_Update()
{
	wish_assetmanager_context& assetManager = Wish_Engine_GetContext()->AssetManager;
	// somewhere in your update loop call update
	/*if (assetManager.m_pFileWatcher != 0) {
		assetManager.m_pFileWatcher->update();
	}*/
}