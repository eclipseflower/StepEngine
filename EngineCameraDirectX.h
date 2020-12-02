#ifndef __ENGINE_CAMERA_DIRECTX_H__
#define __ENGINE_CAMERA_DIRECTX_H__

#include "EngineUtilDirectX.h"

namespace Engine
{
	namespace Camera
	{
		class EngineCameraDirectX
		{
		public:
			EngineCameraDirectX();
			~EngineCameraDirectX();

			void SetLens(float fovY, float aspect, float zn, float zf);
			void LookAt(const XMVECTOR& pos, const XMVECTOR& target, const XMVECTOR& up);

			XMFLOAT4X4 mViewMatrix;
			XMFLOAT4X4 mProjMatrix;
			XMFLOAT3 mViewPos;
		};
	}
}

#endif // !__ENGINE_CAMERA_DIRECTX_H__