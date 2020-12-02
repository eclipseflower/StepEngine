float3 BlinnPhong(float3 lightStrength, float3 lightVec, float3 normal, float3 toEye, Material mat)
{

}

float3 ComputeDirectionalLight(Light light, Material mat, float3 normal, float3 toEye)
{
	float3 dir = -light.direction;
	float nDotL = max(0, dot(dir, normal));
	float3 lightStrength = light.color * nDotL;

	return BlinnPhong(lightStrength, dir, normal, toEye, mat);
}

float3 ComputePointLight(Light light, Material mat, float3 pos, float3 normal, float3 toEye)
{
	float3 dir = light.position - pos;
	float dist = length(dir);
	if (dist >= light.falloffEnd)
	{
		return 0.0f;
	}

	dir = dir / dist;

	float nDotL = max(0, dot(dir, normal));
	float falloff = saturate((light.falloffEnd - dist) / (light.falloffEnd - light.falloffStart));
	float3 lightStrength = light.color * nDotL * falloff;

	return BlinnPhong(lightStrength, dir, normal, toEye, mat);
}

float3 ComputeSpotLight(Light light, Material mat, float3 pos, float3 normal, float3 toEye)
{
	float3 dir = light.position - pos;
	float dist = length(dir);
	if (dist >= light.falloffEnd)
	{
		return 0.0f;
	}

	dir = dir / dist;
	float nDotL = max(0, dot(dir, normal));
	float falloff = saturate((light.falloffEnd - dist) / (light.falloffEnd - light.falloffStart));
	float spotFactor = pow(max(0, dot(-dir, light.direction)), light.spotPower);
	float3 lightStrength = light.color * nDotL * falloff * spotFactor;

	return BlinnPhong(lightStrength, dir, normal, toEye, mat);
}

float4 ComputeLighting(Light gLights[16], int gLightCount, Material mat, float3 pos, float3 normal, float3 toEye)
{
	float3 result = 0.0f;
	for (int i = 0; i < gLightCount; i++)
	{
		// directional
		if (gLights[i].type == 0)
		{
			result += ComputeDirectionalLight(gLights[i], mat, normal, toEye);
		}
		// point
		else if (gLights[i].type == 1)
		{
			result += ComputePointLight(gLights[i], mat, pos, normal, toEye);
		}
		// spot
		else if (gLights[i].type == 2)
		{
			result += ComputeSpotLight(gLights[i], mat, pos, normal, toEye);
		}
	}

	return float4(result, 0.0f);
}