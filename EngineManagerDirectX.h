#ifndef __ENGINE_MANAGER_H__
#define __ENGINE_MANAGER_H__

#include "EngineWindowDirectX.h"
#include "EngineCoreDirectX.h"
#include "EngineTimer.h"
#include "EngineSceneManagerDirectX.h"
#include "EngineShaderDirectX.h"
#include "EngineCameraDirectX.h"
#include "EngineMaterialDirectX.h"
#include "EngineTextureDirectX.h"

using Engine::Window::EngineWindowDirectX;
using Engine::Core::EngineCoreDirectX;
using Engine::Core::EngineSceneManagerDirectX;
using Engine::Shader::EngineShaderDirectX;
using Engine::Camera::EngineCameraDirectX;
using Engine::Shader::EngineMaterialDirectX;
using Engine::Shader::EngineTextureDirectX;
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

		bool CreateDefaultBuffer(void *data, UINT byteWidth, ID3D12Resource **bufferGPU, ID3D12Resource **uploadBuffer);
		bool CreateBoxObject(EngineObjectDirectX **object, float width, float height, float depth, float posx, float posy, float posz);
		bool CreatePyramidObject(EngineObjectDirectX **object);
		bool CreateCylinderObject(float topRadius, float bottomRadius, float height, EngineObjectDirectX **object);
		bool CreateObjectFromFile(string filename, EngineObjectDirectX ** object);
		bool CreateBillBoard(float posx, float posy, float posz, float sizex, float sizey, EngineObjectDirectX ** object);
		bool CreateShader(wstring srcFile, EngineShaderDirectX **shader, bool hasGS = false);
		bool CreateMaterial(EngineMaterialDirectX **material);
		bool CreateTexture(wstring srcFile, TextureType textureType, EngineTextureDirectX **texture);
		bool CreatePipelineStateObject(RenderType renderType, EngineShaderDirectX *shader, ID3D12PipelineState **pipelineStateObject);
		bool CreateSobelPostProgressingEffect();
		bool UpdatePointVertexBuffer(void * data, UINT byteWidth, int *mBaseVertexLocation);
		bool UpdatePosVertexBuffer(void * data, UINT byteWidth, int *mBaseVertexLocation);
		bool UpdatePropVertexBuffer(void * data, UINT byteWidth);
		bool UpdateIndexBuffer(void * data, UINT byteWidth, UINT *mStartIndexLocation);
		bool UpdatePointIndexBuffer(void * data, UINT byteWidth, UINT *mStartIndexLocation);

		EngineWindowDirectX *mWindowInst;
		EngineCoreDirectX *mCoreInst;
		EngineTimer mTimerInst;
		EngineCameraDirectX mCameraInst;
		EngineSceneManagerDirectX mSceneMgrInst;

		function <void(float)> mUpdateFunc;
		function <void(WPARAM, int, int)> mMouseDownFunc;
		function <void(WPARAM, int, int)> mMouseUpFunc;
		function <void(WPARAM, int, int)> mMouseMoveFunc;

		UINT mCurSceneMaterialIndex = 0;
		UINT mCurSceneTextureIndex = 0;
	};

	extern EngineManagerDirectX * gManagerDirectX;
}

#endif // __ENGINE_MANAGER_H__
