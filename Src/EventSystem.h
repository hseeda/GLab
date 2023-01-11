#pragma once
#include "pch.h"

//--------------------------------------------
	namespace ES
	{
		union i3 {int i[3];};
		inline int											m_needs_update = 0;
		enum ET
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
		inline std::string eventTypeName(ET _type) {
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
		struct packet {
			inline char*			cp(int _i) { return &data.__c[_i]; }
			inline bool*			bp(int _i) { return &data.__b[_i]; }
			inline int*				ip(int _i) { return &data.__i[_i]; }
			inline unsigned int*	up(int _i) { return &data.__u[_i]; }
			inline float*			fp(int _i) { return &data.__f[_i]; }
			inline double*			dp(int _i) { return &data.__d[_i]; }
			inline char&			c(int _i)  { return data.__c[_i]; }
			inline bool&			b(int _i)  { return data.__b[_i]; }
			inline int&				i(int _i)  { return data.__i[_i]; }
			inline unsigned int&	u(int _i)  { return data.__u[_i]; }
			inline float&			f(int _i)  { return data.__f[_i]; }
			inline double&			d(int _i)  { return data.__d[_i]; }
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
#define EVENT_SYSTEM_MAX_EVENTS 32
	//------------------------------------------------------------------------------ Classes
		struct event{
			ET typeID;
			int eventID;
			packet data;
			void* p;
			std::string eventName = "";
			
			event() {
				typeID = ET::none; eventID = -1;}
			
			event(ET itypeID) { typeID = itypeID; eventID = -1; }
			
			event(ET itypeID, packet& idata) { 
				typeID = itypeID;
				eventID = idata.ID;
				data = idata;
			}
			event(ET itypeID, double d0, double d1) {
				typeID = itypeID;
				eventID = -1;
				data.d(0) = d0; data.d(1) = d1; 
			}
			event(ET itypeID, void* _p, int i0, int i1, int i2){
				typeID = itypeID;
				eventID = -1;
				p = _p; data.i(0) = i0; data.i(1) = i1; data.i(2) = i2; 
			}
			
			event(ET itypeID, int ieventID) { typeID = itypeID; eventID = ieventID; }

			event(ET typeID, packet& idata, int ieventID) {
				typeID = typeID;
				eventID = ieventID;
				data = idata; 
			}
			event(ET typeID, int ieventID, double d0, double d1) {
				typeID = typeID;
				eventID = ieventID;
				data.d(0) = d0; data.d(1) = d1;
			}
			event(ET typeID, int ieventID, void* _p, int i0, int i1, int i2) {
				typeID = typeID;
				eventID = ieventID;
				p = _p; data.i(0) = i0; data.i(1) = i1; data.i(2) = i2;
			}
		};
	//============================================================================================

	//typedef std::function<void(const ES::event&)> SlotWithEvent;
	typedef std::function<void(const ES::event&)> Slot;

		// this is how to bind
		// #include <functional>    #for std::placeholders::_1
		// HS::Dispatcher.subscribe(char[8], std::bind( &HS::CLASS::handle_function, this));
		// HS::Dispatcher.subscribe(char[8], std::bind( &HS::CLASS::handle_function, this, std::placeholders::_1 ));
		// handle_function signature is:
		//                         void handle()  }
		//                         void handle(const Event& e) { }
		
		std::pair<int/*event_id*/, Slot> *subscribe(ET event_type, Slot&& slot, int event_id = -1);

		// dispatch event right away
		void dispatch(ET event_type, int event_id, const event* ievent = nullptr);
		void dispatch(const ES::event & event);
		void dispatch(ET event_type, int event_id = -1);
		void dispatch(int event_id);
		
		void post(ET event_type, int event_id, const event* ievent = nullptr);
		void post(const ES::event &ievent);
		void post(ET event_type, int event_id = -1);
		void post(ET event_type,packet &data);
		
		void postSkipRepeat(const ES::event& event);
		void postReplaceRepeat(const ES::event& event);

		void flush();

		void clear();
		
		void needsUpdate();
		
		int isUpdateNeeded();
		
		void updated();

	
	//-------------------------------------------- Global
}// end namespace
