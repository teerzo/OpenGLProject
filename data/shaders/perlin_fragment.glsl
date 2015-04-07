#version 410

in vec2 frag_uv;
in vec4 frag_color;

out vec4 out_color;

uniform sampler2D perlin_height_texture;
uniform sampler2D perlin_grass_texture;
uniform sampler2D perlin_rock_texture;


void main()
{
	vec4 red = vec4(1,0,0,1);
	vec4 green = vec4(0,1,0,1);
	vec4 blue = vec4(0,0,1,1);
	vec4 black = vec4(0,0,0,1);
	vec4 white = vec4(1,1,1,1);

	vec4 height = texture(perlin_height_texture, frag_uv).rrrr;
	
	
	//color *= step( 0.5, height.r) * black;

	vec4 color = blue;
	color += step( 0.5, height.r ) * white;
	//color += smoothstep( 0.5, 0.7, height.r ) * green;
	//color += smoothstep( 0.7, 0.9, height.r ) * red;
	
	//color += step( 0.5, height.r ) * mix( black, green, 1.0 );
	//color += step( 0.7, height.r ) * mix( black, blue, 1.0 );
	//color += step( 0.9, height.r ) * mix( black, red, 1.0 ); 



	


	//color *= frag_color;

	out_color = color;
	out_color.a = 1;
}