#version 410

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;

// view-space normal and position
out vec4 vPosition;
out vec4 vNormal;
out vec4 aPosition;

uniform mat4 View; // needed in addition to Projection * View
uniform mat4 ProjectionView;
uniform vec4 MeshPosition;

void main()
{
	vec4 offset = MeshPosition;
	offset.w = 1;

	aPosition = ProjectionView * ( Position );

	// first store view-space position and normal
	vPosition = View * vec4( Position.xyz, 1 );
	vNormal = View * vec4(Normal.xyz, 0);
	gl_Position = ProjectionView * ( Position );
}