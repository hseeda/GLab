#pragma once
#include "pch.h"
#include "EventSystem.h"
#include "RenderSystem.h"

namespace WS
{
	inline int			swapinterval = 1;
	inline int			logLevel = 4;
	inline bool			imgui		= true;
	inline bool			show_imgui  = false;
	inline bool			implot		= false;
	inline bool			show_demo_window = true;
	inline bool			show_another_window = false;
	
	inline std::string	appNameDefault = "GLab Ver. 1.0";
	inline std::string	appName;

	inline float		Width		 = 2000;
	inline float		Height		 = 1300;
	inline float		bottomHeight = 300;
	inline float		topHeight	 = 100;
	inline float		leftWidth	 = 300;
	inline float		rightWidth	 = 300;
	inline float		statusHeight = 45;
	inline float		Aspect;
		
	inline float		default_bottomHeight ;
	inline float		default_topHeight	 ;
	inline float		default_leftWidth	 ;
	inline float		default_rightWidth	 ;
	inline float		default_statusHeight;
	
	inline float		fullWidth;
	inline float		fullHeight;
	inline float		winPosX = 400;
	inline float		winPosY = 200;

	inline const char*  glsl_version = "#version 130";
	inline int			frame_buffer_w, frame_buffer_h;
	inline GLFWwindow*	window;

	inline ImVec4		clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// mouse buttons
	inline double		xd, yd, xu, yu, xc, yc, xc_1, yc_1;
	inline double		deltaX, deltaY;
	inline int			mouse_down = -1;
	inline int			action_down = -1;
	inline int			resizing = 0;
	inline int			mods = 0;
	inline int			action = 0;
	inline int			key = 0;

	static void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void glfw_error_callback(int error, const char* description);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void character_callback(GLFWwindow* window, unsigned int codepoint);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	void init();
	
	void exit();
	
	void preRender();
	
	void postRender();	
	
	void toggleMSAA();
	
	void doWhileResizing();
	
	void setSwapInterval(int interval);
}

