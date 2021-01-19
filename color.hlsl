//***************************************************************************************
// color.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

#include "Lighting.hlsl"

Texture2D gDiffuseMap : register(t0);
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
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
    float4 Color   : COLOR;
	float2 UV	   : TEXCOORD0;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
	float3 NormalW : NORMAL;
    float4 Color   : COLOR;
	float2 UV	   : TEXCOORD0;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	float4 posW = mul(float4(vin.PosL, 1.0f), gWorld);
	vout.PosW = posW.xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)transpose(gInvWorld));
	// Just pass vertex color into the pixel shader.
    vout.Color = vin.Color;
	vout.UV = vin.UV;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	float4 diffuse = gDiffuseMap.Sample(gSampler, pin.UV)* diffuseAlbedo;
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


