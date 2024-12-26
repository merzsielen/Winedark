#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace Winedark
{
	/*----------------------------------------------------------------------------------------------*/
	/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
	/* Shader																						*/
	/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
	/*----------------------------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------*/
	/* Utility Functions													 */
	/*-----------------------------------------------------------------------*/
	/*
		These utility functions just set values in the shader on the GPU.
		They're all variations on the same formula.
	*/
	void SetBool(GLuint id, const std::string& name, bool value)
	{
		glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
	}

	void SetInt(GLuint id, const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(id, name.c_str()), value);
	}

	void SetFloat(GLuint id, const std::string& name, float value)
	{
		glUniform1f(glGetUniformLocation(id, name.c_str()), value);
	}

	void SetMatrix(GLuint id, const std::string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void SetVector2(GLuint id, const std::string& name, const glm::vec2& value)
	{
		glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value));
	}

	void SetVector3(GLuint id, const std::string& name, const glm::vec3& value)
	{
		glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value));
	}

	/*-----------------------------------------------------------------------*/
	/* Compilation Functions												 */
	/*-----------------------------------------------------------------------*/
	/* ReadCode ---------------------------------------------*/
	/*
		ReadCode() takes in the path to a file containing shader code
		and returns the code, if read properly.

		Input: Shader path
		Output: Shader code
	*/
	std::string ReadCode(const char* path)
	{
		std::string code;
		std::ifstream file;
		std::stringstream stream;

		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			file.open(path);
			stream << file.rdbuf();
			file.close();
			code = stream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ in " << path << std::endl;
		}

		return code;
	}

	/* CompileShader ----------------------------------------*/
	/*
		CompileShader() takes a shader type and code, then
		asks the GPU to compile the shader and returns the
		ID.

		Input: Shader type & code
		Output: Shader ID
	*/
	GLuint CompileShader(GLenum type, std::string code)
	{
		const char* c = code.c_str();

		int success;
		char infoLog[512];

		GLuint id = glCreateShader(type);
		glShaderSource(id, 1, &c, NULL);
		glCompileShader(id);

		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED" << " / " << type << std::endl << infoLog << std::endl;
		}

		return id;
	}

	/* CreateProgram ----------------------------------------*/
	/*
		CreateProgram() takes a set of constituent shaders
		and puts together a program composed of them.

		Input: Shader ids
		Output: Program id
	*/
	GLuint CreateProgram(std::vector<GLuint> components)
	{
		int success;
		char infoLog[512];

		GLuint id = glCreateProgram();
		for (int i = 0; i < components.size(); i++) glAttachShader(id, components[i]);
		glLinkProgram(id);

		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << std::endl << infoLog << std::endl;
		}

		return id;
	}

	/*-----------------------------------------------------------------------*/
	/* Shader																 */
	/*-----------------------------------------------------------------------*/
	/*-------------------------------------------------------*/
	/* Use Function											 */
	/*-------------------------------------------------------*/
	/*
		Use() just tells the GPU to use this shader.

		Input: None
		Output: None
	*/
	void Shader::Use()
	{
		glUseProgram(id);
	}

	/*-------------------------------------------------------*/
	/* Constructors											 */
	/*-------------------------------------------------------*/
	/* Shader :: Vertex & Fragment --------------------------*/
	/*
		This constructor prepares vertex and fragment
		shaders.

		Input: Vertex & Fragment Shader Code Path
		Output: (Shader Program)
	*/
	Shader::Shader(std::string vertexPath, std::string fragmentPath)
	{
		/*
			First, we read in the code from the two shader
			files and compile them.
		*/
		GLuint vertex = CompileShader(GL_VERTEX_SHADER, ReadCode(vertexPath.c_str()));
		GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, ReadCode(fragmentPath.c_str()));

		/*
			Next, we create the shader program,
			combining the two shaders.
		*/
		this->id = CreateProgram({ vertex, fragment });

		/*
			And finally we clean up.
		*/
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	/* Shader :: Compute ------------------------------------*/
	/*
		This constructor prepares a compute shader.

		Input: Compute Shader Code Path
		Output: (Shader Program)
	*/
	Shader::Shader(std::string computePath)
	{
		/*
			First, we read in the code and compile the
			shader.
		*/
		GLuint compute = CompileShader(GL_COMPUTE_SHADER, ReadCode(computePath.c_str()));

		/*
			Next, we create the program and attach the
			shader.
		*/
		this->id = CreateProgram({ compute });

		/*
			And finally we clean up.
		*/
		glDeleteShader(compute);
	}
}