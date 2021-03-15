#include "EngineCoreDirectX.h"
#include "EngineManagerDirectX.h"

Engine::Core::EngineCoreDirectX::EngineCoreDirectX(bool enableMsaa, UINT msaaCount) : mVertexBufferViews(2)
{
	mEnableMsaa = enableMsaa;
	mMsaaCount = msaaCount;
	mMsaaQuality = 0;
}

Engine::Core::EngineCoreDirectX::~EngineCoreDirectX()
{

}

bool Engine::Core::EngineCoreDirectX::Init()
{
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	ComPtr<ID3D12Debug> debugController = nullptr;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
	debugController->EnableDebugLayer();
#endif

	// 1. create device and gi factory
	ThrowIfFailed(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&mDevice)));
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&mDxGiFactory)));

	// 2. create gpu related objects, command, fence
	ThrowIfFailed(mDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));

	D3D12_COMMAND_QUEUE_DESC queueDesc;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.NodeMask = 0;

	ThrowIfFailed(mDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue)));
	ThrowIfFailed(mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAlloc)));
	ThrowIfFailed(mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAlloc.Get(), nullptr,
		IID_PPV_ARGS(&mCommandList)));
	// must close otherwise call reset will raise a error
	mCommandList->Close();

	// 3. create swap chain
	UINT windowWidth = gManagerDirectX->GetWindowWidth();
	UINT windowHeight = gManagerDirectX->GetWindowHeight();
	DXGI_SWAP_CHAIN_DESC chainDesc;
	chainDesc.BufferDesc.Format = mBackBufferFormat;
	chainDesc.BufferDesc.Height = windowHeight;
	chainDesc.BufferDesc.Width = windowWidth;
	chainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	chainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	chainDesc.BufferDesc.RefreshRate.Denominator = 1;
	chainDesc.BufferDesc.RefreshRate.Numerator = 60;

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS qualityLevels;
	qualityLevels.Format = mBackBufferFormat;
	qualityLevels.SampleCount = mMsaaCount;
	qualityLevels.NumQualityLevels = 0;
	qualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	ThrowIfFailed(mDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &qualityLevels,
		sizeof(qualityLevels)));
	mMsaaQuality = qualityLevels.NumQualityLevels;

	//https://stackoverflow.com/questions/40110699/creating-a-swap-chain-with-msaa-fails
	/*
		Direct3D 12 don't support creating MSAA swap chains
		--attempts to create a swap chain with SampleDesc.Count > 1 will fail.
		Instead, you create your own MSAA render target and
		explicitly resolve to the DXGI back-buffer for presentation as shown here.
	*/
	chainDesc.SampleDesc.Count = mEnableMsaa ? mMsaaCount : 1;
	chainDesc.SampleDesc.Quality = mEnableMsaa ? mMsaaQuality - 1 : 0;

	chainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	chainDesc.BufferCount = mBackBufferCount;
	chainDesc.OutputWindow = gManagerDirectX->GetHwnd();
	chainDesc.Windowed = true;
	chainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	chainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ThrowIfFailed(mDxGiFactory->CreateSwapChain(mCommandQueue.Get(), &chainDesc, &mSwapChain));

	// 4. create rtv & dsv heap
	mRtvHeapIncSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	mDsvHeapIncSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = mBackBufferCount;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	ThrowIfFailed(mDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&mRtvHeap)));

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	ThrowIfFailed(mDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&mDsvHeap)));

	// 5. create frame resources (command alloc and const buffer)
	UINT objCbSize = CalcConstantBufferByteSize(sizeof(ObjectConstants));
	UINT passCbSize = CalcConstantBufferByteSize(sizeof(PassConstants));
	UINT matCbSize = CalcConstantBufferByteSize(sizeof(MaterialConstants));

	for (UINT i = 0; i < mCoreResourceCount; i++)
	{
		EngineCoreResource res;
		ThrowIfFailed(mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&res.mCommandAlloc)));
		ThrowIfFailed(mDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(mObjectConstBufferCount * objCbSize),
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&res.mObjectConstBuffer)));
		ThrowIfFailed(mDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(mPassConstBufferCount * passCbSize),
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&res.mPassConstBuffer)));
		ThrowIfFailed(mDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(mMaterialConstBufferCount * matCbSize),
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&res.mMaterialConstBuffer)));
		mCoreResource.push_back(res);
	}

	// 6. create cbv&srv heap
	UINT objCbCount = mCoreResourceCount * mObjectConstBufferCount;
	UINT passCbCount = mCoreResourceCount * mPassConstBufferCount;
	UINT matCbCount = mCoreResourceCount * mMaterialConstBufferCount;
	UINT shaderResCount = mShaderResourceCount;

	mCbvSrvHeapIncSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_DESCRIPTOR_HEAP_DESC cbvSrvHeapDesc;
	cbvSrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvSrvHeapDesc.NumDescriptors = objCbCount + passCbCount + matCbCount + shaderResCount;
	cbvSrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvSrvHeapDesc.NodeMask = 0;
	ThrowIfFailed(mDevice->CreateDescriptorHeap(&cbvSrvHeapDesc, IID_PPV_ARGS(&mCbvSrvHeap)));

	// 7. create const buffer view
	UINT cbvHeapIndex = 0;
	for (UINT i = 0; i < mCoreResourceCount; i++)
	{
		EngineCoreResource res = mCoreResource[i];
		D3D12_GPU_VIRTUAL_ADDRESS objCbAddr = res.mObjectConstBuffer->GetGPUVirtualAddress();
		for (UINT j = 0; j < mObjectConstBufferCount; j++)
		{
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
			cbvDesc.BufferLocation = objCbAddr + j * objCbSize;
			cbvDesc.SizeInBytes = objCbSize;
			CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
				mCbvSrvHeap->GetCPUDescriptorHandleForHeapStart());
			handle.Offset(cbvHeapIndex, mCbvSrvHeapIncSize);
			mDevice->CreateConstantBufferView(&cbvDesc, handle);
			cbvHeapIndex++;
		}

		D3D12_GPU_VIRTUAL_ADDRESS passCbAddr = res.mPassConstBuffer->GetGPUVirtualAddress();
		for (UINT j = 0; j < mPassConstBufferCount; j++)
		{
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
			cbvDesc.BufferLocation = passCbAddr + j * passCbSize;
			cbvDesc.SizeInBytes = passCbSize;
			CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
				mCbvSrvHeap->GetCPUDescriptorHandleForHeapStart());
			handle.Offset(cbvHeapIndex, mCbvSrvHeapIncSize);
			mDevice->CreateConstantBufferView(&cbvDesc, handle);
			cbvHeapIndex++;
		}

		D3D12_GPU_VIRTUAL_ADDRESS matCbAddr = res.mMaterialConstBuffer->GetGPUVirtualAddress();
		for (UINT j = 0; j < mMaterialConstBufferCount; j++)
		{
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
			cbvDesc.BufferLocation = matCbAddr + j * matCbSize;
			cbvDesc.SizeInBytes = matCbSize;
			CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
				mCbvSrvHeap->GetCPUDescriptorHandleForHeapStart());
			handle.Offset(cbvHeapIndex, mCbvSrvHeapIncSize);
			mDevice->CreateConstantBufferView(&cbvDesc, handle);
			cbvHeapIndex++;
		}
	}

	// 8. create static samplers
	mStaticSamplers.push_back(CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT, 
		D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP));
	mStaticSamplers.push_back(CD3DX12_STATIC_SAMPLER_DESC(1, D3D12_FILTER_MIN_MAG_MIP_POINT,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP));
	mStaticSamplers.push_back(CD3DX12_STATIC_SAMPLER_DESC(2, D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP));
	mStaticSamplers.push_back(CD3DX12_STATIC_SAMPLER_DESC(3, D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP));
	mStaticSamplers.push_back(CD3DX12_STATIC_SAMPLER_DESC(4, D3D12_FILTER_ANISOTROPIC,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP));
	mStaticSamplers.push_back(CD3DX12_STATIC_SAMPLER_DESC(5, D3D12_FILTER_ANISOTROPIC,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP));


	// 9. create root signature
	CD3DX12_DESCRIPTOR_RANGE cbvSrvTable[4];
	cbvSrvTable[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	cbvSrvTable[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
	cbvSrvTable[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);
	cbvSrvTable[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER rootParams[4];
	rootParams[0].InitAsDescriptorTable(1, &cbvSrvTable[0]);
	rootParams[1].InitAsDescriptorTable(1, &cbvSrvTable[1]);
	rootParams[2].InitAsDescriptorTable(1, &cbvSrvTable[2]);
	rootParams[3].InitAsDescriptorTable(1, &cbvSrvTable[3]);

	CD3DX12_ROOT_SIGNATURE_DESC sigDesc(_countof(rootParams), rootParams, mStaticSamplers.size(), 
		mStaticSamplers.data(), D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	ComPtr<ID3DBlob> signature = nullptr;
	ComPtr<ID3DBlob> error = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	if (error != nullptr)
	{
		EngineLog::LogErrorMessageBox((char *)error->GetBufferPointer());
	}
	ThrowIfFailed(hr);
	ThrowIfFailed(mDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), 
		IID_PPV_ARGS(&mRootSignature)));

	// 10. create input layout
	mInputLayout =
	{
		{"POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0},
		{"NORMAL",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0},
		{"COLOR",		0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0},
		{"TEXCOORD",	0,	DXGI_FORMAT_R32G32_FLOAT,		1,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0}
	};

	// 11. create vertex buffer and index buffer and views
	ThrowIfFailed(mDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(mVertexBufferSize * sizeof(EngineVertexPosDirectX)), 
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mPosVertexBufferGPU)));

	mVertexBufferViews[0].BufferLocation = mPosVertexBufferGPU->GetGPUVirtualAddress();
	mVertexBufferViews[0].SizeInBytes = mVertexBufferSize * sizeof(EngineVertexPosDirectX);
	mVertexBufferViews[0].StrideInBytes = sizeof(EngineVertexPosDirectX);

	ThrowIfFailed(mDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(mVertexBufferSize * sizeof(EngineVertexPropDirectX)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mPropVertexBufferGPU)));

	mVertexBufferViews[1].BufferLocation = mPropVertexBufferGPU->GetGPUVirtualAddress();
	mVertexBufferViews[1].SizeInBytes = mVertexBufferSize * sizeof(EngineVertexPropDirectX);
	mVertexBufferViews[1].StrideInBytes = sizeof(EngineVertexPropDirectX);

	ThrowIfFailed(mDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(mIndexBufferSize * sizeof(UINT)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mIndexBufferGPU)));

	mIndexBufferView.BufferLocation = mIndexBufferGPU->GetGPUVirtualAddress();
	mIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	mIndexBufferView.SizeInBytes = mIndexBufferSize * sizeof(UINT);

	ResizeBuffer();

	return true;
}

