#pragma once
//#include "pch.h"
#include "Camera.h"


namespace BS {
	inline Camera* camera;
	
	class body {
	public:
		GLuint VAO, VBO, CBO;
		GLuint IBO;
		
		GLuint mvpID;
		GLuint flagID;
		GLuint TextureID;
		
		int shaderNumber = 0;
		int textureNumber = 0;
		int flag1 = 0;
		Camera* cam;
		
		body();
		~body();
		
		virtual void build();
		virtual void init();
		virtual void do1();
		virtual void exit();
		virtual void addImguiControls(int  imguiWID, std::string imguiItemName);

		std::vector<GLfloat> vertexData; // 3 x no. of vertices
		std::vector<GLfloat> colorData;	 // 3 x no. of vertices
		std::vector<GLuint>  indexData;	 // 3 x no. of elements
	private:
		GLint polygonMode = GL_FILL;
	};

	class defaultBody :public body 
	{
	public:
		virtual void build() override;
	};

	inline std::vector<body*> bodies;

	// body not build by default
	body* addBody(int shaderIndex = 0, Camera* icam = nullptr,  int flag1 = 0);


	void pre_init();
	void init();

	void exit();

	void do1();

	void genVB_FLOAT3(GLuint& vbuffer, const GLfloat* data, int size);
	
	//(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
	void bindVB_FLOAT3(GLuint index, GLuint& vbuffer);


	
}