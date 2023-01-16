#include "pch.h"
#include "WindowSystem.h"
#include "imguiSystem.h"
#include "EventSystem.h"
#include "TextSystem.h"
#include "FileSystem.h"
#include "RenderSystem.h"

namespace WS
{
	inline RS::Renderer*		ren;

	void init() {
		// Setup window
		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit()) return;
		//IS::log.AddLog(0,"[init] glfw\n");
		//---------------------------------------------- glfw hints
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//---------------------------------------------- set window size
		fullWidth  = leftWidth + Width + rightWidth;
		fullHeight = topHeight + Height + bottomHeight + statusHeight;
		Aspect     = (float)Width / (float)Height;
		default_bottomHeight	= bottomHeight;
		default_rightWidth		= rightWidth;
		default_leftWidth		= leftWidth;
		default_topHeight		= topHeight;
		default_statusHeight	= statusHeight;
		//---------------------------------------------- app name
		appName = appNameDefault;
		//---------------------------------------------- Create window with graphics context
		window = glfwCreateWindow(fullWidth, fullHeight, appName.c_str(), NULL, NULL);
		if (window == NULL) return;
		glfwSetWindowPos(window, winPosX, winPosY);
		//---------------------------------------------- set window icon
		std::string icon1 = FS::_icon_path + "icon_Blueprint_Find_512px.png";
		std::string icon2 = FS::_icon_path + "icon_Blueprint_Find_40px.png";
		GLFWimage icons[2];
		icons[0].pixels = SOIL_load_image(icon1.c_str(), &icons[0].width, &icons[0].height, 0, SOIL_LOAD_RGBA);
		icons[1].pixels = SOIL_load_image(icon1.c_str(), &icons[1].width, &icons[1].height, 0, SOIL_LOAD_RGBA);
		glfwSetWindowIcon(window, 2, icons);
		SOIL_free_image_data(icons[0].pixels);
		SOIL_free_image_data(icons[1].pixels);
		//images[0].pixels = stbi_load("PATH", &images[0].width, &images[0].height, 0, 4); //rgba channels 
		//stbi_image_free(images[0].pixels);

