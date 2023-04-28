sampler LinearSampler : register(s0);
Texture2D AlbedoTexture : register(t0);

static const float2 invAtan = float2(0.1591f, 0.3183f);

float2 SampleSphericalMap(float3 v) {
    float2 uv = float2(atan2(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5f;
    return float2(uv.x, 1.0f - uv.y);
}

float4 main(float4 localPosition : POSITION, 
            float4 position : SV_POSITION) : SV_TARGET0 {
    float2 uv = SampleSphericalMap(normalize(localPosition.xyz));
    float3 color = AlbedoTexture.Sample(LinearSampler, uv).rgb;
    return float4(color, 1.0f);
}