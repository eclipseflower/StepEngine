#include "EngineManagerDirectX.h"

Engine::EngineManagerDirectX::EngineManagerDirectX()
{
	gManagerDirectX = this;
	mWindowInst = nullptr;
}

Engine::EngineManagerDirectX::~EngineManagerDirectX()
{
	if (mWindowInst)
	{
		delete mWindowInst;
		mWindowInst = nullptr;
	}
}

bool Engine::EngineManagerDirectX::InitEngineWindow(HINSTANCE hInstance)
{
	mWindowInst = new EngineWindowDirectX(hInstance);
	if (!mWindowInst->Init())
	{
		delete mWindowInst;
		mWindowInst = nullptr;
		return false;
	}
}