bool Engine::Core::EngineCoreDirectX::ResizeBuffer()
{
	assert(mDevice);
	assert(mSwapChain);
	assert(mCommandAlloc);

	FlushCommandQueue();

	UINT windowWidth = gManagerDirectX->GetWindowWidth();
	UINT windowHeight = gManagerDirectX->GetWindowHeight();

	// 1. re-create back buffer and view
	// You need to release all resources associated to your swap chain before to resize it
	for (int i = 0; i < mBackBufferCount; i++)
	{
		mBackBuffer[i].Reset();
	}
	mCurBackBuffer = 0;
	ThrowIfFailed(mSwapChain->ResizeBuffers(mBackBufferCount, windowWidth, windowHeight, mBackBufferFormat,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		mRtvHeap->GetCPUDescriptorHandleForHeapStart());

	for (int i = 0; i < mBackBufferCount; i++)
	{
		ThrowIfFailed(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mBackBuffer[i])));
		mDevice->CreateRenderTargetView(mBackBuffer[i].Get(), nullptr, handle);
		handle.Offset(1, mRtvHeapIncSize);
	}

	// 2. re-create depth stencil buffer and view
	// You need to release all resources associated to your swap chain before to resize it
	mDepthStencilBuffer.Reset();
	D3D12_RESOURCE_DESC desc;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment = 0;
	desc.Width = windowWidth;
	desc.Height = windowHeight;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = mDepthStencilBufferFormat;
	desc.SampleDesc.Count = mEnableMsaa ? mMsaaCount : 1;
	desc.SampleDesc.Quality = mEnableMsaa ? mMsaaQuality - 1 : 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = mDepthStencilBufferFormat;
	clearValue.DepthStencil.Depth = 1.0f;
	clearValue.DepthStencil.Stencil = 0;
	ThrowIfFailed(mDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COMMON, &clearValue, IID_PPV_ARGS(&mDepthStencilBuffer)));

	D3D12_DEPTH_STENCIL_VIEW_DESC viewDesc;
	viewDesc.Format = mDepthStencilBufferFormat;
	viewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	viewDesc.Flags = D3D12_DSV_FLAG_NONE;
	viewDesc.Texture2D.MipSlice = 0;

	handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(mDsvHeap->GetCPUDescriptorHandleForHeapStart());
	mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), &viewDesc, handle);

	ThrowIfFailed(mCommandAlloc->Reset());
	ThrowIfFailed(mCommandList->Reset(mCommandAlloc.Get(), nullptr));

	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mDepthStencilBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));
	ThrowIfFailed(mCommandList->Close());

	ID3D12CommandList *cmdList[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdList), cmdList);

	FlushCommandQueue();

	// 3. set viewport and scissor rect
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.Width = windowWidth;
	mViewport.Height = windowHeight;
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;

	mScissorRect.left = 0;
	mScissorRect.right = windowWidth;
	mScissorRect.top = 0;
	mScissorRect.bottom = windowHeight;

	return true;
}

