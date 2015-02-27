#ifndef _GPU_EMITTER_H_
#define _GPU_EMITTER_H_

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


struct GPUParticle
{
	GPUParticle() : lifespan(0),lifetime(1) {}
	glm::vec4 position;
	glm::vec4 velocity;
	float lifetime;
	float lifespan;
};

// Whad does an emitter do?
// it contains all of the particles it controls
// it spawns new particles

class GPUPointEmitter
{
private:

	// Particle data
	GPUParticle* m_particles;
	unsigned int m_max_particles;

	// OpenGL data
	unsigned int m_active_buffer;
	unsigned int m_vao[2];
	unsigned int m_vbo[2];

	unsigned int m_update_shader;
	unsigned int m_draw_shader;

	float m_last_draw_time;


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
	vec4 m_gravity_direction;

	// Force Variables
	bool m_wind_bool;
	float m_wind_value;
	vec4 m_wind_direction;

public:
	GPUPointEmitter();
	~GPUPointEmitter();

	void Initialise(unsigned int a_max_particles, glm::vec4 a_position, float a_emit_rate,
		float m_min_lifespan, float m_max_lifespan,
		float a_min_velocity, float m_max_velocity,
		float a_start_size, float a_end_size,
		glm::vec4 a_start_color, glm::vec4 a_end_color);

	void Draw(float a_current_time, mat4 a_camera_transform, mat4 a_projection_view);

	void CreateBuffers();
	void CreateUpdateShader();
	void CreateDrawShader();




	void EmitParticles();
	void UpdateVertexData(glm::vec3 a_camera_position, glm::vec3 a_camera_up);
	void Update(float a_delta_time);
	void Render();

	void SetDirection(vec4 a_direction);
	void SetWeight(float a_weight);
	void SetGravityBool(bool a_bool);
	void SetGravityValue(float a_gravity);
	void SetGravityDirection(vec3 a_direction);

};


#endif // _GPU_EMITTER_H_