#include <glm/gtc/type_ptr.hpp>
#include "geometry.h"
#include "graphics.h"

using namespace fw;

namespace{
	GLuint CreateSkyBoxProgram()
	{
		static const GLchar* vertex_src[] =
		{
			"#version 330 core"
			"layout(location = 0) in vec3 position;"
			"out vec3 TexCoords;"
			"uniform mat4 viewproj;"
			"void main(){"
			"	vec4 pos = viewproj * vec4(position, 1);"
			"	gl_Position = pos.xyww;"
			"	TexCoords = position;}"
		};
		static const GLchar* frag_src[] =
		{
			"#version 330 core"
			"in vec3 TexCoords;"
			"out vec4 color;"
			"uniform samplerCube skybox;"
			"void main(){"
			"	color = texture(skybox, TexCoords);}"
		};
		// Create and compile vertex shader
		auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_src, NULL);
		glCompileShader(vertex_shader);
		// Create and compile fragment shader
		auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, frag_src, NULL);
		glCompileShader(fragment_shader);
		// Create program, attach shaders to it, and link it
		auto program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);
		// Delete the shaders as the program has them now
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		return program;
	}
}

SkyBox::SkyBox(std::array<std::string, 6> textures)
{
	cubemap_ = LoadCubemap(textures);
	cube_ = CreateCube();
	program_ = CreateSkyBoxProgram();
}

SkyBox::~SkyBox()
{
	glDeleteProgram(program_);
	glDeleteTextures(1, &cubemap_);
	glDeleteBuffers(1, &cube_);
}

void SkyBox::Draw(glm::mat4 viewproj)
{
	// draw skybox
	// note: the sky box depth is always 1
	glDepthFunc(GL_LEQUAL);
	glUseProgram(program_);
	auto viewproj_pos = glGetUniformLocation(program_, "viewproj");
	glUniformMatrix4fv(viewproj_pos, 1, GL_FALSE, glm::value_ptr(viewproj));

	glBindVertexArray(cube_);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(program_, "skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}
