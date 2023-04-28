#include "Shader/EngineCommon.hlsli"

float4 main(float3 position : POSITION, float3 tangent : TANGENT, 
            float3 binormal : BINORMAL, float3 normal : NORMAL, 
            float2 uv : TEXCOORD0) : SV_POSITION {
    float4 outPosition = mul(float4(position.xyz, 1.0f), WorldViewProjection);
    return outPosition;
}