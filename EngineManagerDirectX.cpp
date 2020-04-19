#include "EngineManagerDirectX.h"

namespace Engine
{
	EngineManagerDirectX * gManagerDirectX = nullptr;
}

Engine::EngineManagerDirectX::EngineManagerDirectX()
{
	gManagerDirectX = this;
	mWindowInst = nullptr;
	mCoreInst = nullptr;
}

Engine::EngineManagerDirectX::~EngineManagerDirectX()
{
	if (mWindowInst)
	{
		delete mWindowInst;
		mWindowInst = nullptr;
	}

	if (mCoreInst)
	{
		delete mCoreInst;
		mCoreInst = nullptr;
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

	return true;
}

bool Engine::EngineManagerDirectX::InitEngineCore(bool enableMsaa, UINT msaaCount)
{
	mCoreInst = new EngineCoreDirectX(enableMsaa, msaaCount);
	if (!mCoreInst->Init())
	{
		delete mCoreInst;
		mCoreInst = nullptr;
		return false;
	}
	return true;
}

int Engine::EngineManagerDirectX::RunEngine()
{
	if (mWindowInst)
	{
		return mWindowInst->WindowLoop();
	}
	return 0;
}

void Engine::EngineManagerDirectX::EngnineLoop(bool paused)
{
	mTimerInst.Tick();
	if (!paused)
	{
		float fps = mTimerInst.FPS();
		if (mWindowInst)
		{
		}
	}
}

void Engine::EngineManagerDirectX::OnPause(bool paused)
{
	if (paused)
	{
		mTimerInst.Stop();
	}
	else
	{
		mTimerInst.Start();
	}
}

void Engine::EngineManagerDirectX::OnResize()
{
}

UINT Engine::EngineManagerDirectX::GetWindowWidth()
{
	if (mWindowInst)
	{
		return mWindowInst->GetWidth();
	}
	return 0;
}

UINT Engine::EngineManagerDirectX::GetWindowHeight()
{
	if (mWindowInst)
	{
		return mWindowInst->GetHeight();
	}
	return 0;
}

HWND Engine::EngineManagerDirectX::GetHwnd()
{
	if (mWindowInst)
	{
		return mWindowInst->GetHandle();
	}
	return nullptr;
}