// https://gamedev.stackexchange.com/questions/60668/how-to-use-updatesubresource-and-map-unmap
/*
	They may have been referring to the actual act of updating a resource and not to the actual function call. 
	In general, UpdateSubResource should be used for default resources that are not subject to frequent updates 
	(i.e.: not every frame. 
	In this case, it is more likely that the buffer could be to be copied to a temporary buffer accessible from 
	the command buffer (due to race conditions for example). 
	It will also allow you the update sub resources (in textures, for example).

	Map/Unmap should be used when a resource is going to be updated very frequently (i.e. every frame), 
	such as some constant buffers. 
	The most common case is when you are overwriting the whole buffer with WriteDiscard. 
	There's an nVidia presentation where they reccommend this practice.
*/
bool Engine::Core::EngineCoreDirectX::CreateDefaultBuffer(void *data, UINT byteWidth, ID3D12Resource **defaultBuffer, ID3D12Resource **uploadBuffer)
{
	ID3DBlob *bufferCPU = nullptr;
	D3DCreateBlob(byteWidth, &bufferCPU);
	CopyMemory(bufferCPU->GetBufferPointer(), data, byteWidth);

	ThrowIfFailed(mDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 
		D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(byteWidth), D3D12_RESOURCE_STATE_GENERIC_READ, 
		nullptr, IID_PPV_ARGS(uploadBuffer)));

	ThrowIfFailed(mDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(byteWidth), D3D12_RESOURCE_STATE_COMMON,
		nullptr, IID_PPV_ARGS(defaultBuffer)));

	ThrowIfFailed(mCommandAlloc->Reset());
	ThrowIfFailed(mCommandList->Reset(mCommandAlloc.Get(), nullptr));

	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(*defaultBuffer, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
	D3D12_SUBRESOURCE_DATA subResourceData;
	subResourceData.pData = data;
	subResourceData.RowPitch = byteWidth;
	subResourceData.SlicePitch = byteWidth;
	UpdateSubresources<1>(mCommandList.Get(), *defaultBuffer, *uploadBuffer, 0, 0, 1, &subResourceData);
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(*defaultBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));
	ThrowIfFailed(mCommandList->Close());

	ID3D12CommandList *cmdList[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdList), cmdList);
	FlushCommandQueue();
	return true;
}

