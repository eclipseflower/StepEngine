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

void Engine::EngineManagerDirectX::EngineLoop(bool paused)
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

		if (mUpdateFunc)
		{
			mUpdateFunc(mTimerInst.DeltaTime());
		}

		EngineDraw();
	}
}

void Engine::EngineManagerDirectX::EngineDraw()
{
	if (mCoreInst)
	{
		mCoreInst->BeginDraw();
		for (UINT i = 0; i < mSceneMgrInst.mSceneObjects.size(); ++i)
		{
			EngineObjectDirectX *object = mSceneMgrInst.mSceneObjects[i];
			mCoreInst->DrawObject(object, &mCameraInst);
		}
		mCoreInst->EndDraw();
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
	if (mCoreInst)
	{
		mCoreInst->ResizeBuffer();
	}
	float aspect = 0.0f;
	if (mWindowInst)
	{
		aspect = mWindowInst->GetAspectRatio();
		mCameraInst.SetLens(XM_PIDIV4, aspect, 1.0f, 1000.0f);
	}
}

void Engine::EngineManagerDirectX::OnMouseDown(WPARAM btnState, int x, int y)
{
	mMouseDownFunc(btnState, x, y);
}

void Engine::EngineManagerDirectX::OnMouseUp(WPARAM btnState, int x, int y)
{
	mMouseUpFunc(btnState, x, y);
}

void Engine::EngineManagerDirectX::OnMouseMove(WPARAM btnState, int x, int y)
{
	mMouseMoveFunc(btnState, x, y);
}

void Engine::EngineManagerDirectX::CameraLookAt(const XMVECTOR & pos, const XMVECTOR & target, const XMVECTOR & up)
{
	mCameraInst.LookAt(pos, target, up);
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

bool Engine::EngineManagerDirectX::CreateDefaultBuffer(void *data, UINT byteWidth, ID3D12Resource **bufferGPU, ID3D12Resource **uploadBuffer)
{
	if (mCoreInst)
	{
		return mCoreInst->CreateDefaultBuffer(data, byteWidth, bufferGPU, uploadBuffer);
	}
	return false;
}

bool Engine::EngineManagerDirectX::CreateBoxObject(EngineObjectDirectX **object)
{
	return mSceneMgrInst.CreateBoxObject(object);
}

bool Engine::EngineManagerDirectX::CreatePyramidObject(EngineObjectDirectX ** object)
{
	return mSceneMgrInst.CreateCylinderObject(object);
}

bool Engine::EngineManagerDirectX::CreateCylinderObject(float topRadius, float bottomRadius, float height, EngineObjectDirectX ** object)
{
	return mSceneMgrInst.CreateCylinderObject(topRadius, bottomRadius, height, object);
}

bool Engine::EngineManagerDirectX::CreateShader(wstring srcFile, EngineShaderDirectX ** shader)
{
	if (mCoreInst)
	{
		*shader = new EngineShaderDirectX;
		if (mCoreInst->CreateShader(srcFile, &(*shader)->mVSByteCode, &(*shader)->mPSByteCode))
		{
			return true;
		}
		return false;
	}
	return false;
}

bool Engine::EngineManagerDirectX::CreatePipelineStateObject(EngineShaderDirectX * shader, ID3D12PipelineState **pipelineStateObject)
{
	if (mCoreInst)
	{
		if (mCoreInst->CreatePipelineStateObject(shader->mVSByteCode.Get(), shader->mPSByteCode.Get(), pipelineStateObject))
		{
			return true;
		}
	}
	return false;
}

bool Engine::EngineManagerDirectX::UpdatePosVertexBuffer(void * data, UINT byteWidth)
{
	if (mCoreInst)
	{
		if (mCoreInst->UpdatePosVertexBuffer(data, byteWidth))
		{
			return true;
		}
	}
	return false;
}

bool Engine::EngineManagerDirectX::UpdatePropVertexBuffer(void * data, UINT byteWidth)
{
	if (mCoreInst)
	{
		if (mCoreInst->UpdatePropVertexBuffer(data, byteWidth))
		{
			return true;
		}
	}
	return false;
}

bool Engine::EngineManagerDirectX::UpdateIndexBuffer(void * data, UINT byteWidth)
{
	if (mCoreInst)
	{
		if (mCoreInst->UpdateIndexBuffer(data, byteWidth))
		{
			return true;
		}
	}
	return false;
}
