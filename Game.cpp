#include "Game.h"

#include "DDSTextureLoader.h"
#include "GeneralUtils.h"

//////////////
/// Modes ////
//////////////
#include "Mode_SplashScreen.h"
#include "Mode_MainGame.h"
#include "Mode_MainMenu.h"
using namespace DirectX;

Game* Game::m_Game = nullptr;
Game::Game(HINSTANCE hInstance)
	: D3D12Application(hInstance)
{
	assert(m_Game == nullptr);
	m_Game = this;
}

Game::~Game()
{
	if (m_D3DDevice != nullptr)
		FlushCommandQueue();
}

bool Game::Initialize()
{
	/////////////////
	/// Pre-amble ///
	/////////////////

	//Call parent Init for window/d3d setup
	if (!D3D12Application::Initialize())
		return false;
	//Reset CL for init
	ThrowIfFailed(m_CommandList->Reset(m_DirectCommandListAllocator.Get(), nullptr));
	//Get the increment size of a descriptor for this heap type.
	//Hardware specific, so need to query
	m_CbvSrvUavDescriptorSize = m_D3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


	//////////////////////////////
	/// Primary Initialisation ///
	//////////////////////////////

	//Loads up the master manifest for filepath information for following setups
	rapidjson::Document masterManifestDoc;
	ParseNewJSONDocument(masterManifestDoc, std::string(MASTER_MANIFEST_FP));

	//Init XTK, load textures & fonts
	
	//Initialises the XTK portion of the engine (fonts, sprites etc). 
	//Load base line textures and fonts here (To-Do: Make a texture loading function so can do actual loads between levels).
	XTKSetup(
		std::string(masterManifestDoc["Textures"][0]["Manifest Filepath"].GetString()),
		std::string(masterManifestDoc["Fonts"][0]["Manifest Filepath"].GetString())
	);

	//Build frame resources
	BuildFrameResources();

	//Give pass data a heap pointer
	m_PassData.m_Heap = m_ResourceDescriptors.get();


	//////////////////
	/// Post-Setup ///
	//////////////////

	//Execute init commands
	ThrowIfFailed(m_CommandList->Close());
	ID3D12CommandList* cmdsList[] = { m_CommandList.Get() };
	m_CommandQueue->ExecuteCommandLists(_countof(cmdsList), cmdsList);

	//Wait till init is complete
	FlushCommandQueue();

	/////////////////////
	/// Manager Setup ///
	/////////////////////
	
	//Setup the core managers
	SetupCoreManagers(masterManifestDoc);
	//Setup the optional managers + core manager configuations
	SetupOptionalManagers(masterManifestDoc);

	//Set starting mode
	m_ModeManager.SwitchMode(ModeNames::MODE_SPLASH_SCREEN);

	//End Init
	return true;
}

void Game::Update(GameTimer& gt)
{
	//Cycle through the circular frame of frame resources
	m_CurrentFrameResourceIndex = (m_CurrentFrameResourceIndex + 1) % NUM_FRAME_RESOURCES;
	m_CurrentFrameResource2D = m_FrameResources2D[m_CurrentFrameResourceIndex].get();

	//Has the GPU finished proccessing the commands of the current frame resource?
	//If not, wait here till GPU has completed commands upto this fence point
	if (m_CurrentFrameResource2D->Fence != 0 && m_Fence->GetCompletedValue() < m_CurrentFrameResource2D->Fence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
		ThrowIfFailed(m_Fence->SetEventOnCompletion(m_CurrentFrameResource2D->Fence, eventHandle));
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}

	//Update Controller States
	m_Gamepad.Update();
	//Update Current Mode
	m_ModeManager.Update(gt);
}

void Game::Render(GameTimer& gt)
{
	//Run pre-frame setup
	PreFrame();

	ID3D12DescriptorHeap* heaps[] = { m_ResourceDescriptors->Heap(), m_CommonStates->Heap() };
	m_CommandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

	//Call render for current mode (NOTE: Modes are responsible for starting end ending Spritebatches calls themselves)
	m_ModeManager.Render(m_PassData);

	//Run post frame clean up, flip the buffer etc.
	PostFrame();
	CycleFrame();
	m_KBM.PostProcess();


	//Commit SB Data
	m_GraphicsMemory->Commit(m_CommandQueue.Get());
}

