#include "Shader/EngineCommon.hlsli"

PixelShaderOutput main(VertexShaderOutput IN) {
    PixelShaderOutput OUT;

    OUT.AlbedoRTV = float4(IN.wsNormal, 1.0f);
    OUT.NormalRTV = float4(0.0f, 0.0f, 0.0f, 0.0f);
    OUT.OrmRTV = float4(0.0f, 0.0f, 0.0f, 0.0f);

    return OUT;
}