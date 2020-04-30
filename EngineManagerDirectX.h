#ifndef __ENGINE_MANAGER_H__
#define __ENGINE_MANAGER_H__

#include "EngineWindowDirectX.h"
#include "EngineCoreDirectX.h"
#include "EngineTimer.h"
#include "EngineSceneManagerDirectX.h"

using Engine::Window::EngineWindowDirectX;
using Engine::Core::EngineCoreDirectX;
using Engine::Core::EngineSceneManagerDirectX;
using std::ostringstream;

namespace Engine
{
	class EngineManagerDirectX
	{
	public:
		EngineManagerDirectX();
		~EngineManagerDirectX();
		bool InitEngineWindow(HINSTANCE hInstance);
		bool InitEngineCore(bool enableMsaa, UINT msaaCount);
		int RunEngine();
		void EngnineLoop(bool paused);
		void OnPause(bool paused);
		void OnResize();
		UINT GetWindowWidth();
		UINT GetWindowHeight();
		HWND GetHwnd();

		bool CreateVertexBuffer(void * vertices, UINT byteWidth, D3D11_USAGE usage, UINT cpuAccessFlags, ID3D11Buffer ** buffer);
		bool CreateIndexBuffer(void * indices, UINT byteWidth, D3D11_USAGE usage, ID3D11Buffer ** buffer);
		void CreateBoxObject(EngineObjectDirectX **object);

		EngineWindowDirectX *mWindowInst;
		EngineCoreDirectX *mCoreInst;
		EngineTimer mTimerInst;
		EngineSceneManagerDirectX mSceneMgrInst;
	};

	extern EngineManagerDirectX * gManagerDirectX;
}

#endif // __ENGINE_MANAGER_H__
