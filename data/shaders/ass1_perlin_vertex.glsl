#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec2 uv;

out vec4 vtx_color;
out vec2 vtx_uv;

uniform mat4 projection_view;
uniform sampler2D perlin_1_texture;

void main()
{
	vec4 pos = position;
	pos.y += texture(perlin_1_texture, uv).r * 50;
	vtx_uv = uv;	

	//color += step( 0.5, height.r ) * mix( black, white, 1.0 );
	pos.y = max(0.5*50, pos.y);//-( step( 0.5*50, pos.y ) *  1 );

	//frag_color = vec4( step( -0.5, pos.y *10 ),step( 0.0, pos.y * 10 ),step( 0.5, pos.y *10), 1 );
	gl_Position = projection_view * pos;
}
