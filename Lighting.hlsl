float4 ComputeLighting(Light gLights[16], int gLightCount, Material mat, float3 pos, float3 normal, float3 toEye)
{
	float3 result = 0.0f;
	for (int i = 0; i < gLightCount; i++)
	{
		// directional
		if (gLights[i].type == 0)
		{
			result += ComputeDirectionalLight();
		}
		// point
		else if (gLights[i].type == 1)
		{
			result += ComputePointLight();
		}
		// spot
		else if (gLights[i].type == 2)
		{
			result += ComputeSpotLight();
		}
	}
}