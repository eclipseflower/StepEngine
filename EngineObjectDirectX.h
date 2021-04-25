#ifndef __ENGINE_OBJECT_DIRECTX_H__
#define __ENGINE_OBJECT_DIRECTX_H__

#include "EngineUtilDirectX.h"
#include "EngineShaderDirectX.h"
#include "EngineMaterialDirectX.h"
#include "EngineTextureDirectX.h"

using Engine::Shader::EngineShaderDirectX;
using Engine::Shader::EngineMaterialDirectX;
using Engine::Shader::EngineTextureDirectX;
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
			vector<EngineVertexPointDirectX> mPointVertices;
			vector<UINT> mIndices;
			vector<UINT> mPointIndices;
			vector<D3D12_VERTEX_BUFFER_VIEW> mVertexBufferViews;
			D3D12_INDEX_BUFFER_VIEW mIndexBufferView;

			UINT mID = 0;
			bool mBatched = false;
			UINT mStartIndexLocation = 0;
			int mBaseVertexLocation = 0;
			UINT mStartPointIndexLocation = 0;
			int mBasePointVertexLocation = 0;
			
			ComPtr<ID3D12Resource> mPosVertexBufferGPU = nullptr;
			ComPtr<ID3D12Resource> mPosVertexBufferUploader = nullptr;
			ComPtr<ID3D12Resource> mPropVertexBufferGPU = nullptr;
			ComPtr<ID3D12Resource> mPropVertexBufferUploader = nullptr;
			ComPtr<ID3D12Resource> mIndexBufferGPU = nullptr;
			ComPtr<ID3D12Resource> mIndexBufferUploader = nullptr;
			ComPtr<ID3D12PipelineState> mPipelineState = nullptr;

			XMFLOAT4X4 mWorldMatrix;

			RenderType mRenderType;
			EngineShaderDirectX *mShader;
			EngineMaterialDirectX *mMaterial;
			EngineTextureDirectX *mTexture;

			EngineObjectDirectX();
			~EngineObjectDirectX();

			const D3D12_VERTEX_BUFFER_VIEW *VertexBufferViews();
			const D3D12_INDEX_BUFFER_VIEW *IndexBufferView();

			void SetShader(RenderType renderType, EngineShaderDirectX *shader);
			void SetMaterial(EngineMaterialDirectX *material);
			void SetTexture(EngineTextureDirectX *texture);
		};
	}
}

#endif // !__ENGINE_OBJECT_DIRECTX_H__
