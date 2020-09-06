#ifndef __ENGINE_OBJECT_DIRECTX_H__
#define __ENGINE_OBJECT_DIRECTX_H__

#include "EngineUtilDirectX.h"
#include "EngineShaderDirectX.h"

using Engine::Shader::EngineShaderDirectX;
using std::vector;

namespace Engine
{
	namespace Object
	{
		class EngineObjectDirectX
		{
		public:
			int mVertexCount;
			int mIndexCount;
			vector<EngineVertexPosDirectX> mPosVertices;
			vector<EngineVertexPropDirectX> mPropVertices;
			vector<UINT> mIndices;
			vector<D3D12_VERTEX_BUFFER_VIEW> mVertexBufferViews;
			
			ComPtr<ID3D12Resource> mPosVertexBufferGPU = nullptr;
			ComPtr<ID3D12Resource> mPosVertexBufferUploader = nullptr;
			ComPtr<ID3D12Resource> mPropVertexBufferGPU = nullptr;
			ComPtr<ID3D12Resource> mPropVertexBufferUploader = nullptr;
			ComPtr<ID3D12Resource> mIndexBufferGPU = nullptr;
			ComPtr<ID3D12Resource> mIndexBufferUploader = nullptr;
			ComPtr<ID3D12PipelineState> mPipelineState = nullptr;

			XMFLOAT4X4 mWorldMatrix;

			EngineShaderDirectX *mShader;

			EngineObjectDirectX();
			~EngineObjectDirectX();

			const D3D12_VERTEX_BUFFER_VIEW *VertexBufferViews();
			D3D12_INDEX_BUFFER_VIEW IndexBufferView() const;

			void SetShader(EngineShaderDirectX *shader);
		};
	}
}

#endif // !__ENGINE_OBJECT_DIRECTX_H__
