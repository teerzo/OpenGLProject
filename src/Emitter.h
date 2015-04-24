#ifndef _EMITTER_H_
#define _EMITTER_H_

#include "glm_header.h"
#include "Vertex.h"

static enum emitter_type
{
	point,
	direction,
	sphere,
	rectangle,
	plane,
	line,
	sphere_surface,
	ring,
	mesh
};


struct Particle
{
	glm::vec4 position;
	glm::vec4 velocity;
	glm::vec4 color;
	float size;
	float lifetime;
	float lifespan;
};

// Whad does an emitter do?
// it contains all of the particles it controls
// it spawns new particles

class Emitter
{
	public:
	// Type of emitter
	emitter_type m_type;
	glm::vec4 m_direction;

	// Particle data
	Particle* m_particles;
	unsigned int m_max_particles;
	unsigned int m_alive_count;

	// OpenGL data
	OpenGLData m_buffers;
	VertexParticle* m_vertex_data;
	unsigned int* m_index_data;

	// Emitter data
	glm::vec4 m_position;
	float m_emit_rate;
	float m_emit_timer;

	float m_min_lifespan;
	float m_max_lifespan;

	float m_min_velocity;
	float m_max_velocity;

	float m_start_size;
	float m_end_size;

	glm::vec4 m_start_color;
	glm::vec4 m_end_color;
		
	// Gravity Variables
	float m_Weight;
	bool m_gravity_bool;
	float m_gravity_value;
	glm::vec4 m_gravity_direction;

	// Force Variables
	bool m_wind_bool;
	float m_wind_value;
	glm::vec4 m_wind_direction;


	Emitter();
	~Emitter();

	void Initialise( emitter_type a_type, unsigned int a_max_particles, glm::vec4 a_position, float a_emit_rate,
		float m_min_lifespan, float m_max_lifespan,
		float a_min_velocity, float m_max_velocity,
		float a_start_size, float a_end_size,
	glm::vec4 a_start_color, glm::vec4 a_end_color );

	void EmitParticles();
	void UpdateVertexData(glm::vec3 a_camera_position, glm::vec3 a_camera_up, glm::vec4 offset );
	void Update(float a_delta_time);
	void Render();

	void SetDirection(glm::vec4 a_direction);
	void SetWeight(float a_weight);
	void SetGravityBool(bool a_bool);
	void SetGravityValue(float a_gravity);
	void SetGravityDirection(glm::vec3 a_direction);

};


#endif // _EMITTER_H_