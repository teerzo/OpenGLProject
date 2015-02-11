#version 410
layout(location=0) in vec4 Position; 
layout(location=1) in vec4 Color;	
out vec4 out_color;
uniform float timer;
uniform vec3 cameraPos;
uniform mat4 ProjectionView;
void main()
{
	vec4 tempColor = Color;
	tempColor.x +=  sin(cameraPos.x + timer ) * 1.0f; 
	tempColor.y +=  sin(cameraPos.y + timer ) * 1.0f; 
	tempColor.z +=  sin(cameraPos.z + timer ) * 1.0f; 
		//tempColor.x += sin(timer + Position.z) * 1.0f;
		//tempColor.y += sin(timer + Position.x) * 1.0f;
		//tempColor.z += sin(timer + Position.y) * 1.0f;
	out_color = tempColor;
	vec4 Pos = Position;
	Pos.x += sin( tempColor.z);
	Pos.y += sin(tempColor.x);
	Pos.z += sin(tempColor.y);
	gl_Position = ProjectionView * Pos;
}