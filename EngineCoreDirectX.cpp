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
	chainDesc.SampleDesc.Count = mMsaaCount;
	chainDesc.SampleDesc.Quality = mMsaaQuality - 1;

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
	if (mRenderTargetView)
	{
		mRenderTargetView->Release();
		mRenderTargetView = nullptr;
	}
	if (mDepthStencilView)
	{
		mDepthStencilView->Release();
		mDepthStencilView = nullptr;
	}
	if (mDepthStencilBuffer)
	{
		mDepthStencilBuffer->Release();
		mDepthStencilBuffer = nullptr;
	}

	UINT windowWidth = gManagerDirectX->GetWindowWidth();
	UINT windowHeight = gManagerDirectX->GetWindowHeight();

	HRESULT hr = mSwapChain->ResizeBuffers(1, windowWidth, windowHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	if (FAILED(hr))
	{
		EngineLog::LogErrorMessageBox("ResizeBuffers Failed");
		return false;
	}
	ID3D11Texture2D *backBuffer = nullptr;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&backBuffer));
	if (FAILED(hr))
	{
		EngineLog::LogErrorMessageBox("GetBuffer Failed");
		return false;
	}
	hr = mD3dDevice->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView);
	// MUST RELEASE
	backBuffer->Release();
	if (FAILED(hr))
	{
		EngineLog::LogErrorMessageBox("CreateRenderTargetView Failed");
		return false;
	}

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Height = windowHeight;
	depthStencilDesc.Width = windowWidth;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.MiscFlags = 0;
	if (mEnableMsaa)
	{
		depthStencilDesc.SampleDesc.Count = mMsaaCount;
		depthStencilDesc.SampleDesc.Quality = mMsaaQuality - 1;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	hr = mD3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &mDepthStencilBuffer);
	if (FAILED(hr))
	{
		EngineLog::LogErrorMessageBox("CreateTexture2D Failed");
		return false;
	}
	mD3dDevice->CreateDepthStencilView(mDepthStencilBuffer, nullptr, &mDepthStencilView);
	mD3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	mScreenViewport.Height = windowHeight;
	mScreenViewport.MaxDepth = 1.0f;
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.TopLeftX = 0.0f;
	mScreenViewport.TopLeftY = 0.0f;
	mScreenViewport.Width = windowWidth;
	mD3dImmediateContext->RSSetViewports(1, &mScreenViewport);
	return true;
}

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

void Engine::Core::EngineCoreDirectX::BeginDraw()
{
	mD3dImmediateContext->ClearRenderTargetView(mRenderTargetView, Black);
	mD3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Engine::Core::EngineCoreDirectX::DrawObject(EngineObjectDirectX * object, EngineCameraDirectX * camera)
{
	UINT stride = sizeof(EngineVertexDirectX);
	UINT offset = 0;
	mD3dImmediateContext->IASetInputLayout(object->mShader->mInputLayout);
	mD3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mD3dImmediateContext->IASetVertexBuffers(0, 1, &object->mVertexBuffer, &stride, &offset);
	mD3dImmediateContext->IASetIndexBuffer(object->mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	ID3DX11EffectMatrixVariable *shaderMVP = object->mShader->mMVPMatrix;
	if (shaderMVP && shaderMVP->IsValid())
	{
		XMMATRIX world = XMLoadFloat4x4(&object->mWorldMatrix);
		XMMATRIX view = XMLoadFloat4x4(&camera->mViewMatrix);
		XMMATRIX proj = XMLoadFloat4x4(&camera->mProjMatrix);
		XMMATRIX worldViewProj = world * view * proj;
		shaderMVP->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
	}

	ID3DX11EffectTechnique *shaderTech = object->mShader->mMainTech;
	if (shaderTech && shaderTech->IsValid())
	{
		D3DX11_TECHNIQUE_DESC techDesc;
		shaderTech->GetDesc(&techDesc);
		for (UINT i = 0; i < techDesc.Passes; ++i)
		{
			shaderTech->GetPassByIndex(i)->Apply(0, mD3dImmediateContext);
			mD3dImmediateContext->DrawIndexed(object->mIndexCount, 0, 0);
		}
	}
}

void Engine::Core::EngineCoreDirectX::EndDraw()
{
	mSwapChain->Present(0, 0);
}
