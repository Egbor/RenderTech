#include "Shader/EngineCommon.hlsli"

cbuffer VS_ENGINE_CAMERA_BUFFER {
    float4x4 ViewProjection;
    float4x4 World;
};

float4 main(float3 position : POSITION, float3 tangent : TANGENT, 
            float3 binormal : BINORMAL, float3 normal : NORMAL, 
            float2 uv : TEXCOORD0) : SV_POSITION {
    float4x4 wvp = mul(tarnspose(World), ViewProjection);
    float4 outPosition = mul(float4(position.xyz, 1.0f), wvp);

    return outPosition;
}