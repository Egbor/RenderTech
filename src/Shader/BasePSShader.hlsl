#include "Shader/EngineCommon.hlsli"

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