		//---------------------------------------------- set window icon
		glfwMakeContextCurrent        (window);
		glfwSwapInterval              (swapinterval); // Disable vsync
		glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);
		glfwSetInputMode              (window, GLFW_STICKY_KEYS, GL_TRUE);
		glfwSetCursorPosCallback      (window, cursor_position_callback);
		glfwSetMouseButtonCallback    (window, mouse_button_callback);
		glfwSetKeyCallback            (window, key_callback);
		glfwSetCharCallback           (window, character_callback);
		glfwSetScrollCallback         (window, scroll_callback);
		glfwGetFramebufferSize        (WS::window, &frame_buffer_w, &frame_buffer_h);
		//---------------------------------------------- load glad
		gladLoadGL();		
		//---------------------------------------------- openGL Cleqar color
		glClearColor(0.45f, 0.55f, 0.60f, 1.0f);
		glfwPollEvents();
		printf(">>> OpenGL version supported by this platform (%s): \n",glGetString(GL_VERSION));
		//---------------------------------------------- Setup Render System
		
		//glViewport(leftWidth, bottomHeight, WS::Width, WS::Height);
		//renderer.setMultisample(4);
		//RS::Renderer renderer;
		//ren = new RS::Renderer();
		ren = new RS::FBRendererMS();
		ren->init();
		return;
	}



	void exit()
	{
		ren->exit();
		delete(ren);
		glfwDestroyWindow(WS::window);
		glfwTerminate();
	}

	void preRender()
	{
		if(IS::imgui_alive)
		appName = appNameDefault + _fprint("  average {:3.3f} ms/frame fps = {:<5.0f}",
			1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		glfwSetWindowTitle(WS::window, appName.c_str());
		ren->preRender();
	}

	void postRender()
	{
		//GLint pname[4];
		//glGetIntegerv(GL_VIEWPORT, pname);
		//glViewport(leftWidth/2, bottomHeight/2, Width, Height);
		ren->postRender();
	}

	void toggleMSAA()
	{
		ren->toggleMultisample();
	}

	void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		fullWidth  = width;
		fullHeight = height;
		Width  = fullWidth  - leftWidth    - rightWidth;
		Height = fullHeight - bottomHeight - topHeight - statusHeight;
		Aspect = (float)Width / (float)Height;
		
		glfwGetFramebufferSize(WS::window, &frame_buffer_w, &frame_buffer_h);
		
		//_pl(">>> resizing window to " , Width , "x" , Height , " (" , frame_buffer_w , "x" , frame_buffer_h , ")");
		resizing = 1;

		WS::doWhileResizing();
		ES::needsUpdate();
		ES::postSkipRepeat(ET::window_resize);

	}
	
	static void glfw_error_callback(int error, const char* description)
	{
		fprintf(stderr, "Glfw Error %d: %s\n", error, description);
		ES::dispatch(CEvent(ET::glfw_error));
	}

	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (IS::wantCaptureMouse()) return;
		if (mouse_down < 0) return;
		xc_1 = xc;
		yc_1 = yc;
		xc = xpos - leftWidth;
		yc = ypos - topHeight ;
		deltaX = xc - xd;
		deltaY = yc - yd;
		ES::post(CEvent(ET::mouse_move_while_pressed, deltaX, deltaY));
	}

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
		//_pl("mouse_button_callback: button ", button, " action ", action, " mods ", mods);

		if (IS::wantCaptureMouse()) return;
		
		if (action == GLFW_PRESS) {
			glfwGetCursorPos(window, &xd, &yd);
			//_pl(xd, " ", yd);
			xd -= leftWidth;
			yd -= topHeight;
			if (xd < 0 || yd < 0) return;
			xc_1 = xd;
			yc_1 = yd;
			xc = xd;
			yc = yd;
			mouse_down = button;
			action_down = action;
		}
		else if (action == GLFW_RELEASE) {
			glfwGetCursorPos(window, &xu, &yu);
			xu -= leftWidth;
			yu -= topHeight;
			if (xu < 0 || yu < 0) return;
			xc = xu;
			yc = yu;
			xd = xu;
			yd = yu;
			xc_1 = xu;
			yc_1 = yu;
			mouse_down = -1;
			action_down = -1;
			//if (resizing) {
			//	resizing = 0;
			//	_pl(">>> window resized");
			//	ES::dispatch(event(2, Width, Height));
			//}
		}
			ES::post(CEvent(ET::mouse_click, (void*)window, button, action, mods));
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (IS::wantCaptureKeyboard()) return;
		//_pl("key_callback: key ", key, " scancode ", scancode, " action ", action, " mods ", mods);
		WS::action = action;
		WS::mods = mods;
		WS::key = key;
	}

	static void character_callback(GLFWwindow* window, unsigned int codepoint) {
		//_pl("character_callback: codepoint ", codepoint);
		ES::post(ET::character, (int) codepoint);
	}

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
		if (IS::wantCaptureMouse()) return;
		//_pl("scroll_callback: xoffset ", xoffset, " yoffset ", yoffset); 
		//cam->Move(vec3(0.0, 0.0, yoffset * 0.1f));
			ES::post(CEvent(ET::mouse_wheel, xoffset, yoffset));
	}

	void doWhileResizing() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//resizeImgui();
		//WS::preRenderOverlay();
		//TS::RenderText("GLAB Rev 0.1 (C)", 25.0f, 25.0f, 1.0f, glm::vec3(0.3, 0.3f, 0.3f));
		//WS::postRenderOverlay();
		glfwSwapBuffers(WS::window);
	}

	void setSwapInterval(int interval)
	{
		swapinterval = interval;
		glfwSwapInterval(swapinterval); // Disable vsync
	}
}