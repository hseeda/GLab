#pragma once
#include <functional>

enum ET
{
	none = -1,
	// GLFW
	glfw_error = 0,		// event(0, description)
	window_resize = 1,		// event(1, Width, Height)	double,double
	window_finished_resizing = 2,		// event(1, Width, Height)	double,double

	// Mouse
	mouse_wheel = 9,		// ES::dispatch(event(9, xoffset, yoffset)	double,double
	mouse_move_while_pressed = 10,		// event(10, deltaX, deltaY) double,double
	mouse_click = 11,		// event(11, (void*)window, button, action, mods) int button, int action, int mods
	mouse_move = 12,		// event(12, (void*)window, xpos, ypos) double xpos, double ypos
	character = 13,
	imgui_button = 51,
	imgui_slider = 52,
	imgui_checkbox = 53,
	imgui_textbox = 54,
	imgui_text = 55,
	imgui_color_edit3 = 56,

	openfile = 100,
};

enum BT {
	defaulBody = 0,
	body = 1
};
inline std::string eventTypeName(ET _type) {
	switch (_type)
	{
	case ET::none: return "none";
	case ET::glfw_error: return "glfw_error";
	case ET::window_resize: return "window_resize";
	case ET::window_finished_resizing: return "window_finished_resizing";
	case ET::mouse_wheel: return "mouse_wheel";
	case ET::mouse_move_while_pressed: return "mouse_move_while_pressed";
	case ET::mouse_click: return "mouse_click";
	case ET::mouse_move: return "mouse_move";
	case ET::character: return "character";
	case ET::imgui_button: return "imgui_button";
	case ET::imgui_slider: return "imgui_slider";
	case ET::imgui_checkbox: return "imgui_checkbox";
	case ET::imgui_textbox: return "imgui_textbox";
	case ET::imgui_text: return "imgui_text";
	case ET::imgui_color_edit3: return "imgui_color_edit3";
	default: return "unknown";
	}
}

#define EVENT_SYSTEM_MAX_EVENTS 32
//------------------------------------------------------------------------------ Classes

union i3 { int i[3]; };

struct alignas (4) CStatus {
	unsigned int data = 0;
	inline bool get(int i) { return data >> i & 1; }
	inline void set(int i, bool b = true) {
		if (b)
			data |= 1 << i;
		else
			data &= ~(1 << i);
	}
	inline bool b(int i) { return data >> i & 1; }
	inline void t(int i) { data |= 1 << i; }
	inline void f(int i) { data &= ~(1 << i);}
};

struct packet {
	inline char* cp(int _i) { return &data.__c[_i]; }
	inline bool* bp(int _i) { return &data.__b[_i]; }
	inline int* ip(int _i) { return &data.__i[_i]; }
	inline unsigned int* up(int _i) { return &data.__u[_i]; }
	inline float* fp(int _i) { return &data.__f[_i]; }
	inline double* dp(int _i) { return &data.__d[_i]; }
	inline char& c(int _i) { return data.__c[_i]; }
	inline bool& b(int _i) { return data.__b[_i]; }
	inline int& i(int _i) { return data.__i[_i]; }
	inline unsigned int& u(int _i) { return data.__u[_i]; }
	inline float& f(int _i) { return data.__f[_i]; }
	inline double& d(int _i) { return data.__d[_i]; }
	int ID;
	std::string str;
	packet(int iid = -1) :ID(iid) {}
private:
	union {
		bool		 __b[8 * 4];
		char		 __c[8 * 4];
		float		 __f[8];
		unsigned int __u[8];
		int			 __i[8];
		double		 __d[8 / 2];
		void* __v[8];
	} data;
};

struct CEvent {
	ET typeID;
	int eventID;
	packet data;
	void* p;
	std::string eventName;

	CEvent() {typeID = ET::none; eventID = -1;}

	CEvent(ET itypeID) { typeID = itypeID; eventID = -1; }

	CEvent(ET itypeID, packet& idata) {typeID = itypeID;eventID = idata.ID;data = idata;}
	
	CEvent(ET itypeID, double d0, double d1) {
		typeID = itypeID;
		eventID = -1;
		data.d(0) = d0; data.d(1) = d1;
	}
	
