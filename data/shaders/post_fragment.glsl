#version 410

in vec2 tex_coord;
out vec4 frag_color;

uniform sampler2D input_texture;
uniform float timer;

vec4 BoxBlur()
{
	vec2 texel_uv = 1.0f / textureSize(input_texture,0).xy;
	vec4 color = texture(input_texture, tex_coord);
	color += texture(input_texture, tex_coord + texel_uv );

	color += texture(input_texture, tex_coord + vec2( texel_uv.x, 0 ));
	color += texture(input_texture, tex_coord + vec2( texel_uv.x, -texel_uv.y ));
	color += texture(input_texture, tex_coord + vec2( 0, texel_uv.y ));
	color += texture(input_texture, tex_coord + vec2( 0, -texel_uv.y ));
	color += texture(input_texture, tex_coord + vec2( -texel_uv.x, texel_uv.y ));
	color += texture(input_texture, tex_coord + vec2( -texel_uv.x, 0 ));
	color += texture(input_texture, tex_coord + vec2( -texel_uv.x, -texel_uv.y ));

	color /= 9 * timer;
	return color;

}

vec4 Distort() 
{
	vec2 mid = vec2(0.5f);
	float distanceFromCentre = distance(tex_coord, mid);
	vec2 normalizedCoord = normalize(tex_coord - mid);
	float bias = distanceFromCentre +
	sin(distanceFromCentre * 15) * 0.02f;
	vec2 newCoord = mid + bias * normalizedCoord;
	return texture(input_texture, newCoord);
}

vec4 Darken()
{
	vec4 color = texture(input_texture, tex_coord);

	if( color.x > 0.5f )
	{
		if( color.y > 0.5f )
		{
			if( color.z > 0.5f )
			{
				color.xyz *= 10.5f;
			}
		}
	}
	return color;
}

void main()
{

	frag_color = BoxBlur();
}

