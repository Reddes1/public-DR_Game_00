#include <windows.h>            //WinMain
#include "D3DUtils_Debug.h"     //Error Message Stuff
#include "GeneralUtils.h"       //wstring converter
#include "Game.h"               //The game..duh

#include <crtdbg.h>

//Lets go make a game lad!
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
    PSTR cmdLine, int showCmd)
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
#endif

    try
    {
        Game game(hInstance);
        if (!game.Initialize())
            return 0;
        return game.Run();
    }
    catch (DxException& e)
    {
        LPCSTR str = "HR Failed";
        std::string utf8String = WStringToString(e.ToString());
        LPCSTR lpStr = utf8String.c_str();
        MessageBox(nullptr, lpStr, str, MB_OK);
        return 0;

    }
    return 0;
}
