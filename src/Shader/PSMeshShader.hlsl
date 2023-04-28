#include "Shader/EngineCommon.hlsli"

PixelShaderOutput main(VertexShaderOutput IN) {
	PixelShaderOutput OUT;

	float3x3 TBN = float3x3(
		normalize(IN.vsTangent),
		normalize(IN.vsBinormal),
		normalize(IN.vsNormal)
	);

	OUT.AlbedoRTV = AlbedoTexture.Sample(LinearSampler, IN.TexCoord);
	OUT.NormalRTV = MapNormal(TBN, NormalTexture, LinearSampler, IN.TexCoord);
	OUT.OrmRTV = float4(
		AmbiantTexture.Sample(LinearSampler, IN.TexCoord).r,
		RoughnessTexture.Sample(LinearSampler, IN.TexCoord).r,
		MetalicTexture.Sample(LinearSampler, IN.TexCoord).r,
		1.0f
	);

	return OUT;
}