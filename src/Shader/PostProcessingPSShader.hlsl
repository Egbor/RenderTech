#include "EngineCommon.hlsli"

Texture2D HdrFrame : register(t1);

float4 main(float4 position : SV_POSITION) : SV_TARGET0 {
    float3 nonHdrPixel = HdrFrame.Load(int3(position.xy, 0)).rgb;
    
    nonHdrPixel = ACESFitted(nonHdrPixel);
	nonHdrPixel = LinearTosRGB(nonHdrPixel);
    return float4(nonHdrPixel, 0.0f);
}