bool Engine::Core::EngineCoreDirectX::CreateShader(wstring srcFile, ID3DBlob **vs, ID3DBlob **ps)
{
	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	ID3DBlob *error = nullptr;
	HRESULT hr;
	hr = D3DCompileFromFile(srcFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", 
		compileFlags, 0, vs, &error);
	if (error != nullptr)
	{
		EngineLog::LogErrorMessageBox((char *)error->GetBufferPointer());
	}
	ThrowIfFailed(hr);

	error = nullptr;
	hr = D3DCompileFromFile(srcFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", 
		compileFlags, 0, ps, &error);
	if (error != nullptr)
	{
		EngineLog::LogErrorMessageBox((char *)error->GetBufferPointer());
	}
	ThrowIfFailed(hr);
	return true;
}

bool Engine::Core::EngineCoreDirectX::CreateTexture(wstring srcFile, UINT texId, ComPtr<ID3D12Resource> &res, ComPtr<ID3D12Resource> &uploadHeap)
{
	ThrowIfFailed(mCommandAlloc->Reset());
	ThrowIfFailed(mCommandList->Reset(mCommandAlloc.Get(), nullptr));
	ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(mDevice.Get(), mCommandList.Get(), srcFile.c_str(), res, uploadHeap));
	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList *cmdList[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdList), cmdList);
	FlushCommandQueue();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = res->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = res->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

	CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		mCbvSrvHeap->GetCPUDescriptorHandleForHeapStart());
	UINT objCbCount = mCoreResourceCount * mObjectConstBufferCount;
	UINT passCbCount = mCoreResourceCount * mPassConstBufferCount;
	UINT matCbCount = mCoreResourceCount * mMaterialConstBufferCount;
	UINT srvHeapIndex = objCbCount + passCbCount + matCbCount + texId;

	handle.Offset(srvHeapIndex, mCbvSrvHeapIncSize);
	mDevice->CreateShaderResourceView(res.Get(), &srvDesc, handle);
	return true;
}

