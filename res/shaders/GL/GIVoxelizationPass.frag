// shadertype=<glsl>
#version 450
#extension GL_ARB_shader_image_load_store : require

in GS_OUT
{
	vec3 wsPosition;
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	flat vec4 triangleAABB;
} gs_in;

layout(location = 0) out vec4 fragColor;
layout(pixel_center_integer) in vec4 gl_FragCoord;

layout(binding = 0, r32ui) uniform volatile coherent uimage3D uni_voxelAlbedo;
layout(binding = 1, r32ui) uniform volatile coherent uimage3D uni_voxelNormal;

//layout(binding = 2) uniform sampler2D uni_albedo;

uniform uint uni_volumeDimension;

vec4 convRGBA8ToVec4(uint val)
{
	return vec4(float((val & 0x000000FF)),
		float((val & 0x0000FF00) >> 8U),
		float((val & 0x00FF0000) >> 16U),
		float((val & 0xFF000000) >> 24U));
}

uint convVec4ToRGBA8(vec4 val)
{
	return (uint(val.w) & 0x000000FF) << 24U |
		(uint(val.z) & 0x000000FF) << 16U |
		(uint(val.y) & 0x000000FF) << 8U |
		(uint(val.x) & 0x000000FF);
}

void imageAtomicRGBA8Avg(layout(r32ui) volatile coherent uimage3D grid, ivec3 coords, vec4 value)
{
	value.rgb *= 255.0;                 // optimize following calculations
	uint newVal = convVec4ToRGBA8(value);
	uint prevStoredVal = 0;
	uint curStoredVal;
	uint numIterations = 0;

	while ((curStoredVal = imageAtomicCompSwap(grid, coords, prevStoredVal, newVal))
		!= prevStoredVal
		&& numIterations < 255)
	{
		prevStoredVal = curStoredVal;
		vec4 rval = convRGBA8ToVec4(curStoredVal);
		rval.rgb = (rval.rgb * rval.a); // Denormalize
		vec4 curValF = rval + value;    // Add
		curValF.rgb /= curValF.a;       // Renormalize
		newVal = convVec4ToRGBA8(curValF);

		++numIterations;
	}
}

vec3 EncodeNormal(vec3 normal)
{
	return normal * 0.5f + vec3(0.5f);
}

vec3 DecodeNormal(vec3 normal)
{
	return normal * 2.0f - vec3(1.0f);
}

void main()
{
	// Conservative Rasterization
	if (gs_in.position.x < gs_in.triangleAABB.x || gs_in.position.y < gs_in.triangleAABB.y ||
		gs_in.position.x > gs_in.triangleAABB.z || gs_in.position.y > gs_in.triangleAABB.w)
	{
		discard;
	}

	// writing coords position
	ivec3 position = ivec3(gs_in.wsPosition);

	// fragment albedo
	//vec4 albedo = texture(uni_albedo, gs_in.texCoord.xy);
	vec4 albedo = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// bring normal to 0-1 range
	vec4 normal = vec4(EncodeNormal(normalize(gs_in.normal)), 1.0f);

	// average normal per fragments sorrounding the voxel volume
	imageAtomicRGBA8Avg(uni_voxelNormal, position, normal);

	// average albedo per fragments sorrounding the voxel volume
	imageAtomicRGBA8Avg(uni_voxelAlbedo, position, albedo);
}