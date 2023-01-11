#include "pch.h"
#include "WindowSystem.h"
#include "EventSystem.h"
#include "TextSystem.h"
#include "FileSystem.h"
#include "RenderSystem.h"

namespace WS
{
	
	int							swapinterval = 0;
	int							last_added_imgui_item = -1;
	
	std::vector<imguiWindow>	imguiWindows;
	inline RS::Renderer*		ren;
	inline ImGuiIO*				io;

	void resizeImgui()
	{
		imguiWindows[0].setSize(ImVec2(leftWidth, Height), ImGuiCond_Always);
		imguiWindows[0].setPos(ImVec2(0, topHeight));
		imguiWindows[1].setSize(ImVec2(fullWidth, bottomHeight - statusHeight), ImGuiCond_Always);
		imguiWindows[1].setPos(ImVec2(0.0f, topHeight + Height));
		imguiWindows[2].setSize(ImVec2(fullWidth, statusHeight), ImGuiCond_Always);
		imguiWindows[2].setPos(ImVec2(0.0f, fullHeight - statusHeight));
		imguiWindows[3].setSize(ImVec2(fullWidth, topHeight), ImGuiCond_Always);
		imguiWindows[3].setPos(ImVec2(0.0f, 0.0f));
		imguiWindows[4].setSize(ImVec2(rightWidth, Height), ImGuiCond_Always);
		imguiWindows[4].setPos(ImVec2(leftWidth + Width, topHeight));
	}

	int init() {
		// Setup window
		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit()) return 1;
		WS::log.AddLog(0,"[init] glfw\n");
		//---------------------------------------------- glfw hints
		const char* glsl_version = "#version 130";
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//---------------------------------------------- set window size
		fullWidth  = leftWidth + Width + rightWidth;
		fullHeight = topHeight + Height + bottomHeight;
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
		if (window == NULL) return 1;
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
		//---------------------------------------------- Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		WS::io = &ImGui::GetIO();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
		
