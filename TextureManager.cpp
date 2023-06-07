#include "TextureManager.h"
#include "RapidJSONLoaderUtils.h"			//For frame loading

#include "Game.h"							//Manager Access
#include "GeneralUtils.h"					//String conversion

using namespace DirectX;

void TextureManager::LoadNewSpriteTexture(std::string& name, std::wstring& filePath, size_t handle,
	ID3D12Device* device, ResourceUploadBatch& resourceUpload, DescriptorHeap* heap, bool coreLoad)
{
	std::unique_ptr<SpriteTexture> newTex = std::make_unique<SpriteTexture>();

	LoadFile(newTex, name, filePath, handle, device, resourceUpload, heap);
	
	//Check where to store texture, then store
	if (coreLoad)
		m_CoreTexturesMap[name] = std::move(newTex);
	else
		m_ContextTextures[name] = std::move(newTex);
}

void TextureManager::LoadNewSpriteTexture(std::string& name, std::wstring& filePath, std::string framesFilePath, size_t handle,
	ID3D12Device* device, DirectX::ResourceUploadBatch& resourceUpload, DirectX::DescriptorHeap* heap, bool coreLoad)
{
	std::unique_ptr<SpriteTexture> newTex = std::make_unique<SpriteTexture>();

	LoadFile(newTex, name, filePath, handle, device, resourceUpload, heap);
	LoadFramesAndAnimations(newTex, framesFilePath);

	//Check where to store texture, then store
	if (coreLoad)
		m_CoreTexturesMap[name] = std::move(newTex);
	else
		m_ContextTextures[name] = std::move(newTex);;
}

void TextureManager::LoadTexturesFromFile(std::string& texManifestFP, DirectX::ResourceUploadBatch& resourceUpload,  unsigned& srvCount)
{
	//Get game for device access
	Game* game = Game::GetGame();

	rapidjson::Document textureManifestDoc;
	ParseNewJSONDocument(textureManifestDoc, texManifestFP);

	//Loads textures to container, increments srvCount with each texture
	for (unsigned i(0); i < textureManifestDoc["Textures"].GetArray().Size(); ++i)
	{
		//If the texture has frame/animation data, load this way
		if (textureManifestDoc["Textures"][i].HasMember("Frames Filepath"))
		{
			//Load texture
			LoadNewSpriteTexture(
				std::string(textureManifestDoc["Textures"][i]["Texture Name"].GetString()),
				std::wstring(StringtoWString(std::string(textureManifestDoc["Textures"][i]["Texture Filepath"].GetString()))),
				std::string(textureManifestDoc["Textures"][i]["Frames Filepath"].GetString()),
				srvCount++,
				game->GetD3DDevice().Get(),
				resourceUpload,
				game->GetDescriptorHeap().get()
			);
		}
		//No frames, so load differently
		else
		{
			//Load texture
			LoadNewSpriteTexture(
				std::string(textureManifestDoc["Textures"][i]["Texture Name"].GetString()),
				std::wstring(StringtoWString(std::string(textureManifestDoc["Textures"][i]["Texture Filepath"].GetString()))),
				srvCount++,
				game->GetD3DDevice().Get(),
				resourceUpload,
				game->GetDescriptorHeap().get()
			);
		}
	}
}

bool TextureManager::ClearContextTextureFromMap(std::string& texName)
{
	SpriteTextureMap::iterator it = m_ContextTextures.find(texName);

	//if texture found, erase
	if (it != m_ContextTextures.end())
	{
		m_ContextTextures.erase(it);
		return true;
	}
	else
		return false;

	return false;
}

bool TextureManager::ClearCoreTextureFromMap(std::string& texName)
{
	return false;
}

std::shared_ptr<SpriteTexture> TextureManager::GetSpriteTextureData(std::string& name, bool searchCoreTextures)
{
	//Check what container to search in
	if (searchCoreTextures)
	{
		SpriteTextureMap::iterator it = m_CoreTexturesMap.find(name);
		if (it != m_CoreTexturesMap.end())
			return it->second;
		else
			return nullptr;
	}
	else
	{
		SpriteTextureMap::iterator it = m_ContextTextures.find(name);

		if (it != m_ContextTextures.end())
			return it->second;
		else
			return nullptr;
	}
}

