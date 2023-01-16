#include "pch.h"
#include "WindowSystem.h"
#include "EventSystem.h"
#include "imguiSystem.h"
#include "FileSystem.h"
#include "RenderSystem.h"

namespace IS
{
	int							last_added_imgui_item = -1;	
	bool						standard_windows_initialized = false;
	bool						standard_SLots_initialized = false;
	void*						imgui_subscribe_0 = nullptr;
	void*						imgui_subscribe_1 = nullptr;
	//------------------------------------------------------
	void imguiOFF()
	{
		want_imgui = false;
		show_imgui = false;
		
		WS::fullWidth = WS::leftWidth + WS::Width + WS::rightWidth;
		WS::fullHeight = WS::topHeight + WS::Height + WS::bottomHeight + WS::statusHeight;
		
		WS::Width = WS::fullWidth;
		WS::Height = WS::fullHeight;

		WS::default_bottomHeight = WS::bottomHeight;
		WS::default_topHeight = WS::topHeight;
		WS::default_leftWidth = WS::leftWidth;
		WS::default_rightWidth = WS::rightWidth;
		WS::default_statusHeight = WS::statusHeight;

		WS::bottomHeight = 0;
		WS::topHeight = 0;
		WS::leftWidth = 0;
		WS::rightWidth = 0;
		WS::statusHeight = 0;	
		exit();
		ES::postSkipRepeat(ET::window_resize);
		return;
	}

	void imguiON() {
		want_imgui = true;
		show_imgui = true;
		
		WS::bottomHeight = WS::default_bottomHeight;
		WS::topHeight    = WS::default_topHeight;
		WS::leftWidth    = WS::default_leftWidth;
		WS::rightWidth   = WS::default_rightWidth;
		WS::statusHeight = WS::default_statusHeight;

		WS::Width  = WS::fullWidth - WS::leftWidth - WS::rightWidth;
		WS::Height = WS::fullHeight - WS::topHeight - WS::bottomHeight - WS::statusHeight;

		init();
		ES::postSkipRepeat(ET::window_resize);
		return;
	}
	
	void imguiToggle() {
		if (want_imgui) imguiOFF();
		else imguiON();
	}

	void resizeStandardWindows()
	{
		if (!show_imgui) return;
		if (!standard_windows_initialized) return;
		io = &ImGui::GetIO();

		// Tools Window
		imguiWindows[0].setSize(ImVec2(WS::leftWidth, WS::Height), ImGuiCond_Always);
		imguiWindows[0].setPos(ImVec2(0.0f, WS::topHeight));

		// Log Window
		imguiWindows[1].setSize(ImVec2(WS::fullWidth, WS::bottomHeight), ImGuiCond_Always);
		imguiWindows[1].setPos(ImVec2(0.0f, WS::topHeight + WS::Height));

		// Status Window
		imguiWindows[2].setSize(ImVec2(WS::fullWidth, WS::statusHeight), ImGuiCond_Always);
		imguiWindows[2].setPos(ImVec2(0.0f, WS::fullHeight - WS::statusHeight));
		
		// Menu Window
		imguiWindows[3].setSize(ImVec2(WS::fullWidth, WS::topHeight), ImGuiCond_Always);
		imguiWindows[3].setPos(ImVec2(0.0f, 0.0f));
		
		// GLab Options Window
		imguiWindows[4].setSize(ImVec2(WS::rightWidth, WS::Height), ImGuiCond_Always);
		imguiWindows[4].setPos(ImVec2(WS::leftWidth + WS::Width, WS::topHeight));
	}	

