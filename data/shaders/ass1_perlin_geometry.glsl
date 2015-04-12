#version 410

layout(points) in;
layout(triangle_strip, max_vertices = 4 ) out;

in vec4 vtx_color[];
//in vec3 geometry_position[];

in VTX_OUT { vec3 position; } geo_in[];

out vec4 frag_color;

uniform mat4 projection_view;
uniform mat4 world;

void main()
{

	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	
	//gl_Position = gl_in[0].gl_Position + vec4( geo_in[0].position, 0.0f) * 3.0f;
	gl_Position = gl_in[0].gl_Position + vec4( 0,10,0,1);
	EmitVertex();

	EndPrimitive();

	

	
		//				float half_size = 1;
		//				
		//				vec3 corners[4];
		//				
		//				corners[0] = vec3( half_size, 0, -half_size);
		//				corners[1] = vec3( half_size, 0, half_size);
		//				corners[2] = vec3( -half_size, 0, -half_size);
		//				corners[3] = vec3( -half_size, 0, half_size);
		//				
		//				// add the geometry position
		//				//corners[0] += geometry_position[0];
		//				//corners[1] +=  geometry_position[0];
		//				//corners[2] +=  geometry_position[0];
		//				//corners[3] +=  geometry_position[0];
		//				
		//				corners[0] = gl_in[0].gl_Position.xyz + corners[0];
		//				corners[1] = gl_in[0].gl_Position.xyz + corners[1];
		//				corners[2] = gl_in[0].gl_Position.xyz + corners[2];
		//				corners[3] = gl_in[0].gl_Position.xyz + corners[3];
		//
		//				frag_color = vec4(1,0,0,1);
		//
		//				//gl_Position = projection_view * vec4( corners[0].xyz, 1 );
		//				//EmitVertex();
		//				//gl_Position = projection_view * vec4(corners[1].xyz ,1);
		//				//EmitVertex();
		//				//gl_Position = projection_view * vec4(corners[2].xyz ,1);
		//				//EmitVertex();
		//				//gl_Position = projection_view * vec4(corners[3].xyz ,1);
		//				//EmitVertex();
		//

}