	CEvent(ET itypeID, void* _p, int i0, int i1, int i2) {
		typeID = itypeID;
		eventID = -1;
		p = _p; data.i(0) = i0; data.i(1) = i1; data.i(2) = i2;
	}

	CEvent(ET itypeID, int ieventID) { typeID = itypeID; eventID = ieventID; }

	CEvent(ET typeID, packet& idata, int ieventID) {
		typeID = typeID;
		eventID = ieventID;
		data = idata;
	}
	CEvent(ET typeID, int ieventID, double d0, double d1) {
		typeID = typeID;
		eventID = ieventID;
		data.d(0) = d0; data.d(1) = d1;
	}
	CEvent(ET typeID, int ieventID, void* _p, int i0, int i1, int i2) {
		typeID = typeID;
		eventID = ieventID;
		p = _p; data.i(0) = i0; data.i(1) = i1; data.i(2) = i2;
	}
};

typedef std::function<void(const CEvent&)> Slot;

typedef std::function<void(packet)> imguiSlot;

struct imguiSlotPair {
	imguiSlotPair(imguiSlot islt, packet ipkt) : slot(islt), pkt(ipkt) {}
	imguiSlot slot;
	packet pkt;
};

enum		imguiType {
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

struct		imguiItem {
	//---------------------------------
	static int		uniqueIDCounter;
	//---------------------------------
	int				windowID;
	packet			data;
	std::string		name;
	imguiType		type = IMGUI_NONE;
	bool			same_line = false;
	std::function<void(imguiItem& item)> func;

	imguiItem(std::string iname = "") { name = iname; data.ID = uniqueIDCounter++; }

	inline int uniqueID() { return data.ID; }

	void render() {
		if (same_line) ImGui::SameLine();
		switch (type)
		{
		case IMGUI_CHECKBOX:
			if (ImGui::Checkbox(name.c_str(), data.bp(0))) {
				func(*this);
			}
			break;
		case IMGUI_BUTTON:
			if (ImGui::Button(name.c_str())) {
				func(*this);
			}
			break;
		case IMGUI_SLIDER:
			if (ImGui::SliderFloat(name.c_str(), data.fp(0), data.f(1), data.f(2))) {
				func(*this);
			}
			break;
		case IMGUI_COLOR_EDIT3:
			if (ImGui::ColorEdit3(name.c_str(), data.fp(0))) {
				func(*this);
			}
			break;
		case IMGUI_NONE:
			break;
		case IMGUI_SEPARATOR:
			ImGui::Separator();
			break;
		case IMGUI_TEXT:
			ImGui::Text(name.c_str());
			break;
		case IMGUI_INPUT:
			break;
		case IMGUI_WINDOW:
			break;
		case IMGUI_COLOR:
			break;
		case IMGUI_COMBO:
			break;
		case IMGUI_LISTBOX:
			break;
		case IMGUI_TREE:
			break;
		case IMGUI_TAB:
			break;
		case IMGUI_TABBAR:
			break;
		case IMGUI_TABCONTENT:
			break;
		case IMGUI_TABCONTENTEND:
			break;
		case IMGUI_TABEND:
			break;
		case IMGUI_TREEEND:
			break;
		case IMGUI_LISTBOXEND:
			break;
		case IMGUI_COMBOEND:
			break;
		case IMGUI_COLOREND:
			break;
		case IMGUI_TEXTEND:
			break;
		case IMGUI_INPUTEND:
			break;
		case IMGUI_SLIDEREND:
			break;
		case IMGUI_CHECKBOXEND:
			break;
		case IMGUI_BUTTONEND:
			break;
		case IMGUI_WINDOWEND:
			break;
		case IMGUI_END:
			break;
		default:
			break;
		}
	}
};

struct		imguiWindow {
	int ID;
	std::string name;
	ImGuiWindowFlags layoutFlags = 0;
	ImGuiCond cond = 0;
	ImVec2    sz = ImVec2(-1, -1);
	ImVec2    pos = ImVec2(-1, -1);

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

	imguiItem& getImguiItem(int id)
	{
		return imguiItems[id];
	}
};

inline int	imguiItem::uniqueIDCounter = 0;

inline std::vector<imguiWindow>		imguiWindows;

inline std::vector<imguiSlotPair>	imguiSlots;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ events