	void initStandardWindows()
	{
		if (standard_windows_initialized) return;
		standard_windows_initialized = true;
		//-------------------------------
		int i = addWindow("Tools"); // =0
		imguiWindows[i].setFlags(ImGuiWindowFlags_NoDecoration);
		imguiWindows[i].setSize(ImVec2(WS::leftWidth, WS::Height), ImGuiCond_Always);
		imguiWindows[i].setPos(ImVec2(0, WS::topHeight));
		addText(i, "Tools");
		addSeparator(i);
		//-------------------------------
		i = addWindow("Log"); // = 1
		imguiWindows[i].setFlags(ImGuiWindowFlags_NoDecoration);
		imguiWindows[i].setSize(ImVec2(WS::fullWidth, WS::bottomHeight), ImGuiCond_Always);
		imguiWindows[i].setPos(ImVec2(0.0f, WS::topHeight + WS::Height));
		addText(i, "Log");
		//-------------------------------
		i = addWindow("Status"); // = 2
		imguiWindows[i].setFlags(ImGuiWindowFlags_NoDecoration);
		imguiWindows[i].setSize(ImVec2(WS::fullWidth, WS::statusHeight), ImGuiCond_Always);
		imguiWindows[i].setPos(ImVec2(0.0f, WS::fullHeight - WS::statusHeight));
		//-------------------------------
		i = addWindow("Menu"); // = 3
		imguiWindows[i].setFlags(ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar);
		imguiWindows[i].setSize(ImVec2(WS::fullWidth, WS::topHeight), ImGuiCond_Always);
		imguiWindows[i].setPos(ImVec2(0.0f, 0.0));
		//-------------------------------
		i = addWindow("GLab Options"); // = 4
		imguiWindows[i].setFlags(ImGuiWindowFlags_NoDecoration);
		imguiWindows[i].setSize(ImVec2(WS::rightWidth, WS::Height), ImGuiCond_Always);
		imguiWindows[i].setPos(ImVec2(WS::leftWidth + WS::Width, WS::topHeight));
		addText(i, "GLab Options");
		addSeparator(i);
	}

	void init() {
		if(!imgui_subscribe_1) imgui_subscribe_1 = ES::subscribe(ET::character, std::bind(imguiToggle), (int)'i');
		if(!imgui_subscribe_0) imgui_subscribe_0 = ES::subscribe(ET::window_resize, std::bind(resizeStandardWindows));
		if (!standard_SLots_initialized) {
			standard_SLots_initialized = true;
			addFunction(std::bind(drawMenu));
			addFunction(std::bind(drawLog));
			addFunction(std::bind(drawStatus));
			//imguiSlots.push_back(std::bind(drawStatus));
			//imguiSlots.push_back(std::bind(plot, &WS::appName));
			addFunction(std::bind(plot, &WS::appName));
		}
		
		if (!want_imgui) {
			imguiOFF();
			return;
		}
		//---------------------------------------------- Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		io = &ImGui::GetIO();
		ImGui_ImplGlfw_InitForOpenGL(WS::window, true);
		ImGui_ImplOpenGL3_Init(WS::glsl_version);
		
		ImGui::StyleColorsDark();
		io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io->Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 24.0f);
		imgui_alive = true;
		
		//ImGui::StyleColorsLight();
		// Load Fonts
		// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
		// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
		// - If the file cannot be loaded, the function will return NULL. Please handle those [ERROR]s in your application (e.g. use an assertion, or display an [ERROR] and quit).
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
		//		auto fcol = [](event e)
		//		{
		//			glClearColor(e.data.f(0), e.data.f(1), e.data.f(2), 1.0f);
		//			ES::needsUpdate();
		//		};
		//		ES::subscribe(ES::imgui_color_edit3, fcol, WS::addColorEdit3("BG color", 0.45f, 0.55f, 0.60f));

		//---------------------------------------------- add imgui windows
		//ImGuiWindowFlags layoutFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;
		
