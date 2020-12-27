Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

cbuffer cbuff0 : register(b0)
{
	matrix world; // ワールド行列
	matrix viewproj; // ビュープロジェクション行列
	float3 cameraPos; // カメラ座標(ワールド座標)
};
cbuffer cbuff1 : register(b1)
{
	float3 m_ambient:packoffset(c0); // アンビエント係数
	float3 m_diffuse:packoffset(c1); // ディフューズ係数
	float3 m_specular:packoffset(c2); // スペキュラー係数
	float m_alpha : packoffset(c2.w); // アルファ
};

cbuffer cbuff2 : register(b2)
{
	float3 lightv; // ライトの単位ベクトル
	float3 lightcolor; // ライトの色(RGB)
}

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float3 normal :NORMAL; // 法線ベクトル
	float2 uv  :TEXCOORD; // uv値

	//float4 color : COLOR; // 色
	float4 worldpos : POSITION;// ワールド座標
};
