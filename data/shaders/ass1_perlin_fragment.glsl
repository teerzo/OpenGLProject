#version 410

in vec4 vtx_position;
in vec4 vtx_color;
in vec4 vtx_normal;
in vec2 vtx_uv;
in vec2 vtx_uv_offset;

out vec4 frg_color;

uniform float timer;
uniform vec3 lava_direction;

uniform float lava_height;

uniform sampler2D perlin_1_texture;
uniform sampler2D dirt_texture;
uniform sampler2D grass_texture;
uniform sampler2D rock_texture;
uniform sampler2D lava_texture;


void main()
{
	vec4 height = texture(perlin_1_texture, vtx_uv).rrrr;
	vec4 color;

	if( vtx_position.y < lava_height ) {
		color = texture( lava_texture, (vtx_uv + vtx_uv_offset) * 3 );
	}
	else if( vtx_position.y < 3 ) {
		color = texture( dirt_texture, vtx_uv * 5);
		//color += mod( vtx_position.x, 2 ) * texture( grass_texture, vtx_uv * 5 );
		//color += mod( vtx_position.z, 2 ) * texture( grass_texture, vtx_uv * 5 );
	}
	else {
		color = texture( rock_texture, vtx_uv * 5 );
	}

	
	



	frg_color = vtx_normal;
	frg_color = color;
	frg_color.a = 1;
}