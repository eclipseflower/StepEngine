#ifndef __ENGINE_UTIL_DIRECTX_H__
#define __ENGINE_UTIL_DIRECTX_H__

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx11effect.h>
#include <directxmath.h>
#include <windows.h>


using DirectX::XMVECTORF32;
using DirectX::XMFLOAT3;
using DirectX::XMFLOAT4;
using DirectX::XMFLOAT4X4;
using DirectX::XMMATRIX;
using DirectX::XMMatrixIdentity;

namespace Engine
{
	XMGLOBALCONST XMVECTORF32 Zero = { 0.0f, 0.0f, 0.0f, 0.0f };

	XMGLOBALCONST XMVECTORF32 White		= { 1.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Black		= { 0.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Red		= { 1.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Green		= { 0.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Blue		= { 0.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Yellow	= { 1.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Cyan		= { 0.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Magenta	= { 1.0f, 0.0f, 1.0f, 1.0f };

	struct EngineVertexDirectX
	{
		XMFLOAT3 position;
		XMFLOAT4 color;

		EngineVertexDirectX(XMFLOAT3 position, const float *color)
		{
			this->position = position;
			this->color = XMFLOAT4(color);
		}
	};
}

#endif // __ENGINE_UTIL_DIRECTX_H__
