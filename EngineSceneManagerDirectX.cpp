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
}
