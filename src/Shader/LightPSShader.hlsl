#include "EngineCommon.hlsli"
#include "EngineCommonPBR.hlsli"

CBUFFER_OBJECT(b0)
CBUFFER_OBJECT_HELPER(b1)
CBUFFER_CAMERA(b2)
CBUFFER_LIGHT(b3)

Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D OrmTexture : register(t2);
Texture2D DepthTexture : register(t3);

float4 main(float4 position : SV_POSITION) : SV_TARGET0 {
	float2 texCoord = position.xy;

	float4 depth = DepthTexture.Load(int3(texCoord, 0));
	float4 albedo = AlbedoTexture.Load(int3(texCoord, 0));
	float4 normal = NormalTexture.Load(int3(texCoord, 0));
    float4 orm = OrmTexture.Load(int3(texCoord, 0));

    float2 texUV = GetCorrectedTextureCoordinate(texCoord / Resolution);
    float4 texWorldPosition = FindWorldFromDepth(depth.r, texUV, invView, invProjection);
    float4 eyePosition = EyePosition;
	
	float4 L = mul(Value, World) - texWorldPosition;
	float4 V = eyePosition - texWorldPosition;
    float4 color = Color * Brightness;

	L = normalize(L);
	V = normalize(V);

	float4 H = normalize(V + L);
	float4 N = normalize(normal);

	float3 outgoingLight = FindOutgoingPBRLight(N.xyz, V.xyz, L.xyz, H.xyz, color.rgb, sRGBToLinear(albedo.rgb), orm.rgb);	
	outgoingLight = ACESFitted(outgoingLight);
	outgoingLight = LinearTosRGB(outgoingLight);

	return float4(outgoingLight, 1.0f);
}