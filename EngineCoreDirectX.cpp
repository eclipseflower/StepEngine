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

/*
bool Engine::Core::EngineCoreDirectX::CreateVertexBuffer(void *vertices, UINT byteWidth, D3D11_USAGE usage, UINT cpuAccessFlags, ID3D11Buffer **buffer)
{
	D3D11_BUFFER_DESC desc;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = byteWidth;
	desc.CPUAccessFlags = cpuAccessFlags;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.Usage = usage;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertices;
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

bool Engine::Core::EngineCoreDirectX::CreateShader(string srcFile, ID3DX11Effect **effect)
{
	DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob *compileShader = nullptr;
	ID3D10Blob *compileMsg = nullptr;
	HRESULT hr = D3DX11CompileFromFile(srcFile.c_str(), nullptr, nullptr, nullptr, "fx_5_0", shaderFlags, 0, nullptr,
		&compileShader, &compileMsg, nullptr);
	if (compileMsg != nullptr)
	{
		EngineLog::LogErrorMessageBox((char *)compileMsg->GetBufferPointer());
		compileMsg->Release();
		return false;
	}
	if (FAILED(hr))
	{
		EngineLog::LogErrorMessageBox("D3DX11CompileFromFile Failed");
		return false;
	}

	hr = D3DX11CreateEffectFromMemory(compileShader->GetBufferPointer(), compileShader->GetBufferSize(), 0, mD3dDevice, 
		effect);

	compileShader->Release();
	if (FAILED(hr))
	{
		EngineLog::LogErrorMessageBox("D3DX11CreateEffectFromMemory Failed");
		return false;
	}
	return true;
}

bool Engine::Core::EngineCoreDirectX::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC * vertexDesc, const UINT vertexDescCount, D3DX11_PASS_DESC * passDesc, ID3D11InputLayout ** layout)
{
	HRESULT hr = mD3dDevice->CreateInputLayout(vertexDesc, vertexDescCount, passDesc->pIAInputSignature, 
		passDesc->IAInputSignatureSize, layout);
	if (FAILED(hr))
	{
		EngineLog::LogErrorMessageBox("CreateInputLayout Failed");
		return false;
	}
	return true;
}
*/

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

		The resource is used as a render target.
		A subresource must be in this state when it is rendered to or when it is cleared with
		ID3D12GraphicsCommandList::ClearRenderTargetView.
	*/
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBackBuffer[mCurBackBuffer].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
	ThrowIfFailed(mCommandList->Close());

	ID3D12CommandList *cmdList[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdList), cmdList);

	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurBackBuffer = (mCurBackBuffer + 1) % 2;
	FlushCommandQueue();
}
