#version 410

layout(location=0) in vec4 Position;
layout(location=1) in vec2 UV;


out vec2 frag_uv;
out vec4 frag_color;

uniform mat4 ProjectionView;
uniform sampler2D perlin_height_texture;
uniform sampler2D perlin_grass_texture;
uniform sampler2D perlin_rock_texture;

void main()
{
	vec4 pos = Position;
	pos.y += texture(perlin_height_texture, UV).r * 50;
	frag_uv = UV;	

	//color += step( 0.5, height.r ) * mix( black, white, 1.0 );
	pos.y = max(0.5*50, pos.y);//-( step( 0.5*50, pos.y ) *  1 );

	//frag_color = vec4( step( -0.5, pos.y *10 ),step( 0.0, pos.y * 10 ),step( 0.5, pos.y *10), 1 );
	gl_Position = ProjectionView * pos;
}