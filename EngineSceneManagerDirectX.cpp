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
	(*object)->mPosVertices.push_back({ XMFLOAT3(-1.0f, -1.0f, -1.0f) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(-1.0f, +1.0f, -1.0f) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(+1.0f, +1.0f, -1.0f) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(+1.0f, -1.0f, -1.0f) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(-1.0f, -1.0f, +1.0f) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(-1.0f, +1.0f, +1.0f) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(+1.0f, +1.0f, +1.0f) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(+1.0f, -1.0f, +1.0f) });

	bool res = gManagerDirectX->CreateDefaultBuffer((*object)->mPosVertices.data(),
		sizeof(EngineVertexPosDirectX) * (*object)->mVertexCount,
		&(*object)->mPosVertexBufferGPU, &(*object)->mPosVertexBufferUploader);

	if (!res)
	{
		return false;
	}

	(*object)->mPropVertices.push_back({ (XMFLOAT4)White });
	(*object)->mPropVertices.push_back({ (XMFLOAT4)Black });
	(*object)->mPropVertices.push_back({ (XMFLOAT4)Red });
	(*object)->mPropVertices.push_back({ (XMFLOAT4)Green });
	(*object)->mPropVertices.push_back({ (XMFLOAT4)Blue });
	(*object)->mPropVertices.push_back({ (XMFLOAT4)Yellow });
	(*object)->mPropVertices.push_back({ (XMFLOAT4)Cyan });
	(*object)->mPropVertices.push_back({ (XMFLOAT4)Magenta });

	res = gManagerDirectX->CreateDefaultBuffer((*object)->mPropVertices.data(),
		sizeof(EngineVertexPropDirectX) * (*object)->mVertexCount,
		&(*object)->mPropVertexBufferGPU, &(*object)->mPropVertexBufferUploader);

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

	res = gManagerDirectX->CreateDefaultBuffer((*object)->mIndices.data(), sizeof(UINT) * (*object)->mIndexCount,
		&(*object)->mIndexBufferGPU, &(*object)->mIndexBufferUploader);

	if (!res)
	{
		return false;
	}

	XMMATRIX i = XMMatrixIdentity();
	XMStoreFloat4x4(&(*object)->mWorldMatrix, i);

	mSceneObjects.push_back(*object);

	return true;
}

bool Engine::Core::EngineSceneManagerDirectX::CreateCylinderObject(float topRadius, float bottomRadius, float height, EngineObjectDirectX ** object)
{
	/*
	UINT sliceCount = 100;
	UINT stackCount = 1;
	float halfHeight = height / 2;
	float heightPerStack = height / stackCount;
	float radiusPerStack = (topRadius - bottomRadius) / stackCount;
	float thetaPerSlice = XM_2PI / sliceCount;

	*object = new EngineObjectDirectX;
	(*object)->mVertexCount = (sliceCount + 1) * (stackCount + 1) + 2;
	
	for (UINT i = 0; i <= stackCount; i++)
	{
		float y = -halfHeight + i * heightPerStack;
		float r = bottomRadius + i * radiusPerStack;

		for (UINT j = 0; j <= sliceCount; j++)
		{
			float theta = j * thetaPerSlice;
			float x = r * cos(theta);
			float z = r * sin(theta);
			(*object)->mVertices.push_back({ XMFLOAT3(x, y, z), (const float*)&Red });
		}
	}
	(*object)->mVertices.push_back({ XMFLOAT3(0, -halfHeight, 0), (const float*)&Red });
	(*object)->mVertices.push_back({ XMFLOAT3(0, halfHeight, 0), (const float*)&Red });

	bool res = gManagerDirectX->CreateVertexBuffer(&(*object)->mVertices[0],
		sizeof(EngineVertexDirectX) * (*object)->mVertexCount, D3D11_USAGE_IMMUTABLE, 0, &(*object)->mVertexBuffer);
	
	if (!res)
	{
		return false;
	}

	(*object)->mIndexCount = sliceCount * 3 * 2 + sliceCount * 6 * stackCount;
	for (UINT i = 0; i < stackCount; i++)
	{
		for (UINT j = 0; j < sliceCount; j++)
		{
			(*object)->mIndices.push_back(i * (sliceCount + 1) + j);
			(*object)->mIndices.push_back((i + 1) * (sliceCount + 1) + j);
			(*object)->mIndices.push_back(i * (sliceCount + 1) + j + 1);

			(*object)->mIndices.push_back((i + 1) * (sliceCount + 1) + j);
			(*object)->mIndices.push_back((i + 1) * (sliceCount + 1) + j + 1);
			(*object)->mIndices.push_back(i * (sliceCount + 1) + j + 1);
		}
	}

	for (UINT i = 0; i < sliceCount; i++)
	{
		(*object)->mIndices.push_back((sliceCount + 1) * (stackCount + 1));
		(*object)->mIndices.push_back(i);
		(*object)->mIndices.push_back(i + 1);
	}

	for (UINT i = 0; i < sliceCount; i++)
	{
		(*object)->mIndices.push_back((sliceCount + 1) * stackCount + i + 1);
		(*object)->mIndices.push_back((sliceCount + 1) * stackCount + i);
		(*object)->mIndices.push_back((sliceCount + 1) * (stackCount + 1) + 1);
	}



	res = gManagerDirectX->CreateIndexBuffer(&(*object)->mIndices[0], sizeof(UINT) * (*object)->mIndexCount,
		D3D11_USAGE_IMMUTABLE, &(*object)->mIndexBuffer);

	if (!res)
	{
		return false;
	}

	XMMATRIX i = XMMatrixIdentity();
	XMStoreFloat4x4(&(*object)->mWorldMatrix, i);

	mSceneObjects.push_back(*object);
	*/
	return true;
}
