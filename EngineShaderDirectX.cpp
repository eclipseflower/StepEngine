#include "EngineShaderDirectX.h"

Engine::Shader::EngineShaderDirectX::EngineShaderDirectX()
{
}

Engine::Shader::EngineShaderDirectX::~EngineShaderDirectX()
{
}

void Engine::Shader::EngineShaderDirectX::InitEffect()
{
	if (mEffect)
	{
		ID3DX11EffectVariable *var = mEffect->GetVariableByName("_MVP")->AsMatrix();
		if (var->IsValid())
		{
			mMVPMatrix = var;
		}
	}
}
