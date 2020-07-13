#include "EngineCameraDirectX.h"

Engine::Camera::EngineCameraDirectX::EngineCameraDirectX()
{
}

Engine::Camera::EngineCameraDirectX::~EngineCameraDirectX()
{
}

void Engine::Camera::EngineCameraDirectX::SetLens(float fovY, float aspect, float zn, float zf)
{
	//XMMATRIX p = XMMatrixPerspectiveFovLH(fovY, aspect, zn, zf);
	//XMStoreFloat4x4(&mProjMatrix, p);
}

/*
void Engine::Camera::EngineCameraDirectX::LookAt(const XMVECTOR & pos, const XMVECTOR & target, const XMVECTOR & up)
{
	XMMATRIX v = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mViewMatrix, v);
}
*/