#version 410

layout(location = 0 ) in vec4 Position;

// position in world space
uniform vec3	light_position;
uniform float	light_radius;
uniform mat4	ProjectionView;

void main()
{
	gl_Position = ProjectionView * vec4(Position.xyz * light_radius + light_position, 1);
}