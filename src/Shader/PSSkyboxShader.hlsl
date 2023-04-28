#include "Shader/EngineCommon.hlsli"

TextureCube CubeTexture : register(t0);

float4 main(float4 position : SV_POSITION, float3 uvw : TEXCOORD) : SV_TARGET0 {
    float3 color = CubeTexture.Sample(LinearSampler, uvw).rgb;
    color = TonMappingForHDR(color);
    color = LinearTosRGB(color);
    return float4(color, 1.0f);
}