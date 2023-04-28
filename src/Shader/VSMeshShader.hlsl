#include "Shader/EngineCommon.hlsli"

VertexShaderOutput main(float3 position : POSITION, float3 tangent : TANGENT, 
                        float3 binormal : BINORMAL, float3 normal : NORMAL, float2 uv : TEXCOORD0) {
    VertexShaderOutput output;

    output.Position = mul(float4(position.xyz, 1.0f), WorldViewProjection);

    output.TexCoord = GetCorrectedTextureCoordinate(uv);
    output.vsTangent = mul(tangent, (float3x3)WorldView);
    output.vsBinormal = mul(binormal, (float3x3)WorldView);
    output.vsNormal = mul(normal, (float3x3)WorldView);

    return output;
}