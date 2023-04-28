#include "Shader/EngineCommon.hlsli"

float4 main(PS_INPUT input) : SV_TARGET0 {
    return input.color;
}