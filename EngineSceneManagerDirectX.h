#ifndef __ENGINE_SCENE_MANAGER_DIRECTX_H__
#define __ENGINE_SCENE_MANAGER_DIRECTX_H__

#include "EngineObjectDirectX.h"
#include <fstream>
using Engine::Object::EngineObjectDirectX;
using std::ifstream;

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
			bool CreatePyramidObject(EngineObjectDirectX **object);
			bool CreateCylinderObject(float topRadius, float bottomRadius, float height, EngineObjectDirectX **object);
			bool CreateObjectFromFile(string filename, EngineObjectDirectX **object);

			vector<EngineObjectDirectX *> mSceneObjects;
			UINT mCurSceneObjectIndex = 0;
		};
	}
}

#endif // __ENGINE_SCENE_MANAGER_DIRECTX_H__
