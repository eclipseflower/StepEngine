#include "EngineCoreDirectX.h"
#include "EngineManagerDirectX.h"

Engine::Core::EngineCoreDirectX::EngineCoreDirectX(bool enableMsaa, UINT msaaCount)
{
	mEnableMsaa = enableMsaa;
	mMsaaCount = msaaCount;
	mMsaaQuality = 0;
	mD3dDevice = nullptr;
	mD3dImmediateContext = nullptr;
	mSwapChain = nullptr;
	mDepthStencilBuffer = nullptr;
	mRenderTargetView = nullptr;
	mDepthStencilView = nullptr;
}

Engine::Core::EngineCoreDirectX::~EngineCoreDirectX()
{
	if (mD3dDevice)
	{
		mD3dDevice->Release();
	}
	if (mD3dImmediateContext)
	{
		mD3dImmediateContext->Release();
	}
	if (mSwapChain)
	{
		mSwapChain->Release();
	}
	if (mDepthStencilBuffer)
	{
		mDepthStencilBuffer->Release();
	}
	if (mRenderTargetView)
	{
		mRenderTargetView->Release();
	}
	if (mDepthStencilView)
	{
		mDepthStencilView->Release();
	}
}

bool Engine::Core::EngineCoreDirectX::Init()
{
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, nullptr, 0, 
		D3D11_SDK_VERSION, &mD3dDevice, &featureLevel, &mD3dImmediateContext);

	if (FAILED(hr))
	{
		EngineLog::LogErrorMessageBox("D3D11CreateDevice Failed");
		return false;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		EngineLog::LogErrorMessageBox("Direct3D Feature Level 11 Unsupported");
		return false;
	}

	if (mEnableMsaa)
	{
		hr = mD3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, mMsaaCount, &mMsaaQuality);
		if (FAILED(hr))
		{
			EngineLog::LogErrorMessageBox("CheckMultisampleQualityLevels Failed");
			return false;
		}

		if (mMsaaQuality <= 0)
		{
			EngineLog::LogErrorMessageBox("Current MSAA Count Unsupported");
			return false;
		}
	}

	IDXGIDevice *dxgiDevice = nullptr;
	hr = mD3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&dxgiDevice);
	if (FAILED(hr))
	{
		EngineLog::LogErrorMessageBox("QueryInterface Failed");
		if (dxgiDevice)
		{
			dxgiDevice->Release();
			dxgiDevice = nullptr;
		}
		return false;
	}

	IDXGIAdapter *dxgiAdapter = nullptr;
	hr = dxgiDevice->GetAdapter(&dxgiAdapter);
	if (FAILED(hr))
	{
		EngineLog::LogErrorMessageBox("IDXGIDevice GetAdapter Failed");
		if (dxgiAdapter)
		{
			dxgiAdapter->Release();
			dxgiAdapter = nullptr;
		}
		return false;
	}

	IDXGIFactory *dxgiFactory = nullptr;
	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&dxgiFactory);
	if (FAILED(hr))
	{
		EngineLog::LogErrorMessageBox("IDXGIAdapter GetParent Failed");
		if (dxgiFactory)
		{
			dxgiFactory->Release();
			dxgiFactory = nullptr;
		}
		return false;
	}

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Width = gManagerDirectX->GetWindowWidth();
	swapChainDesc.BufferDesc.Height = gManagerDirectX->GetWindowHeight();
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.Flags = 0;
	swapChainDesc.OutputWindow = gManagerDirectX->GetHwnd();
	if (mEnableMsaa)
	{
		swapChainDesc.SampleDesc.Count = mMsaaCount;
		swapChainDesc.SampleDesc.Quality = mMsaaQuality - 1;
	}
	else
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = true;

	hr = dxgiFactory->CreateSwapChain(mD3dDevice, &swapChainDesc, &mSwapChain);

	if (dxgiDevice)
	{
		dxgiDevice->Release();
		dxgiDevice = nullptr;
	}
	if (dxgiAdapter)
	{
		dxgiAdapter->Release();
		dxgiAdapter = nullptr;
	}
	if (dxgiFactory)
	{
		dxgiFactory->Release();
		dxgiFactory = nullptr;
	}

	if (FAILED(hr))
	{
		EngineLog::LogErrorMessageBox("CreateSwapChain Failed");
		return false;
	}

	if (!ResizeBuffer())
	{
		EngineLog::LogErrorMessageBox("ResizeBuffer Failed");
		return false;
	}

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
