
#include"PMDShaderHeader.hlsli"
float4 main(Output input) :SV_TARGET
{
	float3 light = normalize(float3(1,-1,1));
	float brightness = dot(-light, input.normal);
	return float4(brightness, brightness, brightness, 1)*diffuse*tex.Sample(smp, input.uv);
}