		if (!standard_windows_initialized) initStandardWindows();
		//-----------------------------------------------
		IS::log(0,"[init] IMGui\n");
		return;
	}

	void exit()
	{
		if (implot_alive) {
			ImPlot::DestroyContext();
			implot_alive = false;
		}
		if (imgui_alive) {
			imgui_alive = false;
			show_imgui = false;
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
	}

	void preRender()
	{
		if (!show_imgui || !imgui_alive) return;
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
		for (int i = 0; i < imguiSlots.size(); i++)
		{
			imguiSlots[i].slot(imguiSlots[i].pkt);
		}
		//----------------------------------------------- render imgui
		ImGui::Render();
	}

	void postRender() {
		if (!show_imgui || !imgui_alive) return;
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());		
	}

	int addWindow(std::string name)
	{
		if (!standard_windows_initialized) { log(0, "[ERROR] cannot add window at this time\n"); return -1; }
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
		if (imguiWinID < 0) { log(0,"[ERROR] cannot add imgui item to this window\n"); return -1; }
		auto *iw = &imguiWindows[imguiWinID];
		iw->imguiItems.emplace_back(imguiItem(name.c_str()));
		int n = (int)iw->imguiItems.size() - 1;
		auto* ii = &iw->imguiItems[n];
		ii->windowID = imguiWinID;
		ii->same_line = sameline;
		
		ii->func = [](imguiItem &i) { ES::post(ET::imgui_checkbox, i.data); };
		ii->type = IMGUI_CHECKBOX;
		*(ii->data.bp(0)) = initialState;
	
		return ii->uniqueID();
	}

	int addButton(int imguiWinID, std::string name, bool sameline /*= false */)
	{
		if (imguiWinID < 0) { log(0, "[ERROR] cannot add imgui item to this window\n"); return -1; }
		auto* iw = &imguiWindows[imguiWinID];
		iw->imguiItems.emplace_back(imguiItem(name.c_str()));
		int n = (int)iw->imguiItems.size() - 1;
		auto* ii = &iw->imguiItems[n];
		ii->windowID = imguiWinID;
		ii->same_line = sameline;

		iw->imguiItems[n].func = [](imguiItem &i) { ES::post(ET::imgui_button, i.data); };
		iw->imguiItems[n].type = IMGUI_BUTTON;

		return ii->uniqueID();
	}

	int addSlider(int imguiWinID, std::string name, float value, float start, float end, bool sameline /*= false */)
	{
		if (imguiWinID < 0) { log(0, "[ERROR] cannot add imgui item to this window\n"); return -1; }
		auto* iw = &imguiWindows[imguiWinID];
		iw->imguiItems.emplace_back(imguiItem(name.c_str()));
		int n = (int)iw->imguiItems.size() - 1;
		auto* ii = &iw->imguiItems[n];
		ii->windowID = imguiWinID;
		ii->same_line = sameline;
		
		iw->imguiItems[n].func = [](imguiItem& i) { ES::post(ET::imgui_slider, i.data); };

		iw->imguiItems[n].type = IMGUI_SLIDER;
		*iw->imguiItems[n].data.fp(0) = value;
		*iw->imguiItems[n].data.fp(1) = start;
		*iw->imguiItems[n].data.fp(2) = end;

		return ii->uniqueID();
	}

	int addColorEdit3(int imguiWinID, std::string name, float r /*= 0.1f*/, float g/* = 0.1f*/, float b/* = 0.1f*/, bool sameline /*= false */)
	{
		if (imguiWinID < 0) { log(0, "[ERROR] cannot add imgui item to this window\n"); return -1; }
		auto* iw = &imguiWindows[imguiWinID];
		iw->imguiItems.emplace_back(imguiItem(name.c_str()));
		int n = (int)iw->imguiItems.size() - 1;
		auto* ii = &iw->imguiItems[n];
		ii->windowID = imguiWinID;
		ii->same_line = sameline;
		
		iw->imguiItems[n].func = [](imguiItem& i) { ES::post(ET::imgui_color_edit3, i.data); };
		iw->imguiItems[n].type = IMGUI_COLOR_EDIT3;
		*iw->imguiItems[n].data.fp(0) = r;
		*iw->imguiItems[n].data.fp(1) = g;
		*iw->imguiItems[n].data.fp(2) = b;

		return ii->uniqueID();
	}

	int addText(int imguiWinID, std::string name, bool sameline /*= false */)
	{
		if (imguiWinID < 0) { log(0, "[ERROR] cannot add imgui item to this window\n"); return -1; }
		auto* iw = &imguiWindows[imguiWinID];
		iw->imguiItems.emplace_back(imguiItem(name.c_str()));
		int n = (int)iw->imguiItems.size() - 1;
		auto* ii = &iw->imguiItems[n];
		ii->windowID = imguiWinID;
		ii->same_line = sameline;
		
		iw->imguiItems[n].func = [](imguiItem& i) { ES::post(ET::imgui_text, i.data); };
		iw->imguiItems[n].type = IMGUI_TEXT;

		return ii->uniqueID();
	}

	int addSeparator(int imguiWinID)
	{
		if (imguiWinID < 0) { log(0, "[ERROR] cannot add imgui item to this window\n"); return -1; }
		auto* iw = &imguiWindows[imguiWinID];
		iw->imguiItems.emplace_back(imguiItem(""));
		int n = (int)iw->imguiItems.size() - 1;
		auto* ii = &iw->imguiItems[n];
		ii->windowID = imguiWinID;

		iw->imguiItems[n].type = IMGUI_SEPARATOR;
		return ii->uniqueID();
	}

	imguiSlotPair* addFunction(imguiSlot&& slot, packet &pkt /*= dummyPacket*/)
	{
		//if(pkt.ID == -2)
		//	imguiSlots.push_back(imguiSlotPair(slot, dummyPacket));
		//else
			imguiSlots.push_back(imguiSlotPair(slot, pkt));
		return &imguiSlots.back();
	}

	void drawLog()
	{
		
		bool* p_open = 0;
		ImGui::Begin("Log", p_open);
		ImGui::SameLine();
		if (ImGui::SmallButton("[Debug] Add 5 entries"))
		{
			static int counter = 0;
			const char* categories[3] = { "info", "warn", "[ERROR]" };
			const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
			for (int n = 0; n < 5; n++)
			{
				const char* category = categories[counter % IM_ARRAYSIZE(categories)];
				const char* word = words[counter % IM_ARRAYSIZE(words)];
				logger.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
					ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
				counter++;
			}
		}
		ImGui::End();

		//ImGui::End();
		// Actually call in the regular Log helper (which will Begin() into the same window as we just did)
		logger.Draw("Log", p_open);
	}

	void drawMenu() {
		bool* p_open = 0;
		ImGui::Begin("Menu", p_open);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open"))
				{
					ES::post("open__file");
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

	void drawStatus() {
		bool* p_open = 0;
		ImGui::Begin("Status", p_open);
		ImGui::Text("%8.3f , %8.3f :", WS::xc, WS::yc);
		ImGui::End();
	}

	bool wantCaptureMouse()
	{
		if (!show_imgui || !imgui_alive) return false;
		if (io->WantCaptureMouse) return true;
		return false;
	}

	bool wantCaptureKeyboard()
	{
		if (!show_imgui || !imgui_alive) return false;
		if (io->WantCaptureKeyboard) return true;
		return false;
	}

	void log(int lvl,const char* fmt, ...) IM_FMTARGS(2)
	{
		if (lvl > WS::logLevel) return;
		if (show_imgui) {
			int old_size = logger.Buf.size();
			va_list args;
			va_start(args, fmt);
			logger.Buf.appendfv(fmt, args);
			va_end(args);
			for (int new_size = logger.Buf.size(); old_size < new_size; old_size++)
				if (logger.Buf[old_size] == '\n')
					logger.LineOffsets.push_back(old_size + 1);
		}
		else
		{
			va_list args;
			va_start(args, fmt);
			vprintf(fmt, args);
			va_end(args);
		}
	}

	void plot(std::string *pp) {
		if (!implot_alive) {
			ImPlot::CreateContext();
			implot_alive = true;
		}
		if (ImGui::Begin("imPlot"))
		{
			ImGui::Text(pp->c_str());

			if (ImPlot::BeginPlot("My Plot"))
			{
				static double values[] = { 1., 3., 5. };
				ImPlot::PlotLine("Values", values, 3);
				ImPlot::EndPlot();
			}

#ifdef IMPLOT_INSTANTIATE_ALL_NUMERIC_TYPES
			if (ImPlot::BeginPlot("My Plot (long double)"))
			{
				static long double values[] = { 1., 3., 5. };
				ImPlot::PlotLine("Values", values, 3);
				ImPlot::EndPlot();
			}
#endif
			ImGui::End();
		}
	}
		
}