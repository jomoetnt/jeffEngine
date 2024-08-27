cbuffer jeffVBuf : register(b0)
{
	matrix mProj;
	matrix mTransform;
}

struct Input 
{
	float3 position : POSITION;
	float3 color : COLOR;
	float2 texcord : TEXCOORD;
	float3 norm : NORMAL;
};

struct Output 
{
	float4 position : SV_POSITION;
	float3 n : NORM;
	float3 color : COLOR;
};

Output main(Input input) 
{
	Output output;

	output.position = float4(input.position, 1);
	output.color = input.color;

	output.position = mul(output.position, mTransform);
	output.position = mul(output.position, mProj);

	float4 transformedNormal = mul(float4(input.norm, 0.0f), mTransform);

	output.n = float3(transformedNormal.x, transformedNormal.y, transformedNormal.z);

	return output;
}