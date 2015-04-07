#version 410

in vec4 vPosition;
in vec4 vNormal;
in vec4 aPosition;

layout(location = 0) out vec3 albedo;
layout(location = 1) out vec3 position;
layout(location = 2) out vec3 normal;

uniform mat4 ProjectionView;

void main()
{
	// we simply output the data
	// Note: you could use a material colour,
	// or sample a texture for albedo
	albedo = vec3(1,1,1);

	vec4 tempPos = aPosition;


	vec3 colour = vec3(1);
	// grid every 1-unit
	if (fract( vPosition.x ) < 0.05f ||	fract( vPosition.y ) < 0.05f ||	fract( vPosition.z ) < 0.05f)
	{ 
		colour = vec3(0);
	}
	
	//albedo.rgb = colour * 0.25f + colour;


	//albedo = vNormal.xyz;
	position = vPosition.xyz;
	normal = vNormal.xyz;
}