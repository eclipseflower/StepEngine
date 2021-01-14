#ifndef __ENGINE_TEXTURE_DIRECTX_H__
#define __ENGINE_TEXTURE_DIRECTX_H__

#include "EngineUtilDirectX.h"

namespace Engine
{
	namespace Shader
	{
		class EngineTextureDirectX
		{
		public:
			ComPtr<ID3D12Resource> mRescource = nullptr;
			ComPtr<ID3D12Resource> mUploadHeap = nullptr;
		};
	}
}

#endif // __ENGINE_TEXTURE_DIRECTX_H__


#pragma once
