#include "EngineCommon.hlsli"

CBUFFER_OBJECT(b0)

struct VertexOutput {
    float4 localPosition : POSITION;
    float4 position : SV_POSITION;
};

VertexOutput main(float3 position : POSITION, float3 tangent : TANGENT, 
                    float3 binormal : BINORMAL, float3 normal : NORMAL, float2 uv : TEXCOORD0) {
    VertexOutput OUT;
    
    OUT.localPosition = float4(-position.x, position.yz, 1.0f);
    OUT.position = mul(float4(position.xyz, 1.0f), ViewProjection);
    return OUT;
}