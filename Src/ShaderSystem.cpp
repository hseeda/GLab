#include "pch.h"
#include "ShaderSystem.h"
#include "WindowSystem.h"
#include "ImguiSystem.h"
#include "FileSystem.h"

namespace SS {

	inline std::vector<Shader*> shaders;
	inline std::unordered_map<std::string, int /*shaderIndex*/ > shaderNameMap;

	inline std::vector<Texture*> textures;
	inline std::unordered_map<std::string, int /*shaderIndex*/ > textureNameMap;
	void init()
	{
		// texture 0
		addTexture("matlab.png");
		// shader 0  no textures
		addShader("o1");
		// shader 1 with texture as image
		addShader("t1"); 
		IS::log(0,"[init] shader system\n");
	}

	int addShader(std::string short_name_without_ext)
	{
		std::string vs = FS::_shaders_path + short_name_without_ext + ".vs";
		std::string fs = FS::_shaders_path + short_name_without_ext + ".fs";
		Shader* s = new Shader;
		s->load(vs.c_str(), fs.c_str());
		s->short_name = vs;
		shaders.push_back(s);
		shaderNameMap[short_name_without_ext] = shaders.size() - 1;
		return (int) shaders.size() - 1;		
	}

	int addTexture(std::string short_texture_name)
	{
		std::string path = FS::_texture_path + short_texture_name;
		Texture* t = new Texture;
		t->load(path);
		textures.push_back(t);
		textureNameMap[short_texture_name] = textures.size() - 1;
		return (int) shaders.size() - 1;
	}

	void exit()
	{
		for (auto s : shaders)
			delete s;
		shaders.clear();
		for (auto t : textures)
			delete t;
		textures.clear();
	}

	void checkCompileErrors(GLuint shader, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
	
	GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

		// Create the shaders
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		// Read the Vertex Shader code from the file
		std::string VertexShaderCode;
		std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
		if (VertexShaderStream.is_open()) {
			std::stringstream sstr;
			sstr << VertexShaderStream.rdbuf();
			VertexShaderCode = sstr.str();
			VertexShaderStream.close();
		}
		else {
			printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
			getchar();
			return 0;
		}

		// Read the Fragment Shader code from the file
		std::string FragmentShaderCode;
		std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
		if (FragmentShaderStream.is_open()) {
			std::stringstream sstr;
			sstr << FragmentShaderStream.rdbuf();
			FragmentShaderCode = sstr.str();
			FragmentShaderStream.close();
		}

		GLint Result = GL_FALSE;
		int InfoLogLength;


		// Compile Vertex Shader
		printf(">>> Compiling shader : %s\n", vertex_file_path);
		char const* VertexSourcePointer = VertexShaderCode.c_str();
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
		glCompileShader(VertexShaderID);

		// Check Vertex Shader
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			printf("%s\n", &VertexShaderErrorMessage[0]);
		}



		// Compile Fragment Shader
		printf(">>> Compiling shader : %s\n", fragment_file_path);
		char const* FragmentSourcePointer = FragmentShaderCode.c_str();
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
		glCompileShader(FragmentShaderID);

