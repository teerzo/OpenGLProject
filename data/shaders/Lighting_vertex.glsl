#version 410

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Normal;

out vec4 frag_position;
out vec4 frag_normal;

uniform mat4 ProjectionView;

void main()
{
	// To Fragment Shader
	frag_normal = Normal;
	frag_position = Position;

	// Vertex Position
	gl_Position = ProjectionView * Position;
}