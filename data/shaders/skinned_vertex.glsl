#version 410

layout(location=0) in vec4 Position; 
layout(location=1) in vec2 UV;	
layout(location=2) in vec4 Bone_Indices;	
layout(location=3) in vec4 Bone_Weights;	

out vec2 out_uv;

uniform mat4 ProjectionView;
uniform mat4 World;

const int MAX_BONES = 128;
uniform mat4 bones[MAX_BONES];

void main()
{
	out_uv = UV;
	//
	
	ivec4 indices = ivec4(Bone_Indices);
	vec4 final_position = vec4( 0, 0, 0, 0 );

	final_position += bones[indices.x] * Position * Bone_Weights.x;
	final_position += bones[indices.y] * Position * Bone_Weights.y;
	final_position += bones[indices.z] * Position * Bone_Weights.z;
	final_position += bones[indices.w] * Position * Bone_Weights.w;

	final_position.w = 1;

	gl_Position = ProjectionView * World * final_position;
}