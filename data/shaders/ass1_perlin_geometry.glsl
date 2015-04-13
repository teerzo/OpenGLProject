#version 410

layout(triangles) in;
layout(triangle_strip, max_vertices = 4 ) out;

uniform mat4 projection_view;
uniform mat4 world;

out vec3

void main()
{
	vec3 ab = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 ac = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 face_normal = normalize(cross(ab,ac));

	vec4 tri_centroid = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position / 3.0f );
	
	gl_Position = projection_view * tri_centroid;
	gs_out.color = gs_in[0].color;
	EmitVertex();

	gl_Position = projection_view * tri_centroid + (vec4( face_normal + 3.0f, 0.0f ));
	gs_out.color = gs_in[0].color;
	EmitVertex();

	gl_Position = projection_view * tri_centroid + (vec4( face_normal + 3.0f, 0.0f ));
	gs_out.color = gs_in[0].color;
	EmitVertex();

}
