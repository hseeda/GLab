#include "pch.h"
#include "BodySystem.h"
#include "GLabSystem.h"
#include "WindowSystem.h"
#include "EventSystem.h"
#include "ShaderSystem.h"

using namespace glm;


namespace BS {
	
	// body not build by default
	BS::body* addBody(int shaderIndex /*= 0*/, Camera* icam /*= nullptr*/, int flag1 /*= 0*/)
	{
		bodies.push_back(new BS::defaultBody());
		if(icam !=nullptr) bodies.back()->cam = icam;
		else bodies.back()->cam = BS::camera;
		bodies.back()->shaderNumber = shaderIndex;
		bodies.back()->flag1 = flag1;
		return bodies.back();
	}

	void pre_init()
	{
		camera = new Camera();
	}

	void init() {
		for (auto& b : bodies) {
			b->init();
		}
	}

	void exit() {
		delete camera;
		for (auto b : bodies) {
			b->exit();
			delete b;
		}
	}

	void do1() 
	{
		for (auto b : bodies) {
			b->do1();
		}
	}

	body::body(){}

	body::~body(){}

	void body::build()	{}

	void body::init()
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &CBO);
		glBindBuffer(GL_ARRAY_BUFFER, CBO);
		glBufferData(GL_ARRAY_BUFFER, colorData.size() * sizeof(GLfloat), colorData.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(GLuint), indexData.data(), GL_STATIC_DRAW);

		GLuint shaderID = SS::shaderID(shaderNumber);

		mvpID = glGetUniformLocation(shaderID, "MVP");
		flagID = glGetUniformLocation(shaderID, "FLAG1");
		TextureID = glGetUniformLocation(shaderID, "IMAGE");

		ES::needsUpdate();
	}

	void body::do1()
	{
		//GLuint shaderID = SS::getShaderID(shaderNumber);
		//glUseProgram(shaderID);
		SS::useShader(shaderNumber);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, SS::textureID(textureNumber));
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);

		glUniformMatrix4fv(mvpID, 1, GL_FALSE, &cam->PVmat[0][0]);
		glUniform1i(flagID, flag1);

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
		
		GLint tmp_polygon_mode;
		glGetIntegerv(GL_POLYGON_MODE, &tmp_polygon_mode);
		glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
		
		glDrawElements( 
			GL_TRIANGLES,		// mode
			(GLsizei) indexData.size(),   // count
			GL_UNSIGNED_INT,	// type
			(void*)0			// element array buffer offset
		);

		glPolygonMode(GL_FRONT_AND_BACK, tmp_polygon_mode);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);
	}

	void body::exit()
	{
		glDeleteBuffers		(1, &VBO);
		glDeleteBuffers		(1, &CBO);
		glDeleteBuffers		(1, &IBO);
		glDeleteVertexArrays(1, &VAO);
	}


	void body::addImguiControls(int imguiWID, std::string imguiItemName)
	{
		auto ff = [this](ES::event e) 
		{
			if (e.data.b(0)) this->polygonMode = GL_LINE;
			else this->polygonMode = GL_FILL;
			ES::needsUpdate();
		};
		ES::subscribe(ES::imgui_checkbox, ff, WS::addCheckbox(imguiWID, imguiItemName, false));
	}

	void defaultBody::build()
	{
		GLfloat g_vertex_buffer_data[] = {
		-1.0f,-1.0f,-1.0f,		-1.0f,-1.0f, 1.0f,		-1.0f, 1.0f, 1.0f,		1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,		-1.0f, 1.0f,-1.0f,		1.0f,-1.0f, 1.0f,		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,		1.0f, 1.0f,-1.0f,		1.0f,-1.0f,-1.0f,		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,		-1.0f, 1.0f, 1.0f,		-1.0f, 1.0f,-1.0f,		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,		-1.0f,-1.0f,-1.0f,		-1.0f, 1.0f, 1.0f,		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,		1.0f, 1.0f, 1.0f,		1.0f,-1.0f,-1.0f,		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,		1.0f, 1.0f, 1.0f,		1.0f,-1.0f, 1.0f,		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,		-1.0f, 1.0f,-1.0f,		1.0f, 1.0f, 1.0f,		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f,		-1.0f, 1.0f, 1.0f,		1.0f,-1.0f, 1.0f
		};
		for (int i = 0; i < 108; i++)
			vertexData.push_back(g_vertex_buffer_data[i]);
		
		// One color for each vertex. They were generated randomly.
		static const GLfloat g_color_buffer_data[] = {
			0.583f,  0.771f,  0.014f,			0.609f,  0.115f,  0.436f,			0.327f,  0.483f,  0.844f,			0.822f,  0.569f,  0.201f,
			0.435f,  0.602f,  0.223f,			0.310f,  0.747f,  0.185f,			0.597f,  0.770f,  0.761f,			0.559f,  0.436f,  0.730f,
			0.359f,  0.583f,  0.152f,			0.483f,  0.596f,  0.789f,			0.559f,  0.861f,  0.639f,			0.195f,  0.548f,  0.859f,
			0.014f,  0.184f,  0.576f,			0.771f,  0.328f,  0.970f,			0.406f,  0.615f,  0.116f,			0.676f,  0.977f,  0.133f,
			0.971f,  0.572f,  0.833f,			0.140f,  0.616f,  0.489f,			0.997f,  0.513f,  0.064f,			0.945f,  0.719f,  0.592f,
			0.543f,  0.021f,  0.978f,			0.279f,  0.317f,  0.505f,			0.167f,  0.620f,  0.077f,			0.347f,  0.857f,  0.137f,
			0.055f,  0.953f,  0.042f,			0.714f,  0.505f,  0.345f,			0.783f,  0.290f,  0.734f,			0.722f,  0.645f,  0.174f,
			0.302f,  0.455f,  0.848f,			0.225f,  0.587f,  0.040f,			0.517f,  0.713f,  0.338f,			0.053f,  0.959f,  0.120f,
			0.393f,  0.621f,  0.362f,			0.673f,  0.211f,  0.457f,			0.820f,  0.883f,  0.371f,			0.982f,  0.099f,  0.879f
		};
		for (int i = 0; i < 108; i++)
			colorData.push_back(g_color_buffer_data[i]);
		
		static const GLuint indices[] = { 
			0,1,2,				3,4,5, 				6,7,8,				9,10,11,
			12,13,14,			15,16,17,			18,19,20,			21,22,23,
			24,25,26,			27,28,29,			30,31,32,			33,34,35,
			36,37,38,			39,40,41,			42,43,44,			45,46,47,
			48,49,50,			51,52,53,			54,55,56,			57,58,59,
			60,61,62,			63,64,65,			66,67,68,			69,70,71,
			72,73,74,			75,76,77,			78,79,80,			81,82,83,
			84,85,86,			87,88,89,			90,91,92,			93,94,95,
			96,97,98,			99,100,101,			102,103,104,			105,106,107 };
		
		for (int i = 0; i < 108; i++)
			indexData.push_back(indices[i]);
	}

}