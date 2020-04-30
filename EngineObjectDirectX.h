#ifndef __ENGINE_OBJECT_DIRECTX_H__
#define __ENGINE_OBJECT_DIRECTX_H__

#include "EngineUtil.h"

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
			vector<EngineVertexDirectX> mVertices;
			vector<UINT> mIndices;
			ID3D11Buffer *mVertexBuffer;
			ID3D11Buffer *mIndexBuffer;

			XMFLOAT4X4 mWorldMatrix;

			EngineObjectDirectX();
			~EngineObjectDirectX();
		};
	}
}

#endif // !__ENGINE_OBJECT_DIRECTX_H__