void Game::PreFrame()
{

	//Get the Current Frames Allocator
	auto cmdListAlloc = m_CurrentFrameResource2D->CmdListAlloc;

	//Reuse the memory associated with command recording
	//We can only reset when the associated command lists have finished execution on the GPU.
	ThrowIfFailed(cmdListAlloc->Reset());

	//A CL can be reset after it has been added to the command queue via ExecutCommandList
	//Reusing the CL reuses memory
	ThrowIfFailed(m_CommandList->Reset(cmdListAlloc.Get(), nullptr));

	//Indicate a state transition on the resource usage
	m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		m_SwapChainBuffer[m_CurrentBackBuffer].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	//Clear views
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHdl = {
		m_RTVHeap->GetCPUDescriptorHandleForHeapStart(),
		m_CurrentBackBuffer,
		m_RTVDescriptorSize };
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHdl(m_DSVHeap->GetCPUDescriptorHandleForHeapStart());

	//Clear the back and depth buffers
	m_CommandList->ClearRenderTargetView(rtvHdl, Colors::Black, 0, nullptr);
	m_CommandList->ClearDepthStencilView(dsvHdl, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	//Specify the buffers being rendered to
	m_CommandList->OMSetRenderTargets(1, &rtvHdl, FALSE, &dsvHdl);

	//Set viewport and scissor rects
	m_CommandList->RSSetViewports(1, &m_ScreenViewport);
	m_CommandList->RSSetScissorRects(1, &m_ScissorRect);

}

void Game::PostFrame()
{
	//Indicate a state transition on the resource usage
	m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		m_SwapChainBuffer[m_CurrentBackBuffer].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT));

	//Done recording commands
	ThrowIfFailed(m_CommandList->Close());

	//Add CL to the queue for execution
	ID3D12CommandList* cmdsList[] = { m_CommandList.Get() };
	m_CommandQueue->ExecuteCommandLists(_countof(cmdsList), cmdsList);

	//Swap the back and front buffers
	ThrowIfFailed(m_SwapChain->Present(0, 0));
}

void Game::CycleFrame()
{
	//Swap current buffer target
	m_CurrentBackBuffer = (m_CurrentBackBuffer + 1) % SWAP_CHAIN_BUFFER_COUNT;

	//Advance the current fence value to mark the commands upto this point
	m_CurrentFrameResource2D->Fence = ++m_CurrentFence;

	/*
		Add an instruction to the CQ to set a new fence point.
		Because we are the GPU's timeline, the new fence point won't be set until
		the GPU finishes processing all the commands prior to this Signal().
	*/
	m_CommandQueue->Signal(m_Fence.Get(), m_CurrentFence);

}

void Game::XTKSetup(std::string& textureFP, std::string& fontFP)
{
	//Setup graphics memory (used in spritebatch)
	m_GraphicsMemory = std::make_unique<GraphicsMemory>(m_D3DDevice.Get());

	//Create a number of descriptors equal to the expected descriptor count
	m_ResourceDescriptors = std::make_unique<DescriptorHeap>(m_D3DDevice.Get(), RESOURCE_HEAP_RESERVE_STANDARD);
	//Setup common states
	m_CommonStates = std::make_unique<CommonStates>(m_D3DDevice.Get());

	//XTK Management of resources for textures and SpriteBatch
	ResourceUploadBatch resourceUpload(m_D3DDevice.Get());

	//Load Assets From Here
	resourceUpload.Begin();

	//Loads all fonts at start up and initial textures
	LoadInitialTextures(resourceUpload, textureFP, fontFP);

	//Setup spritebatches
	SetupSpritebatches(resourceUpload);

	//End of setup and loading
	auto uploadResourcesFinished = resourceUpload.End(m_CommandQueue.Get());
	uploadResourcesFinished.wait();

}

void Game::LoadInitialTextures(DirectX::ResourceUploadBatch& resourceUpload, std::string& textureFP, std::string& fontFP)
{
	//Parse out font file
	rapidjson::Document fontDoc;
	ParseNewJSONDocument(fontDoc, fontFP);

	//Track and increment each time a resource is loaded
	unsigned count = 0;

	//Load fonts
	for (unsigned i(0); i < fontDoc["Fonts"].GetArray().Size(); ++i)
	{
		std::wstring fp = StringtoWString(std::string(fontDoc["Fonts"][i]["Font Filepath"].GetString()));

		m_PassData.m_Spritefonts.push_back(std::make_unique<SpriteFont>(
			m_D3DDevice.Get(),
			resourceUpload,
			fp.c_str(),
			m_ResourceDescriptors->GetCpuHandle(count),
			m_ResourceDescriptors->GetGpuHandle(count)
			));

		++count;
	}

	//Store the SRV count where fonts end (only textures should be beyond this point, and all fonts loaded ONCE)
	postFontSrvCount = count - 1;

	//Loads all textures in stored in the file given
	m_TextureManager.LoadTexturesFromFile(
		textureFP,
		resourceUpload,
		count
	);

	//Store the total SRV count in use
	totalSrvCount = count;
}

