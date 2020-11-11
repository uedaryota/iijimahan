#include"FloorShaderHeader.hlsli"
Output main(float4 pos:POSITION,float2 uv:TEXCOORD) 
{
	Output output;
	output.svpos = mul(mul(viewproj, world), pos);
output.uv = uv;
return output;
};

