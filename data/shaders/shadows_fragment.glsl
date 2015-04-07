#version 410
in vec4 vNormal;
in vec4 vShadowCoord;
out vec4 FragColour;
uniform vec3 light_dir;
uniform sampler2D shadow_map;
void main() 
{
	float d = max(0, dot( -light_dir, normalize(vNormal.xyz) ));
	if (texture(shadow_map, vShadowCoord.xy).r < vShadowCoord.z - 0.01f) 
	{
		d = 0;
	}
	FragColour = vec4(d, d, d, 1);
}
