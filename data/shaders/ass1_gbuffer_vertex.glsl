#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;

// view-space normal and position
out vec4 vtx_position;
out vec4 vtx_normal;
out vec4 vtx_offset;

uniform mat4 view; // needed in addition to Projection * View
uniform mat4 projection_view;
uniform vec3 mesh_offset;

void main()
{
	vec4 Offset = vec4(mesh_offset.xyz, 1 );
	
	vtx_position = view * vec4( position.xyz, 1 );
	vtx_normal	 = view * vec4(	normal.xyz, 0);
	vtx_offset	 = projection_view * ( position + Offset );

	gl_Position = projection_view * ( position + Offset );
}