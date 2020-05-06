#include "EngineShaderDirectX.h"
#include "EngineManagerDirectX.h"

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
		ID3DX11EffectTechnique *tech = mEffect->GetTechniqueByName("MainTech");
		if (tech->IsValid())
		{
			mMainTech = tech;
		}

		ID3DX11EffectMatrixVariable *var = mEffect->GetVariableByName("_MVP")->AsMatrix();
		if (var->IsValid())
		{
			mMVPMatrix = var;
		}

		if (mMainTech)
		{
			D3DX11_PASS_DESC passDesc;
			ID3DX11EffectPass *pass = mMainTech->GetPassByIndex(0);
			if (pass->IsValid())
			{
				HRESULT hr = pass->GetDesc(&passDesc);
				if (FAILED(hr))
				{
					EngineLog::LogErrorMessageBox("GetDesc Failed");
				}
				else
				{
					gManagerDirectX->CreateInputLayout(&passDesc, &mInputLayout);
				}
			}
		}
	}
}
