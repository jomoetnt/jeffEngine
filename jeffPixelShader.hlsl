cbuffer jeffPBuf : register(b1)
{
	float4 pointLight[4];
	float4 pointLightColour[4];
	float4 pointLightParams[4];
	float4 dirLight;
	float4 dirLightColour;
}

struct Input 
{
	float4 position : SV_POSITION;
	float4 worldPosition : POSITIONT;
	float3 n : NORM;
	float2 texcrd : TEXCOORD;
};

Texture2D	jDiffuse : register(t0);
SamplerState jLinearSample : register(s0);

float4 main(Input input) : SV_TARGET
{
	float3 diffuseColour = jDiffuse.Sample(jLinearSample, input.texcrd);
	float3 ambientLight = float3(0.05f, 0.05f, 0.05f);

	float3 realWorldPos = float3(input.worldPosition.x, input.worldPosition.y, input.worldPosition.z);
	float3 realNormal = normalize(input.n);

	float3 realDirLight = -normalize(float3(dirLight.x, dirLight.y, dirLight.z));
	float3 realDirLightColour = dirLightColour.w * float3(dirLightColour.x, dirLightColour.y, dirLightColour.z);

	float3 brightness = dot(realNormal, realDirLight);
	float3 colour = brightness * diffuseColour * realDirLightColour;
	for (int i = 0; i < 4; i++)
	{
		float3 realPointLight = float3(pointLight[i].x, pointLight[i].y, pointLight[i].z);
		float3 realPointLightColour = pointLightColour[i].w * float3(pointLightColour[i].x, pointLightColour[i].y, pointLightColour[i].z);

		float3 ray = realWorldPos - realPointLight;
		float rayLength = length(ray);
		float3 realRay = -normalize(float3(ray.x, ray.y, ray.z));

		float quadraticAttenuation = pointLightParams[i].x;
		float linearAttenuation = pointLightParams[i].y;
		float constantAttenuation = pointLightParams[i].z;
		float attenuation = quadraticAttenuation * pow(rayLength, 2) + linearAttenuation * rayLength + constantAttenuation;

		brightness = saturate(dot(realNormal, realRay) / attenuation + ambientLight);
		colour += brightness * diffuseColour * realPointLightColour;
	}


	return float4(saturate(colour), 1.0f);
}