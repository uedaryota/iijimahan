#include"TexShaderHeader.hlsli"
Output main(float4 pos:POSITION,float2 uv:TEXCOORD) 
{
	Output output;
	output.svpos = mul(world, pos);
	output.uv = uv;
	return output;
};

