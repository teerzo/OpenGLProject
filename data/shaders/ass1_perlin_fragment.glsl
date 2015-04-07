#version 410

in vec4 vtx_color;
in vec2 vtx_uv;

out vec4 frg_color;

uniform sampler2D perlin_1_texture;

void main()
{
	vec4 red = vec4(1,0,0,1);
	vec4 green = vec4(0,1,0,1);
	vec4 blue = vec4(0,0,1,1);
	vec4 black = vec4(0,0,0,1);
	vec4 white = vec4(1,1,1,1);

	vec4 height = texture(perlin_1_texture, vtx_uv).rrrr;
	
	
	//color *= step( 0.5, height.r) * black;

	vec4 color = blue;
	color += step( 0.5, height.r ) * white;
	//color += smoothstep( 0.5, 0.7, height.r ) * green;
	//color += smoothstep( 0.7, 0.9, height.r ) * red;
	
	//color += step( 0.5, height.r ) * mix( black, green, 1.0 );
	//color += step( 0.7, height.r ) * mix( black, blue, 1.0 );
	//color += step( 0.9, height.r ) * mix( black, red, 1.0 ); 

	//color *= frag_color;

	frg_color = color;
	frg_color.a = 1;
}