void TextureManager::LoadFile(std::unique_ptr<SpriteTexture>& newTex, std::string& name, std::wstring& filePath, size_t handle,
	ID3D12Device* device, DirectX::ResourceUploadBatch& resourceUpload, DirectX::DescriptorHeap* heap)
{
	//Create a new Resource
	Microsoft::WRL::ComPtr<ID3D12Resource> texture;

	//Do a little conversion for the following call
	const wchar_t* fp = filePath.c_str();

	//Create texture
	ThrowIfFailed(CreateDDSTextureFromFile(
		device,
		resourceUpload,
		fp,
		texture.ReleaseAndGetAddressOf())
	);

	//Create a SRV for this new texture
	CreateShaderResourceView(
		device,
		texture.Get(),
		heap->GetCpuHandle(handle)
	);

	//Create new SpriteTexture object, populate and move into container

	newTex->m_Filename = filePath;
	newTex->m_Resource = texture;
	newTex->m_SrvIndex = handle;
}

void TextureManager::LoadFramesAndAnimations(std::unique_ptr<SpriteTexture>& newTex, std::string& framesFilePath)
{
	//Parse file
	rapidjson::Document doc;
	ParseNewJSONDocument(doc, framesFilePath);

	//Load individual frame data
	newTex->m_Frames.reserve(doc["frames"].GetArray().Size());
	for (auto& a : doc["frames"].GetArray())
	{
		//Store new frame
		RECT newFrame = {
			a["frame"]["x"].GetInt(),
			a["frame"]["y"].GetInt(),
			a["frame"]["x"].GetInt() + a["frame"]["w"].GetInt(),
			a["frame"]["y"].GetInt() + a["frame"]["h"].GetInt()
		};
		newTex->m_Frames.push_back(newFrame);
		
		DirectX::XMFLOAT2 origin;
		//If pivoting enabled, store origin value adjusted by pivot
		if (a.HasMember("pivot"))
		{
			//Store accociated origin
			origin =
			{
				a["spriteSourceSize"]["w"].GetFloat() * a["pivot"]["x"].GetFloat(),
				a["spriteSourceSize"]["h"].GetFloat() * a["pivot"]["y"].GetFloat()
			};
		}
		//Not pivot assigned, so do a safe adjustment to centre for origin data
		else
		{
			origin = 
			{
				a["spriteSourceSize"]["w"].GetFloat() * 0.5f,
				a["spriteSourceSize"]["h"].GetFloat() * 0.5f
			};
		}

		newTex->m_Origins.push_back(origin);
	}

	//Load animations if doc member is found
	if (doc.HasMember("Animations"))
	{
		//Reserve space of animations
		newTex->m_Animations.reserve(doc["Animations"].GetArray().Size());
		
		//For each animation
		for (unsigned int i(0); i < doc["Animations"].Size(); ++i)
		{
			AnimationData newAnim;

			//Animator manages and uses the data different 
			AnimationData::AnimID animID = (AnimationData::AnimID)doc["Animations"][i]["Animation Type ID"].GetUint();
			switch (animID)
			{
			case(AnimationData::AnimID::LINEAR_FRAMES):
				newAnim.m_Name = doc["Animations"][i]["Animation Name"].GetString();
				newAnim.m_TypeID = (AnimationData::AnimID)doc["Animations"][i]["Animation Type ID"].GetUint();

				newAnim.m_StartFrame = doc["Animations"][i]["Animation Start"].GetInt();
				newAnim.m_EndFrame = doc["Animations"][i]["Animation End"].GetInt();
				newAnim.m_Speed = doc["Animations"][i]["Animation Speed"].GetFloat();
				break;

			case(AnimationData::AnimID::NON_LINEAR_FRAMES):
				newAnim.m_Name = doc["Animations"][i]["Animation Name"].GetString();
				newAnim.m_TypeID = (AnimationData::AnimID)doc["Animations"][i]["Animation Type ID"].GetUint();
				newAnim.m_Speed = doc["Animations"][i]["Animation Speed"].GetFloat();

				//Store animation frame indexes
				for (auto& f : doc["Animations"][i]["Animation Frames"].GetArray())
					newAnim.m_FrameIndexes.push_back(f.GetInt());

				break;
			}
			newTex->m_Animations.push_back(newAnim);
		}
	}
}

void TextureManager::Release()
{
	m_CoreTexturesMap.clear();
	m_ContextTextures.clear();
}





