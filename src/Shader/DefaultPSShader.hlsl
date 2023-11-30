#include "Shader/EngineCommon.hlsli"

PixelShaderOutput main(VertexShaderOutput IN) {
    PixelShaderOutput OUT;

    OUT.AlbedoRTV = float4(IN.wsNormal, 1.0f);

    return input.color;
}