#include "EngineSceneManagerDirectX.h"
#include "EngineManagerDirectX.h"

Engine::Core::EngineSceneManagerDirectX::EngineSceneManagerDirectX()
{
}

Engine::Core::EngineSceneManagerDirectX::~EngineSceneManagerDirectX()
{

}

bool Engine::Core::EngineSceneManagerDirectX::CreateBoxObject(EngineObjectDirectX ** object, float width, float height, float depth, float posx, float posy, float posz)
{
	float w2 = 0.5f * width;
	float h2 = 0.5f * height;
	float d2 = 0.5f * depth;

	*object = new EngineObjectDirectX;

	(*object)->mVertexCount = 24;
	(*object)->mBatched = true;

	// Fill in the front face vertex data.
	(*object)->mPosVertices.push_back({ XMFLOAT3(-w2, -h2, -d2) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(-w2, +h2, -d2) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(+w2, +h2, -d2) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(+w2, -h2, -d2) });

	// Fill in the back face vertex data.
	(*object)->mPosVertices.push_back({ XMFLOAT3(-w2, -h2, +d2) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(+w2, -h2, +d2) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(+w2, +h2, +d2) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(-w2, +h2, +d2) });

	// Fill in the top face vertex data.
	(*object)->mPosVertices.push_back({ XMFLOAT3(-w2, +h2, -d2) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(-w2, +h2, +d2) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(+w2, +h2, +d2) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(+w2, +h2, -d2) });

	// Fill in the bottom face vertex data.
	(*object)->mPosVertices.push_back({ XMFLOAT3(-w2, -h2, -d2) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(+w2, -h2, -d2) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(+w2, -h2, +d2) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(-w2, -h2, +d2) });

	// Fill in the left face vertex data.
	(*object)->mPosVertices.push_back({ XMFLOAT3(-w2, -h2, +d2) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(-w2, +h2, +d2) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(-w2, +h2, -d2) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(-w2, -h2, -d2) });

	// Fill in the right face vertex data.
	(*object)->mPosVertices.push_back({ XMFLOAT3(+w2, -h2, -d2) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(+w2, +h2, -d2) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(+w2, +h2, +d2) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(+w2, -h2, +d2) });

	bool res = gManagerDirectX->CreateDefaultBuffer((*object)->mPosVertices.data(),
		sizeof(EngineVertexPosDirectX) * (*object)->mVertexCount,
		&(*object)->mPosVertexBufferGPU, &(*object)->mPosVertexBufferUploader);

	if (!res)
	{
		return false;
	}

	res = gManagerDirectX->UpdatePosVertexBuffer((*object)->mPosVertices.data(),
		sizeof(EngineVertexPosDirectX) * (*object)->mVertexCount, &(*object)->mBaseVertexLocation);

	if (!res)
	{
		return false;
	}

	// Fill in the front face vertex data.
	(*object)->mPropVertices.push_back({ XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) });
	(*object)->mPropVertices.push_back({ XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) });
	(*object)->mPropVertices.push_back({ XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) });
	(*object)->mPropVertices.push_back({ XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) });

	// Fill in the back face vertex data.
	(*object)->mPropVertices.push_back({ XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) });
	(*object)->mPropVertices.push_back({ XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) });
	(*object)->mPropVertices.push_back({ XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) });
	(*object)->mPropVertices.push_back({ XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) });

	// Fill in the top face vertex data.
	(*object)->mPropVertices.push_back({ XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) });
	(*object)->mPropVertices.push_back({ XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
	(*object)->mPropVertices.push_back({ XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) });
	(*object)->mPropVertices.push_back({ XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) });

	// Fill in the bottom face vertex data.
	(*object)->mPropVertices.push_back({ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) });
	(*object)->mPropVertices.push_back({ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) });
	(*object)->mPropVertices.push_back({ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
	(*object)->mPropVertices.push_back({ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) });

	// Fill in the left face vertex data.
	(*object)->mPropVertices.push_back({ XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) });
	(*object)->mPropVertices.push_back({ XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
	(*object)->mPropVertices.push_back({ XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) });
	(*object)->mPropVertices.push_back({ XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) });

	// Fill in the right face vertex data.
	(*object)->mPropVertices.push_back({ XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) });
	(*object)->mPropVertices.push_back({ XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
	(*object)->mPropVertices.push_back({ XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) });
	(*object)->mPropVertices.push_back({ XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) });

	res = gManagerDirectX->CreateDefaultBuffer((*object)->mPropVertices.data(),
		sizeof(EngineVertexPropDirectX) * (*object)->mVertexCount,
		&(*object)->mPropVertexBufferGPU, &(*object)->mPropVertexBufferUploader);

	if (!res)
	{
		return false;
	}

	res = gManagerDirectX->UpdatePropVertexBuffer((*object)->mPropVertices.data(),
		sizeof(EngineVertexPropDirectX) * (*object)->mVertexCount);

	if (!res)
	{
		return false;
	}

	(*object)->mIndexCount = 36;
	UINT indices[] = {
		// Fill in the front face index data
		0, 1, 2,
		0, 2, 3,

		// Fill in the back face index data
		4, 5, 6,
		4, 6, 7,

		// Fill in the top face index data
		8, 9, 10,
		8, 10, 11,

		// Fill in the bottom face index data
		12, 13, 14,
		12, 14, 15,

		// Fill in the left face index data
		16, 17, 18,
		16, 18, 19,

		// Fill in the right face index data
		20, 21, 22,
		20, 22, 23,
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

	res = gManagerDirectX->UpdateIndexBuffer((*object)->mIndices.data(), sizeof(UINT) * (*object)->mIndexCount,
		&(*object)->mStartIndexLocation);

	if (!res)
	{
		return false;
	}

	XMMATRIX i = XMMatrixIdentity();
	XMStoreFloat4x4(&(*object)->mWorldMatrix, i);

	(*object)->mID = mCurSceneObjectIndex++;
	mSceneObjects.push_back(*object);

	return true;
}

