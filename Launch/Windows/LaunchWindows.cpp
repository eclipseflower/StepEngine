#include <windows.h>

int LaunchWindowsStartup(HINSTANCE hInstance)
{

}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ char* pCmdLine, _In_ int nCmdShow)
{
    int result = LaunchWindowsStartup(hInstance);
    LaunchWindowsShutdown();
    return result;
}