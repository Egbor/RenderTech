cbuffer VS_ENGINE_CAMERA_BUFFER : register(b0) {
	float4x4 WorldViewProjection;
}

struct VertexOutput {
    float4 position : SV_POSITION;
    float3 uvw : TEXCOORD;
};

VertexOutput main(float3 position : POSITION, float3 tangent : TANGENT, 
            float3 binormal : BINORMAL, float3 normal : NORMAL, 
            float2 uv : TEXCOORD0) {
    VertexOutput OUT;

    OUT.position = mul(float4(position.xyz, 1.0f), WorldViewProjection);
    OUT.uvw = float3(position.x, -position.y, position.z);

    return OUT;
}