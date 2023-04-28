#ifndef ENGINE_COMMON_PBR_HLSI
#define ENGINE_COMMON_PBR_HLSI

#define PI 3.14159265359f

/*Trowbridge-Reitz GGX*/
float D(float alpha, float cosH) {
	float numerator = alpha * alpha;
	float denominator = cosH * cosH * (numerator - 1.0f) + 1.0f;

	denominator = (PI * denominator * denominator);
	denominator = max(denominator, 0.000001f);

	return numerator / denominator;
}

/*Smith-Schlick model*/
float G1(float alpha, float cosX) {
	float k = alpha / 2.0f;
	float denominator = cosX * (1.0f - k) + k;

	denominator = max(denominator, 0.000001f);

	return cosX / denominator;
}

float G(float alpha, float cosL, float cosV) {
	return G1(alpha, cosV) * G1(alpha, cosL);
}

/*Schlick's implementation*/
float3 F(float3 F0, float cosVH) {
	return F0 + (float3(1.0f, 1.0f, 1.0f) - F0) * pow(1.0f - cosVH, 5.0);
}

/*Render equation function*/
float3 FindOutgoingPBRLight(float3 normal, float3 view, float3 light, float3 halfv, float3 lightColor, float3 albedoColor, float3 ormColor) {
	float cosH = saturate(dot(halfv, normal));
	float cosL = saturate(dot(light, normal));
	float cosV = saturate(dot(view, normal));
	float cosVH = saturate(dot(halfv, view));
	float cosLV = saturate(dot(light, view));

	float alpha = ormColor.g * ormColor.g;

	float3 F0 = lerp(0.04f, albedoColor, ormColor.b);//float3(IOR, IOR, IOR);
	float3 Ks = F(F0, cosVH);
	float3 Kd = (float3(1.0f, 1.0f, 1.0f) - Ks) * (1.0f - ormColor.b);

	float3 lambert = albedoColor / PI;

	float3 cookTorranceNumerator = D(alpha, cosH) * G(alpha, cosL, cosV) * F(F0, cosVH);
	float cookTorranceDenominator = 4.0f * cosV * cosL;

	cookTorranceDenominator = max(cookTorranceDenominator, 0.000001f);

	float3 cookTorrance = cookTorranceNumerator / cookTorranceDenominator;
	float3 BRDF = Kd * lambert + cookTorrance;
	float3 outgoingLight = BRDF * lightColor * cosL;

	float3 ambiant = float3(0.03f, 0.03f, 0.03f) * albedoColor * ormColor.r;

	return outgoingLight + ambiant;
}

#endif