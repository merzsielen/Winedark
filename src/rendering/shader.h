#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Winedark
{
	class Shader
	{
	public:
		GLuint ID;

		Shader(const char* vertexPath, const char* fragmentPath);

		void Use();

		void SetBool(const std::string& name, bool value) const;
		void SetInt(const std::string& name, int value) const;
		void SetFloat(const std::string& name, float value) const;
		void SetMatrix(const std::string& name, const glm::mat4& value) const;
		void SetVector2(const std::string& name, const glm::vec2& value) const;
		void SetVector3(const std::string& name, const glm::vec3& value) const;
		void SetVector4(const std::string& name, const glm::vec4& value) const;
		void SetVector4Arr(const std::string& name, const glm::vec4& value, const int& size) const;
	};
}

#endif