#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

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
	void SetBool(GLuint id, const std::string& name, bool value);
	void SetUint(GLuint id, const std::string& name, unsigned int value);
	void SetInt(GLuint id, const std::string& name, int value);
	void SetFloat(GLuint id, const std::string& name, float value);
	void SetMatrix(GLuint id, const std::string& name, const glm::mat4& value);
	void SetVector2(GLuint id, const std::string& name, const glm::vec2& value);
	void SetVector3(GLuint id, const std::string& name, const glm::vec3& value);

	/*-----------------------------------------------------------------------*/
	/* Compilation Functions												 */
	/*-----------------------------------------------------------------------*/
	std::string ReadCode(const char* path);
	GLuint CompileShader(GLenum type, std::string code);
	GLuint CreateProgram(std::vector<GLuint> components);

	/*-----------------------------------------------------------------------*/
	/* Shader																 */
	/*-----------------------------------------------------------------------*/
	class Shader
	{
	private:
		/*-----------------------------------------------------*/
		/* ID												   */
		/*-----------------------------------------------------*/
		GLuint				id;

	public:
		/*-----------------------------------------------------*/
		/* ID Function										   */
		/*-----------------------------------------------------*/
		GLuint				GetID() { return id; }

		/*-----------------------------------------------------*/
		/* Use Function										   */
		/*-----------------------------------------------------*/
		void				Use();

		/*-----------------------------------------------------*/
		/* Constructors										   */
		/*-----------------------------------------------------*/
		Shader::Shader(std::string vertexPath, std::string fragmentPath);
		Shader::Shader(std::string computePath);
	};
}

#endif