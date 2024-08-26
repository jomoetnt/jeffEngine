cbuffer jeffPBuf : register(b1)
{
	float4 dirLight;
	float4 dirLightColour;
}

struct Input {
	float4 position : SV_POSITION;
	float3 n : NORM;
	float3 color : COLOR;
};

float4 main(Input input) : SV_TARGET
{
	float3 realDirLight = -normalize(float3(dirLight.x, dirLight.y, dirLight.z));
	float3 realDirLightColour = dirLightColour.w * float3(dirLightColour.x, dirLightColour.y, dirLightColour.z);

	float3 brightness = dot(normalize(input.n), realDirLight);
	float3 colour = brightness * input.color * realDirLightColour;
	return float4(colour, 1.0f);
}