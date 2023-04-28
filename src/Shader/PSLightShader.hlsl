#include "Shader/EngineCommon.hlsli"
#include "Shader/EngineCommonPBR.hlsli"

float4 main(float4 position : SV_POSITION) : SV_TARGET0 {
	float2 texCoord = position.xy;
	float2 texUV = GetCorrectedTextureCoordinate(texCoord / Resolution);
	float4 eyePosition = float4(0.0f, 0.0f, 0.0f, 1.0f);

	float4 depth = DepthTexture.Load(int3(texCoord, 0));
	float4 albedo = AlbedoTexture.Load(int3(texCoord, 0));
	float4 normal = NormalTexture.Load(int3(texCoord, 0));
	float4 orm = float4(
		OrmTexture.Load(int3(texCoord, 0)).rgb,
		1.0f
	);

	float4 texViewPosition = FindViewFromDepth(depth.r, texUV, invProjection);

	float4 color = LightColor;
	float4 L = mul(LightValue, WorldView) - texViewPosition;
	float4 V = eyePosition - texViewPosition;

	L = normalize(L);
	V = normalize(V);

	float4 H = normalize(V + L);
	float4 N = normalize(normal);

	float3 outgoingLight = FindOutgoingPBRLight(N.xyz, V.xyz, L.xyz, H.xyz, color.rgb, sRGBToLinear(albedo.rgb), orm.rgb);
	outgoingLight = ACESFilm(TonMappingForHDR(outgoingLight));
	outgoingLight = LinearTosRGB(outgoingLight);

	return float4(outgoingLight, 1.0f);
}