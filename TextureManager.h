#pragma once

#include "D3DUtils.h"				  //Get Types
#include "RapidJSONLoaderUtils.h"

//Type defs
typedef std::unordered_map<std::string, std::shared_ptr<SpriteTexture>> SpriteTextureMap;

/*
	Dedicated Manager for loading and storing texture resources for the game.
*/
class TextureManager
{
public:

	TextureManager() {}
	~TextureManager() { Release(); }

	//////////////////
	/// Operations ///
	//////////////////

	//Call this ONCE for each texture you want to load
	void LoadNewSpriteTexture(
		std::string& name,
		std::wstring& filePath,
		size_t handle,
		ID3D12Device* device,
		DirectX::ResourceUploadBatch& resourceUpload,
		DirectX::DescriptorHeap* heap,
		bool coreLoad = false
	);
	//Call this ONCE for each texture with frames you want to load (Use "Free Texture Packer" JSON output)
	void LoadNewSpriteTexture(
		std::string& name,
		std::wstring& filePath,
		std::string framesFilePath,
		size_t handle,
		ID3D12Device* device,
		DirectX::ResourceUploadBatch& resourceUpload,
		DirectX::DescriptorHeap* heap,
		bool coreLoad = false
	);

	//Loads files worth of textures from file (Refer to "Texture_Manifest_00" for example structure)
	void LoadTexturesFromFile(std::string& texManifestFP, DirectX::ResourceUploadBatch& resourceUpload, unsigned& srvCount);


	//Clears the level by level textures
	void ClearAllContextTextures() { m_ContextTextures.clear(); }
	//Clears the core textures (do this carefully or sparingly, if you find your calling this a lot then reevaluate loading manifests)
	void ClearAllCoreTextures() { m_CoreTexturesMap.clear(); }
	//Clears specific entry from context textures
	bool ClearContextTextureFromMap(std::string& texName);
	//Clears specific entry from core textures 
	bool ClearCoreTextureFromMap(std::string& texName);

	///////////
	/// Get ///
	///////////

	//Gets sprite texture by name and returns the pointer to the data
	std::shared_ptr<SpriteTexture> GetSpriteTextureData(std::string& name, bool searchCoreTextures = false);

	//Get the number of textures loaded
	size_t GetSprTexCount() { return m_ContextTextures.size(); }

	
private:

	//////////////////
	/// Operations ///
	//////////////////
	
	//Shared Loading Behavior
	void LoadFile(std::unique_ptr<SpriteTexture>& newTex, std::string& name, std::wstring& filePath, size_t handle,
		ID3D12Device* device, DirectX::ResourceUploadBatch& resourceUpload, DirectX::DescriptorHeap* heap);
	void LoadFramesAndAnimations(std::unique_ptr<SpriteTexture>& newTex, std::string& framesFilePath);

	void Release();

	////////////
	/// Data ///
	////////////

	//Core, program-length texture storage for shared/universal textures (loading UIs, general menus etc)
	SpriteTextureMap m_CoreTexturesMap;
	//Storage
	SpriteTextureMap m_ContextTextures;
};