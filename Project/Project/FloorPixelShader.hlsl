
#include"FloorShaderHeader.hlsli"
float4 main(Output input) :SV_TARGET
{
	float4 obj = tex.Sample(smp,input.uv);
	return float4(obj.x, obj.y, obj.z, alpha);
}