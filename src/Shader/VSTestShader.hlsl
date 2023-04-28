#include "Shader/EngineCommon.hlsli"

PS_INPUT main(VS_INPUT input) {
    PS_INPUT output;

    output.position = mul(float4(input.position, 1.0f), WorldViewProjection);
    output.color = float4(input.normal, 1.0f);

    return output;
}