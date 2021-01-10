#include"Collider.hlsli"
float4 main(Output input):SV_TARGET
{
	return float4(color.x,color.y,color.z,color.w);
}