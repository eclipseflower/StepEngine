#include "EngineCoreDirectX.h"

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
		return false;
	}

	IDXGIAdapter *dxgiAdapter = nullptr;
	hr = dxgiDevice->GetAdapter(&dxgiAdapter);
	if (FAILED(hr))
	{
		EngineLog::LogErrorMessageBox("IDXGIDevice GetAdapter Failed");
		return false;
	}

	IDXGIFactory *dxgiFactory = nullptr;
	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&dxgiFactory);
	if (FAILED(hr))
	{
		EngineLog::LogErrorMessageBox("IDXGIAdapter GetParent Failed");
		return false;
	}

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferCount = 1;

	dxgiFactory->CreateSwapChain(mD3dDevice, nullptr, &mSwapChain);

	return true;
}