bool Engine::Core::EngineCoreDirectX::CreatePipelineStateObject(RenderType renderType, ID3DBlob * vs, ID3DBlob * ps, ID3D12PipelineState **pipelineStateObject)
{
	// create pipeline state object
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.DSVFormat = mDepthStencilBufferFormat;
	psoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	psoDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	psoDesc.InputLayout = { mInputLayout.data(), mInputLayout.size() };
	psoDesc.NodeMask = 0;
	psoDesc.NumRenderTargets = 1;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.pRootSignature = mRootSignature.Get();
	CD3DX12_RASTERIZER_DESC rastDesc = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	rastDesc.FillMode = mFillMode;
	rastDesc.CullMode = mCullMode;
	psoDesc.RasterizerState = rastDesc;
	psoDesc.RTVFormats[0] = mBackBufferFormat;
	psoDesc.SampleDesc.Count = mEnableMsaa ? mMsaaCount : 1;
	psoDesc.SampleDesc.Quality = mEnableMsaa ? mMsaaQuality - 1 : 0;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PS = { ps->GetBufferPointer(), ps->GetBufferSize() };
	psoDesc.VS = { vs->GetBufferPointer(), vs->GetBufferSize() };

	switch (renderType)
	{
	case Stencil:
		D3D12_DEPTH_STENCIL_DESC stencilDesc;
		stencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;
		stencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_INCR;
		stencilDesc.DepthEnable = true;
		stencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		stencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		stencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;
		stencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_INCR;
		stencilDesc.StencilEnable = true;
		stencilDesc.StencilReadMask = 0xff;
		stencilDesc.StencilWriteMask = 0xff;
		psoDesc.DepthStencilState = stencilDesc;

	case Transparent:
		D3D12_RENDER_TARGET_BLEND_DESC blendDesc;
		blendDesc.BlendEnable = true;
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		blendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
		blendDesc.LogicOpEnable = false;
		blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		psoDesc.BlendState.RenderTarget[0] = blendDesc;
		break;
	}

	ThrowIfFailed(mDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(pipelineStateObject)));
	return true;
}

