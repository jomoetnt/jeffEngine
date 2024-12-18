
cbuffer jeffPBuf : register(b1)
{
	float4 pointLight[4];
	float4 pointLightColour[4];
	float4 pointLightParams[4];
	float4 dirLight;
	float4 dirLightColour;
	float4 wireframe;
	float k_specular;
	float k_diffuse;
	float k_ambient;
	float shininess;
}

struct Input 
{
	float4 position : SV_POSITION;
	float4 worldPosition : POSITIONT;
	float4 viewPosition : VIEWPOS;
	float3 n : NORM;
	float2 texcrd : TEXCOORD;
};

Texture2D	jDiffuse : register(t0);
SamplerState jLinearSample : register(s0);

float4 main(Input input) : SV_TARGET
{
	if (wireframe.w != 0)
	{
		return float4(1.0f, 1.0f, 0.0f, 1.0f);
	}
	else
	{
		float4 diffuseColour = jDiffuse.Sample(jLinearSample, input.texcrd);
		float4 ambientLight = float4(0.05f, 0.05f, 0.05f, 1.0f);

		float4 realNormal = float4(normalize(input.n), 0.0f);

		float4 realDirLight = -normalize(dirLight);

		float4 brightness = dot(realNormal, realDirLight);
		float4 colour = brightness * diffuseColour * dirLightColour * dirLightColour.w;
		for (int i = 0; i < 4; i++)
		{
			float4 ray = input.worldPosition - pointLight[i];
			float rayLength = length(ray);
			float4 realRay = normalize(ray);

			float quadraticAttenuation = pointLightParams[i].x;
			float linearAttenuation = pointLightParams[i].y;
			float constantAttenuation = pointLightParams[i].z;
			float attenuation = quadraticAttenuation * pow(rayLength, 2) + linearAttenuation * rayLength + constantAttenuation;

			float4 intensity = pointLightColour[i] * pointLightColour[i].w;
			intensity.w = 1.0f;

			float4 diffuseTerm = k_diffuse * max(float4(0.0f, 0.0f, 0.0f, 0.0f), dot(realNormal, realRay)) * intensity;

			float4 viewerDirection = -normalize(input.viewPosition);
			float4 lightReflection = reflect(realRay, realNormal);
			float4 specularTerm = max(float4(0.0f, 0.0f, 0.0f, 0.0f), k_specular * pow(dot(lightReflection, viewerDirection), shininess) * intensity);

			brightness = (diffuseTerm + specularTerm) / attenuation;
			colour += brightness * diffuseColour;
		}
		colour += k_ambient * ambientLight * diffuseColour;

		return saturate(colour);
	}
}