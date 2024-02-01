#include "EngineCommon.hlsli"

VertexShaderOutput main(float3 position : POSITION, float3 tangent : TANGENT,
                        float3 binormal : BINORMAL, float3 normal : NORMAL, float2 uv : TEXCOORD0) {
    VertexShaderOutput output;

    float4x4 wvp = mul(tarnspose(World), ViewProjection);
    output.Position = mul(float4(position.xyz, 1.0f), wvp);
    output.TexCoord = GetCorrectedTextureCoordinate(uv);

    output.wsTangent = mul(tangent, (float3x3)World);
    output.wsBinormal = mul(binormal, (float3x3)World);
    output.wsNormal = mul(normal, transpose((float3x3)invWorld));

    return output;
}