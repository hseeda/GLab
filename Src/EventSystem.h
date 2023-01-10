#pragma once
#include "pch.h"

//--------------------------------------------
	namespace ES
	{
		inline int											m_needs_update = 0;
		enum et
		{
			none                     = -1,
			// GLFW
			glfw_error               = 0,		// ES::event(0, description)
			window_resize            = 1,		// ES::event(1, Width, Height)	double,double
			window_finished_resizing = 2,		// ES::event(1, Width, Height)	double,double

			// Mouse
			mouse_wheel              = 9,		// ES::dispatch(ES::event(9, xoffset, yoffset)	double,double
			mouse_move_while_pressed = 10,		// ES::event(10, deltaX, deltaY) double,double
			mouse_click              = 11,		// ES::event(11, (void*)window, button, action, mods) int button, int action, int mods
			mouse_move               = 12,		// ES::event(12, (void*)window, xpos, ypos) double xpos, double ypos

			imgui_button              = 51,
			imgui_slider              = 52,
			imgui_checkbox            = 53,
			imgui_textbox             = 54,
			imgui_text				  = 55,
			imgui_color_edit3		  = 56
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
			int ID = -1;
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

		inline std::string eventTypeName(et _type) {
			switch (_type)
			{
			case ES::none: return "none";
			case ES::glfw_error: return "glfw_error";
			case ES::window_resize: return "window_resize";
			case ES::window_finished_resizing: return "window_finished_resizing";
			case ES::mouse_wheel: return "mouse_wheel";
			case ES::mouse_move_while_pressed: return "mouse_move_while_pressed";
			case ES::mouse_click: return "mouse_click";
			case ES::mouse_move: return "mouse_move";
			case ES::imgui_button: return "imgui_button";
			case ES::imgui_slider: return "imgui_slider";
			case ES::imgui_checkbox: return "imgui_checkbox";
			case ES::imgui_textbox: return "imgui_textbox";
			case ES::imgui_text: return "imgui_text";
			case ES::imgui_color_edit3: return "imgui_color_edit3";
			default: return "unknown";
			}
		}

#define EVENT_SYSTEM_MAX_EVENTS 32
	
	//------------------------------------------------------------------------------ Classes
	struct event {
		packet data;
		et tID = et::none;
		void* p;
		std::string str;
		event() : tID() {}
		event(et typeID) : tID(typeID) {}
		event(et typeID, packet &idata) : tID(typeID),data(idata) {}
		event(et typeID, double d0, double d1) : tID(typeID) {data.d(0) = d0; data.d(1) = d1;}
		event(et typeID, void* _p, int i0, int i1, int i2)	: tID(typeID) {p = _p;data.i(0) = i0;data.i(1) = i1; data.i(2) = i2; }
	};

	//============================================================================================

	//typedef std::function<void(const ES::event&)> SlotWithEvent;
	typedef std::function<void(const ES::event&)> SlotWithEvent;

		// this is how to bind
		// #include <functional>    #for std::placeholders::_1
		// HS::Dispatcher.subscribe(char[8], std::bind( &HS::CLASS::handle_function, this, std::placeholders::_1 ));
		// handle_function signature is:
		//                         void handle(const Event& e) {  const MouseEvent& ee = e.get<MouseEvent>(); }
		
	void subscribe(et event_type, SlotWithEvent&& slot, int eventID = -1);

		// dispatch event right away
		void dispatch(const ES::event & event);
		void dispatch(et event_type);
		
		void post(const ES::event &event);
		void post(et event_type);
		void post(et event_type,packet &data);
		void postSkipRepeat(const ES::event& event);
		void postReplaceRepeat(const ES::event& event);

		void flush();

		void clear();
		
		void needsUpdate();
		
		int isUpdateNeeded();
		
		void updated();

	
	//-------------------------------------------- Global
}// end namespace
