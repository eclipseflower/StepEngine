#ifndef __ENGINE_SHADER_DIRECTX_H__
#define __ENGINE_SHADER_DIRECTX_H__

#include "EngineUtil.h"

namespace Engine
{
	namespace Shader
	{
		class EngineShaderDirectX
		{
		public:
			ID3DX11Effect *mEffect;
			ID3DX11EffectVariable *mMVPMatrix;

			EngineShaderDirectX();
			~EngineShaderDirectX();
			void InitEffect();
		};
	}
}

#endif // __ENGINE_SHADER_DIRECTX_H__


