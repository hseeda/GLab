#pragma once
#include "pch.h"
#include "EventSystem.h"
#include "RenderSystem.h"


namespace WS
{
#pragma region ImGui_Stuff
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	enum imguiType {
		IMGUI_NONE,
		IMGUI_WINDOW,
		IMGUI_BUTTON,
		IMGUI_CHECKBOX,
		IMGUI_SLIDER,
		IMGUI_COLOR_EDIT3,
		IMGUI_INPUT,
		IMGUI_TEXT,
		IMGUI_SEPARATOR,
		IMGUI_COLOR,
		IMGUI_COMBO,
		IMGUI_LISTBOX,
		IMGUI_TREE,
		IMGUI_TAB,
		IMGUI_TABBAR,
		IMGUI_TABCONTENT,
		IMGUI_TABCONTENTEND,
		IMGUI_TABEND,
		IMGUI_TREEEND,
		IMGUI_LISTBOXEND,
		IMGUI_COMBOEND,
		IMGUI_COLOREND,
		IMGUI_TEXTEND,
		IMGUI_INPUTEND,
		IMGUI_SLIDEREND,
		IMGUI_CHECKBOXEND,
		IMGUI_BUTTONEND,
		IMGUI_WINDOWEND,
		IMGUI_END
	};

	struct imguiItem {
		int				wID;
		ES::packet		data;
		std::string		name;
		imguiType		type = IMGUI_NONE;
		bool			same_line = false;
		std::function<void(imguiItem& item)> func;
		imguiItem(std::string iname = "") : name(iname) {}
		void render() {
			if (same_line) ImGui::SameLine();
			switch (type)
			{
			case WS::IMGUI_CHECKBOX:
				if (ImGui::Checkbox(name.c_str(), data.bp(0))) {
					func(*this);
				}
				break;
			case WS::IMGUI_BUTTON:
				if (ImGui::Button(name.c_str())) {
					func(*this);
				}
				break;
			case WS::IMGUI_SLIDER:
				if (ImGui::SliderFloat(name.c_str(), data.fp(0), data.f(1), data.f(2))) {
					func(*this);
				}
				break;
			case WS::IMGUI_COLOR_EDIT3:
				if (ImGui::ColorEdit3(name.c_str(), data.fp(0))) {
					func(*this);
				}
				break;
			case WS::IMGUI_NONE:
				break;
			case WS::IMGUI_SEPARATOR:
				ImGui::Separator();
				break;
			case WS::IMGUI_TEXT:
				ImGui::Text(name.c_str());
				break;
			case WS::IMGUI_INPUT:
				break;
			case WS::IMGUI_WINDOW:
				break;
			case WS::IMGUI_COLOR:
				break;
			case WS::IMGUI_COMBO:
				break;
			case WS::IMGUI_LISTBOX:
				break;
			case WS::IMGUI_TREE:
				break;
			case WS::IMGUI_TAB:
				break;
			case WS::IMGUI_TABBAR:
				break;
			case WS::IMGUI_TABCONTENT:
				break;
			case WS::IMGUI_TABCONTENTEND:
				break;
			case WS::IMGUI_TABEND:
				break;
			case WS::IMGUI_TREEEND:
				break;
			case WS::IMGUI_LISTBOXEND:
				break;
			case WS::IMGUI_COMBOEND:
				break;
			case WS::IMGUI_COLOREND:
				break;
			case WS::IMGUI_TEXTEND:
				break;
			case WS::IMGUI_INPUTEND:
				break;
			case WS::IMGUI_SLIDEREND:
				break;
			case WS::IMGUI_CHECKBOXEND:
				break;
			case WS::IMGUI_BUTTONEND:
				break;
			case WS::IMGUI_WINDOWEND:
				break;
			case WS::IMGUI_END:
				break;
			default:
				break;
			}
		}
	};

	struct imguiWindow {
		int ID;
		std::string name;
		ImGuiWindowFlags layoutFlags = 0;
		ImGuiCond cond = 0;
		ImVec2    sz = ImVec2(-1,-1);
		ImVec2    pos  = ImVec2(-1, -1);
		std::vector<imguiItem>		imguiItems;
		imguiWindow(std::string nm) : name(nm) {}

		bool show = true;
		
		void setFlags(ImGuiWindowFlags flags) {
			layoutFlags = flags;
		}
		void setSize(ImVec2 isz, ImGuiCond icond = 0) {
			sz = isz;
			cond = icond;
		}
		void setPos(ImVec2 p) {
			pos = p;
		}

		inline int getLastImguiItemID()
		{
			return (int)imguiItems.size() - 1;
		}

		WS::imguiItem& getImguiItem(int id)
		{
			return imguiItems[id];
		}
	};

