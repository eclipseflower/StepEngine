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
	mVertexBufferViews[0].BufferLocation = mPosVertexBufferGPU->GetGPUVirtualAddress();
	mVertexBufferViews[0].SizeInBytes = sizeof(EngineVertexPosDirectX) * mVertexCount;
	mVertexBufferViews[0].StrideInBytes = sizeof(EngineVertexPosDirectX);

	mVertexBufferViews[1].BufferLocation = mPropVertexBufferGPU->GetGPUVirtualAddress();
	mVertexBufferViews[1].SizeInBytes = sizeof(EngineVertexPropDirectX) * mVertexCount;
	mVertexBufferViews[1].StrideInBytes = sizeof(EngineVertexPropDirectX);

	return mVertexBufferViews.data();
}

const D3D12_INDEX_BUFFER_VIEW * Engine::Object::EngineObjectDirectX::IndexBufferView()
{
	mIndexBufferView.BufferLocation = mIndexBufferGPU->GetGPUVirtualAddress();
	mIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	mIndexBufferView.SizeInBytes = mIndexCount * sizeof(UINT);

	return &mIndexBufferView;
}

void Engine::Object::EngineObjectDirectX::SetShader(EngineShaderDirectX * shader)
{
	mShader = shader;
	gManagerDirectX->CreatePipelineStateObject(shader, &mPipelineState);
}
