#pragma once
#include "D3DUtils.h"    //Headers, utils etc
#include "GameTimer.h"   //Delta and Game Time

using Microsoft::WRL::ComPtr;

/*
    Wrapped up D3D12 Application - Derive a "Game" object with this.
    Is responsible for setting up and managing D3D proceedures and Windows processes like window creation
    and message pumping.
*/
class D3D12Application
{
protected:
    //Packaged Window Data
    struct WindowData
    {
        std::string m_WindowCaption = "Deaths Rebuke";    //Window Title/Caption
        DWORD       m_WinStyle = (WS_TILEDWINDOW);        //Window Style
        HINSTANCE   m_AppInst = nullptr;                  //Application instance handle
        HWND        m_Window = nullptr;                   //Main window handle
        int         m_Width = 1920;                       //Current Window Width
        int         m_Height = 1080;                      //Current Window Height
        int         m_NativeWidth = 1920;                 //Original Window Width
        int         m_NativeHeight = 1080;                //Original Window Height
        bool        m_AppPaused = false;                  //Is the application paused?
        bool        m_AppMinimized = false;               //Is the application minimized?
        bool        m_AppMaximized = false;               //Is the application maximized?
        bool        m_AppResizing = false;                //Are the resize bars being dragged?
        bool        m_AppFullscreenState = false;         //Fullscreen enabled

    };

    //Windows Creation
    D3D12Application(HINSTANCE hInstance);
    D3D12Application(const D3D12Application& rSide) = delete;
    D3D12Application& operator=(const D3D12Application& rSide) = delete;
    virtual ~D3D12Application();

public:

    ////////////////
    /// Virtuals ///
    ////////////////

    virtual void Update(GameTimer& gt) = 0;
    virtual void Render(GameTimer& gt) = 0;

    //Initialise (Always call this in child object)
    virtual bool Initialize();
    //Windows Message Handler (Holds cookie cutter implementation, redefine and adjust in child class)
    virtual LRESULT MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;

    ///////////
    /// Get ///
    ///////////

    //Get this object
    static D3D12Application* Get() { return m_App; }
    //Get main window data
    const WindowData& GetWindowData() const { return m_WinData; }
    //Get window size as XMINT2
    DirectX::XMINT2 GetWindowSizeI() { return DirectX::XMINT2(m_WinData.m_NativeWidth, m_WinData.m_NativeHeight); }
    //Get window size as XMFLOAT2
    DirectX::XMFLOAT2 GetWindowSizeF() { return DirectX::XMFLOAT2(static_cast<float>(m_WinData.m_NativeWidth), static_cast<float>(m_WinData.m_NativeHeight)); }
    //Get window width as int
    int GetWindowWidthI() { return m_WinData.m_NativeWidth; }
    //Get window height as int
    int GetWindowHeightI() { return m_WinData.m_NativeHeight; }
    //Get window width as float
    float GetWindowWidthF() { return static_cast<float>(m_WinData.m_NativeWidth); }
    //Get window height as float
    float GetWindowHeightF() { return static_cast<float>(m_WinData.m_NativeHeight); }
    //Get current window width
    int GetCurrentWindowWidth() { return m_WinData.m_Width; }
    //Get current window height
    int GetCurrentWindowHeight() { return m_WinData.m_Height; }


    //Get window aspect ratio
    float GetAspectRatio() const { return static_cast<float>(m_WinData.m_NativeWidth / m_WinData.m_NativeHeight); }
    //Get 4xMsaaState
    bool Get4xMsaaState() const { return m_Enable4xMsaa; }
    //Set 4xMsaaState
    void Set4xMsaaState(bool value);

    //Get GameTime
    GameTimer& GetGameTime() { return m_Timer; }

    //Get D3D Device
    ComPtr<ID3D12Device>& GetD3DDevice() { return m_D3DDevice; }
    //Get Command List
    ComPtr<ID3D12GraphicsCommandList>& GetCommandList() { return m_CommandList; }

    //////////////////
    /// Operations ///
    //////////////////

    //Main Run Loop
    int Run();

    void ReleaseD3D(bool extraReporting = true);

protected:

    ////////////
    /// Data ///
    ////////////

    //Window Data Structure
    WindowData m_WinData;
    //Track gametime and delta time;
    GameTimer m_Timer;
    //Singleton stuff
    static D3D12Application* m_App;


    ///////////
    /// Get ///
    ///////////

    D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferView() const;
    D3D12_CPU_DESCRIPTOR_HANDLE GetDSVHeapStart() const { return m_DSVHeap->GetCPUDescriptorHandleForHeapStart(); }


    ////////////////////
    /// Initializers ///
    ////////////////////

    //Initialise Window
    bool InitMainWindow();
    //Initialise D3D
    bool InitD3D();
    //Create CO, SC, D.Heaps
    void CreateCommandObjects();
    void CreateSwapChain();
    void CreateRtvAndDsvDescriptorHeaps();

    //Adapter setup
    void LogAdapters();
    void LogAdapterOutputs(IDXGIAdapter* adapter);
    void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

    ////////////////
    /// Updaters ///
    ////////////////

    //Update game time and calc Delta Time
    void CalculateFrameStats();
    //Call whenever a resize is needed
    virtual void OnResize();
    //Clears the command queue
    void FlushCommandQueue();

    //Call this inside of the resize call to snap resolution to nearest 16:9
    void SnapToRes16_9();
    //Support function for above
    int GetClosestInt(int val1, int val2, int target);

protected:

    /////////////////
    /// DX12 Data ///
    /////////////////

    // Derived class should set these in derived constructor to customize starting values.
    DXGI_FORMAT m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT m_DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    // Set true to use 4X MSAA (§4.1.8).  The default is false.
    UINT m_4xMsaaQuality = 0;
    bool m_Enable4xMsaa = false;


    //Main Device, Factory and Swap Chain
    ComPtr<ID3D12Device> m_D3DDevice;
    ComPtr<IDXGIFactory4> m_DXGIFactory;
    ComPtr<IDXGISwapChain> m_SwapChain;

    //Command Queue/Allocator/List
    ComPtr<ID3D12CommandQueue> m_CommandQueue;
    ComPtr<ID3D12CommandAllocator> m_DirectCommandListAllocator;
    ComPtr<ID3D12GraphicsCommandList> m_CommandList;

    //Fence (for syncing)
    ComPtr<ID3D12Fence> m_Fence;
    UINT64 m_CurrentFence = 0;

    //Swap Chain Buffer values
    static const int SWAP_CHAIN_BUFFER_COUNT = 2;
    int m_CurrentBackBuffer = 0;
    //Swap Chain Buffer & Depth Stencil buffer
    ComPtr<ID3D12Resource> m_SwapChainBuffer[SWAP_CHAIN_BUFFER_COUNT];
    ComPtr<ID3D12Resource> m_DepthStencilBuffer;

    //Render Target View & Depth Stencil View
    ComPtr<ID3D12DescriptorHeap> m_RTVHeap;
    ComPtr<ID3D12DescriptorHeap> m_DSVHeap;

    //Viewport and Scissor
    D3D12_VIEWPORT m_ScreenViewport;
    D3D12_RECT m_ScissorRect;

    //Descriptor sizes
    UINT m_RTVDescriptorSize = 0;
    UINT m_DSVDescriptorSize = 0;
    UINT m_CbvSrvUavDescriptorSize = 0;


};

