cbuffer jeffVBuf : register(b0)
{
	matrix mProj;
	matrix mTransform;
	matrix mCam;
}

struct Input 
{
	float3 position : POSITION;
	float3 norm : NORMAL;
	float2 texcord : TEXCOORD;
};

struct Output 
{
	float4 position : SV_POSITION;
	float4 worldPosition : POSITIONT;
	float4 viewPosition : VIEWPOS;
	float3 n : NORM;
	float2 texcrd : TEXCOORD;
};

Output main(Input input) 
{
	Output output;

	output.position = float4(input.position, 1);

	output.worldPosition = mul(output.position, mTransform);
	output.position = mul(output.worldPosition, mCam);
	output.viewPosition = output.position;
	output.position = mul(output.position, mProj);

	float4 transformedNormal = mul(float4(input.norm, 0.0f), mTransform);

	output.n = float3(transformedNormal.x, transformedNormal.y, transformedNormal.z);
	output.texcrd = input.texcord;

	return output;
}