bool Engine::Core::EngineSceneManagerDirectX::CreatePyramidObject(EngineObjectDirectX ** object)
{
	*object = new EngineObjectDirectX;

	(*object)->mVertexCount = 5;
	(*object)->mBatched = true;

	(*object)->mPosVertices.push_back({ XMFLOAT3(+0.0f, +2.0f, +0.0f) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(-1.0f, +0.0f, +1.0f) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(+1.0f, +0.0f, +1.0f) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(-1.0f, +0.0f, -1.0f) });
	(*object)->mPosVertices.push_back({ XMFLOAT3(+1.0f, +0.0f, -1.0f) });

	bool res = gManagerDirectX->CreateDefaultBuffer((*object)->mPosVertices.data(),
		sizeof(EngineVertexPosDirectX) * (*object)->mVertexCount,
		&(*object)->mPosVertexBufferGPU, &(*object)->mPosVertexBufferUploader);

	if (!res)
	{
		return false;
	}

	res = gManagerDirectX->UpdatePosVertexBuffer((*object)->mPosVertices.data(),
		sizeof(EngineVertexPosDirectX) * (*object)->mVertexCount, &(*object)->mBaseVertexLocation);

	if (!res)
	{
		return false;
	}

	(*object)->mPropVertices.push_back({ (XMFLOAT4)White });
	(*object)->mPropVertices.push_back({ (XMFLOAT4)Black });
	(*object)->mPropVertices.push_back({ (XMFLOAT4)Red });
	(*object)->mPropVertices.push_back({ (XMFLOAT4)Green });
	(*object)->mPropVertices.push_back({ (XMFLOAT4)Blue });

	res = gManagerDirectX->CreateDefaultBuffer((*object)->mPropVertices.data(),
		sizeof(EngineVertexPropDirectX) * (*object)->mVertexCount,
		&(*object)->mPropVertexBufferGPU, &(*object)->mPropVertexBufferUploader);

	if (!res)
	{
		return false;
	}

	res = gManagerDirectX->UpdatePropVertexBuffer((*object)->mPropVertices.data(),
		sizeof(EngineVertexPropDirectX) * (*object)->mVertexCount);

	if (!res)
	{
		return false;
	}

	(*object)->mIndexCount = 18;
	UINT indices[] = {
		// front face
		0, 3, 1,
		0, 2, 4,

		// back face
		0, 4, 3,
		0, 1, 2,

		// left face
		1, 3, 2,
		2, 3, 4
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

	res = gManagerDirectX->UpdateIndexBuffer((*object)->mIndices.data(), sizeof(UINT) * (*object)->mIndexCount,
		&(*object)->mStartIndexLocation);

	if (!res)
	{
		return false;
	}

	XMMATRIX i = XMMatrixIdentity();
	i = i * XMMatrixTranslation(0, 1, 0);
	XMStoreFloat4x4(&(*object)->mWorldMatrix, i);

	(*object)->mID = mCurSceneObjectIndex++;
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

bool Engine::Core::EngineSceneManagerDirectX::CreateObjectFromFile(string filename, EngineObjectDirectX ** object)
{
	*object = new EngineObjectDirectX;
	(*object)->mBatched = true;

	ifstream fin(filename);
	if (!fin)
	{
		return false;
	}

	string ignore;
	fin >> ignore >> (*object)->mVertexCount;
	fin >> ignore >> (*object)->mIndexCount;
	(*object)->mIndexCount *= 3;
	fin >> ignore >> ignore >> ignore >> ignore;

	float posx, posy, posz;
	float normalx, normaly, normalz;
	for (int i = 0; i < (*object)->mVertexCount; i++)
	{
		fin >> posx >> posy >> posz;
		fin >> normalx >> normaly >> normalz;
		(*object)->mPosVertices.push_back({ XMFLOAT3(posx, posy, posz) });
		(*object)->mPropVertices.push_back({ XMFLOAT3(normalx, normaly, normalz), XMFLOAT2(0, 0) });
	}

	fin >> ignore >> ignore >> ignore;

	int index;
	for (int i = 0; i < (*object)->mIndexCount; i++)
	{
		fin >> index;
		(*object)->mIndices.push_back(index);
	}

	fin.close();

	bool res = gManagerDirectX->CreateDefaultBuffer((*object)->mPosVertices.data(),
		sizeof(EngineVertexPosDirectX) * (*object)->mVertexCount,
		&(*object)->mPosVertexBufferGPU, &(*object)->mPosVertexBufferUploader);

	if (!res)
	{
		return false;
	}

	res = gManagerDirectX->UpdatePosVertexBuffer((*object)->mPosVertices.data(),
		sizeof(EngineVertexPosDirectX) * (*object)->mVertexCount, &(*object)->mBaseVertexLocation);

	if (!res)
	{
		return false;
	}

	res = gManagerDirectX->CreateDefaultBuffer((*object)->mPropVertices.data(),
		sizeof(EngineVertexPropDirectX) * (*object)->mVertexCount,
		&(*object)->mPropVertexBufferGPU, &(*object)->mPropVertexBufferUploader);

	if (!res)
	{
		return false;
	}

	res = gManagerDirectX->UpdatePropVertexBuffer((*object)->mPropVertices.data(),
		sizeof(EngineVertexPropDirectX) * (*object)->mVertexCount);

	if (!res)
	{
		return false;
	}

	res = gManagerDirectX->CreateDefaultBuffer((*object)->mIndices.data(), sizeof(UINT) * (*object)->mIndexCount,
		&(*object)->mIndexBufferGPU, &(*object)->mIndexBufferUploader);

	if (!res)
	{
		return false;
	}

	res = gManagerDirectX->UpdateIndexBuffer((*object)->mIndices.data(), sizeof(UINT) * (*object)->mIndexCount,
		&(*object)->mStartIndexLocation);

	if (!res)
	{
		return false;
	}

	XMMATRIX i = XMMatrixIdentity();
	XMStoreFloat4x4(&(*object)->mWorldMatrix, i);

	(*object)->mID = mCurSceneObjectIndex++;
	mSceneObjects.push_back(*object);

	return true;
}