bool Engine::Core::EngineCoreDirectX::UpdatePosVertexBuffer(void * data, UINT byteWidth, int *mBaseVertexLocation)
{
	ThrowIfFailed(mPosVertexBufferGPU->Map(0, nullptr, &mPosVertexBufferData));
	memcpy((BYTE *)mPosVertexBufferData + mPosVertexBufferOffset, data, byteWidth);
	mPosVertexBufferGPU->Unmap(0, nullptr);
	*mBaseVertexLocation = mPosVertexBufferOffset / sizeof(EngineVertexPosDirectX);
	mPosVertexBufferOffset += byteWidth;
	return true;
}

bool Engine::Core::EngineCoreDirectX::UpdatePropVertexBuffer(void * data, UINT byteWidth)
{
	ThrowIfFailed(mPropVertexBufferGPU->Map(0, nullptr, &mPropVertexBufferData));
	memcpy((BYTE *)mPropVertexBufferData + mPropVertexBufferOffset, data, byteWidth);
	mPropVertexBufferGPU->Unmap(0, nullptr);
	mPropVertexBufferOffset += byteWidth;
	return true;
}

bool Engine::Core::EngineCoreDirectX::UpdateIndexBuffer(void * data, UINT byteWidth, UINT *mStartIndexLocation)
{
	ThrowIfFailed(mIndexBufferGPU->Map(0, nullptr, &mIndexBufferData));
	memcpy((BYTE *)mIndexBufferData + mIndexBufferOffset, data, byteWidth);
	mIndexBufferGPU->Unmap(0, nullptr);
	*mStartIndexLocation = mIndexBufferOffset / sizeof(UINT);
	mIndexBufferOffset += byteWidth;
	return true;
}

void Engine::Core::EngineCoreDirectX::FlushCommandQueue()
{
	mCurrentFence++;
	ThrowIfFailed(mCommandQueue->Signal(mFence.Get(), mCurrentFence));
	if (mFence->GetCompletedValue() < mCurrentFence)
	{
		HANDLE handle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
		ThrowIfFailed(mFence->SetEventOnCompletion(mCurrentFence, handle));

		WaitForSingleObject(handle, INFINITE);
		CloseHandle(handle);
	}
}

