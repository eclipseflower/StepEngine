#ifndef __ENGINE_SCENE_MANAGER_DIRECTX_H__
#define __ENGINE_SCENE_MANAGER_DIRECTX_H__

#include "EngineObjectDirectX.h"
using Engine::Object::EngineObjectDirectX;

namespace Engine
{
	namespace Core
	{
		class EngineSceneManagerDirectX
		{
		public:
			EngineSceneManagerDirectX();
			~EngineSceneManagerDirectX();
			bool CreateBoxObject(EngineObjectDirectX **object);
			bool CreateCylinderObject(float topRadius, float bottomRadius, float height, EngineObjectDirectX **object);

			vector<EngineObjectDirectX *> mSceneObjects;

			vector<ComPtr<ID3D12Resource>> mPosVertexBuffersGPU;
			vector<ComPtr<ID3D12Resource>> mPosVertexBuffersUploader;
			vector<ComPtr<ID3D12Resource>> mPropVertexBuffersGPU;
			vector<ComPtr<ID3D12Resource>> mPropVertexBuffersUploader;
			vector<ComPtr<ID3D12Resource>> mIndexBuffersGPU;
			vector<ComPtr<ID3D12Resource>> mIndexBuffersUploader;
		};
	}
}

#endif // __ENGINE_SCENE_MANAGER_DIRECTX_H__
