#include "EngineObjectDirectX.h"
#include "EngineManagerDirectX.h"

Engine::Object::EngineObjectDirectX::EngineObjectDirectX() : mVertexBufferViews(2)
{

}

Engine::Object::EngineObjectDirectX::~EngineObjectDirectX()
{
}

const D3D12_VERTEX_BUFFER_VIEW * Engine::Object::EngineObjectDirectX::VertexBufferViews()
{
	for (int i = 0; i < mVertexBufferViews.size(); i++)
	{
		mVertexBufferViews[i].BufferLocation = mVertexBufferGPU->GetGPUVirtualAddress();
	}
	D3D12_VERTEX_BUFFER_VIEW vbv;
	vbv.BufferLocation = mVertexBufferGPU->GetGPUVirtualAddress();
	vbv.SizeInBytes = sizeof(EngineVertexDirectX) * mVertexCount;
	vbv.StrideInBytes = sizeof(EngineVertexDirectX);

	return mVertexBufferViews.data();
}

D3D12_INDEX_BUFFER_VIEW Engine::Object::EngineObjectDirectX::IndexBufferView() const
{
	D3D12_INDEX_BUFFER_VIEW ibv;
	ibv.BufferLocation = mIndexBufferGPU->GetGPUVirtualAddress();
	ibv.Format = DXGI_FORMAT_R32_UINT;
	ibv.SizeInBytes = mIndexCount * sizeof(UINT);

	return ibv;
}

void Engine::Object::EngineObjectDirectX::SetShader(EngineShaderDirectX * shader)
{
	mShader = shader;
	gManagerDirectX->CreatePipelineStateObject(shader, &mPipelineState);
}
