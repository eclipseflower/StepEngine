#ifndef __ENGINE_CORE_DIRECTX_H__
#define __ENGINE_CORE_DIRECTX_H__

#include "EngineUtilDirectX.h"
#include "EngineLog.h"
#include "EngineObjectDirectX.h"
#include "EngineCameraDirectX.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

using Engine::Debug::EngineLog;
using Engine::Object::EngineObjectDirectX;
using Engine::Camera::EngineCameraDirectX;

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
			bool CreateShader(wstring srcFile, ID3DBlob **vs, ID3DBlob **ps);
			bool CreatePipelineStateObject(ID3DBlob *vs, ID3DBlob *ps, ID3D12PipelineState **pipelineStateObject);
			bool UpdatePosVertexBuffer(void * data, UINT byteWidth, int *mBaseVertexLocation);
			bool UpdatePropVertexBuffer(void * data, UINT byteWidth);
			bool UpdateIndexBuffer(void * data, UINT byteWidth, UINT *mStartIndexLocation);

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

			UINT mObjectConstBufferSize = 1024;
			UINT mPassConstBufferSize = 1024;

			ComPtr<ID3D12Resource> mConstBuffer = nullptr;
			BYTE *mConstBufferData = nullptr;
			ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

			vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

			UINT mVertexBufferSize = 1024;
			ComPtr<ID3D12Resource> mPosVertexBufferGPU = nullptr;
			void *mPosVertexBufferData = nullptr;
			UINT mPosVertexBufferOffset = 0;

			ComPtr<ID3D12Resource> mPropVertexBufferGPU = nullptr;
			void *mPropVertexBufferData = nullptr;
			UINT mPropVertexBufferOffset = 0;

			UINT mIndexBufferSize = 1024;
			UINT mIndexBufferOffset = 0;
			ComPtr<ID3D12Resource> mIndexBufferGPU = nullptr;
			void *mIndexBufferData = nullptr;

			vector<D3D12_VERTEX_BUFFER_VIEW> mVertexBufferViews;
			D3D12_INDEX_BUFFER_VIEW mIndexBufferView;

			D3D12_VIEWPORT mViewport;
			D3D12_RECT mScissorRect;

			D3D12_FILL_MODE mFillMode = D3D12_FILL_MODE_WIREFRAME;
			D3D12_CULL_MODE mCullMode = D3D12_CULL_MODE_NONE;
		};
	}
}

#endif // __ENGINE_CORE_DIRECTX_H__