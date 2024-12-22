#define PI 3.14159265359f

SamplerState LinearSampler : register(s0);
TextureCube EnvironmentTexture : register(t0);

float4 main(float4 localPosition : POSITION, float4 position : SV_POSITION) : SV_TARGET0 {
    float3 normal = normalize(localPosition.xyz);
    
    float3 up = float3(0.0f, 1.0f, 0.0f);
    float3 right = normalize(cross(up, normal));
    up = normalize(cross(normal, right));

    float3 sampledColour = float3(0, 0, 0);
    float index = 0;
    for (float phi = 0.0f; phi < (2.0f * PI); phi += 0.025f) {
        for (float theta = 0.0f; theta < (0.5f * PI); theta += 0.1f) {
            float3 temp = cos(phi) * right + sin(phi) * up;
            float3 sampleVector = cos(theta) * temp + sin(theta) * normal;
            
            sampledColour += EnvironmentTexture.Sample(LinearSampler, sampleVector).rgb * cos(theta) * sin(theta);
            index++;
        }
    }

    return float4((PI * sampledColour / index).rgb, 1.0f);
}