
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal			: NORMAL;
	float2 uv				: TEXCOORD;
};

struct DirectionalLight
{
	float4 ambient;
	float4 diffuse;
	float3 normal;
};

cbuffer externalData : register (b0)
{
	DirectionalLight light;
	float4 color;
};

Texture2D diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);

float4 CalculateLight(float3 normal, DirectionalLight light)
{
	float3 lightDirection = normalize(-light.normal);
	float direction = saturate(dot(lightDirection, normal));
	return light.ambient + mul(direction, light.diffuse);
}

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	float4 lightCalc = CalculateLight(input.normal, light);
	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);
	//color *
	return (lightCalc * surfaceColor) * color;
}