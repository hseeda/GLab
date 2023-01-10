#pragma once
#include "pch.h"
#include "Camera.h"


namespace GS {

	inline GLuint VertexArrayID;
	inline GLuint programID;
	inline GLuint vertexbuffer;
	inline GLuint colorbuffer;
	inline GLuint elementbuffer;

	inline GLuint MatrixID;
	inline GLuint Flag1;


	inline GLuint TextureID;
	inline GLuint Texture;

	inline glm::mat4 MVP;
	inline glm::mat4 cMVP;
	inline Camera* cam;

	inline GLfloat *gg;

	void init();

	void exit();

	void do1();

	void genVB_FLOAT3(GLuint& vbuffer, const GLfloat* data, int size);
	
	//(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
	void bindVB_FLOAT3(GLuint index, GLuint& vbuffer);

	
	class body {
	public:
		GLuint VAO;
		GLuint programID;
		GLuint VBO,CBO;
		GLuint colorbuffer;
		GLuint IBO;
		GLuint mvpID;
		GLuint flagID;
		
		
		body();
		~body();
		void init();
		void do1(int flag = 0);
		void exit();
		
		Camera* cam;

		std::vector<GLfloat> vertexData; // 3 x no. of vertices
		std::vector<GLfloat> colorData;	 // 3 x no. of vertices
		std::vector<GLuint>  indexData;	 // 3 x no. of elements
		
	};

	
}