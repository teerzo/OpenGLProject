#version 410

layout(location=0) in vec4 position;
//layout(location=1) in vec2 uv;

layout(location=1) in vec4 normals;
layout(location=2) in vec4 tangents;
layout(location=3) in vec2 uv;

out vec4 vtx_position;
out vec4 vtx_color;
out vec4 vtx_pos;
out vec4 vtx_normal;
out vec2 vtx_uv;
out vec2 vtx_uv_offset;

uniform float timer;
uniform vec3 lava_direction;
uniform float lava_speed;
uniform float lava_height;
uniform float dirt_height;

uniform mat4 projection_view;
uniform mat4 view;

uniform vec3 position_offset;
uniform sampler2D perlin_1_texture;

uniform sampler2D dirt_texture;
uniform sampler2D grass_texture;
uniform sampler2D rock_texture;
uniform sampler2D lava_texture;

void main()
{
	int offset_pos = 30; // 50;	
	vec4 pos = position + vec4(position_offset, 1);
	vec2 temp_uv = uv;

	if( pos.y < lava_height )	{
		temp_uv += vec2(lava_direction.xz * lava_speed * timer/1000);
		//temp_uv += vec2(sin(timer/1000+1),cos(timer/1000+1)); //* lava_speed * timer/1000;
		//pos.y = sin(timer + pos.y/2);
		pos.y = lava_height - sin(timer + pos.y/2) 	;
	}
		
	vec4 final_normal = vec4(0,0,0,0);

	if( pos.y < lava_height )
	{
	
	float texture_size = textureSize( perlin_1_texture, 0 );
	float texture_offset = 1 / texture_size;
	float sample_size = 1.0f;
	
	vec4 pos_up = pos;
	pos_up.x += 0;
	pos_up.y = (sin(timer + texture(perlin_1_texture, uv + vec2( 0, texture_offset).r) * offset_pos) /2 );  
	pos_up.z += sample_size;
	
	vec4 pos_up_right = pos;
	pos_up_right.x += sample_size;
	pos_up_right.y = (sin(timer + texture(perlin_1_texture, uv + vec2( texture_offset, texture_offset).r) * offset_pos)/2 );
	pos_up_right.z += sample_size;
	
	vec4 pos_right = pos;
	pos_right.x += sample_size;
	pos_right.y = (sin(timer + texture(perlin_1_texture, uv + vec2( texture_offset, 0).r) * offset_pos) /2 );
	pos_right.z += 0;
	
	vec4 pos_down_right = pos;
	pos_down_right.x += sample_size;
	pos_down_right.y = (sin(timer + texture(perlin_1_texture, uv + vec2( texture_offset, -texture_offset ).r) * offset_pos)/2 );
	pos_down_right.z -= sample_size;
	
	vec4 pos_down = pos;
	pos_down.x += 0;
	pos_down.y = (sin(timer + texture(perlin_1_texture, uv + vec2( 0, -texture_offset ).r) * offset_pos)/2 );
	pos_down.z -= sample_size;
	
	vec4 pos_down_left = pos;
	pos_down_left.x -= sample_size;
	pos_down_left.y = (sin(timer + texture(perlin_1_texture, uv + vec2( -texture_offset, -texture_offset ).r) * offset_pos)/2 );
	pos_down_left.z -= sample_size;
	
	vec4 pos_left = pos;
	pos_left.x -= sample_size;
	pos_left.y = (sin(timer + texture(perlin_1_texture, uv + vec2( -texture_offset, 0 ).r) * offset_pos)/2 );
	pos_left.z += 0;
	
	vec4 pos_up_left = pos;
	pos_up_left.x -= sample_size;
	pos_up_left.y = (sin(timer + texture(perlin_1_texture, uv + vec2( -texture_offset, texture_offset ).r) * offset_pos)/2 );
	pos_up_left.z += sample_size;
	
	
	final_normal.xyz += cross( pos_up.xyz - pos.xyz, pos_up_right.xyz - pos.xyz);
	final_normal.xyz += cross( pos_up_right.xyz - pos.xyz, pos_right.xyz - pos.xyz);
	final_normal.xyz += cross( pos_right.xyz - pos.xyz, pos_down_right.xyz - pos.xyz);
	final_normal.xyz += cross( pos_down_right.xyz - pos.xyz, pos_down.xyz - pos.xyz);
	
	final_normal.xyz += cross( pos_down.xyz - pos.xyz, pos_down_left.xyz - pos.xyz);
	final_normal.xyz += cross( pos_down_left.xyz - pos.xyz, pos_left.xyz - pos.xyz);
	final_normal.xyz += cross( pos_left.xyz - pos.xyz, pos_up_left.xyz - pos.xyz);
	final_normal.xyz += cross( pos_up_left.xyz - pos.xyz, pos_up.xyz - pos.xyz);
	
	//final_normal.x *= -1;
	//final_normal.z *= -1;
	//final_normal = vec4( 0,1,0,0);
		final_normal = normalize( final_normal );
	}
	else
	{
		final_normal = normalize( -normals );
	}

	vtx_position = view * pos;
	vtx_pos = pos;
	vtx_normal =  view* final_normal;
	vtx_uv = uv;	
	vtx_uv_offset = temp_uv;
	gl_Position = projection_view * (pos);
}
