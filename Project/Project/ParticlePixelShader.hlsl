
#include"ParticleShaderHeader.hlsli"
float4 main(Output input) :SV_TARGET
{ 
	float4 result = tex.Sample(smp,input.uv);
	result = float4(result.x*color.x, result.y*color.y, result.z*color.z, result.w*color.w);
	return result;
}