#pragma once
#include "pch.h"

namespace SS {
	void init();
    void exit();
	class Texture {
	public:
		GLuint ID = -1;
		std::string path;
		Texture();
		~Texture();
		void load(std::string short_name);
		void bind();
	};	
	class Shader
	{
	public:
		unsigned int ID;
		Shader();
		~Shader();
		void load(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
		void use();
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setVec2(const std::string& name, const glm::vec2& value) const;
		void setVec2(const std::string& name, float x, float y) const;
		void setVec3(const std::string& name, const glm::vec3& value) const;
		void setVec3(const std::string& name, float x, float y, float z) const;
		void setVec4(const std::string& name, const glm::vec4& value) const;
		void setVec4(const std::string& name, float x, float y, float z, float w);
		void setMat2(const std::string& name, const glm::mat2& mat) const;
		void setMat3(const std::string& name, const glm::mat3& mat) const;
		void setMat4(const std::string& name, const glm::mat4& mat) const;
		std::string short_name;
	};
	
	void checkCompileErrors(GLuint shader, std::string type);
	
	int addShader(std::string short_name_without_ext);
	int addTexture(std::string short_texture_name);
	
	void useShader(int shader_number);
	GLuint shaderID(int shader_number);
	GLuint textureID(int texture_number);

	GLuint LoadShaderFromString(const std::string& vertexCode, const std::string& fragmentCode);
 //   GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);	
	
	void print();
}

