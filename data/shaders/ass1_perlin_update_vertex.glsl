#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec2 uv;

out vec4 vtx_color;
out vec2 vtx_uv;

out VTX_OUT { vec3 position; } vtx_out;

uniform float timer;

uniform mat4 projection_view;
uniform sampler2D perlin_1_texture;

void main()
{
	int offset_pos = 30; // 50;

	
	vec4 pos = position;
	pos.y += (texture(perlin_1_texture, uv).r * offset_pos) - (offset_pos/2) ;
	vtx_uv = uv;	

	//pos.y = max(0.9*offset_pos, pos.y) - (offset_pos / 2);										
															//color += step( 0.5, height.r ) * mix( black, white, 1.0 );
	
	if( pos.y < 0 )
	{
		vtx_color = vec4(0,0,1,1);
		
		pos.y = sin(timer + pos.y) ;
	}
	else if( pos.y < 3 )
	{
		pos.y = 0;
		vtx_color = vec4(1,1,1,1);

		if( mod( pos.x, 2 ) == 0 )
		{
			if( mod( pos.z, 2 ) == 0 )
			{
				// draw building here
				vtx_color = vec4(0,1,0,1);
			}
		}
	}
	else
	{
		pos.y -= 3;
		vtx_color = vec4(1,0,0,1);
	}


	
	


	//if(max(1 *offset_pos, pos.y) == 0.5*offset_pos )
	//{
	//	//pos.y = 0;									
	//	//vtx_color = vec4(0,0,1,1);								
	//}
	//else if( max(0.6*offset_pos, pos.y) == 0.6*offset_pos )
	//{
	//	//pos.y = 1;
	//	//vtx_color = vec4(1,0,0,1);
	//}
	//else
	//{
	//	//pos.y = 10;
	//	//vtx_color = vec4(1,1,1,1);		
	//}

															//frag_color = vec4( step( -0.5, pos.y *10 ),step( 0.0, pos.y * 10 ),step( 0.5, pos.y *10), 1 );

	vtx_out.position = (projection_view * pos).xyz;
	gl_Position = projection_view * pos;
}