void Game::SetupSpritebatches(DirectX::ResourceUploadBatch& resourceUpload)
{
	/////////////////////////////////////////////////////////////////
	/// These are default SB Setups, replace/remove at users need ///
	/////////////////////////////////////////////////////////////////

	//Setup Spritebatches for use (Using default settings)
	RenderTargetState rtState(m_BackBufferFormat, DXGI_FORMAT_UNKNOWN);
	//Point clamp
	auto sampler = m_CommonStates->PointWrap();
	SpriteBatchPipelineStateDescription pd(rtState, &CommonStates::NonPremultiplied);

	//Main SB
	m_PassData.m_Spritebatches.push_back(std::make_unique<SpriteBatch>(
		m_D3DDevice.Get(),
		resourceUpload,
		pd
		));

	//Point Wrap SB
	m_PassData.m_Spritebatches.push_back(std::make_unique<SpriteBatch>(
		m_D3DDevice.Get(),
		resourceUpload,
		SpriteBatchPipelineStateDescription(rtState, &CommonStates::NonPremultiplied, nullptr, nullptr, &sampler)
		));

	//Set VP for all SBs
	for (auto& sb : m_PassData.m_Spritebatches)
		sb->SetViewport(m_ScreenViewport);
}

void Game::BuildFrameResources()
{
	for (int i(0); i < NUM_FRAME_RESOURCES; ++i)
	{
		m_FrameResources2D.push_back(std::make_unique<FrameResource2D>(m_D3DDevice.Get()));
	}
}

void Game::SetupCoreManagers(rapidjson::Document& masterManifestDoc)
{	
	////////////////////
	/// Mode Manager ///
	////////////////////

	//Load required modes
	m_ModeManager.AddMode(new Mode_SplashScreen(ModeNames::MODE_SPLASH_SCREEN));
	m_ModeManager.AddMode(new Mode_MainMenu(ModeNames::MODE_MAIN_MENU));
	m_ModeManager.AddMode(new Mode_MainGameScene(ModeNames::MODE_GAME_SCENE));


	/////////////////////
	/// Audio Manager ///
	/////////////////////

	//Initialise manager
	m_AudioManager.InitAudioEngine(m_WinData.m_Window);

	//Load wavebanks that store the games music and SFX
	m_AudioManager.LoadSingleWaveBank(L"data/audio/wavebanks/MusicWavebank.xwb");
	m_AudioManager.LoadSingleWaveBank(L"data/audio/wavebanks/SoundEffectsWavebank.xwb");

	//Create Soundeffect instances for the loopable sounds (just music at the moment)
	//Main menu music
	Game::GetGame()->GetAudioManager().LoadSoundEffectInstanceFromWaveBank((unsigned)WavebankIDs::MUSIC, (unsigned)MusicIDs::MAIN_MENU, GameVolumes::MUSIC);
	// Grassland music
	Game::GetGame()->GetAudioManager().LoadSoundEffectInstanceFromWaveBank((unsigned)WavebankIDs::MUSIC, (unsigned)MusicIDs::GRASSLAND, GameVolumes::MUSIC);
	//Cave music
	Game::GetGame()->GetAudioManager().LoadSoundEffectInstanceFromWaveBank((unsigned)WavebankIDs::MUSIC, (unsigned)MusicIDs::CAVE, GameVolumes::MUSIC);


	
	/////////////////////////////////
	/// Mouse/KB/Gamepad Managers ///
	/////////////////////////////////

	//Initialise managers
	m_KBM.Initialise(m_WinData.m_Window, true, false);
	m_Gamepad.Initialise();

	m_ControlScheme.BindToKBM(&m_KBM);
}

