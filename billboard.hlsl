#include "Lighting.hlsl"

Texture2DArray gDiffuseMap : register(t0);
SamplerState gSampler : register(s0);

cbuffer cbPerObject : register(b0)
{
	float4x4 gWorld;
	float4x4 gInvWorld;
	float4x4 gWorldViewProj;
};

cbuffer cbPerPass : register(b1)
{
	float3 gEyePosW;
	int gLightCount;
	float4 gAmbientLight;
	Light gLights[16];
};

cbuffer cbPerMaterial : register(b2)
{
	float4 diffuseAlbedo;
	float3 fresnelR0;
	float shininess;
};

struct VertexIn
{
	float3 PosL : POSITION;
	float2 Size : SIZE;
};

struct VertexOut
{
	float3 Center	: POSITION;
	float2 Size		: SIZE;
};

struct GeoOut
{
	float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
	float3 NormalW : NORMAL;
	float2 TexC    : TEXCOORD;
    uint   PrimID  : SV_PrimitiveID;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.Center = vin.PosL;
	vout.Size = vin.Size;

	return vout;
}

[maxvertexcount(4)]
void GS(point VertexOut gin[1],
	uint primID : SV_PrimitiveID,
	inout TriangleStream<GeoOut> triStream)
{
	float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 forward = normalize(gin[0].Center - gEyePosW);
	forward.y = 0.0f;
	float3 right = normalize(cross(up, forward));

	//
	// Compute triangle strip vertices (quad) in world space.
	//
	float halfWidth  = 0.5f*gin[0].Size.x;
	float halfHeight = 0.5f*gin[0].Size.y;

	float4 v[4];
	v[0] = float4(gin[0].Center + halfWidth*right - halfHeight*up, 1.0f);
	v[1] = float4(gin[0].Center + halfWidth*right + halfHeight*up, 1.0f);
	v[2] = float4(gin[0].Center - halfWidth*right - halfHeight*up, 1.0f);
	v[3] = float4(gin[0].Center - halfWidth*right + halfHeight*up, 1.0f);

	//
	// Transform quad vertices to world space and output
	// them as a triangle strip.
	//

	float2 texC[4] =
	{
		float2(0.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 1.0f),
		float2(1.0f, 0.0f)
	};

	GeoOut gout;
	[unroll]
	for(int i = 0; i < 4; ++i)
	{
		gout.PosH     = mul(v[i], gWorldViewProj);
		gout.PosW     = v[i].xyz;
		gout.NormalW  = forward;
		gout.TexC     = texC[i];
		gout.PrimID   = primID;

		triStream.Append(gout);
	}
}

float4 PS(GeoOut pin) : SV_Target
{
	float3 uvw = float3(pin.TexC, pin.PrimID%3);
	float4 diffuse = gDiffuseMap.Sample(gSampler, uvw)* diffuseAlbedo;
	float4 ambient = gAmbientLight * diffuse;

	pin.NormalW = normalize(pin.NormalW);
	Material mat;
	mat.diffuseAlbedo = diffuse;
	mat.fresnelR0 = fresnelR0;
	mat.shininess = shininess;
	float3 toEyeW = normalize(gEyePosW - pin.PosW);

	float4 directLight = ComputeLighting(gLights, gLightCount, mat, pin.PosW, pin.NormalW, toEyeW);
	float4 litColor = ambient + directLight;
	return float4(litColor.rgb, diffuse.a);
}