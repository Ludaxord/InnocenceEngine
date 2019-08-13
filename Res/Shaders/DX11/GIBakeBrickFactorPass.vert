// shadertype=hlsl
#include "common/common.hlsl"

struct VertexInputType
{
	float4 position : POSITION;
	float2 texcoord : TEXCOORD;
	float2 pada : PADA;
	float4 normal : NORMAL;
	float4 padb : PADB;
};

struct GeometryInputType
{
	float4 posWS : SV_POSITION;
	float UUID : ID;
};

GeometryInputType main(VertexInputType input)
{
	GeometryInputType output;

	output.posWS = mul(input.position, meshCBuffer.m);
	output.UUID = meshCBuffer.UUID;

	return output;
}