#pragma once
#include "pch.h"
#include "Camera.h"


namespace RS {

	class Renderer {
	public:
		Renderer() {};
		~Renderer() {};
		virtual void init(GLuint ms = 0);
		virtual void preRender();
		virtual void postRender();
		virtual void exit();
		void setMultisample(GLuint ms);
	protected:
		GLuint multisample = 0;
	};

	class FBRenderer : public Renderer {
	public:
		FBRenderer() {};
		~FBRenderer() {};
		virtual void init(GLuint ms = 0) override;
		void resize(ES::event e);
		virtual void preRender() override;
		virtual void postRender() override;
		virtual void exit() override;

	private:
		GLuint screenVAO, screenVBO;
		GLuint framebuffer;
		GLuint rbo;
		GLuint textureColorbuffer;
		GLuint shaderID;
		GLuint textureID;
	};
		
	class FBRendererMS : public Renderer {
	public:
		FBRendererMS() { multisample = 4; };
		~FBRendererMS() {};
		virtual void init(GLuint ms = 0) override;
		void resize(ES::event e);
		virtual void preRender() override;
		virtual void postRender() override;
		virtual void exit() override;
		void setMultisample(GLuint ms);

	private:
		GLuint screenVAO, screenVBO;
		GLuint framebuffer;
		GLuint screenTexture;
		GLuint textureColorBufferMultiSampled;
		GLuint rbo;
		GLuint intermediateFBO;
		GLuint shaderID;
		GLuint textureID;
	};
}