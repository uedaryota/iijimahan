Texture2D<float4>tex:register(t0);
SamplerState smp:register(s0);

cbuffer cbuff0:register(b0)
{
	matrix world;
	matrix viewproj;
	float4 color;
}

struct Output
{
	float4 svpos:SV_POSITION;
};