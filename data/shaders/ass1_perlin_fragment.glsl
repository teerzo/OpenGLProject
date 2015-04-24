#version 410

in vec4 vtx_position;
in vec4 vtx_color;
in vec4 vtx_pos;
in vec4 vtx_normal;
in vec2 vtx_uv;
in vec2 vtx_uv_offset;

//out vec4 frg_color;
//out vec4 frg_normal;

layout(location = 0) out vec3 albedo;
layout(location = 1) out vec3 position;
layout(location = 2) out vec3 normal;

uniform float timer;
uniform vec3 lava_direction;

uniform float lava_height;
uniform float dirt_height;
uniform float rock_height;

uniform sampler2D perlin_1_texture;
uniform sampler2D dirt_texture;
uniform sampler2D grass_texture;
uniform sampler2D rock_texture;
uniform sampler2D lava_texture;


void main()
{
	vec4 height = texture(perlin_1_texture, vtx_uv).rrrr;
	vec4 color;

	float rock_value = 0.0f;
	float grass_value = 0.0f;
	float dirt_value = 0.0f;
	float lava_value = 0.0f;
	
//// Rock texture
//rock_value = step( vtx_position.y, 10000 );
//color += rock_value * texture( rock_texture, vtx_uv * 5 );
//
//// Grass Texture
////grass_value = 
//
//// Dirt Texture
//dirt_value = step( vtx_position.y, 3 );
//color += dirt_value * texture (dirt_texture, vtx_uv * 5 );
//
//// Lava Texture
//lava_value = step( vtx_position.y, lava_height );
//color += lava_value *  texture ( lava_texture, (vtx_uv + vtx_uv_offset ) * 3);	


	//if( vtx_pos.y < lava_height-0.5 ) {
	//color = mix( texture( dirt_texture, (vtx_uv + vtx_uv_offset) * 5 ), texture( lava_texture, (vtx_uv + vtx_uv_offset) * 5 ), 0.8);
	//}
	if( vtx_pos.y < lava_height ) {
		color = mix( texture( dirt_texture, (vtx_uv + vtx_uv_offset) * 5 ), texture( lava_texture, (vtx_uv + vtx_uv_offset) * 5 ), 0.8);
	}
	else if( vtx_pos.y < dirt_height ) {
		color = texture( dirt_texture, vtx_uv * 5);
		//color += mod( vtx_position.x, 2 ) * texture( grass_texture, vtx_uv * 5 );
		//color += mod( vtx_position.z, 2 ) * texture( grass_texture, vtx_uv * 5 );
	}
	else if( vtx_pos.y < rock_height ){
		color = texture( rock_texture, vtx_uv * 5 );
	}
	else
	{
		color.w = 0;
	}

	/// $$$ ITS FUCKED
	

	//frg_normal = vtx_normal;
	//frg_color = vtx_normal;
	//frg_color = color;
	//frg_color.a = 1;

	//color = texture(perlin_1_texture, vtx_uv).rrrr;
	//color.w = 1;
	
	//albedo = vtx_normal.xyz;
	albedo = color.xyz;
	position = vtx_position.xyz;
	normal = vtx_normal.xyz;
}