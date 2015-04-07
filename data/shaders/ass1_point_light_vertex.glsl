#version 410

layout(location = 0 ) in vec4 position;

// position in world space
uniform vec3	light_position;
uniform float	light_radius;
uniform mat4	projection_view;

void main()
{
	gl_Position = projection_view * vec4(position.xyz * light_radius + light_position, 1);
}