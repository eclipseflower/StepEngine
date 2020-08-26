#ifndef __ENGINE_MANAGER_H__
#define __ENGINE_MANAGER_H__

#include "EngineWindowDirectX.h"
#include "EngineCoreDirectX.h"
#include "EngineTimer.h"
#include "EngineSceneManagerDirectX.h"
#include "EngineShaderDirectX.h"
#include "EngineCameraDirectX.h"

using Engine::Window::EngineWindowDirectX;
using Engine::Core::EngineCoreDirectX;
using Engine::Core::EngineSceneManagerDirectX;
using Engine::Shader::EngineShaderDirectX;
using Engine::Camera::EngineCameraDirectX;
using std::ostringstream;
using std::function;

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
		void EngineLoop(bool paused);
		void EngineDraw();

		void OnPause(bool paused);
		void OnResize();
		void OnMouseDown(WPARAM btnState, int x, int y);
		void OnMouseUp(WPARAM btnState, int x, int y);
		void OnMouseMove(WPARAM btnState, int x, int y);

		void CameraLookAt(const XMVECTOR & pos, const XMVECTOR & target, const XMVECTOR & up);

		UINT GetWindowWidth();
		UINT GetWindowHeight();
		HWND GetHwnd();

		//bool CreateVertexBuffer(void * vertices, UINT byteWidth, D3D11_USAGE usage, UINT cpuAccessFlags, ID3D11Buffer ** buffer);
		//bool CreateIndexBuffer(void * indices, UINT byteWidth, D3D11_USAGE usage, ID3D11Buffer ** buffer);
		bool CreateBoxObject(EngineObjectDirectX **object);
		bool CreateCylinderObject(float topRadius, float bottomRadius, float height, EngineObjectDirectX **object);
		bool CreateShader(string srcFile, EngineShaderDirectX **shader);

		EngineWindowDirectX *mWindowInst;
		EngineCoreDirectX *mCoreInst;
		EngineTimer mTimerInst;
		EngineCameraDirectX mCameraInst;
		EngineSceneManagerDirectX mSceneMgrInst;

		function <void(float)> mUpdateFunc;
		function <void(WPARAM, int, int)> mMouseDownFunc;
		function <void(WPARAM, int, int)> mMouseUpFunc;
		function <void(WPARAM, int, int)> mMouseMoveFunc;
	};

	extern EngineManagerDirectX * gManagerDirectX;
}

#endif // __ENGINE_MANAGER_H__
