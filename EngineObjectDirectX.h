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
			//vector<EngineVertexDirectX> mVertices;
			vector<UINT> mIndices;
			//ID3D11Buffer *mVertexBuffer;
			//ID3D11Buffer *mIndexBuffer;

			//XMFLOAT4X4 mWorldMatrix;

			EngineShaderDirectX *mShader;

			EngineObjectDirectX();
			~EngineObjectDirectX();
		};
	}
}

#endif // !__ENGINE_OBJECT_DIRECTX_H__
