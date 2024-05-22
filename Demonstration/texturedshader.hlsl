cbuffer ConstantBuffer
{
	matrix	worldViewProjection;
	matrix  worldTransformation;
	float4	ambientLightColour;
	float4  directionalLightColour;
	float4  directionalLightVector;
	float4	pointLightColour;
	float3	pointLightPosition;
	float	pointLightRange;
	float4	specularColour;
	float	specularPower;
	float3	eyePosition;
	float3	pad;
};

Texture2D Texture;
SamplerState ss;

struct VertexIn
{
	float3 InputPosition : POSITION;
	float3 Normal		 : NORMAL;
	float2 TexCoord		 : TEXCOORD;
};

struct VertexOut
{
	float4 OutputPosition	: SV_POSITION;
	float4 Normal			: TEXCOORD0;
	float4 WorldPosition	: TEXCOORD1;
	float2 TexCoord			: TEXCOORD2;	
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	// Convert inputs for each vertex to the formatted output position, normal, world position, and texture coordinates
	vout.OutputPosition = mul(worldViewProjection, float4(vin.InputPosition, 1.0f));
	vout.Normal = normalize(mul(worldTransformation, float4(vin.Normal, 0.0f)));
	vout.WorldPosition = mul(worldTransformation, float4(vin.InputPosition, 1.0f));
	vout.TexCoord = vin.TexCoord;

	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	// Calculate the diffuse light
	float4 lightVector = -directionalLightVector;
	float diffuseBrightness = saturate(dot(pin.Normal, lightVector));
	float4 diffuseLight = saturate(ambientLightColour + diffuseBrightness * directionalLightColour);

	// Calcuate the point light
	float3 pointVector = normalize(pointLightPosition.xyz - pin.WorldPosition.xyz);
	float pointDistance = length(pointVector);
	float pointBrightness = saturate(dot(pin.Normal.xyz, pointVector));
	float pointAttenuation = saturate(1 - (pointDistance / pointLightRange));
	float4 pointLight = saturate(pointBrightness * pointLightColour) * pointAttenuation;

	// Calculate the specular light
	float3 viewVector = normalize(eyePosition - pin.WorldPosition.xyz);
	float3 reflectionVector = normalize(reflect(lightVector.xyz, pin.Normal.xyz));
	float specularAngle = saturate(dot(viewVector, reflectionVector));
	float specularBrightness = pow(saturate(specularAngle), specularPower);
	float4 specularLight = specularBrightness * specularColour;

	// Calculate the final colour of the lighting
	float4 finalColour = saturate(diffuseLight + pointLight + specularLight) * Texture.Sample(ss, pin.TexCoord);
	return finalColour;
}


