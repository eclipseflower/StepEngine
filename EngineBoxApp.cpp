#include "EngineBoxApp.h"

EngineManagerDirectX manager;

EngineBoxApp::EngineBoxApp()
{
	mLastMousePos.x = 0;
	mLastMousePos.y = 0;
}


EngineBoxApp::~EngineBoxApp()
{
}

int EngineBoxApp::Run(HINSTANCE hInstance)
{
	if (!manager.InitEngineWindow(hInstance))
	{
		return -1;
	}

	if (!manager.InitEngineCore(false, 4))
	{
		return -1;
	}

	EngineShaderDirectX *shader;
	if (!manager.CreateShader(L"color.hlsl", &shader))
	{
		return -1;
	}

	EngineMaterialDirectX *material;
	if (!manager.CreateMaterial(&material))
	{
		return -1;
	}

	material->diffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
	material->fresnelR0 = XMFLOAT3(0.05f, 0.05f, 0.05f);
	material->shininess = 179.2f;

	EngineTextureDirectX *texture;
	if (!manager.CreateTexture(L"WoodCrate01.dds", &texture))
	{
		return -1;
	}

	EngineObjectDirectX *boxObject;
	if (!manager.CreateBoxObject(&boxObject, 1, 1, 1, 0, 0.5, 0))
	{
		return -1;
	}

	boxObject->SetShader(RenderType::Opaque, shader);
	boxObject->SetMaterial(material);
	boxObject->SetTexture(texture);


	EngineTextureDirectX *texture2;
	if (!manager.CreateTexture(L"water1.dds", &texture2))
	{
		return -1;
	}

	/*
	EngineObjectDirectX *box2Object;
	if (!manager.CreateBoxObject(&box2Object, 1, 1, 1, -0.5, 0, -0.5))
	{
		return -1;
	}

	box2Object->SetShader(RenderType::Transparent, shader);
	box2Object->SetMaterial(material);
	box2Object->SetTexture(texture2);
	*/

	EngineObjectDirectX *box3Object;
	if (!manager.CreateBoxObject(&box3Object, 1, 1, 1, 0, 0.5, 0))
	{
		return -1;
	}

	EngineMaterialDirectX *material2;
	if (!manager.CreateMaterial(&material2))
	{
		return -1;
	}

	material2->diffuseAlbedo = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);
	material2->fresnelR0 = XMFLOAT3(0.001f, 0.001f, 0.001f);
	material2->shininess = 0.0f;

	XMMATRIX world = XMLoadFloat4x4(&box3Object->mWorldMatrix);
	XMVECTOR shadowPlane = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // xz plane
	XMVECTOR toMainLight = XMVectorSet(-0.57735f, 0.57735f, -0.57735f, 0.0f);
	XMMATRIX shadow = XMMatrixShadow(shadowPlane, toMainLight);
	XMMATRIX shadowOffsetY = XMMatrixTranslation(0.0f, 0.001f, 0.0f);
	XMStoreFloat4x4(&box3Object->mWorldMatrix, world * shadow * shadowOffsetY);

	box3Object->SetShader(RenderType::Stencil, shader);
	box3Object->SetMaterial(material2);
	box3Object->SetTexture(texture);

	/*
	EngineObjectDirectX *fileObject;
	if (!manager.CreateObjectFromFile("skull.txt", &fileObject))
	{
		return -1;
	}
	fileObject->SetShader(RenderType::Opaque, shader);
	fileObject->SetMaterial(material);
	fileObject->SetTexture(texture);
	*/
	
	manager.mUpdateFunc = bind(&EngineBoxApp::Update, this, _1);
	manager.mMouseDownFunc = bind(&EngineBoxApp::OnMouseDown, this, _1, _2, _3);
	manager.mMouseUpFunc = bind(&EngineBoxApp::OnMouseUp, this, _1, _2, _3);
	manager.mMouseMoveFunc = bind(&EngineBoxApp::OnMouseMove, this, _1, _2, _3);
	
	return manager.RunEngine();
}

void EngineBoxApp::Update(float deltaTime)
{
	float x = mRadius * sinf(mPhi) * cosf(mTheta);
	float z = mRadius * sinf(mPhi) * sinf(mTheta);
	float y = mRadius * cosf(mPhi);

	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	manager.CameraLookAt(pos, target, up);
}

void EngineBoxApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(manager.GetHwnd());
}

void EngineBoxApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void EngineBoxApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		mTheta += dx;
		mPhi += dy;

		if (mPhi < 0.1f)
		{
			mPhi = 0.1f;
		}
		if (mPhi > XM_PI - 0.1f)
		{
			mPhi = XM_PI - 0.1f;
		}
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		float dx = 0.005f*static_cast<float>(x - mLastMousePos.x);
		float dy = 0.005f*static_cast<float>(y - mLastMousePos.y);

		mRadius += dx - dy;

		if (mRadius < 3.0f)
		{
			mRadius = 3.0f;
		}
		if (mRadius > 15.0)
		{
			mRadius = 15.0;
		}
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}
