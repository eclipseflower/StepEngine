#include "EngineObjectDirectX.h"
#include "EngineManagerDirectX.h"

Engine::Object::EngineObjectDirectX::EngineObjectDirectX()
{
}

Engine::Object::EngineObjectDirectX::~EngineObjectDirectX()
{
}

void Engine::Object::EngineObjectDirectX::SetShader(EngineShaderDirectX * shader)
{
	mShader = shader;
	gManagerDirectX->CreatePipelineStateObject(shader, &mPipelineState);
}
