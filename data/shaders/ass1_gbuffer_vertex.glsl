#version 410

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

// view-space normal and position
out vec4 vtx_position;
out vec4 vtx_normal;
out vec4 vtx_offset;
out vec2 vtx_uv;

uniform mat4 view; // needed in addition to Projection * View
uniform mat4 projection_view;
uniform vec3 mesh_offset;

void main()
{
	vec4 Offset = vec4(mesh_offset.xyz, 1 );
	
	vtx_position = view * vec4( position.xyz + Offset.xyz, 1 );
	vtx_normal	 = view * vec4(	normal.xyz, 0);
	vtx_offset	 = projection_view * ( vec4(position.xyz,1) + Offset );
	vec4 temp = vec4(10,10,10,1);
	vtx_uv = uv;

	gl_Position = projection_view * ( vec4(position.xyz,1) + Offset); // + Offset + temp);
}