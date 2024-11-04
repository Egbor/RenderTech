#include "EngineCommon.hlsli"
#include "EngineCommonPBR.hlsli"

CBUFFER_CAMERA(b2)
CBUFFER_OBJECT_HELPER(b1)

Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D OrmTexture : register(t2);
Texture2D DepthTexture : register(t3);
TextureCube CubeTexture : register(t4);

float4 main(float4 position : SV_POSITION) : SV_TARGET0 {
    float2 texCoord = position.xy;
    float2 texUV = GetCorrectedTextureCoordinate(texCoord / Resolution);
    
    float4 albedo = AlbedoTexture.Load(int3(texCoord, 0));
    float4 normal = NormalTexture.Load(int3(texCoord, 0));
    float4 depth = DepthTexture.Load(int3(texCoord, 0));
    float4 orm = OrmTexture.Load(int3(texCoord, 0));
    
    float3 L = normalize(normal.xyz);
    float3 V = normalize((EyePosition - FindWorldFromDepth(depth.r, texUV, invView, invProjection)).xyz);
    float3 H = normalize(L + V);
    
    float3 color = float3(0.0f, 0.0f, 0.0f);
    
    if (length(normal) > 0.0f) {
        float cosVH = saturate(dot(H, V));
        
        float3 F0 = lerp(0.04f, albedo.rgb, orm.b);
        float3 Ks = F(F0, cosVH);
        float3 Kd = (float3(1.0f, 1.0f, 1.0f) - Ks) * (1.0f - orm.b);
        
        color = Kd * sRGBToLinear(albedo.rgb) * CubeTexture.Sample(LinearSampler, normalize(normal.xyz)).rgb;
    }

    return float4(color.rgb, 0.0f);
}