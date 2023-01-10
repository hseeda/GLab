#include "pch.h"
#include "GLabSystem.h"
#include "WindowSystem.h"
#include "EventSystem.h"
#include "ShaderSystem.h"
#include "camera.h"

namespace GS {
	
	GLfloat g_vertex_buffer_data[] = {
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	 1.0f, 1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	 1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	 1.0f,-1.0f,-1.0f,
	 1.0f, 1.0f,-1.0f,
	 1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	 1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	 1.0f,-1.0f, 1.0f,
	 1.0f, 1.0f, 1.0f,
	 1.0f,-1.0f,-1.0f,
	 1.0f, 1.0f,-1.0f,
	 1.0f,-1.0f,-1.0f,
	 1.0f, 1.0f, 1.0f,
	 1.0f,-1.0f, 1.0f,
	 1.0f, 1.0f, 1.0f,
	 1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	 1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	 1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	 1.0f,-1.0f, 1.0f
	};
	
	void init() {

		gg = &g_vertex_buffer_data[0];

		cam = new Camera();

		// Dark blue background
		//glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);
		
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		// Create and compile our GLSL program from the shaders
		programID = SS::LoadShaders("..\\Misc\\Shaders\\HVertexShader.vs", "..\\Misc\\Shaders\\HFragmentShader.fs");

		// Get a handle for our "MVP" uniform
		MatrixID = glGetUniformLocation(programID, "MVP");
		Flag1 = glGetUniformLocation(programID, "FLAG1");


		const char* tn = "..\\Misc\\images\\matlab.png";
		Texture = SOIL_load_OGL_texture(tn, 4, 0, SOIL_FLAG_TEXTURE_REPEATS);
		TextureID = glGetUniformLocation(programID, "myTextureSampler");


		// One color for each vertex. They were generated randomly.
		static const GLfloat g_color_buffer_data[] = {
			0.583f,  0.771f,  0.014f,
			0.609f,  0.115f,  0.436f,
			0.327f,  0.483f,  0.844f,
			0.822f,  0.569f,  0.201f,
			0.435f,  0.602f,  0.223f,
			0.310f,  0.747f,  0.185f,
			0.597f,  0.770f,  0.761f,
			0.559f,  0.436f,  0.730f,
			0.359f,  0.583f,  0.152f,
			0.483f,  0.596f,  0.789f,
			0.559f,  0.861f,  0.639f,
			0.195f,  0.548f,  0.859f,
			0.014f,  0.184f,  0.576f,
			0.771f,  0.328f,  0.970f,
			0.406f,  0.615f,  0.116f,
			0.676f,  0.977f,  0.133f,
			0.971f,  0.572f,  0.833f,
			0.140f,  0.616f,  0.489f,
			0.997f,  0.513f,  0.064f,
			0.945f,  0.719f,  0.592f,
			0.543f,  0.021f,  0.978f,
			0.279f,  0.317f,  0.505f,
			0.167f,  0.620f,  0.077f,
			0.347f,  0.857f,  0.137f,
			0.055f,  0.953f,  0.042f,
			0.714f,  0.505f,  0.345f,
			0.783f,  0.290f,  0.734f,
			0.722f,  0.645f,  0.174f,
			0.302f,  0.455f,  0.848f,
			0.225f,  0.587f,  0.040f,
			0.517f,  0.713f,  0.338f,
			0.053f,  0.959f,  0.120f,
			0.393f,  0.621f,  0.362f,
			0.673f,  0.211f,  0.457f,
			0.820f,  0.883f,  0.371f,
			0.982f,  0.099f,  0.879f
		};

		static const GLuint indices[] = { 0,1,2,
3,4,5,
6,7,8,
9,10,11,
12,13,14,
15,16,17,
18,19,20,
21,22,23,
24,25,26,
27,28,29,
30,31,32,
33,34,35,
36,37,38,
39,40,41,
42,43,44,
45,46,47,
48,49,50,
51,52,53,
54,55,56,
57,58,59,
60,61,62,
63,64,65,
66,67,68,
69,70,71,
72,73,74,
75,76,77,
78,79,80,
81,82,83,
84,85,86,
87,88,89,
90,91,92,
93,94,95,
96,97,98,
99,100,101,
102,103,104,
105,106,107 };
		//
		//glGenBuffers(1, &vertexbuffer);
		//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

		genVB_FLOAT3(vertexbuffer, g_vertex_buffer_data, sizeof(g_vertex_buffer_data));

		//glGenBuffers(1, &colorbuffer);
		//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

		genVB_FLOAT3(colorbuffer, g_color_buffer_data, sizeof(g_color_buffer_data));

		// Generate a buffer for the indices as well
		glGenBuffers(1, &elementbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicsize() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		ES::needsUpdate();
	}

	void exit() {
		delete cam;
		// Cleanup VBO
		glDeleteBuffers(1, &vertexbuffer);
		glDeleteBuffers(1, &colorbuffer);
		glDeleteBuffers(1, &elementbuffer);
		
		glDeleteProgram(programID);
		glDeleteTextures(1, &Texture);
		glDeleteVertexArrays(1, &VertexArrayID);
	}

	void do1() {



		// Use our shader
		glUseProgram(programID);


		cMVP = cam->PVmat;


		float flag = 0.0f;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &cMVP[0][0]);
		glUniform1fv(Flag1, 1, &flag);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);

