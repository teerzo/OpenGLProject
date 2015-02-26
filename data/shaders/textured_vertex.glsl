#version 410
layout(location=0) in vec4 Position; 
layout(location=1) in vec4 Color;	
layout(location=2) in vec2 UV;	
out vec4 out_color;
out vec2 out_uv;

uniform mat4 ProjectionView;
uniform vec3 cameraPos;

void main()
{
	out_color = Color;
	out_uv = UV;
	//vec2 tempPos = cameraPos.xy;
	//tempPos.x / 1;
	//tempPos.y / 1;
	//out_uv = tempPos;
	gl_Position = ProjectionView * Position;
}