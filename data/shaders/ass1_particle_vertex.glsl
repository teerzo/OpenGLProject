#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec4 color;

out vec4 vtx_position;
out vec4 vtx_color;
out vec4 vtx_normal;

uniform mat4 projection_view;
uniform mat4 view;
uniform vec4 offset;

void main()
{
	vtx_position = projection_view * vec4( position.xyz + offset.xyz, 1);
	vtx_color = color;
	vtx_normal = view * vec4(0,0,1,0);
	gl_Position = projection_view * ( vec4(position.xyz,1) + offset);
}