void Game::SetupOptionalManagers(rapidjson::Document& masterManifestDoc)
{
	/////////////////
	/// Game Data ///
	/////////////////

	//Character Data
	m_GameplayManager.GetDataManager().LoadCharacterData(std::string("data/files/Character_Data_00.json"), true);
	//Weapon
	m_GameplayManager.GetDataManager().LoadWeaponData(std::string("data/files/Weapon_Data_00.json"), true);
	//Enemy Data
	m_GameplayManager.GetDataManager().LoadEnemyData(std::string("data/files/Enemy_Data_00.json"), true);
	//Accessory Data
	m_GameplayManager.GetDataManager().LoadAccessoryData(std::string("data/files/Accessory_Data_00.json"), true);
	//Spawn Data
	m_GameplayManager.GetDataManager().LoadSpawnData(std::string("data/files/Spawn_Data_00.json"), true);
	//Object Collision data
	m_GameplayManager.GetDataManager().LoadObjectCollisionData(std::string("data/textures/tilemaps/Grassland_Map_00.json"),true);
	m_GameplayManager.GetDataManager().LoadObjectCollisionData(std::string("data/textures/tilemaps/Cave_Map_00.json"), true);

	//////////////////////
	/// Entity Manager ///
	//////////////////////

	m_GameplayManager.GetEntityManager().InitialiseEntityManager(std::string("data/files/Pool_Data_00.json"));


	/////////////////////
	/// Tilesets/Maps ///
	/////////////////////

	//Grasslands Map
	m_GameplayManager.GetTileManager().LoadMap(SM_TilemapManager::LoaderTypes::MAP_LOADER_01,
		std::string(masterManifestDoc["Level Layouts"][1]["Associated JSON Filepath"].GetString()),
		std::string(masterManifestDoc["Level Layouts"][1]["Tilesets Used"][0]["Associated JSON Filepath"].GetString()),
		std::string(masterManifestDoc["Level Layouts"][1]["Tilesets Used"][0]["Tileset Name"].GetString()), true);
	//Cave Map
	m_GameplayManager.GetTileManager().LoadMap(SM_TilemapManager::LoaderTypes::MAP_LOADER_01,
		std::string(masterManifestDoc["Level Layouts"][2]["Associated JSON Filepath"].GetString()),
		std::string(masterManifestDoc["Level Layouts"][2]["Tilesets Used"][0]["Associated JSON Filepath"].GetString()),
		std::string(masterManifestDoc["Level Layouts"][2]["Tilesets Used"][0]["Tileset Name"].GetString()), true);

	///////////////////////
	/// Spawner Manager ///
	///////////////////////

	m_GameplayManager.GetSpawnManager().InitialiseSpawnData(0);

}

void Game::InitSceneCamera()
{
	
}

LRESULT Game::MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	/*
		Before handling any other messages, we wanna intercept here and check for WM_CHAR (Slow Inputs) and 
		WM_INPUT (Raw Fast Inputs) and pass them to the appropriate function calls.
	*/

	switch (msg)
	{
	case WM_CHAR:
		m_ModeManager.ProcessKey(static_cast<char>(wParam));
		break;

	case WM_INPUT:
		m_KBM.MessageEvent((HRAWINPUT)lParam);
		break;
	}
	
	//Hack fix for alt key release causing the program to freeze due to alt being a menu key
	//This causes knock on effects with delta timing.
	if (wParam == SC_KEYMENU && (lParam >> 16) <= 0)
		return 0;

	//Handle Windows Messages
	switch (msg)
	{
		// WM_ACTIVATE is sent when the window is activated or deactivated.  
		// We pause the game when the window is deactivated and unpause it 
		// when it becomes active.  
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			m_WinData.m_AppPaused = true;
			m_Timer.Stop();
		}
		else
		{
			m_WinData.m_AppPaused = false;
			m_Timer.Start();
		}
		return 0;

		// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		// Save the new client area dimensions.
		m_WinData.m_Width = LOWORD(lParam);
		m_WinData.m_Height = HIWORD(lParam);
		SnapToRes16_9();
		if (m_D3DDevice)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				m_WinData.m_AppPaused = true;
				m_WinData.m_AppMinimized = true;
				m_WinData.m_AppMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				m_WinData.m_AppPaused = false;
				m_WinData.m_AppMinimized = false;
				m_WinData.m_AppMaximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{

				// Restoring from minimized state?
				if (m_WinData.m_AppMinimized)
				{
					m_WinData.m_AppPaused = false;
					m_WinData.m_AppMinimized = false;
					OnResize();
				}

				// Restoring from maximized state?
				else if (m_WinData.m_AppMaximized)
				{
					m_WinData.m_AppPaused = false;
					m_WinData.m_AppMaximized = false;
					OnResize();
				}
				else if (m_WinData.m_AppResizing)
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
				{
					OnResize();
				}
			}
		}
		return 0;

		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		m_WinData.m_AppPaused = true;
		m_WinData.m_AppResizing = true;
		m_Timer.Stop();
		return 0;

		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		m_WinData.m_AppPaused = false;
		m_WinData.m_AppResizing = false;
		m_Timer.Start();
		SetWindowPos(m_WinData.m_Window, HWND_TOP, 0, 0, m_WinData.m_Width, m_WinData.m_Height, SWP_FRAMECHANGED | SWP_NOMOVE);
		OnResize();
		return 0;

		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// The WM_MENUCHAR message is sent when a menu is active and the user presses 
		// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return MAKELRESULT(0, MNC_CLOSE);

		// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

	case WM_DEVICECHANGE:
		if (wParam == DBT_DEVICEARRIVAL)
		{
			auto pDev = reinterpret_cast<PDEV_BROADCAST_HDR>(lParam);
			if (pDev)
			{
				if (pDev->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
				{
					auto pInter = reinterpret_cast<
						const PDEV_BROADCAST_DEVICEINTERFACE>(pDev);
					if (pInter->dbcc_classguid == KSCATEGORY_AUDIO)
					{
						if (&m_AudioManager)
							m_AudioManager.OnNewAudioDevice();
					}
				}
			}
		}
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