		// Check Fragment Shader
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			printf("%s\n", &FragmentShaderErrorMessage[0]);
		}

		// Link the program
		printf(">>> Linking program\n");
		GLuint ProgramID = glCreateProgram();
		glAttachShader(ProgramID, VertexShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glLinkProgram(ProgramID);

		// Check the program
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			printf("%s\n", &ProgramErrorMessage[0]);
		}


		glDetachShader(ProgramID, VertexShaderID);
		glDetachShader(ProgramID, FragmentShaderID);

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);

		return ProgramID;
	}

	GLuint LoadShaderFromString(const std::string & vertexCode, const std::string& fragmentCode)
	{
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		// 2. compile shaders
		unsigned int vertex, fragment;
		// vertex shader

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		_pl(">>> Compiling shader : from string");
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		_pl(">>> Compiling shader : from string");

		GLuint ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		return ID;
	}

	void print()
	{
		int cc = 0; 
		_ps2;
		_pl(">>> Shader Count: ", shaders.size());
		for (auto s : shaders)
			_pl(">>> ID[",cc++, "] ", s->ID," ", s->short_name);
		cc = 0;
		_pl(">>> Texture Count: ", textures.size());
		for (auto t : textures)
			_pl(">>> ID[",cc++,"] ", t->ID, " ", t->path);
		_pse2;
	}

	void useShader(int shader_number)
	{
		glUseProgram(shaders[shader_number]->ID);
	}

	GLuint shaderID(int shader_number) {
		return shaders[shader_number]->ID;
	}
	
	GLuint textureID(int texture_number)
	{
		return textures[texture_number]->ID;
	}

	Shader::Shader() { }

	Shader::~Shader()
	{
		glDeleteProgram(ID);
	}

	void Shader::load(const char* vertexPath, const char* fragmentPath, const char* geometryPath /*= nullptr*/)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
			// if geometry shader path is present, also load a geometry shader
			if (geometryPath != nullptr)
			{
				gShaderFile.open(geometryPath);
				std::stringstream gShaderStream;
				gShaderStream << gShaderFile.rdbuf();
				gShaderFile.close();
				geometryCode = gShaderStream.str();
			}
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		// 2. compile shaders
		unsigned int vertex, fragment;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		_pl(">>> Compiling shader : ", vertexPath);
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		_pl(">>> Compiling shader : ", fragmentPath);
		// if geometry shader is given, compile geometry shader
		unsigned int geometry;
		if (geometryPath != nullptr)
		{
			const char* gShaderCode = geometryCode.c_str();
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &gShaderCode, NULL);
			glCompileShader(geometry);
			checkCompileErrors(geometry, "GEOMETRY");
		}
		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		if (geometryPath != nullptr)
			glAttachShader(ID, geometry);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		if (geometryPath != nullptr)
			glDeleteShader(geometry);
	}

	void Shader::use() 
	{ 
		glUseProgram(ID); 
	}

	void Shader::setBool(const std::string& name, bool value) const
	{

		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void Shader::setInt(const std::string& name, int value) const
	{

		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void Shader::setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void Shader::setVec2(const std::string& name, const glm::vec2& value) const
	{

		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}

	void Shader::setVec2(const std::string& name, float x, float y) const
	{

		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}

	void Shader::setVec3(const std::string& name, const glm::vec3& value) const
	{

		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}

	void Shader::setVec3(const std::string& name, float x, float y, float z) const
	{

		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}

	void Shader::setVec4(const std::string& name, const glm::vec4& value) const
	{

		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}

	void Shader::setVec4(const std::string& name, float x, float y, float z, float w)
	{

		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}

	void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	Texture::Texture()
	{	}

	Texture::~Texture()
	{glDeleteTextures(1, &ID);}

	void Texture::load(std::string ipath)
	{
		path = ipath;
		if (ID != -1) glDeleteTextures(1, &ID);
		//glGenTextures(1, &ID);
		ID = SOIL_load_OGL_texture(path.c_str(), 4, 0, SOIL_FLAG_TEXTURE_REPEATS| SOIL_FLAG_MIPMAPS | SOIL_FLAG_POWER_OF_TWO);

		//int width, height, nrComponents;
		//unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
		//if (data)
		//{
		//	GLenum format = GL_RGBA;
		//	if (nrComponents == 1)
		//		format = GL_RED;
		//	else if (nrComponents == 3)
		//		format = GL_RGB;
		//	else if (nrComponents == 4)
		//		format = GL_RGBA;
		//	glBindTexture(GL_TEXTURE_2D, ID);
		//	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		//	glGenerateMipmap(GL_TEXTURE_2D);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//	stbi_image_free(data);
		//}
		//else
		//{
		//	std::cout << "ERROR: Texture failed to load at path: " << path << std::endl;
		//	stbi_image_free(data);
		//}

	}

	void Texture::bind()
	{
		glBindTexture(GL_TEXTURE_2D, ID);
	}
};