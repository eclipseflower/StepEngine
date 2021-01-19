struct Light
{
	int type;
	float3 color;
	float falloffStart;
	float3 direction;
	float falloffEnd;
	float3 position;
	float spotPower;
};

struct Material
{
	float4 diffuseAlbedo;
	float3 fresnelR0;
	float shininess;
};

float3 SchlickFresnel(float3 r0, float3 normal, float3 lightVec)
{
	float nDotL = max(0, dot(normal, lightVec));
	float3 reflectPercent = r0 + (1 - r0) * pow(1 - nDotL, 5);

	return reflectPercent;
}

float3 BlinnPhong(float3 lightStrength, float3 lightVec, float3 normal, float3 toEye, Material mat)
{
	float3 halfVec = normalize(lightVec + toEye);
	float roughness = (mat.shininess + 8) * pow(max(0, dot(halfVec, normal)), mat.shininess) / 8.0f;
	float3 fresnel = SchlickFresnel(mat.fresnelR0, halfVec, lightVec);
	float3 specularAlbedo = fresnel * roughness;
	return (mat.diffuseAlbedo.rgb + specularAlbedo) * lightStrength;
}

float3 ComputeDirectionalLight(Light light, Material mat, float3 normal, float3 toEye)
{
	float3 dir = -light.direction;
	float nDotL = max(0, dot(dir, normal));
	float3 lightStrength = light.color * nDotL;

	return BlinnPhong(lightStrength, dir, normal, toEye, mat);
}

// https://stackoverflow.com/questions/54781030/why-warning-x4000-use-of-potentially-uninitialized-variable-shows-for-more-th

float3 ComputePointLight(Light light, Material mat, float3 pos, float3 normal, float3 toEye)
{
	float3 result = 0.0f;
	float3 dir = light.position - pos;
	float dist = length(dir);
	if (dist >= light.falloffEnd)
	{
		result = 0.0f;
	}
	else
	{
		dir /= dist;

		float nDotL = max(0, dot(dir, normal));
		float falloff = saturate((light.falloffEnd - dist) / (light.falloffEnd - light.falloffStart));
		float3 lightStrength = light.color * nDotL * falloff;

		result = BlinnPhong(lightStrength, dir, normal, toEye, mat);
	}
	return result;
}

float3 ComputeSpotLight(Light light, Material mat, float3 pos, float3 normal, float3 toEye)
{
	float3 result = 0.0f;
	float3 dir = light.position - pos;
	float dist = length(dir);
	if (dist >= light.falloffEnd)
	{
		result = 0.0f;
	}
	else
	{
		dir /= dist;
		float nDotL = max(0, dot(dir, normal));
		float falloff = saturate((light.falloffEnd - dist) / (light.falloffEnd - light.falloffStart));
		float spotFactor = pow(max(0, dot(-dir, light.direction)), light.spotPower);
		float3 lightStrength = light.color * nDotL * falloff * spotFactor;

		result = BlinnPhong(lightStrength, dir, normal, toEye, mat);
	}
	return result;
}

float4 ComputeLighting(Light gLights[16], int gLightCount, Material mat, float3 pos, float3 normal, float3 toEye)
{
	float3 result = 0.0f;

	for (int i = 0; i < 3; i++)
	{
		//if (i < gLightCount)
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
	}

	return float4(result, 0.0f);
}