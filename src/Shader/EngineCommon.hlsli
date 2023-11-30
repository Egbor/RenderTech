#ifndef ENGINE_COMMON_HLSLI
#define ENGINE_COMMON_HLSLI

#if defined(ENGINE_MESH_RENDERPASS)

cbuffer VS_ENGINE_CAMERA_BUFFER : register(b0) {
	float4x4 ViewProjection;
	float4x4 invWorld;
	float4x4 World;
}

Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D MetalicTexture : register(t2);
Texture2D RoughnessTexture : register(t3);
Texture2D AmbiantTexture : register(t4);

#endif

#if defined(ENGINE_LIGHT_RENDERPASS)

cbuffer PS_ENGINE_ACCUMULATION_BUFFER : register(b0) {
	float4x4 WorldView;
	float4x4 Projection;
	float4x4 invProjection;
	float2 Resolution;
}

cbuffer PS_ENGINE_LIGHT_BUFFER : register(b1) {
	float4 LightValue;
	float4 LightColor;
}

Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D DepthTexture : register(t2);
Texture2D OrmTexture : register(t3);

#endif

sampler LinearSampler : register(s0);

struct VertexShaderOutput {
	float2 TexCoord : TEXCOORD1;
	float3 wsTangent : TANGENT;
	float3 wsBinormal : BINORMAL;
	float3 wsNormal : NORMAL;
	float4 Position : SV_POSITION;
};

struct PixelShaderOutput {
	float4 AlbedoRTV : SV_TARGET0;
	float4 NormalRTV : SV_TARGET1;
	float4 OrmRTV : SV_TARGET2;
};

float2 GetCorrectedTextureCoordinate(float2 coordinate) {
	return float2(coordinate.x, 1.0f - coordinate.y);
}

float3 GetCorrectedNormalCoordinate(float3 coordinate) {
	return coordinate * 2.0f - 1.0f;
}

float4 FindViewFromDepth(float depth, float2 uv, float4x4 invProjection) {
	float z = depth * 2.0f - 1.0f;

	float4 clipSpacePosition = float4(uv * 2.0f - 1.0f, z, 1.0f);
	float4 viewSpacePosition = mul(clipSpacePosition, invProjection);

	viewSpacePosition /= viewSpacePosition.w;

	return viewSpacePosition;
}

/*float4 FindWorldFromDepth(float depth, float2 uv, float4x4 invView, float4x4 invProjection) {
	float4 viewSpacePosition = FindViewFromDepth(depth, uv, invProjection);
	float4 worldSpacePosition = mul(viewSpacePosition, invView);

	return worldSpacePosition;
}*/

float4 MapNormal(float3x3 TBN, Texture2D tex, sampler sam, float2 uv) {
	float3 normal = GetCorrectedNormalCoordinate(tex.Sample(sam, uv).xyz);

	normal = mul(normal, TBN);
	return normalize(float4(normal, 0.0f));
}

#include "ACES.hlsli"

float3 LinearTosRGB(float3 color) {
	float3 x = color * 12.92f;
	float3 y = 1.055f * pow(saturate(color), 1.0f / 2.4f) - 0.055f;

	float3 clr = color;
	clr.r = color.r < 0.0031308f ? x.r : y.r;
	clr.g = color.g < 0.0031308f ? x.g : y.g;
	clr.b = color.b < 0.0031308f ? x.b : y.b;

	return clr;
}

float3 sRGBToLinear(float3 color) {
	float3 x = color / 12.92f;
	float3 y = pow(saturate((color + 0.055f) / 1.055f), 2.4f);

	float3 clr = color;
	clr.r = color.r <= 0.04045f ? x.r : y.r;
	clr.g = color.g <= 0.04045f ? x.g : y.g;
	clr.b = color.b <= 0.04045f ? x.b : y.b;

	return clr;
}

#endif