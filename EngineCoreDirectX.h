#ifndef __ENGINE_CORE_DIRECTX_H__
#define __ENGINE_CORE_DIRECTX_H__

#include "EngineUtil.h"
#include "EngineLog.h"

using Engine::Debug::EngineLog;

namespace Engine
{
	namespace Core
	{
		class EngineCoreDirectX
		{
		public:
			EngineCoreDirectX(bool enableMsaa, UINT msaaCount);
			~EngineCoreDirectX();
			bool Init();
			bool ResizeBuffer();
			bool CreateVertexBuffer(void * vertices, UINT byteWidth, D3D11_USAGE usage, UINT cpuAccessFlags, ID3D11Buffer ** buffer);

		private:
			bool mEnableMsaa;
			UINT mMsaaCount;
			UINT mMsaaQuality;
			ID3D11Device *mD3dDevice;
			ID3D11DeviceContext *mD3dImmediateContext;
			IDXGISwapChain *mSwapChain;
			ID3D11Texture2D *mDepthStencilBuffer;
			ID3D11RenderTargetView *mRenderTargetView;
			ID3D11DepthStencilView *mDepthStencilView;
			D3D11_VIEWPORT mScreenViewport;
		};
	}
}

#endif // __ENGINE_CORE_DIRECTX_H__