#include "EngineCommon.hlsli"

CBUFFER_OBJECT(b0)

SkyboxVSOutput main(float3 position : POSITION, float3 tangent : TANGENT, 
                  float3 binormal : BINORMAL, float3 normal : NORMAL, float2 uv : TEXCOORD0) {
    SkyboxVSOutput OUT;

    OUT.wsPosition = float3(position.x, -position.y, position.z);
    OUT.Position = mul(float4(position.xyz, 0.0f), ViewProjection);
    OUT.Position.z = OUT.Position.w;
    
    return OUT;
}