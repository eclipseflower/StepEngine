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
	mTimerInst.Reset();
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
			static float interval = 0.0f;
			interval += mTimerInst.DeltaTime();
			if (interval >= 1.0)
			{
				interval = 0.0f;
				ostringstream outs;
				outs.precision(6);
				outs << " FPS: " << fps;
				mWindowInst->SetCaption(outs.str());
			}
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

bool Engine::EngineManagerDirectX::CreateVertexBuffer(void *vertices, UINT byteWidth, D3D11_USAGE usage, UINT cpuAccessFlags, ID3D11Buffer **buffer)
{
	if (mCoreInst)
	{
		return mCoreInst->CreateVertexBuffer(vertices, byteWidth, usage, cpuAccessFlags, buffer);
	}
	return false;
}

void Engine::EngineManagerDirectX::CreateBoxObject(EngineObjectDirectX **object)
{
	mSceneMgrInst.CreateBoxObject(object);
}
