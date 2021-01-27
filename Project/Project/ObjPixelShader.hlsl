#include "ObjShaderHeader.hlsli"

float4 main(VSOutput input) : SV_TARGET
{
	//float3 light = normalize(float3(1,-1,1)); // 右下奥　向きのライト
	//float diffuse = saturate(dot(-light, input.normal));

	//テクスチャマッピング
	float4 texcolor = tex.Sample(smp, input.uv);

	//シェーディングによる色
	float4 shadecolor;

	//光沢度
	const float shininess = 4.0f;

	//頂点から視点への方向ベクトル
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	//ライトに向かうベクトルと法線の内積
	float3 dotlightnormal = dot(lightv, input.normal);

	//反射光ベクトル
	float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);

	//環境反射光
	float3 ambient = m_ambient;

	//拡散反射光
	float3 diffuse = dotlightnormal * m_diffuse;
	//応急処置
	//diffuse = clamp(diffuse, 0.5, 1);
	//鏡面反射光
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	shadecolor.rgb = (ambient + diffuse + specular) * lightcolor;
	shadecolor.a = m_alpha;

	return shadecolor * texcolor;
	//return float4(texcolor.rgb * shade_color, texcolor.a*m_alpha);
	//return float4(1, 1, 1, 1);
}