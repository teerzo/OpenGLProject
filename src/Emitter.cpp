#include "Emitter.h"

#include "gl_core_4_4.h"

Emitter::Emitter() :
m_particles(nullptr),
m_max_particles(0),
m_alive_count(0),
m_vertex_data(nullptr),
m_index_data(nullptr)
{
	m_buffers = {};
}
Emitter::~Emitter()
{
	delete[] m_particles;
	delete[] m_vertex_data;
	delete[] m_index_data;

	glDeleteVertexArrays(1, &m_buffers.m_VAO);
	glDeleteBuffers(1, &m_buffers.m_VBO);
	glDeleteBuffers(1, &m_buffers.m_IBO);
}

void Emitter::Initialise(emitter_type a_type, unsigned int a_max_particles, glm::vec4 a_position, float a_emit_rate,
	float a_min_lifespan, float a_max_lifespan,
	float a_min_velocity, float a_max_velocity,
	float a_start_size, float a_end_size,
	glm::vec4 a_start_color, glm::vec4 a_end_color)
{
	m_max_particles = a_max_particles;
	m_position = a_position;
	m_emit_rate = 1.0f / a_emit_rate;
	m_min_lifespan = a_min_lifespan;
	m_max_lifespan = a_max_lifespan;
	m_min_velocity = a_min_velocity;
	m_max_velocity = a_max_velocity;
	m_start_size = a_start_size;
	m_end_size = a_end_size;
	m_start_color = a_start_color;
	m_end_color = a_end_color;

	m_Weight = 1.0f;
	// gravity variables
	m_gravity_bool = true;
	m_gravity_value = 10.0f;
	m_gravity_direction = vec4(0, -1, 0, 1);
	// Wind Variables
	m_wind_bool = true;
	m_wind_value = 10.0f;
	m_wind_direction = vec4(-1, 0, 0, 1);


	m_alive_count = 0;
	m_particles = new Particle[m_max_particles];
	m_vertex_data = new VertexParticle[m_max_particles * 4];
	m_index_data = new unsigned int[m_max_particles * 6];

	for (unsigned int i = 0; i < m_max_particles; ++i)
	{
		unsigned int start = 4 * i;
		m_index_data[i * 6 + 0] = start + 0;
		m_index_data[i * 6 + 1] = start + 1;
		m_index_data[i * 6 + 2] = start + 2;
		m_index_data[i * 6 + 3] = start + 0;
		m_index_data[i * 6 + 4] = start + 2;
		m_index_data[i * 6 + 5] = start + 3;
	}

	glGenVertexArrays(1, &m_buffers.m_VAO);
	glGenBuffers(1, &m_buffers.m_VBO);
	glGenBuffers(1, &m_buffers.m_IBO);

	glBindVertexArray(m_buffers.m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_max_particles * 4 * sizeof(VertexParticle), m_vertex_data, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers.m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_max_particles * 6 * sizeof(unsigned int), m_index_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // Position
	glEnableVertexAttribArray(1); // Color

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexParticle), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexParticle), (void*)sizeof(glm::vec4));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Emitter::EmitParticles()
{
	unsigned int particles_to_emit = (unsigned int )(m_emit_timer / m_emit_rate );
	m_emit_timer -= particles_to_emit * m_emit_rate;

	if (m_type == emitter_type::direction || m_type == emitter_type::point)
	{
		//m_particles[m_alive_count].velocity.xyzw = m_direction;
	//	m_direction *= 
	}
	
	{
		//m_particles[m_alive_count].velocity.xyz = glm::sphericalRand(velocity_length);

	}


	for (unsigned int i = 0; i < particles_to_emit && m_alive_count < m_max_particles; ++i )
	{
		
		m_particles[m_alive_count].position = m_position;
		m_particles[m_alive_count].lifetime = 0;
		m_particles[m_alive_count].lifespan = glm::linearRand(m_min_lifespan, m_max_lifespan);
		m_particles[m_alive_count].color = m_start_color;
		m_particles[m_alive_count].size = m_start_size;
		float velocity_length = glm::linearRand(m_min_velocity, m_max_velocity);
		m_particles[m_alive_count].velocity.xyz = glm::sphericalRand(velocity_length);
		m_particles[m_alive_count].velocity.w = 0;

		++m_alive_count;
	

	}
}

void Emitter::UpdateVertexData(glm::vec3 a_camera_position, glm::vec3 a_camera_up, glm::vec4 offset  )
{
	for (unsigned int i = 0; i < m_alive_count; ++i)
	{
		glm::mat4 particle_transform(1);

		glm::vec3 to = a_camera_position - m_particles[i].position.xyz;
		glm::vec3 f = glm::normalize(to);
		glm::vec3 r = glm::cross(a_camera_up, f);
		glm::vec3 u = glm::cross(f, r);

		f *= m_particles[i].size;
		r *= m_particles[i].size;
		u *= m_particles[i].size;

		particle_transform[0].xyz = r;
		particle_transform[1].xyz = u;
		particle_transform[2].xyz = f;
		m_particles[i].position.w = 1;
		particle_transform[3] = m_particles[i].position;

		m_vertex_data[i * 4 + 0].Position = particle_transform * glm::vec4(-1, 1, 0, 1);
		m_vertex_data[i * 4 + 1].Position = particle_transform * glm::vec4(-1, -1, 0, 1);
		m_vertex_data[i * 4 + 2].Position = particle_transform * glm::vec4(1, -1, 0, 1);
		m_vertex_data[i * 4 + 3].Position = particle_transform * glm::vec4(1, 1, 0, 1);

		m_vertex_data[i * 4 + 0].Color = m_particles[i].color;
		m_vertex_data[i * 4 + 1].Color = m_particles[i].color;
		m_vertex_data[i * 4 + 2].Color = m_particles[i].color;
		m_vertex_data[i * 4 + 3].Color = m_particles[i].color;


	}
}

void Emitter::Update(float a_delta_time)
{
	// Clean up dead Particles
	for (unsigned int i = 0; i < m_alive_count; ++i)
	{
		m_particles[i].lifetime += a_delta_time;

		if (m_particles[i].lifetime > m_particles[i].lifespan)
		{
			--m_alive_count;
			m_particles[i] = m_particles[m_alive_count];
			--i;

		}

	}


	m_emit_timer += a_delta_time;
	EmitParticles();

	for (unsigned int i = 0; i < m_alive_count; ++i)
	{

		m_particles[i].position += a_delta_time * m_particles[i].velocity;
		
		// Gravity
		// add gravity value + direciton to velocity
		if ( m_gravity_bool)
			m_particles[i].velocity += a_delta_time * ( m_Weight *  m_gravity_value * m_gravity_direction);

		// Forces
		// add forces to 
		if (m_wind_bool)
			m_particles[i].velocity += a_delta_time * ( m_Weight / m_wind_value * m_wind_direction);


		float t = m_particles[i].lifetime / m_particles[i].lifespan;
		m_particles[i].color = glm::mix(m_start_color, m_end_color, t);
	
								
		m_particles[i].size = glm::mix(m_start_size, m_end_size, t);
	}

}

void Emitter::Render()
{
	
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers.m_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_alive_count * 4 * sizeof(VertexParticle), m_vertex_data);



	glBindVertexArray(m_buffers.m_VAO);
	glDrawElements(GL_TRIANGLES, 6 * m_alive_count, GL_UNSIGNED_INT, 0);
}


void Emitter::SetDirection(vec4 a_direction)
{
	m_direction = a_direction;
}