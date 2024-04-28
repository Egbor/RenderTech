#include "EngineCommon.hlsli"

TextureCube CubeTexture : register(t0);

float4 main(float3 position : POSITION) : SV_TARGET0 {
    float3 color = CubeTexture.Sample(LinearSampler, position).rgb;
    color = TonMappingForHDR(color);
    color = LinearTosRGB(color);
    return float4(color, 1.0f);
}