	int addImguiWindow(std::string name);
	int addCheckbox  (int imguiWinID,std::string name,bool initialState = false,bool sameline = false);
	int addButton    (int imguiWinID,std::string name,bool sameline = false);
	int addSlider    (int imguiWinID,std::string name,float value,float start,float end,bool sameline = false);
	int addColorEdit3(int imguiWinID,std::string name,float r = 0.1f, float g = 0.1f, float b = 0.1f,bool sameline = false);
	int addText      (int imguiWinID,std::string name,bool sameline = false);
	int addSeparator (int imguiWinID );
	
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#pragma endregion ImGui_Stuff

	inline float		winPosX = 400;
	inline float		winPosY = 200;

	inline float		Width = 2000;
	inline float		Height = 1300;
	inline float		bottomHeight = 300;
	inline float		topHeight = 100;
	inline float		leftWidth = 300;
	inline float		rightWidth = 300;
	inline float		statusHeight = 30;
	
	inline float		fullWidth;
	inline float		fullHeight;
	
	inline float		Aspect = (float) Width / (float)Height;
	inline std::string	GLabTitleDafault = "GLab Ver. 1.0";
	inline std::string	GLabTitle;

	inline int			frame_buffer_w, frame_buffer_h;
	inline GLFWwindow* window;
	// Our state
	inline bool			show_demo_window = false;
	inline bool			show_another_window = false;
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

	int init();
	
	void exit();
	
	void overlayRender();
	
	void preRenderOverlay();
	
	void postRenderOverlay();
	
	void preRender();
	
	void postRender();	
	
	void doWhileResizing();
	
	void setSwapInterval(int interval);

	struct ExampleAppLog
	{
		ImGuiTextBuffer     Buf;
		ImGuiTextFilter     Filter;
		ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
		bool                AutoScroll;  // Keep scrolling if already at the bottom.

		ExampleAppLog()
		{
			AutoScroll = true;
			Clear();
		}

		void    Clear()
		{
			Buf.clear();
			LineOffsets.clear();
			LineOffsets.push_back(0);
		}

		void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
		{
			int old_size = Buf.size();
			va_list args;
			va_start(args, fmt);
			Buf.appendfv(fmt, args);
			va_end(args);
			for (int new_size = Buf.size(); old_size < new_size; old_size++)
				if (Buf[old_size] == '\n')
					LineOffsets.push_back(old_size + 1);
		}

		void    Draw(const char* title, bool* p_open = NULL)
		{
			if (!ImGui::Begin(title, p_open))
			{
				ImGui::End();
				return;
			}

			// Options menu
			if (ImGui::BeginPopup("Options"))
			{
				ImGui::Checkbox("Auto-scroll", &AutoScroll);
				ImGui::EndPopup();
			}

			// Main window
			if (ImGui::Button("Options"))
				ImGui::OpenPopup("Options");
			ImGui::SameLine();
			bool clear = ImGui::Button("Clear");
			ImGui::SameLine();
			bool copy = ImGui::Button("Copy");
			ImGui::SameLine();
			Filter.Draw("Filter", -100.0f);

			ImGui::Separator();

			if (ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
			{
				if (clear)
					Clear();
				if (copy)
					ImGui::LogToClipboard();

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				const char* buf = Buf.begin();
				const char* buf_end = Buf.end();
				if (Filter.IsActive())
				{
					// In this example we don't use the clipper when Filter is enabled.
					// This is because we don't have random access to the result of our filter.
					// A real application processing logs with ten of thousands of entries may want to store the result of
					// search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
					for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
					{
						const char* line_start = buf + LineOffsets[line_no];
						const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
						if (Filter.PassFilter(line_start, line_end))
							ImGui::TextUnformatted(line_start, line_end);
					}
				}
				else
				{
					// The simplest and easy way to display the entire buffer:
					//   ImGui::TextUnformatted(buf_begin, buf_end);
					// And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
					// to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
					// within the visible area.
					// If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
					// on your side is recommended. Using ImGuiListClipper requires
					// - A) random access into your data
					// - B) items all being the  same height,
					// both of which we can handle since we have an array pointing to the beginning of each line of text.
					// When using the filter (in the block of code above) we don't have random access into the data to display
					// anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
					// it possible (and would be recommended if you want to search through tens of thousands of entries).
					ImGuiListClipper clipper;
					clipper.Begin(LineOffsets.Size);
					while (clipper.Step())
					{
						for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
						{
							const char* line_start = buf + LineOffsets[line_no];
							const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
							ImGui::TextUnformatted(line_start, line_end);
						}
					}
					clipper.End();
				}
				ImGui::PopStyleVar();

				// Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
				// Using a scrollbar or mouse-wheel will take away from the bottom edge.
				if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
					ImGui::SetScrollHereY(1.0f);
			}
			ImGui::EndChild();
			ImGui::End();
		}
	};

	void drawImguiLog();
	void drawImguiMenu();

	static ExampleAppLog log;
}

