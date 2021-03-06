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
		struct EngineCoreResource
		{
			ComPtr<ID3D12CommandAllocator> mCommandAlloc = nullptr;
			BYTE *mConstBufferData = nullptr;

			ComPtr<ID3D12Resource> mObjectConstBuffer = nullptr;
			ComPtr<ID3D12Resource> mPassConstBuffer = nullptr;
			ComPtr<ID3D12Resource> mMaterialConstBuffer = nullptr;

			UINT mCurrentFence = 0;
		};

		struct EngineCoreCompute
		{
			bool mEnable = false;
			ComPtr<ID3D12Resource> mComputeBuffer = nullptr;
			CD3DX12_CPU_DESCRIPTOR_HANDLE mCpuSrv;
			CD3DX12_CPU_DESCRIPTOR_HANDLE mCpuUav;
			CD3DX12_GPU_DESCRIPTOR_HANDLE mGpuSrv;
			CD3DX12_GPU_DESCRIPTOR_HANDLE mGpuUav;
			ComPtr<ID3D12RootSignature> mComputeSignature = nullptr;
			ComPtr<ID3D12PipelineState> mComputePipelineState = nullptr;
		};

		struct EngineCoreRenderTarget
		{
			bool mEnable = false;
			ComPtr<ID3D12Resource> mRenderTargetBuffer = nullptr;
			CD3DX12_CPU_DESCRIPTOR_HANDLE mCpuSrv;
			CD3DX12_GPU_DESCRIPTOR_HANDLE mGpuSrv;
			CD3DX12_CPU_DESCRIPTOR_HANDLE mCpuRtv;
			ComPtr<ID3D12RootSignature> mGraphicSignature = nullptr;
			ComPtr<ID3D12PipelineState> mGraphicPipelineState = nullptr;
		};

		class EngineCoreDirectX
		{
		public:
			EngineCoreDirectX(bool enableMsaa, UINT msaaCount);
			~EngineCoreDirectX();
			bool Init();
			bool ResizeBuffer();
			bool CreateDefaultBuffer(void *data, UINT byteWidth, ID3D12Resource **bufferGPU, ID3D12Resource **uploadBuffer);
			bool CreateShader(wstring srcFile, ID3DBlob **vs, ID3DBlob **ps, ID3DBlob **gs, bool hasGS);
			bool CreateTexture(wstring srcFile, UINT texId, TextureType textureType, ComPtr<ID3D12Resource> &res, ComPtr<ID3D12Resource> &uploadHeap);
			bool CreatePipelineStateObject(RenderType renderType, ID3DBlob *vs, ID3DBlob *ps, ID3DBlob * gs, ID3D12PipelineState **pipelineStateObject);
			bool CreateSobelPostProgressingEffect();
			bool CreateSobelPostProgressingResourceAndView();
			bool UpdatePointVertexBuffer(void * data, UINT byteWidth, int *mBaseVertexLocation);
			bool UpdatePosVertexBuffer(void * data, UINT byteWidth, int *mBaseVertexLocation);
			bool UpdatePropVertexBuffer(void * data, UINT byteWidth);
			bool UpdateIndexBuffer(void * data, UINT byteWidth, UINT *mStartIndexLocation);
			bool UpdatePointIndexBuffer(void * data, UINT byteWidth, UINT *mStartIndexLocation);

			void FlushCommandQueue();
			void BeginDraw(EngineCameraDirectX * camera);
			void DrawObject(EngineObjectDirectX * object, EngineCameraDirectX * camera);
			void EndDraw();
			void PostDraw();

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
			ComPtr<ID3D12CommandAllocator> mCommandAlloc = nullptr;
			ComPtr<ID3D12GraphicsCommandList> mCommandList = nullptr;
			ComPtr<ID3D12Fence> mFence = nullptr;
			UINT mCurrentFence = 0;

			ComPtr<ID3D12DescriptorHeap> mRtvHeap = nullptr;
			ComPtr<ID3D12DescriptorHeap> mDsvHeap = nullptr;
			ComPtr<ID3D12DescriptorHeap> mCbvSrvHeap = nullptr;

			UINT mRtvHeapIncSize = 0;
			UINT mDsvHeapIncSize = 0;
			UINT mCbvSrvHeapIncSize = 0;

			vector<EngineCoreResource> mCoreResource;
			UINT mObjectConstBufferCount = 1024;
			UINT mPassConstBufferCount = 1;
			UINT mMaterialConstBufferCount = 256;
			UINT mCoreResourceCount = 3;
			UINT mCurCoreResourceIndex = 0;

			UINT mShaderResourceCount = 16;
			UINT mComputeResourceCount = 2; /* one for srv, one for uav */
			UINT mRenderTargetResourceCount = 1;

			vector<D3D12_STATIC_SAMPLER_DESC> mStaticSamplers;
			ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

			vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
			vector<D3D12_INPUT_ELEMENT_DESC> mPointInputLayout;

			UINT mVertexBufferSize = 65536;
			ComPtr<ID3D12Resource> mPosVertexBufferGPU = nullptr;
			void *mPosVertexBufferData = nullptr;
			UINT mPosVertexBufferOffset = 0;

			ComPtr<ID3D12Resource> mPropVertexBufferGPU = nullptr;
			void *mPropVertexBufferData = nullptr;
			UINT mPropVertexBufferOffset = 0;

			ComPtr<ID3D12Resource> mPointVertexBufferGPU = nullptr;
			void *mPointVertexBufferData = nullptr;
			UINT mPointVertexBufferOffset = 0;

			UINT mIndexBufferSize = 262144;
			UINT mIndexBufferOffset = 0;
			ComPtr<ID3D12Resource> mIndexBufferGPU = nullptr;
			void *mIndexBufferData = nullptr;

			UINT mPointIndexBufferOffset = 0;
			ComPtr<ID3D12Resource> mPointIndexBufferGPU = nullptr;
			void *mPointIndexBufferData = nullptr;

			vector<D3D12_VERTEX_BUFFER_VIEW> mVertexBufferViews;
			D3D12_INDEX_BUFFER_VIEW mIndexBufferView;

			D3D12_VERTEX_BUFFER_VIEW mPointVertexBufferView;
			D3D12_INDEX_BUFFER_VIEW mPointIndexBufferView;

			D3D12_VIEWPORT mViewport;
			D3D12_RECT mScissorRect;

			D3D12_FILL_MODE mFillMode = D3D12_FILL_MODE_SOLID;
			D3D12_CULL_MODE mCullMode = D3D12_CULL_MODE_NONE;

			EngineCoreCompute mCoreCompute;
			EngineCoreRenderTarget mCoreRenderTarget;
		};
	}
}

#endif // __ENGINE_CORE_DIRECTX_H__