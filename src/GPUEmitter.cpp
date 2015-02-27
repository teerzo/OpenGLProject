#include "GPUEmitter.h"

#include "gl_core_4_4.h"
#include "Utility.h"


GPUPointEmitter::GPUPointEmitter() :
m_particles(nullptr),
m_max_particles(0),
m_position(0),
m_draw_shader(0),
m_update_shader(0),
m_last_draw_time(0)
{
	m_vao[0] = 0;
	m_vao[1] = 0;
	m_vbo[0] = 0;
	m_vbo[1] = 0;
}


GPUPointEmitter::~GPUPointEmitter()
{
	delete[] m_particles;
	
	glDeleteVertexArrays(2, m_vao);
	glDeleteBuffers(2, m_vbo);

	glDeleteProgram(m_update_shader);
	glDeleteProgram(m_draw_shader);
}

void GPUPointEmitter::Initialise( unsigned int a_max_particles, glm::vec4 a_position, float a_emit_rate,
	float a_min_lifespan, float a_max_lifespan,
	float a_min_velocity, float a_max_velocity,
	float a_start_size, float a_end_size,
	glm::vec4 a_start_color, glm::vec4 a_end_color)
{
	m_position = a_position;
	m_start_color = a_start_color;
	m_end_color = a_end_color;
	m_start_size = a_start_size;
	m_end_size = a_end_size;
	m_min_velocity = a_min_velocity;
	m_max_velocity = a_max_velocity;
	m_min_lifespan = a_min_lifespan;
	m_max_lifespan = a_max_lifespan;

	m_max_particles = a_max_particles;
	m_particles = new GPUParticle[m_max_particles];

	CreateBuffers();
	CreateUpdateShader();
	CreateDrawShader();
	
	m_Weight = 1.0f;
	// gravity variables
	m_gravity_bool = true;
	m_gravity_value = 10.0f;
	m_gravity_direction = vec4(0, -1, 0, 1);
	// Wind Variables
	m_wind_bool = true;
	m_wind_value = 10.0f;
	m_wind_direction = vec4(-1, 0, 0, 1);
	
	glEnableVertexAttribArray(0); // Position
	glEnableVertexAttribArray(1); // Color
		
} 

void GPUPointEmitter::CreateBuffers()
{
	glGenVertexArrays(2, m_vao);
	glGenBuffers(2, m_vbo);

	for (unsigned int buffer_index = 0; buffer_index < 2; ++buffer_index)
	{
		glBindVertexArray(m_vao[buffer_index]);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[buffer_index]);
		glBufferData(GL_ARRAY_BUFFER, m_max_particles * sizeof(GPUParticle), m_particles, GL_STREAM_DRAW);

		glEnableVertexAttribArray(0); // Position
		glEnableVertexAttribArray(1); // velocity
		glEnableVertexAttribArray(2); // lifetime
		glEnableVertexAttribArray(3); // lifespan

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), (void*)(16));
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), (void*)(32));
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), (void*)(36));
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GPUPointEmitter::CreateUpdateShader()
{
	unsigned int vertex_shader;
	LoadShaderType("../data/shaders/particle_update_vertex.glsl", GL_VERTEX_SHADER, &vertex_shader);

	m_update_shader = glCreateProgram();
	glAttachShader(m_update_shader, vertex_shader);

	const char* outputs[4] = { "updated_position",
		"updated_velocity",
		"updated_lifetime",
	"updated_lifespan"};

	glTransformFeedbackVaryings(m_update_shader, 4, outputs, GL_INTERLEAVED_ATTRIBS);
	glLinkProgram(m_update_shader);
	glDeleteShader(vertex_shader);
}
void GPUPointEmitter::CreateDrawShader()
{
	
	if (!LoadShader(&m_draw_shader,
		"../data/shaders/gpu_particle_vertex.glsl",
		"../data/shaders/gpu_particle_fragment.glsl",
		"../data/shaders/gpu_particle_geometry.glsl"))
	{
		printf("itsfucked\n");
	}



}


void GPUPointEmitter::Draw(float a_current_time, mat4 a_camera_transform, mat4 a_projection_view)
{
	// update vertex pass

	glUseProgram(m_update_shader);

	unsigned int uniform_delta_time = glGetUniformLocation(m_update_shader, "delta_time");
	unsigned int uniform_emitter_position = glGetUniformLocation(m_update_shader, "emitter_position");
	unsigned int uniform_min_velocity = glGetUniformLocation(m_update_shader, "min_velocity");
	unsigned int uniform_max_velocity = glGetUniformLocation(m_update_shader, "max_velocity");
	unsigned int uniform_min_lifespan = glGetUniformLocation(m_update_shader, "min_lifespan");
	unsigned int uniform_max_lifespan = glGetUniformLocation(m_update_shader, "max_lifespan");
	unsigned int uniform_time = glGetUniformLocation(m_update_shader, "time");

	glUniform1f(uniform_delta_time, a_current_time - m_last_draw_time);
	glUniform3fv(uniform_emitter_position, 1, (float*)&m_position);
	glUniform1f(uniform_min_velocity, m_min_velocity);
	glUniform1f(uniform_max_velocity, m_max_velocity);
	glUniform1f(uniform_min_lifespan, m_min_lifespan);
	glUniform1f(uniform_max_lifespan, m_max_lifespan);
	glUniform1f(uniform_time, a_current_time);

	glEnable(GL_RASTERIZER_DISCARD);

	glBindVertexArray(m_vao[m_active_buffer]);

	unsigned int other_buffer = (m_active_buffer + 1) % 2;

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vbo[other_buffer]);

	glBeginTransformFeedback(GL_POINTS); 

	// Actually Draw
	glDrawArrays(GL_POINTS, 0, m_max_particles);

	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK, 0, 0);

	// render pass
	glUseProgram(m_draw_shader);


	// projection view
	unsigned int uniform_projection_view = glGetUniformLocation(m_update_shader, "projection_view");
	// camera world
	unsigned int uniform_camera_world = glGetUniformLocation(m_update_shader, "camera_world");
	// start size
	unsigned int uniform_start_size = glGetUniformLocation(m_update_shader, "start_size");
	// end size
	unsigned int uniform_end_size = glGetUniformLocation(m_update_shader, "end_size");
	// start Color
	unsigned int uniform_start_color = glGetUniformLocation(m_update_shader, "start_color");
	// end Color
	unsigned int uniform_end_color = glGetUniformLocation(m_update_shader, "end_color");

	glUniformMatrix4fv(uniform_projection_view, 1, GL_FALSE, (float*)&a_projection_view);
	glUniformMatrix4fv(uniform_camera_world, 1, GL_FALSE, (float*)&a_camera_transform);
	glUniform1f(uniform_start_size, m_start_size);
	glUniform1f(uniform_end_size, m_end_size);
	glUniform4fv(uniform_start_color, 1,  (float*)&m_start_color);
	glUniform4fv(uniform_end_color, 1, (float*)&m_end_color);

	glBindVertexArray(m_vao[other_buffer]);
	glDrawArrays(GL_POINTS, 0, m_max_particles);

	// swap to next frame
	m_active_buffer = other_buffer;
	
}
