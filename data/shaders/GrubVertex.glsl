#version 410

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Normal;
layout(location=2) in vec2 UV;

out vec3 frag_position;
out vec3 frag_normal;
out vec3 frag_tangent;
out vec3 frag_bitangent;
out vec2 frag_uv;

uniform mat4 ProjectionView;

void main()
{
	// To Fragment Shader
	frag_position = Position.xyz;
	frag_normal = Normal.xyz;
	frag_tangent = Tangent.xyz;
	frag_bitangent = cross(Normal.xyz, Tangent.xyz);
	frag_uv = UV;

	// Vertex Position
	gl_Position = ProjectionView * Position;
}