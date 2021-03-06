#ifndef __ENGINE_UTIL_DIRECTX_H__
#define __ENGINE_UTIL_DIRECTX_H__

#include <string>
#include <sstream>
#include <vector>
#include <functional>

#include <windows.h>
#include <windowsx.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <string>
#include <comdef.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <DirectXColors.h>
#include "d3dx12.h"
#include "DDSTextureLoader.h"

using std::string;
using std::wstring;

using Microsoft::WRL::ComPtr;

using DirectX::Colors::White;
using DirectX::Colors::Black;
using DirectX::Colors::Red;
using DirectX::Colors::Green;
using DirectX::Colors::Blue;
using DirectX::Colors::Yellow;
using DirectX::Colors::Cyan;
using DirectX::Colors::Magenta;
using DirectX::Colors::LightSteelBlue;

using DirectX::XMVECTORF32;
using DirectX::XMVECTOR;
using DirectX::XMVECTORF32;
using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;
using DirectX::XMFLOAT4;
using DirectX::XMFLOAT4X4;
using DirectX::XMMATRIX;
using DirectX::XMMatrixIdentity;
using DirectX::XMVectorSet;
using DirectX::XMVectorZero;
using DirectX::XMMatrixLookAtLH;
using DirectX::XMMatrixPerspectiveFovLH;
using DirectX::XMConvertToRadians;
using DirectX::XMMatrixTranslation;
using DirectX::XMStoreFloat3;
using DirectX::XMMatrixShadow;
using DirectX::XM_PI;
using DirectX::XM_PIDIV4;
using DirectX::XM_2PI;

namespace Engine
{
	class DxException
	{
	public:
		DxException() = default;
		DxException(HRESULT hr, const wstring& functionName, const wstring& filename, int lineNumber);

		wstring ToString()const;

		HRESULT ErrorCode = S_OK;
		wstring FunctionName;
		wstring Filename;
		int LineNumber = -1;
	};

	inline wstring AnsiToWString(const string& str)
	{
		WCHAR buffer[512];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
		return wstring(buffer);
	}

	#ifndef ThrowIfFailed
	#define ThrowIfFailed(x)                                              \
	{                                                                     \
		HRESULT hr__ = (x);                                               \
		wstring wfn = AnsiToWString(__FILE__);                       \
		if(FAILED(hr__)) { throw DxException(hr__, L#x, wfn, __LINE__); } \
	}
	#endif

	struct EngineVertexPosDirectX
	{
		XMFLOAT3 position;

		EngineVertexPosDirectX(XMFLOAT3 position)
		{
			this->position = position;
		}
	};

	struct EngineVertexPropDirectX
	{
		XMFLOAT3 normal;
		XMFLOAT4 color;
		XMFLOAT2 uv;

		EngineVertexPropDirectX(XMFLOAT3 normal, XMFLOAT2 uv)
		{
			this->color = (XMFLOAT4)Black;
			this->normal = normal;
			this->uv = uv;
		}

		EngineVertexPropDirectX(XMFLOAT4 color)
		{
			this->color = color;
		}
	};

	struct EngineVertexPointDirectX
	{
		XMFLOAT3 position;
		XMFLOAT2 size;

		EngineVertexPointDirectX(XMFLOAT3 position, XMFLOAT2 size)
		{
			this->position = position;
			this->size = size;
		}
	};

	static XMFLOAT4X4 Identity4x4()
	{
		static XMFLOAT4X4 I(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		return I;
	}

	enum LightType
	{
		Directional = 0,
		Point = 1,
		Spot = 2,
	};

	struct Light
	{
		int type;
		XMFLOAT3 color;
		float falloffStart;
		XMFLOAT3 direction;
		float falloffEnd;
		XMFLOAT3 position;
		float spotPower;
		XMFLOAT3 preserved;
	};

	struct ObjectConstants
	{
		XMFLOAT4X4 World = Identity4x4();
		XMFLOAT4X4 InvWorld = Identity4x4();
		XMFLOAT4X4 WorldViewProj = Identity4x4();
	};

	struct PassConstants
	{
		XMFLOAT3 eyePosW;
		int lightCount;
		XMFLOAT4 ambientLight;
		Light lights[16];
	};

	struct MaterialConstants
	{
		XMFLOAT4 diffuseAlbedo;
		XMFLOAT3 fresnelR0;
		float shininess;
	};

	static UINT CalcConstantBufferByteSize(UINT byteSize)
	{
		// Constant buffers must be a multiple of the minimum hardware
		// allocation size (usually 256 bytes).  So round up to nearest
		// multiple of 256.  We do this by adding 255 and then masking off
		// the lower 2 bytes which store all bits < 256.
		// Example: Suppose byteSize = 300.
		// (300 + 255) & ~255
		// 555 & ~255
		// 0x022B & ~0x00ff
		// 0x022B & 0xff00
		// 0x0200
		// 512
		return (byteSize + 255) & ~255;
	}

	enum RenderType
	{
		Opaque = 1,
		Transparent = 2,
		Stencil = 3,
		BillBoard = 4
	};

	enum TextureType
	{
		T2D = 1,
		T2DArray = 2
	};
}
/*

namespace Engine
{
	XMGLOBALCONST XMVECTORF32 Zero = { 0.0f, 0.0f, 0.0f, 0.0f };

	extern const D3D11_INPUT_ELEMENT_DESC VertexDesc[];
	extern const UINT VertexCount;
}
*/
#endif // __ENGINE_UTIL_DIRECTX_H__
