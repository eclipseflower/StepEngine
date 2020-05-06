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

			vector<EngineObjectDirectX *> mSceneObjects;
		};
	}
}

#endif // __ENGINE_SCENE_MANAGER_DIRECTX_H__
