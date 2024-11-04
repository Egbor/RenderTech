#include "Shader/EngineCommon.hlsli"

PixelShaderOutput main(VertexShaderOutput IN) {
	PixelShaderOutput OUT;

	//float3x3 TBN = float3x3(
	//	normalize(IN.wsTangent),
	//	normalize(IN.wsBinormal),
	//	normalize(IN.wsNormal)
	//	);

	//OUT.AlbedoRTV = AlbedoTexture.Sample(LinearSampler, IN.TexCoord);
	//OUT.NormalRTV = MapNormal(TBN, NormalTexture, LinearSampler, IN.TexCoord);
	//OUT.OrmRTV = float4(
	//	AmbiantTexture.Sample(LinearSampler, IN.TexCoord).r,
	//	RoughnessTexture.Sample(LinearSampler, IN.TexCoord).r,
	//	MetalicTexture.Sample(LinearSampler, IN.TexCoord).r,
	//	1.0f
	//);

	OUT.AlbedoRTV = float4(1.0f, 0.0f, 0.0f, 1.0f);
	OUT.NormalRTV = float4(1.0f, 0.0f, 0.0f, 1.0f);
	OUT.OrmRTV = float4(1.0f, 0.0f, 0.0f, 1.0f);

	return OUT;
}