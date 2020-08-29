#include "EngineCoreDirectX.h"
#include "EngineManagerDirectX.h"

Engine::Core::EngineCoreDirectX::EngineCoreDirectX(bool enableMsaa, UINT msaaCount)
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

	// 5. create cbv heap
	mCbvHeapIncSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.NumDescriptors = 1;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NodeMask = 0;
	ThrowIfFailed(mDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mCbvHeap)));

	// 6. create const buffer and view
	UINT cbSize = CalcConstantBufferByteSize(sizeof(ObjectConstants));
	ThrowIfFailed(mDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(cbSize), D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&mConstBuffer)));

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = mConstBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = cbSize;
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		mCbvHeap->GetCPUDescriptorHandleForHeapStart());
	mDevice->CreateConstantBufferView(&cbvDesc, handle);

	// 7. create root signature
	CD3DX12_DESCRIPTOR_RANGE cbvTable;
	cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	CD3DX12_ROOT_PARAMETER rootParams[1];
	rootParams[1].InitAsDescriptorTable(1, &cbvTable);
	CD3DX12_ROOT_SIGNATURE_DESC sigDesc(1, rootParams, 0, nullptr, 
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
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

	// 8. create input layout
	mInputLayout =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};

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

	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(*defaultBuffer, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
	D3D12_SUBRESOURCE_DATA subResourceData;
	subResourceData.pData = data;
	subResourceData.RowPitch = byteWidth;
	subResourceData.SlicePitch = byteWidth;
	UpdateSubresources<1>(mCommandList.Get(), *defaultBuffer, *uploadBuffer, 0, 0, 1, &subResourceData);
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(*defaultBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));
	return true;
}

/*
bool Engine::Core::EngineCoreDirectX::CreateIndexBuffer(void *indices, UINT byteWidth, D3D11_USAGE usage, ID3D11Buffer **buffer)
{
	D3D11_BUFFER_DESC desc;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = byteWidth;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.Usage = usage;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = indices;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT hr = mD3dDevice->CreateBuffer(&desc, &data, buffer);
	if (FAILED(hr))
	{
		EngineLog::LogErrorMessageBox("CreateBuffer Failed");
		return false;
	}

	return true;
}
*/

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

bool Engine::Core::EngineCoreDirectX::CreatePipelineStateObject(ID3DBlob * vs, ID3DBlob * ps, ID3D12PipelineState **pipelineStateObject)
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
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.RTVFormats[0] = mBackBufferFormat;
	psoDesc.SampleDesc.Count = mEnableMsaa ? mMsaaCount : 1;
	psoDesc.SampleDesc.Quality = mEnableMsaa ? mMsaaQuality - 1 : 0;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PS = { ps->GetBufferPointer(), ps->GetBufferSize() };
	psoDesc.VS = { vs->GetBufferPointer(), vs->GetBufferSize() };

	ThrowIfFailed(mDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(pipelineStateObject)));
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

void Engine::Core::EngineCoreDirectX::BeginDraw()
{
	ThrowIfFailed(mCommandAlloc->Reset());
	ThrowIfFailed(mCommandList->Reset(mCommandAlloc.Get(), nullptr));

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
}

void Engine::Core::EngineCoreDirectX::DrawObject(EngineObjectDirectX * object, EngineCameraDirectX * camera)
{

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
	FlushCommandQueue();
}