void Engine::Core::EngineCoreDirectX::BeginDraw(EngineCameraDirectX * camera)
{
	EngineCoreResource res = mCoreResource[mCurCoreResourceIndex];
	if (res.mCurrentFence != 0 && mFence->GetCompletedValue() < res.mCurrentFence)
	{
		HANDLE handle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
		ThrowIfFailed(mFence->SetEventOnCompletion(res.mCurrentFence, handle));
		WaitForSingleObject(handle, INFINITE);
		CloseHandle(handle);
	}

	ThrowIfFailed(res.mCommandAlloc->Reset());
	ThrowIfFailed(mCommandList->Reset(res.mCommandAlloc.Get(), nullptr));

	// D3D12_RESOURCE_STATE_COMMON = D3D12_RESOURCE_STATE_PRESENT = 0
	/*
		The resource is used as a render target.
		A subresource must be in this state when it is rendered to or when it is cleared with
		ID3D12GraphicsCommandList::ClearRenderTargetView.
	*/
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBackBuffer[mCurBackBuffer].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
	mCommandList->RSSetScissorRects(1, &mScissorRect);
	mCommandList->RSSetViewports(1, &mViewport);
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsv = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		mDsvHeap->GetCPUDescriptorHandleForHeapStart());
	mCommandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0,
		nullptr);
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtv = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		mRtvHeap->GetCPUDescriptorHandleForHeapStart(), mCurBackBuffer, mRtvHeapIncSize);
	mCommandList->ClearRenderTargetView(rtv, LightSteelBlue, 0, nullptr);
	mCommandList->OMSetRenderTargets(1, &rtv, true, &dsv);

	// only set descriptor heap once
	// https://stackoverflow.com/questions/55440621/how-should-setdescriptorheaps-be-used
	ID3D12DescriptorHeap* descriptorHeaps[] = { mCbvSrvHeap.Get() };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

	mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// copy pass const buffer to GPU
	ThrowIfFailed(res.mPassConstBuffer->Map(0, nullptr, reinterpret_cast<void **>(&res.mConstBufferData)));
	UINT passCbSize = CalcConstantBufferByteSize(sizeof(PassConstants));
	PassConstants passConstants;
	passConstants.eyePosW = camera->mViewPos;
	passConstants.ambientLight = { 0.25f, 0.25f, 0.35f, 1.0f };
	passConstants.lightCount = 3;
	passConstants.lights[0].type = LightType::Directional;
	passConstants.lights[0].direction = { 0.57735f, -0.57735f, 0.57735f };
	passConstants.lights[0].color = { 0.6f, 0.6f, 0.6f };

	passConstants.lights[1].type = LightType::Directional;
	passConstants.lights[1].direction = { -0.57735f, -0.57735f, 0.57735f };
	passConstants.lights[1].color = { 0.3f, 0.3f, 0.3f };

	passConstants.lights[2].type = LightType::Directional;
	passConstants.lights[2].direction = { 0.0f, -0.707f, -0.707f };
	passConstants.lights[2].color = { 0.15f, 0.15f, 0.15f };

	memcpy(res.mConstBufferData + 0 * passCbSize, &passConstants, sizeof(PassConstants));
	res.mPassConstBuffer->Unmap(0, nullptr);

	UINT cbvHeapIndex = mCurCoreResourceIndex * (mObjectConstBufferCount + mPassConstBufferCount + mMaterialConstBufferCount);
	cbvHeapIndex += mObjectConstBufferCount;
	CD3DX12_GPU_DESCRIPTOR_HANDLE cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(
		mCbvSrvHeap->GetGPUDescriptorHandleForHeapStart());
	cbvHandle.Offset(cbvHeapIndex, mCbvSrvHeapIncSize);
	mCommandList->SetGraphicsRootDescriptorTable(1, cbvHandle);
}

