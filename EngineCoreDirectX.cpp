#include "EngineCoreDirectX.h"

Engine::Core::EngineCoreDirectX::EngineCoreDirectX(bool enableMsaa)
{
	mEnableMsaa = enableMsaa;
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
	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, nullptr, 0, D3D11_SDK_VERSION,
		&mD3dDevice, &featureLevel, &mD3dImmediateContext);

	return true;
}