		ImGui::StyleColorsDark();
		io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		WS::io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		WS::io->Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 24.0f);

		//ImGui::StyleColorsLight();
		// Load Fonts
		// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
		// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
		// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
		// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
		// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
		// - Read 'docs/FONTS.md' for more instructions and details.
		// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
		//		io.Fonts->AddFontDefault();
		//		io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
		//		io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
		//		io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
		//		ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
		//		IM_ASSERT(font != NULL);
		//		WS::io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		//		WS::io->ConfigDockingWithShift = true;
		//		auto fcol = [](ES::event e)
		//		{
		//			glClearColor(e.data.f(0), e.data.f(1), e.data.f(2), 1.0f);
		//			ES::needsUpdate();
		//		};
		//		ES::subscribe(ES::imgui_color_edit3, fcol, WS::addColorEdit3("BG color", 0.45f, 0.55f, 0.60f));
		
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
		ren->init(16);
		//---------------------------------------------- add imgui windows
		ImGuiWindowFlags layoutFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;
		//-------------------------------
		int i = addImguiWindow("Tools"); // =0
		imguiWindows[i].setFlags(ImGuiWindowFlags_NoDecoration);
		imguiWindows[i].setSize(ImVec2(leftWidth, Height), ImGuiCond_Always);
		imguiWindows[i].setPos(ImVec2(0, topHeight));
		addText(i, "Tools");
		addSeparator(i);
		//-------------------------------
		i = addImguiWindow("Log"); // = 1
		imguiWindows[i].setFlags(ImGuiWindowFlags_NoDecoration);
		imguiWindows[i].setSize(ImVec2(fullWidth, bottomHeight - statusHeight), ImGuiCond_Always);
		imguiWindows[i].setPos(ImVec2(0.0f, topHeight + Height));
		addText(i, "Log");
		//-------------------------------
		i = addImguiWindow("Status"); // = 2
		imguiWindows[i].setFlags(ImGuiWindowFlags_NoDecoration);
		imguiWindows[i].setSize(ImVec2(fullWidth, statusHeight), ImGuiCond_Always);
		imguiWindows[i].setPos(ImVec2(0.0f, fullHeight - statusHeight));
		//-------------------------------
		i = addImguiWindow("Menu"); // = 3
		imguiWindows[i].setFlags(ImGuiWindowFlags_NoDecoration|ImGuiWindowFlags_MenuBar);
		imguiWindows[i].setSize(ImVec2(fullWidth, topHeight), ImGuiCond_Always);
		imguiWindows[i].setPos(ImVec2(0.0f, 0.0));
		//-------------------------------
		i = addImguiWindow("GLab Options"); // = 4
		imguiWindows[i].setFlags(ImGuiWindowFlags_NoDecoration);
		imguiWindows[i].setSize(ImVec2(rightWidth, Height), ImGuiCond_Always);
		imguiWindows[i].setPos(ImVec2(leftWidth + Width, topHeight));
		addText(i, "GLab Options");
		addSeparator(i);
		//-------------------------------
		auto f = [](ES::event e){	resizeImgui();};
		ES::subscribe(ES::window_resize, f);
		//-----------------------------------------------
		WS::log.AddLog(0,"[init] IMGui\n");
		return 0;
	}

	void exit()
	{
		ren->exit();
		delete(ren);
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwDestroyWindow(WS::window);
		glfwTerminate();
	}

	void preRenderImgui()
	{
		//----------------------------------------------- Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//----------------------------------------------- imgui Demo windows
		if (WS::show_demo_window) ImGui::ShowDemoWindow(&WS::show_demo_window);
		//----------------------------------------------- another windows
		if (0) {
			static float f = 0.0f;
			static int counter = 0;
			ImGui::Begin("GLab Options");							// Create a window called "Hello, world!" and append into it.
			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &WS::show_demo_window);  // Edit bools storing our window open/close state
			if (ImGui::Checkbox("Another Window", &WS::show_another_window))
			{
				_pl(">>> item active");
			}
			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);			// Edit 1 float using a slider from 0.0f to 1.0f
			//ImGui::ColorEdit3("clear color", (float*) &WS::clear_color); // Edit 3 floats representing a color
			if (ImGui::Button("Button")) {							// Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
				_pl(">>> item active");
			}
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}
		//----------------------------------------------- imguiWindos [standard Windows]
		bool b = 0;
		for (int i = 0; i < imguiWindows.size(); i++) {
			auto* iw = &imguiWindows[i];
			if (!iw->show) continue;
			if (iw->sz.x >= 0) ImGui::SetNextWindowSize(iw->sz, iw->cond);
			if (iw->pos.x >= 0) ImGui::SetNextWindowPos(iw->pos, iw->cond);
			ImGui::Begin(iw->name.c_str(), &b, iw->layoutFlags);
			for (int j = 0; j < iw->imguiItems.size(); j++) iw->imguiItems[j].render();
			ImGui::End();
		}
		drawImguiMenu();
		drawImguiLog();
		//----------------------------------------------- render imgui
		ImGui::Render();
	}

	void postRenderImgui() {
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());		
	}
	
	void preRender()
	{
		appName = appNameDefault + _fprint("  average {:3.3f} ms/frame fps = {:<5.0f}",
			1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		glfwSetWindowTitle(WS::window, appName.c_str());
		//GLint pname[4];
		//glGetIntegerv(GL_VIEWPORT, pname);
		ren->preRender();
	}

	void postRender()
	{
		//GLint pname[4];
		//glGetIntegerv(GL_VIEWPORT, pname);
		//glViewport(leftWidth/2, bottomHeight/2, Width, Height);
		ren->postRender();
	}

	int addImguiWindow(std::string name)
	{
		for (int i = 0; i < imguiWindows.size(); i++) {
			if (imguiWindows[i].name == name) {
				return i;
			}
		}
		imguiWindows.emplace_back(imguiWindow(name));
		imguiWindows[imguiWindows.size() - 1].ID = (int) imguiWindows.size() - 1;
		return (int)imguiWindows.size() - 1;
	}

	int addCheckbox(int imguiWinID, std::string name, bool initialState /*= false*/, bool sameline /*= false */)
	{
		auto *iw = &imguiWindows[imguiWinID];
		iw->imguiItems.emplace_back(imguiItem(name.c_str()));
		int n = (int)iw->imguiItems.size() - 1;
		iw->imguiItems[n].data.ID = n;
		iw->imguiItems[n].wID = imguiWinID;
		iw->imguiItems[n].same_line = sameline;
		
		iw->imguiItems[n].func = [](WS::imguiItem &i) { ES::post(ES::imgui_checkbox, i.data); };
		iw->imguiItems[n].type = IMGUI_CHECKBOX;
		*(iw->imguiItems[n].data.bp(0)) = initialState;
	
		return n;
	}

	int addButton(int imguiWinID, std::string name, bool sameline /*= false */)
	{
		auto* iw = &imguiWindows[imguiWinID];
		iw->imguiItems.emplace_back(imguiItem(name.c_str()));
		int n = (int)iw->imguiItems.size() - 1;
		iw->imguiItems[n].data.ID = n;
		iw->imguiItems[n].wID = imguiWinID;
		iw->imguiItems[n].same_line = sameline;

		iw->imguiItems[n].func = [](WS::imguiItem &i) { ES::post(ES::imgui_button, i.data); };
		iw->imguiItems[n].type = IMGUI_BUTTON;

		return n;
	}

	int addSlider(int imguiWinID, std::string name, float value, float start, float end, bool sameline /*= false */)
	{
		auto* iw = &imguiWindows[imguiWinID];
		iw->imguiItems.emplace_back(imguiItem(name.c_str()));
		int n = (int)iw->imguiItems.size() - 1;
		iw->imguiItems[n].data.ID = n;
		iw->imguiItems[n].wID = imguiWinID;
		iw->imguiItems[n].same_line = sameline;
		
		iw->imguiItems[n].func = [](WS::imguiItem& i) { ES::post(ES::imgui_slider, i.data); };

		iw->imguiItems[n].type = IMGUI_SLIDER;
		*iw->imguiItems[n].data.fp(0) = value;
		*iw->imguiItems[n].data.fp(1) = start;
		*iw->imguiItems[n].data.fp(2) = end;

		return n;
	}

	int addColorEdit3(int imguiWinID, std::string name, float r /*= 0.1f*/, float g/* = 0.1f*/, float b/* = 0.1f*/, bool sameline /*= false */)
	{
		auto* iw = &imguiWindows[imguiWinID];
		iw->imguiItems.emplace_back(imguiItem(name.c_str()));
		int n = (int)iw->imguiItems.size() - 1;
		iw->imguiItems[n].data.ID = n;
		iw->imguiItems[n].wID = imguiWinID;
		iw->imguiItems[n].same_line = sameline;
		
		iw->imguiItems[n].func = [](WS::imguiItem& i) { ES::post(ES::imgui_color_edit3, i.data); };
		iw->imguiItems[n].type = IMGUI_COLOR_EDIT3;
		*iw->imguiItems[n].data.fp(0) = r;
		*iw->imguiItems[n].data.fp(1) = g;
		*iw->imguiItems[n].data.fp(2) = b;

		return n;
	}

	int addText(int imguiWinID, std::string name, bool sameline /*= false */)
	{
		auto* iw = &imguiWindows[imguiWinID];
		iw->imguiItems.emplace_back(imguiItem(name.c_str()));
		int n = (int)iw->imguiItems.size() - 1;
		iw->imguiItems[n].data.ID = n;
		iw->imguiItems[n].wID = imguiWinID;
		iw->imguiItems[n].same_line = sameline;
		
		iw->imguiItems[n].func = [](WS::imguiItem& i) { ES::post(ES::imgui_text, i.data); };
		iw->imguiItems[n].type = IMGUI_TEXT;

		return n;
	}

	int addSeparator(int imguiWinID)
	{
		auto* iw = &imguiWindows[imguiWinID];
		iw->imguiItems.emplace_back(imguiItem(""));
		int n = (int)iw->imguiItems.size() - 1;
		iw->imguiItems[n].data.ID = n;
		iw->imguiItems[n].wID = imguiWinID;

		iw->imguiItems[n].type = IMGUI_SEPARATOR;
		return n;
	}

	void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		fullWidth  = width;
		fullHeight = height;
		Width  = fullWidth  - leftWidth    - rightWidth;
		Height = fullHeight - bottomHeight - topHeight;
		Aspect = (float)Width / (float)Height;
		
		glfwGetFramebufferSize(WS::window, &frame_buffer_w, &frame_buffer_h);
		
		//_pl(">>> resizing window to " , Width , "x" , Height , " (" , frame_buffer_w , "x" , frame_buffer_h , ")");
		WS::io = &ImGui::GetIO();
		resizing = 1;

		WS::doWhileResizing();
		ES::needsUpdate();
		ES::postReplaceRepeat(ES::event(ES::ET::window_resize));

	}
	
	static void glfw_error_callback(int error, const char* description)
	{
		fprintf(stderr, "Glfw Error %d: %s\n", error, description);
		ES::dispatch(ES::event(ES::ET::glfw_error));
	}

	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (WS::io->WantCaptureMouse) return;
		if (mouse_down < 0) return;
		xc_1 = xc;
		yc_1 = yc;
		xc = xpos - leftWidth;
		yc = ypos - topHeight ;
		deltaX = xc - xd;
		deltaY = yc - yd;
		ES::post(ES::event(ES::ET::mouse_move_while_pressed, deltaX, deltaY));
	}

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
		//_pl("mouse_button_callback: button ", button, " action ", action, " mods ", mods);

		if (WS::io->WantCaptureMouse) return;
			
		if (action == GLFW_PRESS) {
			glfwGetCursorPos(window, &xd, &yd);
			_pl(xd, " ", yd);
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
			//	ES::dispatch(ES::event(2, Width, Height));
			//}
		}
			ES::post(ES::event(ES::ET::mouse_click, (void*)window, button, action, mods));
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (WS::io->WantCaptureKeyboard) return;
		//_pl("key_callback: key ", key, " scancode ", scancode, " action ", action, " mods ", mods);
		WS::action = action;
		WS::mods = mods;
		WS::key = key;
	}

	static void character_callback(GLFWwindow* window, unsigned int codepoint) {
		_pl("character_callback: codepoint ", codepoint);
	}

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
		if (WS::io->WantCaptureMouse) return;
		//_pl("scroll_callback: xoffset ", xoffset, " yoffset ", yoffset); 
		//cam->Move(vec3(0.0, 0.0, yoffset * 0.1f));

		ImGuiIO& io = ImGui::GetIO();
		if (!io.WantCaptureMouse)
			ES::dispatch(ES::event(ES::ET::mouse_wheel, xoffset, yoffset));
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

	void drawImguiLog()
	{
		
		bool* p_open = 0;
		ImGui::Begin("Log", p_open);
		ImGui::SameLine();
		if (ImGui::SmallButton("[Debug] Add 5 entries"))
		{
			static int counter = 0;
			const char* categories[3] = { "info", "warn", "error" };
			const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
			for (int n = 0; n < 5; n++)
			{
				const char* category = categories[counter % IM_ARRAYSIZE(categories)];
				const char* word = words[counter % IM_ARRAYSIZE(words)];
				log.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
					ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
				counter++;
			}
		}
		ImGui::End();

		//ImGui::End();
		// Actually call in the regular Log helper (which will Begin() into the same window as we just did)
		log.Draw("Log", p_open);
	}

	void drawImguiMenu() {
		bool* p_open = 0;
		ImGui::Begin("Menu", p_open);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::BeginMenu("Open"))
				{
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Exit", "Ctrl+F4")) 
				{
					glfwSetWindowShouldClose(WS::window, GL_TRUE);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}
}