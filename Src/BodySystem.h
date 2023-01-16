#pragma once
//#include "pch.h"
#include "Camera.h"


namespace BS {


	inline Camera* camera;
	
	class base {
	public:

		base()  = default;
		~base() = default;

		Camera* cam;
		int shaderNumber = 0;
		GLuint VAO, VBO, CBO, IBO;
		GLuint mvpID;
		GLuint flagID;
		GLuint TextureID;
		std::vector<GLfloat> vertexData; // 3 x no. of vertices
		std::vector<GLfloat> colorData;	 // 3 x no. of vertices
		std::vector<GLuint>  indexData;	 // 3 x no. of elements
		int textureNumber = 0;
		int flag1 = 0;
		GLint polygonMode = GL_FILL;
		
		virtual void build() {};
		virtual void init();
		virtual void do1();
		virtual void exit();
		virtual void addImguiControls(int  imguiWID, std::string imguiItemName);
	};

	class body : public base{
	public:
		
	};

	class defaultBody : public base
	{
	public:
		void build();
	};


	// body not build by default
	base* addBody(BT bodyType = BT::defaulBody, int shaderIndex = 0, Camera* icam = nullptr,  int flag1 = 0);
	base* addBody(int shaderIndex);

	void pre_init();

	void init();

	void exit();

	void do1();

	//void genVB_FLOAT3(GLuint& vbuffer, const GLfloat* data, int size);
	//
	////(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
	//void bindVB_FLOAT3(GLuint index, GLuint& vbuffer);
}