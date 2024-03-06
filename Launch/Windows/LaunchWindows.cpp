#include "Core/Windows/WindowsPlatform.h"

using namespace Step;

namespace Step
{
    extern int LaunchMain();

    int LaunchWindowsStartup(HINSTANCE hInstance)
    {
        int error = 0;
        g_hInstance = hInstance;
        error = LaunchMain();
        return error;
    }

    void LaunchWindowsShutdown()
    {
    }
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ char* pCmdLine, _In_ int nCmdShow)
{
    int result = LaunchWindowsStartup(hInstance);
    LaunchWindowsShutdown();
    return result;
}