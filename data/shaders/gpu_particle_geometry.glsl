#version 410

layout(points) in;
layout(triangle_strip, max_vertices = 4 ) out;

in vec3 geometry_position[];
in float geometry_lifetime[];
in float geometry_lifespan[];

out vec4 frag_color;

uniform mat4 projection_view;
uniform mat4 camera_world;

uniform float start_size;
uniform float end_size;

uniform vec4 start_color;
uniform vec4 end_color;

void main()
{
	float t = geometry_lifetime[0] / geometry_lifespan[0];
	frag_color = mix(start_color, end_color, t );
	float half_size = mix(start_size, end_size, t );
	
	vec3 corners[4];
	
	corners[0] = vec3( half_size, -half_size, 0);
	corners[1] = vec3( half_size, half_size, 0);
	corners[2] = vec3( -half_size, -half_size, 0);
	corners[3] = vec3( -half_size, half_size, 0);
	
	mat3 billboard_rotation;
	vec3 forward = normalize(camera_world[3].xyz - geometry_position[0]);
	vec3 right = cross(camera_world[1].xyz, forward);
	vec3 up = cross(forward, right );
	// combine axies into matrix
	billboard_rotation = mat3(right, up, forward );
	
	// rotate the verts
	corners[0] = billboard_rotation * corners[0];
	corners[1] = billboard_rotation * corners[1];
	corners[2] = billboard_rotation * corners[2];
	corners[3] = billboard_rotation * corners[3];
	// add the geometry position
	corners[0] += geometry_position[0];
	corners[1] += geometry_position[0];
	corners[2] += geometry_position[0];
	corners[3] += geometry_position[0];
	
	
	gl_Position = projection_view * vec4( corners[0].xyz, 1 );
	EmitVertex();
	gl_Position = projection_view * vec4(corners[1].xyz ,1);
	EmitVertex();
	gl_Position = projection_view * vec4(corners[2].xyz ,1);
	EmitVertex();
	gl_Position = projection_view * vec4(corners[3].xyz ,1);
	EmitVertex();
	//frag_color = vec4(1,0,0,1);
	//gl_Position = vec4(1,0,0,1);
}
