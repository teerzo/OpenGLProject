#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec2 uv;

out vec4 vtx_position;
out vec4 vtx_color;
out vec4 vtx_normal;
out vec2 vtx_uv;
out vec2 vtx_uv_offset;

uniform float timer;
uniform vec3 lava_direction;
uniform float lava_speed;
uniform float lava_height;

uniform mat4 projection_view;
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
	//pos.y = (texture(perlin_1_texture, uv).r * offset_pos) - (offset_pos/2);
	pos.y += (texture(perlin_1_texture, uv).r * offset_pos) - offset_pos/2;
	vec2 temp_uv = uv;

	if( pos.y < lava_height )	{
		temp_uv += vec2(lava_direction.xz * lava_speed * timer/1000);
		//pos.y = sin(timer + pos.y/2);
	}
	//else if( pos.y < 3 ) {
	//	pos.y = 0;
	//	if( mod( pos.x, 2 ) == 0 )
	//	{
	//		if( mod( pos.z, 2 ) == 0 )
	//		{
	//			// draw building here
	//			vtx_color = texture(grass_texture, uv);
	//			//vtx_color = vec4(0,1,0,1);
	//		}
	//	}
	//}
	//else
	//{
	//	pos.y -= 3;
	//}

	vtx_position = pos;
	vtx_normal = vec4(1,0,0,1);
	vtx_uv = uv;	
	vtx_uv_offset = temp_uv;
	gl_Position = projection_view * (pos);
}
