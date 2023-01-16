#pragma once
#include "pch.h"
#include "Camera.h"


namespace RS {

	class Renderer {
	public:
		Renderer() {};
		~Renderer() {};
		virtual void init();
		virtual void preRender();
		virtual void postRender();
		virtual void resize(CEvent e);
		virtual void exit();
		virtual void setMultisample();
		virtual void toggleMultisample();
	protected:
		GLuint multisample = 0;
	};

	class FBRenderer : public Renderer {
	public:
		FBRenderer() {};
		~FBRenderer() {};
		void init() override;
		void resize(CEvent e) override;
		void preRender() override;
		void postRender() override;
		void exit() override;

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
		void init() override;
		void resize(CEvent e) override;
		void preRender() override;
		void postRender() override;
		void exit() override;
		void setMultisample() override;
		void toggleMultisample() override;

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