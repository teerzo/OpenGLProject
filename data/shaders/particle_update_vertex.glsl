#version 410

// move particles based on velocity

// update the lifetime based on delta time

// spawn new particles if we need to

layout(location=0) in vec4 position;
layout(location=1) in vec4 velocity;
layout(location=2) in float lifetime;
layout(location=3) in float lifespan;

out vec4 updated_position;
out vec4 updated_velocity;
out float updated_lifetime;
out float updated_lifespan;

uniform float delta_time;
uniform vec4 emitter_position;
uniform float min_velocity;
uniform float max_velocity;
uniform float min_lifespan;
uniform float max_lifespan;
uniform float time;

const float INVERSE_MAX_UNIT = 1.0f/4294967295.0f;

float rand(uint seed, float range)
{
	float rand(uint seed, float range) {
	uint i = (seed ^ 12345391u) * 2654435769u;
	i ^= (i << 6u) ^ (i >> 26u);
	i *= 2654435769u;
	i += (i << 5u) ^ (i >> 12u);
	return float(range * i) * INVERSE_MAX_UINT;
}

void main()
{
	updated_position = position + velocity * delta_time;
	updated_velocity = velocity;
	updated_lifetime = lifetime + delta_time;
	updated_lifespan = lifespan;

	if( lifetime > lifespan )
	{
		updated_position = emitter_position;
		// set its velocity
		uint seed = uint (gl_VertexID + (time * 1000));
		float velocity_range = max_velocity - min_velocity;
		float velocity_length = rand(seed++, velocity_range) + min_velocity;

		updated_velocity.x = rand(seed++, 2) - 1;
		updated_velocity.y = rand(seed++, 2) - 1;
		updated_velocity.z = rand(seed++, 2) - 1;
		updated_velocity = normalize(updated_velocity) * velocity_length;

		// set its new lifespan
		updated_lifespan = 0;
		float lifespan_range = max_lifespan - min_lifespan;
		updated_lifespan = rand(seed++, lifespan_range) + min_lifespan;
	}
}