void Engine::Core::EngineCoreDirectX::DrawObject(EngineObjectDirectX * object, EngineCameraDirectX * camera)
{
	EngineCoreResource res = mCoreResource[mCurCoreResourceIndex];

	// copy material const buffer to GPU
	ThrowIfFailed(res.mMaterialConstBuffer->Map(0, nullptr, reinterpret_cast<void **>(&res.mConstBufferData)));
	UINT matCbSize = CalcConstantBufferByteSize(sizeof(MaterialConstants));
	MaterialConstants matConstants;
	matConstants.diffuseAlbedo = object->mMaterial->diffuseAlbedo;
	matConstants.fresnelR0 = object->mMaterial->fresnelR0;
	matConstants.shininess = object->mMaterial->shininess;
	memcpy(res.mConstBufferData + object->mMaterial->mID * matCbSize, &matConstants, sizeof(MaterialConstants));
	res.mMaterialConstBuffer->Unmap(0, nullptr);
	UINT cbvHeapIndex = mCurCoreResourceIndex * (mObjectConstBufferCount + mPassConstBufferCount + mMaterialConstBufferCount);
	cbvHeapIndex += mObjectConstBufferCount + mPassConstBufferCount + object->mMaterial->mID;
	CD3DX12_GPU_DESCRIPTOR_HANDLE cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(
		mCbvSrvHeap->GetGPUDescriptorHandleForHeapStart());
	cbvHandle.Offset(cbvHeapIndex, mCbvSrvHeapIncSize);
	mCommandList->SetGraphicsRootDescriptorTable(2, cbvHandle);

	// copy object const buffer to GPU
	ThrowIfFailed(res.mObjectConstBuffer->Map(0, nullptr, reinterpret_cast<void **>(&res.mConstBufferData)));
	XMMATRIX world = XMLoadFloat4x4(&object->mWorldMatrix);
	XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(world), world);
	XMMATRIX view = XMLoadFloat4x4(&camera->mViewMatrix);
	XMMATRIX proj = XMLoadFloat4x4(&camera->mProjMatrix);
	XMMATRIX worldViewProj = world * view * proj;
	ObjectConstants objConstants;
	// https://blog.csdn.net/u014038143/article/details/78192194
	XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
	XMStoreFloat4x4(&objConstants.InvWorld, XMMatrixTranspose(invWorld));
	XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(worldViewProj));

	UINT objCbSize = CalcConstantBufferByteSize(sizeof(ObjectConstants));
	memcpy(res.mConstBufferData + object->mID * objCbSize, &objConstants, sizeof(ObjectConstants));
	res.mObjectConstBuffer->Unmap(0, nullptr);

	cbvHeapIndex = mCurCoreResourceIndex * (mObjectConstBufferCount + mPassConstBufferCount + mMaterialConstBufferCount);
	cbvHeapIndex = cbvHeapIndex + object->mID;
	cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(mCbvSrvHeap->GetGPUDescriptorHandleForHeapStart());
	cbvHandle.Offset(cbvHeapIndex, mCbvSrvHeapIncSize);
	mCommandList->SetGraphicsRootDescriptorTable(0, cbvHandle);

	// set texture descriptor
	UINT objCbCount = mCoreResourceCount * mObjectConstBufferCount;
	UINT passCbCount = mCoreResourceCount * mPassConstBufferCount;
	UINT matCbCount = mCoreResourceCount * mMaterialConstBufferCount;
	UINT srvHeapIndex = objCbCount + passCbCount + matCbCount + object->mTexture->mID;
	CD3DX12_GPU_DESCRIPTOR_HANDLE srvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(
		mCbvSrvHeap->GetGPUDescriptorHandleForHeapStart());
	srvHandle.Offset(srvHeapIndex, mCbvSrvHeapIncSize);
	mCommandList->SetGraphicsRootDescriptorTable(3, srvHandle);

	mCommandList->SetPipelineState(object->mPipelineState.Get());

	mCommandList->IASetVertexBuffers(0, 2, object->mBatched ? mVertexBufferViews.data() : object->VertexBufferViews());

	mCommandList->IASetIndexBuffer(object->mBatched ? &mIndexBufferView : object->IndexBufferView());

	mCommandList->DrawIndexedInstanced(object->mIndexCount, 1, object->mStartIndexLocation, 
		object->mBaseVertexLocation, 0);
}

void Engine::Core::EngineCoreDirectX::EndDraw()
{
	// https://docs.microsoft.com/en-us/windows/win32/direct3d12/using-resource-barriers-to-synchronize-resource-states-in-direct3d-12#initial-states-for-resources
	/*
		Swap chain back buffers automatically start out in the D3D12_RESOURCE_STATE_COMMON state.
		Before a back buffer is presented, it must be in the D3D12_RESOURCE_STATE_COMMON state.
		Note, the resource state D3D12_RESOURCE_STATE_PRESENT is a synonym for D3D12_RESOURCE_STATE_COMMON,
		and they both have a value of 0.

		If IDXGISwapChain::Present (or IDXGISwapChain1::Present1) is called on a resource that
		is not currently in this state, a debug layer warning is emitted.
	*/
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBackBuffer[mCurBackBuffer].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
	ThrowIfFailed(mCommandList->Close());

	ID3D12CommandList *cmdList[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdList), cmdList);

	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurBackBuffer = (mCurBackBuffer + 1) % mBackBufferCount;

	mCurrentFence++;
	ThrowIfFailed(mCommandQueue->Signal(mFence.Get(), mCurrentFence));
	EngineCoreResource res = mCoreResource[mCurCoreResourceIndex];
	res.mCurrentFence = mCurrentFence;
	mCurCoreResourceIndex = (mCurCoreResourceIndex + 1) % mCoreResourceCount;
}
