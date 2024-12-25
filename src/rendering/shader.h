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
	void SetBool(GLuint id, const std::string& name, bool value) const;
	void SetUint(GLuint id, const std::string& name, unsigned int value) const;
	void SetInt(GLuint id, const std::string& name, int value) const;
	void SetFloat(GLuint id, const std::string& name, float value) const;
	void SetMatrix(GLuint id, const std::string& name, const glm::mat4& value) const;
	void SetVector2(GLuint id, const std::string& name, const glm::vec2& value) const;
	void SetVector3(GLuint id, const std::string& name, const glm::vec3& value) const;

	/*-----------------------------------------------------------------------*/
	/* Compilation Functions												 */
	/*-----------------------------------------------------------------------*/
	const char* ReadCode(const char* path);
	GLuint CompileShader(GLenum type, const char* code);
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
		Shader::Shader(const char* vertexPath, const char* fragmentPath);
		Shader::Shader(const char* computePath);
	};
}

#endif