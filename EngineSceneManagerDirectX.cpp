#include "EngineSceneManagerDirectX.h"
#include "EngineManagerDirectX.h"

Engine::Core::EngineSceneManagerDirectX::EngineSceneManagerDirectX()
{
}

Engine::Core::EngineSceneManagerDirectX::~EngineSceneManagerDirectX()
{
	for (UINT i = 0; i < mSceneObjects.size(); ++i)
	{
		EngineObjectDirectX *object = mSceneObjects[i];
		object->mVertexBuffer->Release();
		object->mIndexBuffer->Release();
		object->mShader->mEffect->Release();
		object->mShader->mInputLayout->Release();
	}
}

bool Engine::Core::EngineSceneManagerDirectX::CreateBoxObject(EngineObjectDirectX ** object)
{
	*object = new EngineObjectDirectX;

	(*object)->mVertexCount = 8;
	(*object)->mVertices.push_back({ XMFLOAT3(-1.0f, -1.0f, -1.0f), (const float*)&White }),
	(*object)->mVertices.push_back({ XMFLOAT3(-1.0f, +1.0f, -1.0f), (const float*)&Black });
	(*object)->mVertices.push_back({ XMFLOAT3(+1.0f, +1.0f, -1.0f), (const float*)&Red });
	(*object)->mVertices.push_back({ XMFLOAT3(+1.0f, -1.0f, -1.0f), (const float*)&Green });
	(*object)->mVertices.push_back({ XMFLOAT3(-1.0f, -1.0f, +1.0f), (const float*)&Blue });
	(*object)->mVertices.push_back({ XMFLOAT3(-1.0f, +1.0f, +1.0f), (const float*)&Yellow });
	(*object)->mVertices.push_back({ XMFLOAT3(+1.0f, +1.0f, +1.0f), (const float*)&Cyan });
	(*object)->mVertices.push_back({ XMFLOAT3(+1.0f, -1.0f, +1.0f), (const float*)&Magenta });

	bool res = gManagerDirectX->CreateVertexBuffer(&(*object)->mVertices[0], 
		sizeof(EngineVertexDirectX) * (*object)->mVertexCount, D3D11_USAGE_IMMUTABLE, 0, &(*object)->mVertexBuffer);

	if (!res)
	{
		return false;
	}

	(*object)->mIndexCount = 36;
	UINT indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	for (int i = 0; i < (*object)->mIndexCount; ++i)
	{
		(*object)->mIndices.push_back(indices[i]);
	}

	res = gManagerDirectX->CreateIndexBuffer(indices, sizeof(UINT) * (*object)->mIndexCount,
		D3D11_USAGE_IMMUTABLE, &(*object)->mIndexBuffer);

	if (!res)
	{
		return false;
	}

	XMMATRIX i = XMMatrixIdentity();
	XMStoreFloat4x4(&(*object)->mWorldMatrix, i);

	mSceneObjects.push_back(*object);
	return true;
}
