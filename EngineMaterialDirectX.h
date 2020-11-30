#ifndef __ENGINE_MATERIAL_DIRECTX_H__
#define __ENGINE_MATERIAL_DIRECTX_H__

#include "EngineUtilDirectX.h"

namespace Engine
{
	namespace Shader
	{
		class EngineMaterialDirectX
		{
		public:
			XMFLOAT4 diffuseAlbedo;
			XMFLOAT3 fresnelR0;
			float shininess;

			UINT mID = 0;
		};
	}
}

#endif // __ENGINE_SHADER_DIRECTX_H__


