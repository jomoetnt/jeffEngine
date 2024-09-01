cbuffer jeffPBuf : register(b1)
{
	float4 dirLight;
	float4 dirLightColour;
	float4 pointLight;
	float4 pointLightColour;
	float4 pointLightParams;
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
	float3 ambientLight = float3(0.05f, 0.05f, 0.05f);

	float3 realWorldPos = float3(input.worldPosition.x, input.worldPosition.y, input.worldPosition.z);
	float3 realNormal = normalize(input.n);

	float3 realDirLight = -normalize(float3(dirLight.x, dirLight.y, dirLight.z));
	float3 realDirLightColour = dirLightColour.w * float3(dirLightColour.x, dirLightColour.y, dirLightColour.z);

	float3 realPointLight = float3(pointLight.x, pointLight.y, pointLight.z);
	float3 realPointLightColour = pointLightColour.w * float3(pointLightColour.x, pointLightColour.y, pointLightColour.z);

	//float3 brightness = dot(realNormal, realDirLight);
	float3 ray = realWorldPos - realPointLight;
	float rayLength = length(ray);
	float3 realRay = -normalize(float3(ray.x, ray.y, ray.z));

	float quadraticAttenuation = pointLightParams.x;
	float linearAttenuation = pointLightParams.y;
	float constantAttenuation = pointLightParams.z;
	float attenuation = quadraticAttenuation * pow(rayLength, 2) + linearAttenuation * rayLength + constantAttenuation;

	float3 brightness = dot(realNormal, realRay) / attenuation + ambientLight;

	float3 diffuseColour = jDiffuse.Sample(jLinearSample, input.texcrd);
	//float3 diffuseColour = float3(1.0f, 1.0f, 1.0f);
	float3 colour = brightness * diffuseColour * realPointLightColour;

	return float4(colour, 1.0f);
}