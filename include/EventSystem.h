#pragma once
#include "HUtil.hpp"
#include <map> 
#include <functional> 
#include <GLFW/glfw3.h> 

	namespace ES
	{
#define EVENT_SYSTEM_MAX_EVENTS 32
	union event_storage {
		char   c[32*4];
		float  f[32];
		int    i[32];
		void*  v[32];
		double d[16];
	};
	
	//------------------------------------------------------------------------------ Classes
	struct event {
		event_storage data;
		int event_type;
		void* p;
		
		event() : event_type(-1) {}
		event(int event_type) : event_type(event_type) {}
		event(int event_type, const char* msg) : event_type(event_type) {
			strcpy(data.c, msg);
		}
		event(int event_type, double d0, double d1) : event_type(event_type) {
			data.d[0] = d0;
			data.d[1] = d1;
		}
		//event(int event_type, int i0, int i1, int i2, int i3)
		//	: event_type(event_type) {
		//		data.i[0] = i0;
		//		data.i[1] = i1;
		//		data.i[2] = i2;
		//		data.i[3] = i3;
		//	}
		event(int event_type, void* _p, int i0, int i1, int i2)
			: event_type(event_type) {
			p = _p;
			data.i[0] = i0;
			data.i[1] = i1;
			data.i[2] = i2;
		}

		
	};

	//============================================================================================
	class EventSystem
	{
	public:
		typedef std::function<void(const ES::event&)>	SlotType;
		// this is how to bind
		// #include <functional>    #for std::placeholders::_1
		// HS::Dispatcher.subscribe(char[8], std::bind( &HS::CLASS::handle_function, this, std::placeholders::_1 ));
		// handle_function signature is:
		//                         void handle(const Event& e) {  const MouseEvent& ee = e.get<MouseEvent>(); }
		
		void subscribe(int event_type, SlotType&& slot) 
		{
			m_observers[event_type].push_back(slot);
		}

		// dispatch event right away
		void dispatch(const ES::event & event) const {
			if (m_observers.find(event.event_type) == m_observers.end())	return;
			auto&& observers = m_observers.at(event.event_type);
			for (auto&& observer : observers)
				observer(event);
		}
		
		// post event in event queue to be responded to every frame
		void post(const ES::event &event) const {
			if (m_event_count == EVENT_SYSTEM_MAX_EVENTS)
			{
				_pl("EventSystem::post: event queue full");
				return;
			}
			m_event_store[m_event_count] = event;
			m_event_count++;
		}
		
		void flush() {
			if (m_event_count == 0) return;
			for (int i = 0; i < m_event_count; i++)
				dispatch(m_event_store[i]);
			m_event_count = 0;
		}

		void clear() {
			m_event_count = 0;
		}

	private:
		std::map <int, std::vector<SlotType>>		m_observers;
		mutable ES::event								m_event_store[EVENT_SYSTEM_MAX_EVENTS];
		mutable int										m_event_count = 0;
	};
	
	//-------------------------------------------- Global
	inline EventSystem es;
}// end namespace
