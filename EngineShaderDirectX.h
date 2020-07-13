#ifndef __ENGINE_SHADER_DIRECTX_H__
#define __ENGINE_SHADER_DIRECTX_H__

#include "EngineUtilDirectX.h"
#include "EngineLog.h"

using Engine::Debug::EngineLog;

namespace Engine
{
	namespace Shader
	{
		class EngineShaderDirectX
		{
		public:
			//ID3DX11Effect *mEffect = nullptr;
			//ID3D11InputLayout *mInputLayout = nullptr;
			//ID3DX11EffectTechnique *mMainTech = nullptr;
			//ID3DX11EffectMatrixVariable *mMVPMatrix = nullptr;

			EngineShaderDirectX();
			~EngineShaderDirectX();
			void InitEffect();
		};
	}
}

#endif // __ENGINE_SHADER_DIRECTX_H__


