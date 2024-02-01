#include "EngineCommon.hlsli"

Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D MetalicTexture : register(t2);
Texture2D RoughnessTexture : register(t3);
Texture2D AmbiantTexture : register(t4);

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;

	float3x3 TBN = float3x3(
		normalize(input.wsTangent),
		normalize(input.wsBinormal),
		normalize(input.wsNormal)
	);

	output.AlbedoRTV = AlbedoTexture.Sample(LinearSampler, input.TexCoord);
	output.NormalRTV = MapNormal(TBN, NormalTexture, LinearSampler, input.TexCoord);
	output.OrmRTV = float4(
		AmbiantTexture.Sample(LinearSampler, input.TexCoord).r,
		RoughnessTexture.Sample(LinearSampler, input.TexCoord).r,
		MetalicTexture.Sample(LinearSampler, input.TexCoord).r,
		1.0f
	);

	return output;
}