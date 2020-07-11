#ifndef __ENGINE_CORE_DIRECTX_H__
#define __ENGINE_CORE_DIRECTX_H__

#include "EngineUtilDirectX.h"
#include "EngineLog.h"
#include "EngineObjectDirectX.h"
#include "EngineCameraDirectX.h"

using Engine::Debug::EngineLog;
using Engine::Object::EngineObjectDirectX;
using Engine::Camera::EngineCameraDirectX;

using Microsoft::WRL::ComPtr;

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
			bool CreateIndexBuffer(void * indices, UINT byteWidth, D3D11_USAGE usage, ID3D11Buffer ** buffer);
			bool CreateShader(string srcFile, ID3DX11Effect ** effect);
			bool CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC * vertexDesc, const UINT vertexDescCount, D3DX11_PASS_DESC * passDesc, ID3D11InputLayout ** layout);

			void BeginDraw();
			void DrawObject(EngineObjectDirectX * object, EngineCameraDirectX * camera);
			void EndDraw();

		public:
			bool mEnableMsaa = false;
			UINT mMsaaCount = 1;
			UINT mMsaaQuality = 0;
			DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

			static const UINT mBackBufferCount = 2;

			ComPtr<ID3D12Device> mDevice = nullptr;
			ComPtr<IDXGISwapChain> mSwapChain = nullptr;
			ComPtr<IDXGIFactory4> mDxGiFactory = nullptr;

			ComPtr<ID3D12Resource> mBackBuffer[mBackBufferCount];
			ComPtr<ID3D12Resource> mDepthStencilBuffer = nullptr;

			ComPtr<ID3D12CommandQueue> mCommandQueue = nullptr;
			ComPtr<ID3D12GraphicsCommandList> mCommandList = nullptr;
			ComPtr<ID3D12CommandAllocator> mCommandAlloc = nullptr;
			ComPtr<ID3D12Fence> mFence = nullptr;

			ComPtr<ID3D12DescriptorHeap> mRtvHeap = nullptr;
			ComPtr<ID3D12DescriptorHeap> mDsvHeap = nullptr;
			UINT mRtvHeapIncSize = 0;
			UINT mDsvHeapIncSize = 0;
		};
	}
}

#endif // __ENGINE_CORE_DIRECTX_H__