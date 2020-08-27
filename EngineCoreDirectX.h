#ifndef __ENGINE_CORE_DIRECTX_H__
#define __ENGINE_CORE_DIRECTX_H__

#include "EngineUtilDirectX.h"
#include "EngineLog.h"
#include "EngineObjectDirectX.h"
#include "EngineCameraDirectX.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

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
			bool CreateDefaultBuffer(void *data, UINT byteWidth, ID3D12Resource **bufferGPU, ID3D12Resource **uploadBuffer);
			//bool CreateIndexBuffer(void * indices, UINT byteWidth, D3D11_USAGE usage, ID3D11Buffer ** buffer);
			bool CreateShader(wstring srcFile, ID3DBlob **vs, ID3DBlob **ps);

			void FlushCommandQueue();
			void BeginDraw();
			void DrawObject(EngineObjectDirectX * object, EngineCameraDirectX * camera);
			void EndDraw();

		public:
			bool mEnableMsaa = false;
			UINT mMsaaCount = 1;
			UINT mMsaaQuality = 0;

			ComPtr<ID3D12Device> mDevice = nullptr;
			ComPtr<IDXGISwapChain> mSwapChain = nullptr;
			ComPtr<IDXGIFactory4> mDxGiFactory = nullptr;

			static const UINT mBackBufferCount = 2;
			ComPtr<ID3D12Resource> mBackBuffer[mBackBufferCount];
			DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			UINT mCurBackBuffer = 0;

			ComPtr<ID3D12Resource> mDepthStencilBuffer = nullptr;
			DXGI_FORMAT mDepthStencilBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

			ComPtr<ID3D12CommandQueue> mCommandQueue = nullptr;
			ComPtr<ID3D12GraphicsCommandList> mCommandList = nullptr;
			ComPtr<ID3D12CommandAllocator> mCommandAlloc = nullptr;
			ComPtr<ID3D12Fence> mFence = nullptr;
			UINT mCurrentFence = 0;

			ComPtr<ID3D12DescriptorHeap> mRtvHeap = nullptr;
			ComPtr<ID3D12DescriptorHeap> mDsvHeap = nullptr;
			ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;

			UINT mRtvHeapIncSize = 0;
			UINT mDsvHeapIncSize = 0;
			UINT mCbvHeapIncSize = 0;

			ComPtr<ID3D12Resource> mConstBuffer = nullptr;
			ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

			vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

			D3D12_VIEWPORT mViewport;
			D3D12_RECT mScissorRect;
		};
	}
}

#endif // __ENGINE_CORE_DIRECTX_H__