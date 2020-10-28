Texture2D<float4>tex:register(t0);
SamplerState smp:register(s0);

cbuffer cbuff0:register(b0)
{
	matrix world;
	matrix viewproj;
}
cbuffer Material : register(b1)
{
	float4 diffuse;
	float4 speclar;
	float4 ambient;

}

struct Output
{
	float4 svpos:SV_POSITION;
	float4 normal:NORMAL;
	float2 uv:TEXCOORD;
};