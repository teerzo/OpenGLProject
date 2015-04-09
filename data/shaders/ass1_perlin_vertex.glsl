#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec2 uv;

out vec4 vtx_color;
out vec2 vtx_uv;

uniform mat4 projection_view;
uniform sampler2D perlin_1_texture;

void main()
{
	int offset_pos = 30; // 50;

	vec4 pos = position;
	pos.y += texture(perlin_1_texture, uv).r * offset_pos;
	vtx_uv = uv;	

	//pos.y = max(0.9*offset_pos, pos.y) - (offset_pos / 2);										
															//color += step( 0.5, height.r ) * mix( black, white, 1.0 );
	
	if(max(0.5*offset_pos, pos.y) == 0.5*offset_pos )
	{
		pos.y = 0;									
		vtx_color = vec4(0,0,1,1);								
	}
	else
	{
		pos.y = max(0.5*offset_pos, pos.y) - (offset_pos / 2);		
		if( mod(pos.x, 2) == 0 )
		{
			vtx_color = vec4(0,1,0,1);
		}	
		else
		{
			vtx_color = vec4(1,0.6,0.6,1);
			pos.y = 0;
		}
	}

															//frag_color = vec4( step( -0.5, pos.y *10 ),step( 0.0, pos.y * 10 ),step( 0.5, pos.y *10), 1 );
	gl_Position = projection_view * pos;
}
