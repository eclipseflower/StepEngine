#include "EngineSceneManagerDirectX.h"
#include "EngineManagerDirectX.h"

Engine::Core::EngineSceneManagerDirectX::EngineSceneManagerDirectX()
{
}

Engine::Core::EngineSceneManagerDirectX::~EngineSceneManagerDirectX()
{
}

bool Engine::Core::EngineSceneManagerDirectX::CreateBoxObject(EngineObjectDirectX ** object)
{
	*object = new EngineObjectDirectX;
	(*object)->mVertexCount = 8;
	(*object)->mVertices.push_back({ XMFLOAT3(-1.0f, -1.0f, -1.0f), (const float*)&White });
	(*object)->mVertices.push_back({ XMFLOAT3(-1.0f, -1.0f, -1.0f), (const float*)&White }),
	(*object)->mVertices.push_back({ XMFLOAT3(-1.0f, +1.0f, -1.0f), (const float*)&White });
	(*object)->mVertices.push_back({ XMFLOAT3(+1.0f, +1.0f, -1.0f), (const float*)&White });
	(*object)->mVertices.push_back({ XMFLOAT3(+1.0f, -1.0f, -1.0f), (const float*)&White });
	(*object)->mVertices.push_back({ XMFLOAT3(-1.0f, -1.0f, +1.0f), (const float*)&White });
	(*object)->mVertices.push_back({ XMFLOAT3(-1.0f, +1.0f, +1.0f), (const float*)&White });
	(*object)->mVertices.push_back({ XMFLOAT3(+1.0f, +1.0f, +1.0f), (const float*)&White });
	(*object)->mVertices.push_back({ XMFLOAT3(+1.0f, -1.0f, +1.0f), (const float*)&White });

	bool res = gManagerDirectX->CreateVertexBuffer(&(*object)->mVertices, 
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

	XMMATRIX matrix = XMMatrixIdentity();
	XMStoreFloat4x4(&(*object)->mWorldMatrix, matrix);

	return true;
}
