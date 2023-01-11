#include "pch.h"
#include "RenderSystem.h"
#include "WindowSystem.h"
#include "EventSystem.h"
#include "ShaderSystem.h"
#include "camera.h"

using namespace glm;

namespace RS {
	void Renderer::init(GLuint ms /*= 0*/)
	{
		multisample = ms;
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		if (multisample > 0) {
			glfwWindowHint(GLFW_SAMPLES, multisample);
			glEnable(GL_MULTISAMPLE);
		}
		glViewport(WS::leftWidth, WS::bottomHeight, WS::Width, WS::Height);
		ES::needsUpdate();
	}
	void Renderer::preRender()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void Renderer::postRender()
	{
		ES::needsUpdate();
	}
	void Renderer::exit()
	{

	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	void FBRenderer::init(GLuint ms /*= 0*/)
	{
		ES::subscribe(ES::ET::window_resize, std::bind(&FBRenderer::resize, this, std::placeholders::_1));

		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 1.0f, -1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f,  0.0f, 1.0f, 1.0f, -1.0f,  1.0f, 0.0f,  1.0f,  1.0f,  1.0f, 1.0f
		};

		std::string vs = "#version 330 core\n";
		vs.append("layout(location = 0) in vec2 aPos; \n");
		vs.append("layout(location = 1) in vec2 aTexCoords; \n");
		vs.append("out vec2 TexCoords; \n");
		vs.append("void main() { \n");
		vs.append("TexCoords = aTexCoords; \n");
		vs.append("gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); \n");
		vs.append("} \n");
		vs.append(" \n");
		
		std::string fs = "#version 330 core\n";
		fs.append("out vec4 FragColor; \n");
		fs.append("in vec2 TexCoords; \n");
		fs.append("uniform sampler2D screenTexture; \n");
		fs.append("void main(){ \n");
		fs.append("vec3 col = texture(screenTexture, TexCoords).rgb; \n");
		fs.append("FragColor = vec4(col, 1.0); \n");
		fs.append("} \n");
		fs.append(" \n");
		fs.append(" \n");

		shaderID = SS::LoadShaderFromString(vs, fs);

		glUseProgram(shaderID);
		textureID = glGetUniformLocation(shaderID, "screenTexture");
		
		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);
		//glEnable(GL_CULL_FACE);
		// screen quad VAO
		glGenVertexArrays(1, &screenVAO);
		glGenBuffers(1, &screenVBO);
		glBindVertexArray(screenVAO);
		glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		
		//------------------------------------------------------------------------------- used for rezizing
		// framebuffer configuration
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		// create a color attachment texture
		glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WS::Width, WS::Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);

		// use a single renderbuffer object for both a depth AND stencil buffer.
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WS::Width, WS::Height);
		// now actually attach it
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			_pl("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//------------------------------------------------------------------------------- used for rezizing
		ES::needsUpdate();
	}
	void FBRenderer::resize(ES::event e) {
		glDeleteRenderbuffers(1, &rbo);
		glDeleteTextures(1, &textureColorbuffer);
		glDeleteFramebuffers(1, &framebuffer);

		// framebuffer configuration
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		// create a color attachment texture
		glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WS::Width, WS::Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);

		// use a single renderbuffer object for both a depth AND stencil buffer.
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WS::Width, WS::Height);
		// now actually attach it
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			_pl("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//_pl(">>> Frame buffer resized");

		glUseProgram(shaderID);
		textureID = glGetUniformLocation(shaderID, "screenTexture");
		
		ES::needsUpdate();
	}
	void FBRenderer::preRender()
	{
		glViewport(0,0, WS::Width, WS::Height);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		// enable depth testing (is disabled for rendering screen-space quad)
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void FBRenderer::postRender()
	{
		glViewport(WS::leftWidth, WS::bottomHeight, WS::Width, WS::Height);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		// clear all relevant buffers
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shaderID);
		glActiveTexture(GL_TEXTURE0);	
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
		glUniform1i(textureID, 0);
		
		glBindVertexArray(screenVAO);
		glBindBuffer(GL_ARRAY_BUFFER, screenVBO);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);
		
	}
	void FBRenderer::exit()
	{
		glDeleteProgram(shaderID);
		glDeleteVertexArrays(1, &screenVAO);
		glDeleteBuffers(1, &screenVBO);
		glDeleteRenderbuffers(1, &rbo);
		glDeleteTextures(1, &textureColorbuffer);
		glDeleteFramebuffers(1, &framebuffer);
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	void FBRendererMS::init(GLuint ms /*= 0*/)
	{
		multisample = ms;
		ES::subscribe(ES::ET::window_resize, std::bind(&FBRendererMS::resize, this, std::placeholders::_1));

		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 1.0f, -1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f,  0.0f, 1.0f, 1.0f, -1.0f,  1.0f, 0.0f,  1.0f,  1.0f,  1.0f, 1.0f
		};

		std::string vs = "#version 330 core\n";
		vs.append("layout(location = 0) in vec2 aPos; \n");
		vs.append("layout(location = 1) in vec2 aTexCoords; \n");
		vs.append("out vec2 TexCoords; \n");
		vs.append("void main() { \n");
		vs.append("TexCoords = aTexCoords; \n");
		vs.append("gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); \n");
		vs.append("} \n");
		vs.append(" \n");

		std::string fs = "#version 330 core\n";
		fs.append("out vec4 FragColor; \n");
		fs.append("in vec2 TexCoords; \n");
		fs.append("uniform sampler2D screenTexture; \n");
		fs.append("void main(){ \n");
		fs.append("vec3 col = texture(screenTexture, TexCoords).rgb; \n");
		fs.append("FragColor = vec4(col, 1.0); \n");
		fs.append("} \n");
		fs.append(" \n");
		fs.append(" \n");

		shaderID = SS::LoadShaderFromString(vs, fs);

		glUseProgram(shaderID);
		textureID = glGetUniformLocation(shaderID, "screenTexture");

		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);
		//glEnable(GL_CULL_FACE);

		// screen quad VAO
		glGenVertexArrays(1, &screenVAO);
		glGenBuffers(1, &screenVBO);
		glBindVertexArray(screenVAO);
		glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		//------------------------------------------------------------------------------- used for rezizing
		// framebuffer configuration
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		// create a color attachment texture
		glGenTextures(1, &textureColorBufferMultiSampled);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, multisample, GL_RGB, WS::Width, WS::Height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);

		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, multisample, GL_DEPTH24_STENCIL8, WS::Width, WS::Height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			_pl("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// configure second post-processing framebuffer
		glGenFramebuffers(1, &intermediateFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
		// create a color attachment texture

		glGenTextures(1, &screenTexture);
		glBindTexture(GL_TEXTURE_2D, screenTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WS::Width, WS::Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);	// we only need a color buffer

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			_pl("ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" );
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//------------------------------------------------------------------------------- used for rezizing
		ES::needsUpdate();
	}
	void FBRendererMS::resize(ES::event e) {
		glDeleteRenderbuffers(1, &rbo);
		glDeleteTextures(1, &textureColorBufferMultiSampled);
		glDeleteTextures(1, &screenTexture);
		glDeleteFramebuffers(1, &intermediateFBO);
		glDeleteFramebuffers(1, &framebuffer);
		//------------------------------------------------------------------------------- used for rezizing
// framebuffer configuration
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		// create a color attachment texture
		glGenTextures(1, &textureColorBufferMultiSampled);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, WS::Width, WS::Height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);

		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, WS::Width, WS::Height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			_pl("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// configure second post-processing framebuffer
		glGenFramebuffers(1, &intermediateFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
		// create a color attachment texture

		glGenTextures(1, &screenTexture);
		glBindTexture(GL_TEXTURE_2D, screenTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WS::Width, WS::Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);	// we only need a color buffer

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			_pl("ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glUseProgram(shaderID);
		textureID = glGetUniformLocation(shaderID, "screenTexture");
		
		ES::needsUpdate();
	}
	void FBRendererMS::preRender()
	{
		glViewport(0,0, WS::Width, WS::Height);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		// enable depth testing (is disabled for rendering screen-space quad)
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void FBRendererMS::postRender()
	{
		glViewport(WS::leftWidth, WS::bottomHeight, WS::Width, WS::Height);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// 2. now blit multisampled buffer(s) to normal colorbuffer of intermediate FBO. Image is stored in screenTexture
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
		glBlitFramebuffer(0, 0, WS::Width, WS::Height, 0, 0, WS::Width, WS::Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		//glBlitFramebuffer(0, 0, WS::Width, WS::Height, WS::leftWidth, WS::bottomHeight, WS::Width, WS::Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		//glBlitFramebuffer( 0, 0, WS::Width, WS::Height, WS::leftWidth, WS::bottomHeight, WS::fullWidth, WS::fullHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		
		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		// clear all relevant buffers
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, screenTexture);	// use the color attachment texture as the texture of the quad plane
		glUniform1i(textureID, 0);

		glBindVertexArray(screenVAO);
		glBindBuffer(GL_ARRAY_BUFFER, screenVBO);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);
	}
	void FBRendererMS::exit()
	{
		glDeleteProgram(shaderID);
		glDeleteVertexArrays(1, &screenVAO);
		glDeleteBuffers(1, &screenVBO);
		glDeleteRenderbuffers(1, &rbo);
		glDeleteTextures(1, &textureColorBufferMultiSampled);
		glDeleteTextures(1, &screenTexture);
		glDeleteFramebuffers(1, &intermediateFBO);
		glDeleteFramebuffers(1, &framebuffer);
		
	}
	void FBRendererMS::setMultisample(GLuint ms)
	{
		multisample = ms;
		resize(ES::event());
	}
}