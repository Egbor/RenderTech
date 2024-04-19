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
	float2 texUV = GetCorrectedTextureCoordinate(texCoord / Resolution);
	
	float4 depth = DepthTexture.Load(int3(texCoord, 0));
	float4 albedo = AlbedoTexture.Load(int3(texCoord, 0));
	float4 normal = NormalTexture.Load(int3(texCoord, 0));
    float4 orm = OrmTexture.Load(int3(texCoord, 0));

    float4 texWorldPosition = FindWorldFromDepth(depth.r, texUV, invView, invProjection);
    float4 eyePosition = EyePosition;
	
    float3 L = (mul(Value, World) - texWorldPosition).xyz;
    float3 V = (eyePosition - texWorldPosition).xyz;
    float3 color = (Color * Brightness).rgb / (length(L) * length(L));

	L = normalize(L);
	V = normalize(V);

	float3 H = normalize(V + L);
	float3 N = normalize(normal);

	float3 outgoingLight = FindOutgoingPBRLight(N, V, L, H, color, sRGBToLinear(albedo.rgb), orm.rgb);	
    outgoingLight = ACESFitted(outgoingLight);
	outgoingLight = LinearTosRGB(outgoingLight);

	return float4(outgoingLight, 1.0f);
}