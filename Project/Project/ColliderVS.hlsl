#include"Collider.hlsli"

Output main(float4 pos:POSITION)
{
	Output output;
	output.svpos = mul(mul(viewproj, world), pos);
	return output;
};

