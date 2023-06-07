#pragma once
#include "D3D.h"                        //Parent Class/Core Application (Gets Utils through here)

//Utils & Types
#include "D3DUtils.h"                   //D3D Stuff
#include "TextureDescriptions.h"        //Enum Texture Information like Frames/Animation Indexes
#include "GameConstants.h"              //Game Related Details
#include "GameDefinitions.h"            //Mode, batch, font descriptions
#include "EngineDefinitions.h"          //Engine-Level defintions

//Manager Classes
#include "InputManager.h"               //Mouse/Keyboard/Gamepad input manager
#include "ModeManager.h"                //Game Mode/State manager
#include "TextureManager.h"             //Store data
#include "UIElementManager.h"           //Houses specialist UI Elements
#include "AudioManager.h"               //Wrapped up audio manager
#include "GameplayManager.h"            //Manages gameplay orientated behavior and sub-managers
#include "TilesetManager.h"             //Manages storage of tilemap details

//Control Schemes
#include "CS_Survivor_00.h"

/*
    Main game object, everything runs through this!
*/

class Game : public D3D12Application
{
public:

    ////////////////////
    /// Constructors ///
    ////////////////////

    //Windows Creation (Calls D3D12Application)
    Game(HINSTANCE hInstance);
    Game(const Game& rSide) = delete;
    Game& operator=(const Game& rSide) = delete;
    ~Game();

    /////////////////
    /// Overrides ///
    /////////////////

    bool Initialize() override;


    ///////////
    /// Get ///
    ///////////

    static Game* GetGame()                  { return m_Game; }

    //
    //Managers
    //

    ModeManager& GetModeManager()           { return m_ModeManager; }
    TextureManager& GetTextureManager()     { return m_TextureManager; }
    UIElementManager& GetUIManager()        { return m_UIManager; }
    Gamepads& GetGamepadManager()           { return m_Gamepad; }
    KeyboardAndMouse& GetKBMManager()       { return m_KBM; }
    AudioManager& GetAudioManager()         { return m_AudioManager; }
    GameplayManager& GetGameplayManager()   { return m_GameplayManager; }
    TilesetManager& GetTilesetManager()     { return m_TilesetManager; }

    //
    //Data
    //

    PassData& GetPassData()                 { return m_PassData; }
    CS_Survivor_00& GetControlScheme()      { return m_ControlScheme; }
    std::unique_ptr<DirectX::DescriptorHeap>& GetDescriptorHeap() { return m_ResourceDescriptors; }


private:

    static Game* m_Game;

    /////////////////
    /// Overrides ///
    /////////////////

    void Update(GameTimer& gt) override;
    void Render(GameTimer& gt) override;

    //Need to intercept some message for InputManager, so we override MessageHandler from parent change it slightly
    LRESULT MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    /////////////////
    /// Rendering ///
    /////////////////

    void PreFrame();
    void PostFrame();
    void CycleFrame();

    /////////////
    /// Setup ///
    /////////////

    //Responsible for setting up and loading textures, as well as spritebatch/font setup
    void XTKSetup(std::string& textureFP, std::string& fontFP);
    //Loads fonts and initial textures required for the game
    void LoadInitialTextures(DirectX::ResourceUploadBatch& resourceUpload, std::string& textureFP, std::string& fontFP);
    //Setup defined spritebatches
    void SetupSpritebatches(DirectX::ResourceUploadBatch& resourceUpload);
    void BuildFrameResources();

    //Setup the core engine managers (modes, audio, input). Alter Carefully.
    void SetupCoreManagers(rapidjson::Document& masterManifestDoc);
    //Setup the optional managers related to the game + core manager usage.
    void SetupOptionalManagers(rapidjson::Document& masterManifestDoc);
    void InitSceneCamera();


    ////////////////
    /// Managers ///
    ////////////////

    //
    //Core Managers
    //

    ModeManager m_ModeManager;
    TextureManager m_TextureManager;
    UIElementManager m_UIManager;
    Gamepads m_Gamepad;
    KeyboardAndMouse m_KBM;
    AudioManager m_AudioManager;

    //
    //User Defined Managers
    //

    GameplayManager m_GameplayManager;
    TilesetManager m_TilesetManager;

    //
    //Other
    //

    CS_Survivor_00 m_ControlScheme;

    //////////////////////
    /// DX12 Variables ///
    //////////////////////

    //Stores the point where font SRVs end, and textures begin
    unsigned postFontSrvCount = 0;
    //Stores current count of resources (for debug purposes)
    unsigned totalSrvCount = 0;

    //Package together the data that modes need into one spot (spritebatchs, fonts & heap)
    PassData m_PassData;

    int m_CurrentFrameResourceIndex = 0;
    FrameResource2D* m_CurrentFrameResource2D = nullptr;
    std::vector<std::unique_ptr<FrameResource2D>> m_FrameResources2D;
    
    std::unique_ptr<DirectX::GraphicsMemory> m_GraphicsMemory;
    std::unique_ptr<DirectX::DescriptorHeap> m_ResourceDescriptors;
    std::unique_ptr<DirectX::CommonStates> m_CommonStates;
};