		glBindVertexArray(VertexArrayID);

		//// 1rst attribute buffer : vertices
		//glEnableVertexAttribArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		//glVertexAttribPointer(
		//	0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		//	3,                  // size
		//	GL_FLOAT,           // type
		//	GL_FALSE,           // normalized?
		//	0,                  // stride
		//	(void*)0            // array buffer offset
		//);

		bindVB_FLOAT3(0, vertexbuffer);
		
		//// 2nd attribute buffer : colors
		//glEnableVertexAttribArray(1);
		//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		//glVertexAttribPointer(
		//	1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		//	3,                                // size
		//	GL_FLOAT,                         // type
		//	GL_FALSE,                         // normalized?
		//	0,                                // stride
		//	(void*)0                          // array buffer offset
		//);

		bindVB_FLOAT3(1, colorbuffer);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

		// Draw the triangles !
		glDrawElements(
			GL_TRIANGLES,      // mode
			//indices.size(),    // count
			36,    // count
			GL_UNSIGNED_INT,   // type
			(void*)0           // element array buffer offset
		);


		//// Draw the triangle !
		//glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

	}

	void genVB_FLOAT3(GLuint& vbuffer, const GLfloat* data, int size) {
		glGenBuffers(1, &vbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
	//(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
	void bindVB_FLOAT3(GLuint index, GLuint& vbuffer) {
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(index);
		glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
		glVertexAttribPointer(
			index,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,					// size
			GL_FLOAT,				// type
			GL_FALSE,				// normalized?
			0,                 // stride
			(void*)0				// array buffer offset
		);

	}

	body::body(){}

	body::~body(){}

	void body::init()
	{

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		
		programID = SS::LoadShaders("..\\misc\\Shaders\\o1.vs", "..\\misc\\Shaders\\o1.fs");

		mvpID	= glGetUniformLocation	(programID, "MVP");
		flagID	= glGetUniformLocation	(programID, "FLAG1");

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);


		glGenBuffers(1, &CBO);
		glBindBuffer(GL_ARRAY_BUFFER, CBO);
		glBufferData(GL_ARRAY_BUFFER, colorData.size() * sizeof(GLfloat), colorData.data(), GL_STATIC_DRAW);


		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(GLuint), indexData.data(), GL_STATIC_DRAW);
		ES::needsUpdate();
	}

	void body::do1(int flag)
	{
		glUseProgram(programID);
		cMVP = this->cam->PVmat;
		glUniformMatrix4fv(mvpID, 1, GL_FALSE, &cMVP[0][0]);
		glUniform1i(flagID, flag);

		glBindVertexArray(VAO);

		//// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, CBO);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements( 
			GL_TRIANGLES,		// mode
			(GLsizei) indexData.size(),   // count
			GL_UNSIGNED_INT,	// type
			(void*)0			// element array buffer offset
		);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);
	}

	void body::exit()
	{
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &CBO);
		glDeleteBuffers(1, &IBO);
		glDeleteProgram(programID);
		glDeleteTextures(1, &Texture);
		glDeleteVertexArrays(1, &VAO);
	}

}